#ifndef CASTS_H
#define CASTS_H

#include <type_traits>

template <class T>
constexpr std::make_signed_t<T> make_signed(T t){
	return static_cast<std::make_signed_t<T>>(t);
}

template <class T>
constexpr std::make_unsigned_t<T> make_unsigned(T t){
	return static_cast<std::make_unsigned_t<T>>(t);
}

constexpr float precision_cast(double d){
	return static_cast<float>(d);
}

template <class Tout, class Tin>
constexpr Tout *any_cast(Tin *p){
	void *vp = p;
	return static_cast<Tout *>(vp);
}

#endif // CASTS_H
