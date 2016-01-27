#ifndef ENTITY_BASE_H
#define ENTITY_BASE_H

#include "ecs_impl.h"
#include "Utility/asserts.h"

#include <vector>
#include <algorithm>
#include <tuple>

namespace ECS{
	//an Entity can have any type of component added to it
	//note that you cannot add multiple components with the same type, use vector<component> or array<component> to get around that

	namespace Impl{
		struct Entity_base
		{
			Entity_base(Impl::Id id)
				:id(id)
			{}
			Entity_base(Entity_base &&other) noexcept
				:id(other.id)
			{
				other.id = max_id;
			}
			Entity_base &operator =(Entity_base &&other) noexcept{
				std::swap(id, other.id);
				return *this;
			}
			~Entity_base() = default;

			//emplace a component into an Entity
			template<class Component, class... Args>
			void emplace(Args &&... args);
			//add a component to an Entity
			template<class Component>
			void add(Component &&c){
				emplace<Component>(std::forward<Component>(c));
			}
			//get the component of a given type or nullptr if the Entity has no such component
			template<class Component>
			Component *get();
			//remove a component of a given type, UB if the entity has no such component, test with get to check if the entity has that component
			template<class Component>
			void remove();
		private:
			//remove a component of the given type and id
			template <class Component>
			static void remover(Impl::Id id);

			template <class Component>
			void add_remover(){
				Remover r(id, remover<Component>);
				auto pos = std::lower_bound(begin(removers), end(removers), r);
				removers.insert(pos, std::move(r));
				assert_all(std::is_sorted(begin(removers), end(removers)));
			}

			template <class Component>
			void remove_remover(std::size_t index){
				removers.erase(begin(removers) + index);
			}

			//a struct to remove a component. This is unfortunately necessary, because entities don't know the types of their components
			struct Remover{
				Remover(Impl::Id id, void (*f)(Impl::Id))
					:id(id)
					,f(f)
				{}
				Remover(Remover &&other) noexcept
					:id(other.id)
					,f(other.f){
					other.f = remover_dummy;
				}
				Remover &operator = (Remover &&other) noexcept{
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
				bool operator <(Impl::Id other_id) const{
					return id < other_id;
				}
				bool operator >(Impl::Id other_id) const{
					return id > other_id;
				}
			private:
				//data
				Impl::Id id;
				void (*f)(Impl::Id);
				//empty function to put into removers that have been moved from
				static inline void remover_dummy(Impl::Id){}
			};
			//it is important that removers is cleared before the system component vectors are destroyed
			//it is also necessary to have removers be destroyed after all entities, because entities access removers in the destructor, don't know how to do that without leaking removers
		protected:
			static Impl::Id id_counter;
			Impl::Id id;
			static std::vector<Remover> removers;
			friend bool operator <(Impl::Id id, const Entity_base::Remover &r);
		};
		inline bool operator <(Impl::Id id, const Entity_base::Remover &r){
			return r > id;
		}
	}
}

#include "system.h"

namespace ECS{
	namespace Impl{
		//emplace a component into an Entity
		template<class Component, class... Args>
		void Entity_base::emplace(Args &&... args){
			auto &ids = System::get_ids<Component>();
			auto &components = System::get_components<Component>();
			auto insert_position = std::lower_bound(begin(ids), end(ids), id);
			assert_fast(*insert_position != id); //disallow multiple components of the same type for the same entity
			components.emplace(begin(components) + (insert_position - begin(ids)), std::forward<Args>(args)...);
			ids.insert(insert_position, id);
			add_remover<Component>();
			assert_all(std::is_sorted(begin(ids), end(ids)));
		}
		//get the component of a given type or nullptr if the Entity has no such component
		template<class Component>
		Component *Entity_base::get(){
			auto &ids = System::get_ids<Component>();
			auto id_it = lower_bound(begin(ids), end(ids), id);
			if (*id_it != id)
				return nullptr;
			auto pos = id_it - begin(ids);
			auto &components = System::get_components<Component>();
			return &components.at(pos);
		}
		//remove a component of a given type, UB if the entity has no such component, test with get to check if the entity has that component
		template<class Component>
		void Entity_base::remove(){
			auto &ids = System::get_ids<Component>();
			auto id_pos = lower_bound(begin(ids), end(ids), id);
			assert_fast(id_pos != end(ids));
			remove_remover<Component>(id_pos - begin(ids));
		}
		//remove a component of the given type and id
		template <class Component>
		void Entity_base::remover(Impl::Id id){
			auto &ids = System::get_ids<Component>();
			auto id_it = lower_bound(begin(ids), end(ids), id);
			assert_fast(*id_it == id); //make sure the component to remove exists
			auto &components = System::get_components<Component>();
			components.erase(begin(components) + (id_it - begin(ids)));
			ids.erase(id_it);
			assert_all(std::is_sorted(begin(ids), end(ids)));
		}
	}
}

#endif // ENTITY_BASE_H
