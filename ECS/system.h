#ifndef SYSTEM_H
#define SYSTEM_H

#include "ecs_impl.h"
#include "utility.h"

#include <limits>
#include <vector>
#include <functional>

namespace ECS{
	template<class H, class... T>
	struct System_iterator;
	struct Entity_handle;
	/*
	System keeps the components of all Entitys in a vector per component type and allows to iterate over Entitys with specified components.
	You only use System to iterate, use Entitys to add components.
	Limitations:
		Cannot have multiple components of the same type in one Entity. You can get around that with an array or vector of components
	*/
	struct System{
		template<class Component>
		static std::vector<Utility::remove_cvr<Component>> &get_components(){
			return components<Utility::remove_cvr<Component>>;
		}
		template<class Component>
		static std::vector<Impl::Id> &get_ids(){
			return ids<Utility::remove_cvr<Component>>;
		}
		//get a range iterator for a list of components, range<Position, Direction> iterates over all Entities with both a Position and a Direction component
		template<class... Components>
		static System_iterator<Components...> range();
		//get entity handle from a component that has been added to an entity
		template <class Component>
		static Entity_handle component_to_entity_handle(const Component &component);
		//list of systems that modify the world
		static std::vector<std::function<void()>> &get_systems(){
			static std::vector<std::function<void()>> systems; //could possibly get around using std::function, but it is hard and not worth it
			return systems;
		}
		//run all systems
		static void run_systems(){
			for (auto &f : get_systems()){
				f();
			}
		}
		//add a system
		template <class... Components, class Function>
		static void add_system(Function &&f){
			auto &systems = get_systems();
			systems.push_back([f = std::move(f)]{
				for (auto sit = range<Components...>(); sit; sit.advance()){
					f(sit.get_entity_handle());
				}
			});
		}
		//add a system which computes something once for all entities
		template <class... Components, class Function, class PrecomputeFunction>
		static void add_system(Function &&f, PrecomputeFunction &&pf){
			auto &systems = get_systems();
			systems.push_back([f = std::move(f), pf = std::move(pf)]{
				auto pc = pf();
				for (auto sit = range<Components...>(); sit; sit.advance()){
					f(sit.get_entity_handle(), pc);
				}
			});
		}
	private:
		//vector to store the components
		template<class Component>
		static std::vector<Component> components;
		//vector to store the IDs. ids and components are locked, so components<CTYPE>[x] is the component that belongs to entity ids<CTYPE>[x]
		template<class Component>
		static std::vector<Impl::Id> ids;
		//TODO: could make components and ids use the same memory since they reallocate at the same time, but this only saves a few memory allocations and is probably not worth it
	};
	template<class Component>
	std::vector<Component> ECS::System::components{};
	template<class Component>
	std::vector<ECS::Impl::Id> ECS::System::ids{ECS::Impl::max_id};
}

#include "entity_handle.h"
#include "systemiterator.h"

//get an Entity_handle for an Entity that owns the given component
//the given component must be owned by an Entity, otherwise it is UB!
template <class Component>
ECS::Entity_handle ECS::System::component_to_entity_handle(const Component &component){
	auto &components = get_components<Component>();
	assert_fast(&components.front() <= &component);
	assert_fast(&components.back() >= &component);
	auto index = &component - &components.front();
	return Entity_handle{get_ids<Component>()[index]};
}

template<class... Components>
ECS::System_iterator<Components...> ECS::System::range(){
	System_iterator<Components...> si;
	si.advance(0);
	return si;
}


#endif // SYSTEM_H
