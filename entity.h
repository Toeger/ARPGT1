#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <type_traits>
#include <limits>
#include <iostream>
#include <tuple>
#include <algorithm>
#include <cassert>

namespace Impl{
using Id = unsigned int;
}

template<class H, class... T>
struct System_iterator;
constexpr Impl::Id max_id = std::numeric_limits<Impl::Id>::max();

#if 0
//causes a segfault on exit, don't know why
namespace System{
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

template <class T>
struct System_iterator<T>{
	Impl::Id advance(){
		if (System::get_ids<T>().at(current_index) == max_id)
			return max_id;
		return advance(System::get_ids<T>().at(current_index) + 1);
	}

	Impl::Id advance(Impl::Id target){
		while (System::get_ids<T>().at(current_index) < target){
			current_index++;
		}
		return System::get_ids<T>().at(current_index);
	}
	operator bool(){
		return System::get_ids<T>().at(current_index) != max_id;
	}
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
		/*
		auto rest_target = rest.advance(target);
		while (target != rest_target){
			if (target < rest_target)
				target = t.advance(rest_target);
			else
				rest_target = rest.advance(target);
		}
		return target;
		*/
	}
	operator bool(){
		return t;
	}
//    std::tuple<T&, (Rest &)...>
//    operator *(){
//        return std::tie(*t, (*rest)...);
//    }
//    std::tuple<const T&, const Rest &...> operator *() const{
//        return std::tie(*t, *rest);
//    }
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
