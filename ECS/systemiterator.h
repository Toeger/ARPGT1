#ifndef SYSTEMITERATOR_H
#define SYSTEMITERATOR_H

#include "system.h"
#include "entity_handle.h"

namespace ECS{
	/*
	Iterator over Entitys with specifiable components.
	Iterating over Entitys with some specific components goes through all entities with any of the components
	TODO: It would be more ideomatic but less efficient to use begin and end style iterators.
	TODO: It would make sense to have a get function that returns a tuple of components. For that the struct layout (?) needs to be changed.
	TODO: Add casting/converting iterators. Removing a component would be fairly easy, adding a component would initiate searching. Unrelated iterators don't really make sense.
	*/
	template <class T>
	struct System_iterator<T>{
		//advance to the next entity with all specified components
		//if no more components are left you are at the end which is testable by converting to bool: true = at the end, false = not at the end
		//returns the Id of the next entity that has the specified component
		Impl::Id advance(){
			if (System::get_ids<T>().at(current_index) == Impl::max_id)
				return Impl::max_id;
			return advance(System::get_ids<T>().at(current_index) + 1);
		}
		//same as regular advance, just that you go to the next target with an Id not smaller than the given target's
		Impl::Id advance(Impl::Id target){
			while (System::get_ids<T>().at(current_index) < target){
				current_index++;
			}
			return System::get_ids<T>().at(current_index);
		}
		//check if we are at the end, returns true for at the end and false if not
		operator bool() const{
			return System::get_ids<T>().at(current_index) != Impl::max_id;
		}
		//gets a component from the iterator, the iterator must actually iterate over that component type
		template<class U>
		U &get(){
			static_assert(std::is_same<U, T>::value, "Invalid type for this iterator");
			return *(*this);
		}
		template<class U>
		const U &get() const{
			static_assert(std::is_same<U, T>::value, "Invalid type for this iterator");
			return *(*this);
		}
		Impl::Id get_id(){
			return System::get_ids<T>().at(current_index);
		}
		std::vector<Impl::Id> get_ids() const{
			return {System::get_ids<T>().at(current_index)};
		}
		ECS::Entity_handle get_entity_handle(){
			return System::component_to_entity_handle(**this);
		}

	private:
		T &operator *(){
			return System::get_components<T>().at(current_index);
		}
		const T &operator *() const{
			return System::get_components<T>().at(current_index);
		}
		std::size_t current_index = 0;
	};

	//see single component System_iterator above for comments
	template <class T, class... Rest>
	struct System_iterator{
		Impl::Id advance(){
			return advance(t.get_id() + 1);
		}
		Impl::Id advance(Impl::Id target){
			target = t.advance(target);
			for (auto new_target = rest.advance(target); target != new_target; new_target = rest.advance(target)){
				target = t.advance(new_target);
				if (target == new_target)
					return target;
			}
			return target;
		}
		operator bool(){
			return t;
		}
#if 0
		std::tuple<T&, (Rest &)...>
		operator *(){
			return std::tie(*t, (*rest)...);
		}
		std::tuple<const T&, const Rest &...> operator *() const{
			return std::tie(*t, *rest);
		}
#endif
		template<class U>
		std::enable_if_t<std::is_same<U, T>::value, U&> get(){
			return t.template get<U>();
		}
		template<class U>
		std::enable_if_t<!std::is_same<U, T>::value, U&> get(){
			return rest.template get<U>();
		}
		std::vector<Impl::Id> get_ids() const{
			auto v = rest.get_ids();
			v.push_back(t.get_id());
			return v;
		}

	private:
		System_iterator<T> t;
		System_iterator<Rest...> rest;
		//std::tuple<Rest...> rest;
	};

	template <class... T>
	bool operator ==(const System_iterator<T...> &lhs, const System_iterator<T...> &rhs){
		return lhs.current_index == rhs.current_index;
	}
}

#endif // SYSTEMITERATOR_H
