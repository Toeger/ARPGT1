#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <type_traits>
#include <limits>
#include <iostream>
#include <tuple>
#include <algorithm>
#include <cassert>

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
//			for (auto &f : remove_functions){
//				f(id);
//			}
		}
		//add a component to an Entity
		template<class Component>
		void add(Component &&c){
			auto &ids = System::get_ids<Component>();
			auto &components = System::get_components<Component>();
			auto insert_position = std::lower_bound(begin(ids), end(ids), id);
			assert(*insert_position != id);
			components.insert(begin(components) + (insert_position - begin(ids)), std::forward<Component>(c));
			ids.insert(insert_position, id);
			remove_functions.push_back(remover<Component>);
		}
		//emplace a component into an Entity
		template<class Component, class... Args>
		void emplace(Args &&...c){
			auto &ids = System::get_ids<Component>();
			auto &components = System::get_components<Component>();
			auto insert_position = std::lower_bound(begin(ids), end(ids), id);
			assert(*insert_position != id); //disallow multiple components of the same type for the same entity
			components.emplace(begin(components) + (insert_position - begin(ids)), std::forward<Args>(c)...);
			ids.insert(insert_position, id);
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
			//TODO: find the entry for Remover in System and erase it, which also erases the component from system
			assert(!"TODO");
		}
	private:
		//a struct to remove a component. this is unfortunately necessary, because otherwise entity doesn't know the types of its components
		struct Remover{
			Impl::Id id;
			void (*f)(Impl::Id);
			Remover(Impl::Id id, void (*f)(Impl::Id))
				:id(id)
				,f(f)
			{}
			bool operator <(const Remover &other) const{
				return id < other.id;
			}
			Remover(Remover &&) = default;
			Remover &operator = (Remover &&) = default;
			~Remover(){
				f(id);
			}
		};

		//
		template <class Component>
		void add_remover(){
			auto &remover_ids = System::get_ids<Remover>();
			auto &removers = System::get_components<Remover>();
			auto insert_position = std::lower_bound(begin(ids), end(ids), id);
			//would upper bound work? if so replace lower_bound with upper_bound, because upper_bound costs less relocations
			removers.emplace(begin(components) + (insert_position - begin(ids)), id, remover<Component>);
			remover_ids.insert(insert_position, id);
		}

		//remove a component of the given type
		template <class Component>
		static void remover(Impl::Id id){
			auto &ids = System::get_ids<Component>();
			auto id_it = lower_bound(begin(ids), end(ids), id);
			if (*id_it != id)
				throw std::runtime_error("Trying to remove non-existant component");
			auto &components = System::get_components<Component>();
			components.erase(begin(components) + (id_it - begin(ids)));
			ids.erase(id_it);
		}

		static Impl::Id id_counter;
		Impl::Id id;
	};
}

#endif // ENTITY_H
