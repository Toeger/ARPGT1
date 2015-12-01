#ifndef UTILITY
#define UTILITY

#include <cmath>
#include <limits>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

namespace Utility {
	inline void normalize(float &a, float &b){
		auto length = sqrt(a * a + b * b);
		if (length < std::numeric_limits<float>::epsilon())
			return;
		a /= length;
		b /= length;
	}
	inline void rotate(float &a, float &b, float angle){
		auto a2 = a * cos(angle) - b * sin(angle);
		b = a * sin(angle) + b * cos(angle);
		a = a2;
	}

	inline sf::Vector2f b2s_coords(const b2Vec2 &v){
		return {v.x * 100, v.y * 100};
	}
	template <typename T>
	struct return_type;
	template <typename R, typename... Args>
	struct return_type<R(*)(Args...)> { using type = R; };
	template <typename R, typename... Args>
	struct return_type<R (Args...)> { using type = R; };
	template <class T>
	using return_type_t = typename return_type<T>::type;

	template <class Function>
	struct RAII{
		RAII(Function &&f) : f(std::move(f)){}
		~RAII(){
			f();
		}
	private:
		Function f;
	};
	template <class Function>
	RAII<Function> create_RAII(Function &&f){
		return RAII<Function>(std::move(f));
	}
#define CAT(a,b) CAT_(a,b) // force expand
#define CAT_(a,b) a##b // actually concatenate
#define ON_SCOPE_EXIT(CODE) auto CAT(ON_SCOPE_EXIT_,__LINE__) = Utility::create_RAII([&](){CODE})
}

#endif // UTILITY
