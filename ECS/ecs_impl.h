#ifndef ECS_IMPL_H
#define ECS_IMPL_H

#include <limits>

namespace ECS {
	//every Entity gets a unique Id
	namespace Impl{
		using Id = unsigned int;
		constexpr Impl::Id max_id = std::numeric_limits<Impl::Id>::max();
	}
}


#endif // ECS_IMPL_H
