#ifndef SYSTEM_H
#define SYSTEM_H

#include "ecs_impl.h"
#include "utility.h"

#include <limits>
#include <vector>

namespace ECS{
	//a struct to remove a component. This is unfortunately necessary, because otherwise entity and system don't know the types of its components
	namespace Entity_helper{
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
				return std::tie(id, f) < std::tie(other.id, f);
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
		inline bool operator <(Impl::Id id, const Remover &r){
			return r > id;
		}

		//it is important that removers is destroyed before the system component vectors are destroyed, therefore removers must be above those vectors
		static std::vector<Remover> &removers = * new std::vector<Remover>;
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
		template<class Component>
		static std::vector<Component> components{};
		template<class Component>
		static std::vector<Impl::Id> ids{max_id};

		template<class Component>
		std::vector<Utility::remove_cvr<Component>> &get_components(){
			return components<Utility::remove_cvr<Component>>;
		}
		template<class Component>
		std::vector<Impl::Id> &get_ids(){
			return ids<Utility::remove_cvr<Component>>;
		}
		template<class... Components>
		System_iterator<Components...> range(){
			System_iterator<Components...> si;
			si.advance(0);
			return si;
		}
		template <class... Components>
		void foreach (void (*f)(const System_iterator<Components...> &)){
			for (auto it = range<Components...>(); it; it.advance())
				f(it);
		}
	}

#else

	struct System{
		template<class Component>
		static std::vector<Utility::remove_cvr<Component>> &get_components(){
			return p_get_components<Utility::remove_cvr<Component>>();
		}
		template<class Component>
		static std::vector<Impl::Id> &get_ids(){
			return p_get_ids<Utility::remove_cvr<Component>>();
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
}

#include "systemiterator.h"

#endif // SYSTEM_H
