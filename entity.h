#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <type_traits>
#include <limits>
#include <iostream>

namespace Impl{
using Id = unsigned int;
}

template<class H, class... T>
struct System_iterator;
constexpr Impl::Id max_id = std::numeric_limits<Impl::Id>::max();

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
    static System_iterator<Components...> begin(){
        return System_iterator<Components...>();
    }
    template<class... Components>
    static System_iterator<Components...> end(){
        System_iterator<Components...> si;
        si.current_index = max_id;
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

template <class T>
struct System_iterator<T>{
    Impl::Id advance(Impl::Id target){
        while (System::get_ids<T>()[current_index] < target){
            current_index++;
        }
        return System::get_ids<T>()[current_index];
    }
    std::size_t current_index = 0;
};

template <class T, class... Rest>
struct System_iterator{
    Impl::Id advance(Impl::Id target){
        return rest.advance(t.advance(target));
    }
    System_iterator<T> t;
    System_iterator<Rest...> rest;
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
        System::get_ids<Component>().back() = id;
        System::get_ids<Component>().push_back(max_id);
        System::get_components<Component>().push_back(std::forward<Component>(c));
    }
private:
    static Impl::Id id_counter;
    Impl::Id id;
};

#endif // ENTITY_H
