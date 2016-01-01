#ifndef UTILITY
#define UTILITY

#include <cmath>
#include <limits>
#include <SFML/Graphics.hpp>

namespace Utility {
	inline void normalize(float &a, float &b){
		auto length = sqrt(a * a + b * b);
		if (length < std::numeric_limits<float>::epsilon()){
			//probably not the correct way to handle normalization of a nullvector, throw an exception instead?
			a = 1;
			b = 0;
			return;
		}
		a /= length;
		b /= length;
	}
	inline void rotate(float &a, float &b, float angle){
		//do I need to manually cache results of sin(angle) and cos(angle)?
		//is there a way to provide angle as a vector and would it be more efficient?
		auto a2 = a * cos(angle) - b * sin(angle);
		b = a * sin(angle) + b * cos(angle);
		a = a2;
	}

	template<class T, size_t size>
	constexpr size_t element_count(const T (&)[size]){
		return size;
	}

	template<class T>
	using remove_cvr = std::remove_cv_t<std::remove_reference_t<T>>;

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
			std::move(f)();
		}
	private:
		Function f;
	};
	template <class Function>
	RAII<Function> create_RAII(Function &&f){
		return RAII<remove_cvr<Function>>(std::forward<Function>(f));
	}
#define CAT(a,b) CAT_(a,b) // force expand
#define CAT_(a,b) a##b // actually concatenate
#define ON_SCOPE_EXIT(CODE) auto CAT(ON_SCOPE_EXIT_,__LINE__) = Utility::create_RAII([&](){CODE})
}

#endif // UTILITY
