#ifndef ENTITY_HANDLE_H
#define ENTITY_HANDLE_H

#include "system.h"
#include "entity_base.h"

namespace ECS{
	struct Entity_handle : private Entity_base{
		//users are not supposed to create Entity_handles, instead use System::component_to_entity_handle
		Entity_handle(Impl::Id id)
			:Entity_base(id)
		{}
		Entity_handle(const Entity_handle &other)
			:Entity_base(other.id)
		{}
		Entity_handle &operator =(const Entity_handle &) = default;
		using Entity_base::get;
		using Entity_base::remove;
		//could possibly allow adding/emplacing components through a handle, but destroying an entity and using a handle to add components would leak the components
	};
}

#endif // ENTITY_HANDLE_H
