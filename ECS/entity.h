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
			auto entity_range = std::equal_range(begin(removers), end(removers), id); //FIXME: bug here: equal_range covers too much and too much is being erased
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
		inline void make_automatic(bool (*function)(Entity &)) &&;
		static void make_automatic(Entity &&entity, bool (*function)(Entity &)){
			std::move(entity).make_automatic(function);
		}
		using Entity_base::add;
		using Entity_base::emplace;
		using Entity_base::get;
		using Entity_base::remove;
	};

	struct Remove_checker{
		Remove_checker(bool (*function)(Entity &), Entity &&entity)
			:function(function)
			,entity(std::move(entity))
		{}
		Remove_checker(Remove_checker &&) = default;
		Remove_checker &operator =(Remove_checker &&) = default;
		bool (*function)(Entity &);
		Entity entity;
	};

	void ECS::Entity::make_automatic(bool (*function)(Entity &)) &&{
		System::get_components<Remove_checker>().push_back(Remove_checker{function, std::move(*this)});
	}
}

#endif // ENTITY_H
