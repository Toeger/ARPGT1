#ifndef ENTITY_HANDLE_H
#define ENTITY_HANDLE_H

#include "entity_base.h"

namespace ECS{
	struct Entity_handle : private ECS::Impl::Entity_base{
		//create invalid Entity_handle
		Entity_handle()
			:Entity_base(Impl::max_id)
		{}
		//users are not supposed to create valid Entity_handles directly, instead use System::component_to_entity_handle
		Entity_handle(Impl::Id id)
			:Entity_base(id)
		{}
		Entity_handle(const Entity_handle &other)
			:Entity_base(other.id)
		{}
		Entity_handle &operator =(const Entity_handle &other){
			id = other.id;
			return *this;
		}
		//check if an entity is valid by converting to bool
		operator bool(){
			return id != Impl::max_id;
		}
		//"inherited" functions
		using ECS::Impl::Entity_base::get;
		using ECS::Impl::Entity_base::remove;
		//could maybe allow adding/emplacing components through a handle, but destroying an entity and using a handle to add components would leak the components
	};
}

#endif // ENTITY_HANDLE_H
