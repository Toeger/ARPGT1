#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <type_traits>
#include <limits>
#include <iostream>
#include <tuple>
#include <algorithm>
#include <cassert>

//every Entity gets a unique Id
namespace Impl{
using Id = unsigned int;
}

template<class H, class... T>
struct System_iterator;
constexpr Impl::Id max_id = std::numeric_limits<Impl::Id>::max();

/*
System keeps the components of all Entitys in a vector per component type and allows to iterate over Entitys with specified components.
You only use System to iterate, use Entitys to add components.
Limitations:
	Cannot have multiple components of the same type in one Entity. You can get around that with a tuple or vector of components
*/
#if 0
namespace System{ //this implementation should be better than the one below but causes a segfault on exit, don't know why
	template<class T>
	using remove_cvr = std::remove_cv_t<std::remove_reference_t<T>>;

	template<class Component>
	static std::vector<Component> components{};
	template<class Component>
	static std::vector<Impl::Id> ids{max_id};

	template<class Component>
	std::vector<remove_cvr<Component>> &get_components(){
		return components<remove_cvr<Component>>;
	}
	template<class Component>
	std::vector<Impl::Id> &get_ids(){
		return ids<remove_cvr<Component>>;
	}
	template<class... Components>
	System_iterator<Components...> range(){
		System_iterator<Components...> si;
		si.advance(0);
		return si;
	}
	template<class Component>
	void clear(){
		components<remove_cvr<Component>>.clear();
		ids<remove_cvr<Component>>.clear();
	}
}

#else

struct System{
	template<class T>
	using remove_cvr = std::remove_cv_t<std::remove_reference_t<T>>;

	template<class Component>
	static std::vector<remove_cvr<Component>> &get_components(){
		return p_get_components<remove_cvr<Component>>();
	}
	template<class Component>
	static std::vector<Impl::Id> &get_ids(){
		return p_get_ids<remove_cvr<Component>>();
	}
	template<class... Components>
	static System_iterator<Components...> range(){
		System_iterator<Components...> si;
		si.advance(0);
		return si;
	}
	template<class Component>
	static void clear(){
		p_get_components<remove_cvr<Component>>().clear();
		p_get_ids<remove_cvr<Component>>().clear();
	}
private:
	template<class Component>
	static std::vector<Component> &p_get_components(){
		static std::vector<Component> components;
		return components;
	}
	template<class Component>
	static std::vector<Impl::Id> &p_get_ids(){
		static std::vector<Impl::Id> ids = {max_id};
		return ids;
	}
};

#endif


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
		if (System::get_ids<T>().at(current_index) == max_id)
			return max_id;
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
	operator bool(){
		return System::get_ids<T>().at(current_index) != max_id;
	}
	//gets a component from the iterator, the iterator must actually iterate over that component type
	template<class U>
	U &get(){
		static_assert(std::is_same<U, T>::value, "Invalid type for this iterator");
		return *(*this);
	}
	Impl::Id get_id(){
		return System::get_ids<T>().at(current_index);
	}
	std::vector<Impl::Id> get_ids() const{
		return {System::get_ids<T>().at(current_index)};
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

//an Entity can have any type of component added to it
//note that you cannot add multiple components with the same type, use vector<component> or array<component> to get around that
struct Entity
{
	Entity(){
		id = id_counter++;
	}
	template<class Component>
	void add(Component &&c){
		auto &ids = System::get_ids<Component>();
		auto &components = System::get_components<Component>();
		auto insert_position = std::lower_bound(begin(ids), end(ids), id);
		assert(*insert_position != id);
		components.insert(begin(components) + (insert_position - begin(ids)), std::forward<Component>(c));
		ids.insert(insert_position, id);
	}
	template<class Component, class... Args>
	void emplace(Args &&...c){
		auto &ids = System::get_ids<Component>();
		auto &components = System::get_components<Component>();
		auto insert_position = std::lower_bound(begin(ids), end(ids), id);
		assert(*insert_position != id);
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
private:
	static Impl::Id id_counter;
	Impl::Id id;
};

#endif // ENTITY_H
