#ifndef ENTITY_H
#define ENTITY_H

#include "Utility/asserts.h"
#include "utility.h"
#include "ECS/system.h"
#include "entity_base.h"

#include <iostream>

/*
Overhead of the Entity Component System:
	One Id per Entity and Component + 1 function pointer per Component
	Components are stored in one vector per type
	Ids are stored in one vector per component type
	Putting components and Ids into the same vector would work, not sure if it would be better. Probably depends on component size.
*/

namespace ECS{
	struct Entity_handle;
	//an Entity can have any type of component added to it
	//note that you cannot add multiple components with the same type, use vector<component> or array<component> to get around that
	struct Entity : private Impl::Entity_base{
		Entity()
			:Entity_base(id_counter++)
		{}
		Entity(Entity &&other) noexcept
			:Entity_base(std::move(other))
		{}
		Entity &operator =(Entity &&other) noexcept{
			std::swap(id, other.id);
			return *this;
		}

		~Entity(){
			assert_all(std::is_sorted(begin(removers), end(removers))); //make sure removers are still sorted
			auto entity_range = std::equal_range(begin(removers), end(removers), id);
			removers.erase(entity_range.first, entity_range.second);
			assert_all(std::is_sorted(begin(removers), end(removers))); //make sure removers are still sorted
			assert_all(std::binary_search(begin(removers), end(removers), id) == false); //make sure we deleted all removers with our id
		}
		//clears all components from all entities
		//must call this at the end of main before destructors of static Entities run, otherwise it may crash due to static initialization order fiasco
		static void clear_all(){
			removers.clear();
		}
		//transfer ownership of this entity to the ECS. It is passed a function that takes an Entity& and returns a bool iff the entity should be destroyed now
		inline void make_automatic(bool (*function)(Entity_handle)) &&;
		static void make_automatic(Entity &&entity, bool (*function)(Entity_handle)){
			std::move(entity).make_automatic(function);
		}
		//turn to handle
		Entity_handle to_handle(){
			return Entity_handle{id};
		}
		//inherit from Entity_base
		using Entity_base::add;
		using Entity_base::emplace;
		using Entity_base::get;
		using Entity_base::remove;
		using Entity_base::is_valid;
	};

	struct Remove_checker{
		Remove_checker(bool (*function)(Entity_handle), Entity &&entity)
			:function(function)
			,entity(std::move(entity))
		{
			assert_fast(this->function);
			assert_fast(this->entity.is_valid());
		}
		//Remove_checker(Remove_checker &&) = default;
		Remove_checker(Remove_checker &&other)
			:function(other.function)
			,entity(std::move(other.entity))
		{
			assert_fast(this->function);
			//assert_fast(this->entity.is_valid());
		}
		//Remove_checker &operator =(Remove_checker &&) = default;
		Remove_checker &operator =(Remove_checker &&other){
			//assert_fast(other.entity.is_valid());
			std::swap(entity, other.entity);
			std::swap(function, other.function);
			assert_fast(this->function);
			//assert_fast(this->entity.is_valid());
			return *this;
		}
		bool (*function)(Entity_handle);
		Entity entity;
	};

	void ECS::Entity::make_automatic(bool (*function)(Entity_handle)) &&{
		assert_fast(is_valid());
		System::get_components<Remove_checker>().push_back(Remove_checker{function, std::move(*this)});
	}
}

#endif // ENTITY_H
