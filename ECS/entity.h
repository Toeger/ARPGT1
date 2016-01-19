#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <type_traits>
#include <limits>
#include <iostream>
#include <tuple>
#include <algorithm>
#include "Utility/asserts.h"

#include "utility.h"
#include "ECS/system.h"

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

	struct Entity
	{
		Entity(){
			id = id_counter++;
		}
		Entity(Entity &&other)
			:id(other.id)
		{
			other.id = max_id;
		}
		Entity &operator =(Entity &&other){
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
		//add a component to an Entity
		template<class Component>
		void add(Component &&c){
			auto &ids = System::get_ids<Component>();
			auto &components = System::get_components<Component>();
			auto insert_position = std::lower_bound(begin(ids), end(ids), id);
			assert_fast(*insert_position != id);
			components.insert(begin(components) + (insert_position - begin(ids)), std::forward<Component>(c));
			ids.insert(insert_position, id);
			add_remover<Component>();
			assert_all(std::is_sorted(begin(ids), end(ids)));
		}
		//emplace a component into an Entity
		template<class Component, class... Args>
		void emplace(Args &&...c){
			auto &ids = System::get_ids<Component>();
			auto &components = System::get_components<Component>();
			auto insert_position = std::lower_bound(begin(ids), end(ids), id);
			assert_fast(*insert_position != id); //disallow multiple components of the same type for the same entity
			components.emplace(begin(components) + (insert_position - begin(ids)), std::forward<Args>(c)...);
			add_remover<Component>();
			assert_all(std::is_sorted(begin(ids), end(ids)));
		}
		//get the component of a given type or nullptr if the Entity has no such component
		template<class Component>
		Component *get(){
			auto &ids = System::get_ids<Component>();
			auto id_it = lower_bound(begin(ids), end(ids), id);
			if (*id_it != id)
				return nullptr;
			auto pos = id_it - begin(ids);
			auto &components = System::get_components<Component>();
			return &components.at(pos);
		}
		//remove a component of a given type, throws a runtime_exception if the entity has no such component, test with get to check if the entity has that component
		template<class Component>
		void remove(){
			//TODO: find the entry for Remover and erase it, which also erases the component from system
			assert(!"TODO");
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
	private:
		//remove a component of the given type and id
		template <class Component>
		static void remover(Impl::Id id){
			auto &ids = System::get_ids<Component>();
			auto id_it = lower_bound(begin(ids), end(ids), id);
			assert_fast(*id_it == id); //make sure the component to remove exists
			auto &components = System::get_components<Component>();
			components.erase(begin(components) + (id_it - begin(ids)));
			ids.erase(id_it);
			assert_all(std::is_sorted(begin(ids), end(ids)));
		}

		template <class Component>
		void add_remover(){
			Remover r(id, remover<Component>);
			auto pos = std::lower_bound(begin(removers), end(removers), r);
			removers.insert(pos, std::move(r));
			assert_all(std::is_sorted(begin(removers), end(removers)));
		}

		static Impl::Id id_counter;
		Impl::Id id;
		//a struct to remove a component. This is unfortunately necessary, because entities don't know the types of their components
		struct Remover{
			Remover(Impl::Id id, void (*f)(Impl::Id))
				:id(id)
				,f(f)
			{}
			Remover(Remover &&other)
				:id(other.id)
				,f(other.f){
				other.f = remover_dummy;
			}
			Remover &operator = (Remover &&other){
				using std::swap;
				swap(id, other.id);
				swap(f, other.f);
				return *this;
			}
			~Remover(){
				f(id);
			}
			bool operator <(const Remover &other) const{
				return std::tie(id, f) < std::tie(other.id, other.f);
			}
			bool operator <(Impl::Id id) const{
				return this->id < id;
			}
			bool operator >(Impl::Id id) const{
				return this->id > id;
			}
		private:
			//data
			Impl::Id id;
			void (*f)(Impl::Id);
			//empty function to put into removers that have been moved from
			static inline void remover_dummy(Impl::Id){
			}
		};

		//it is important that removers is cleared before the system component vectors are destroyed
		//it is also necessary to have removers be destroyed after all entities, because entities access removers in the destructor, don't know how to do that without leaking removers
		static std::vector<Remover> removers;
		friend bool operator <(Impl::Id id, const Entity::Remover &r);
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
	inline void ECS::Entity::make_automatic(bool (*function)(Entity &)) &&{
		System::get_components<Remove_checker>().push_back(Remove_checker{function, std::move(*this)});
	}
	inline bool operator <(Impl::Id id, const Entity::Remover &r){
		return r > id;
	}
}

#endif // ENTITY_H
