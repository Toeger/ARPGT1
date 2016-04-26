#include "ecs_test.h"
#include "ECS/entity.h"
#include "ECS/entity_handle.h"
#include "ECS/system.h"

#include <cassert>

//move only test component
struct Test_component {
	Test_component(int n)
		: n(n) {
		object_count++;
	}
	Test_component(Test_component &&other)
		: n(other.n) {
		other.n = 0;
	}

	Test_component &operator=(Test_component &&other) {
		std::swap(n, other.n);
		return *this;
	}
	~Test_component() {
		object_count--;
	}

	int n;
	static size_t object_count;
};

size_t Test_component::object_count = 0;

static void test_entity() {
	ECS::Entity n;
	Test_component tc(42);
	assert(ECS::System::get_components<Test_component>().size() == 0);
	assert(n.get<Test_component>() == nullptr);
	n.add(std::move(tc));
	assert(n.get<Test_component>() != nullptr);
	assert(ECS::System::get_components<Test_component>().size() == 1);
	n.remove<Test_component>();
	assert(n.get<Test_component>() == nullptr);
	assert(ECS::System::get_components<Test_component>().size() == 0);
	n.emplace<Test_component>(1337);
	assert(n.get<Test_component>() != nullptr);
	assert(ECS::System::get_components<Test_component>().size() == 1);
	n.remove<Test_component>();
	assert(n.get<Test_component>() == nullptr);
	assert(ECS::System::get_components<Test_component>().size() == 0);
}

static void test_entity_handle() {
	ECS::Entity n;
	n.emplace<Test_component>(007);
	auto eh = ECS::System::component_to_entity_handle(*n.get<Test_component>());
	assert(eh.get<Test_component>() == n.get<Test_component>());
	n.remove<Test_component>();
	assert(eh.get<Test_component>() == n.get<Test_component>());

	{
		ECS::Entity n;
		n.emplace<Test_component>(224466);
		eh = ECS::System::component_to_entity_handle(*n.get<Test_component>());
		assert(eh.get<Test_component>());
		assert(eh.get<Test_component>() == n.get<Test_component>());
		auto &components = ECS::System::get_components<Test_component>();
		assert(components.size());
	}
	auto &components = ECS::System::get_components<Test_component>();
	assert(!components.size());
	assert(eh.get<Test_component>() == nullptr);
}

void test_ecs() {
	test_entity();
	test_entity_handle();
}
