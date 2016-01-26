#include "ecs_test.h"
#include "ECS/entity.h"
#include "ECS/system.h"
#include <cassert>

//move only test component
struct Test_component{
	Test_component(int n)
		:n(n)
	{
		object_count++;
	}
	Test_component(Test_component &&other)
		:n(other.n){
		other.n = 0;
	}

	Test_component &operator=(Test_component &&other){
		std::swap(n, other.n);
		return *this;
	}
	~Test_component(){
		object_count--;
	}

	int n;
	static size_t object_count;
};

size_t Test_component::object_count = 0;

void test_ecs()
{
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
