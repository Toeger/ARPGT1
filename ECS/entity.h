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
		Entity(Entity &&) = default;
		Entity &operator =(Entity &&) = default;
		~Entity(){
			auto entity_range = std::equal_range(begin(Entity_helper::removers), end(Entity_helper::removers), id);
			assert_all(std::equal_range(entity_range.first, entity_range.second, id) == std::make_pair(begin(Entity_helper::removers), end(Entity_helper::removers))); //make sure we only remove components of this entity
			Entity_helper::removers.erase(entity_range.first, entity_range.second);
			assert_all(std::is_sorted(begin(Entity_helper::removers), end(Entity_helper::removers))); //make sure removers are still sorted
			assert_all(std::binary_search(begin(Entity_helper::removers), end(Entity_helper::removers), id) == false); //make sure we deleted all removers with our id
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
			auto &removers = Entity_helper::removers;
			Entity_helper::Remover r(id, remover<Component>);
			auto pos = std::lower_bound(begin(removers), end(removers), r);
			removers.insert(pos, std::move(r));
			assert_all(std::is_sorted(begin(removers), end(removers)));
		}

		static Impl::Id id_counter;
		Impl::Id id;
	};
}

#endif // ENTITY_H
