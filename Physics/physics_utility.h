#ifndef UTILITY_H
#define UTILITY_H

#include "Utility/casts.h"

#include <array>
#include <cmath>
#include <limits>
#include <ostream>

constexpr const auto pi = precision_cast(M_PI);

namespace Physical {
	//A Vector defines a position through its x and y coordinate
	//has nothing to do with std::vector
	struct Vector {
		constexpr Vector(float x = 0, float y = 0)
			: x(x)
			, y(y) {}
		constexpr Vector(const Vector &) = default;
		constexpr float length() const {
			return std::sqrt(x * x + y * y);
		}
		constexpr Vector &operator*=(float scale) {
			x *= scale;
			y *= scale;
			return *this;
		}
		constexpr Vector &operator/=(float scale) {
			x /= scale;
			y /= scale;
			return *this;
		}

		//data
		float x, y;
	};
	inline std::ostream &operator<<(std::ostream &os, const Vector &v) {
		return os << v.x << '\n' << v.y << '\n';
	}
	inline Vector operator-(const Vector &v) {
		return {-v.x, -v.y};
	}
	inline Vector operator-(const Vector &lhs, const Vector &rhs) {
		return {lhs.x - rhs.x, lhs.y - rhs.y};
	}
	inline Vector operator*(Vector lhs, float rhs) {
		return lhs *= rhs;
	}
	inline Vector operator/(Vector lhs, float rhs) {
		return lhs /= rhs;
	}

	//A Direction defines the direction an object is oriented towards
	//A direction of (x=1, y=0) means the object is rotated by 0 degrees
	//A direction of (x=sqrt(0.5), y=sqrt(0.5)) means the object is rotated by 45 degrees counter clockwise
	//Direction automatically normalizes its values
	struct Direction {
		constexpr Direction()
			: x(1)
			, y(0) {}
		constexpr Direction(float x, float y)
			: x(x)
			, y(y) {
			normalize();
		}
		constexpr float to_radians() const {
			return std::atan2(y, x);
		}
		constexpr float to_degrees() const {
			return std::atan2(y, x) * 180 / pi;
		}
		constexpr float get_x() const {
			return x;
		}
		constexpr float get_y() const {
			return y;
		}
		constexpr Direction &operator+=(const Direction &other) {
			auto newx = x * other.x - y * other.y;
			y = y * other.x + x * other.y;
			x = newx;
			fast_normalize();
			return *this;
		}
		constexpr Direction &operator-=(const Direction &other) {
			auto newx = x * other.x + y * other.y;
			y = y * other.x - x * other.y;
			x = newx;
			fast_normalize();
			return *this;
		}
		constexpr Direction operator-() const {
			return {x, -y};
		}
		static constexpr Direction from_radians(float radians) {
			return {std::cos(radians), std::sin(radians)};
		}
		static constexpr Direction from_degrees(float degrees) {
			return from_radians(degrees * pi / 180);
		}
		constexpr Direction(const Direction &) = default;
		Direction &operator=(const Direction &) = default;

		private:
		constexpr void normalize() {
			auto sq = x * x + y * y;
			if (sq < std::numeric_limits<float>::epsilon()) {
				x = 1;
				y = 0;
				return;
			}
			float q = std::sqrt(sq);
			x /= q;
			y /= q;
		}
		constexpr void fast_normalize() {
			//assumes we don't have length 0
			auto sq = x * x + y * y;
			float q = std::sqrt(sq);
			x /= q;
			y /= q;
		}

		float x, y;
	};
	constexpr inline Direction operator+(Direction lhs, const Direction &rhs) {
		return lhs += rhs;
	}
	constexpr inline Direction operator-(Direction lhs, const Direction &rhs) {
		return lhs -= rhs;
	}

	//a Transformator contains a position vector and a direction vector
	//applying a Transformator will translate by the position vector and then turn it by the direction vector
	struct Transformator {
		Vector vector;
		Direction direction;
		constexpr Transformator() {}
		constexpr Transformator(const Direction &direction)
			: direction(direction) {}
		constexpr Transformator(const Vector &vector)
			: vector(vector) {}
		constexpr Transformator(const Vector &vector, const Direction &direction)
			: vector(vector)
			, direction(direction) {}
		//operators
		constexpr Transformator &operator+=(const Vector &offset) {
			vector.x += direction.get_x() * offset.x - direction.get_y() * offset.y;
			vector.y += direction.get_y() * offset.x + direction.get_x() * offset.y;
			return *this;
		}
		constexpr Transformator &operator-=(const Vector &offset) {
			return *this += -offset;
		}
		constexpr Transformator &operator+=(const Direction &dir) {
			direction += dir;
			return *this;
		}
		constexpr Transformator &operator-=(const Direction &dir) {
			direction -= dir;
			return *this;
		}
		constexpr Transformator &operator+=(const Transformator &other) {
			*this += other.vector; //order of evaluation matters
			direction += other.direction;
			return *this;
		}
		Transformator &operator=(const Transformator &other) = default;
		//inverting transformations
		constexpr Transformator operator-() const {
			Transformator t;
			t -= direction;
			t -= vector;
			return t;
		}
	};
	//Transformator operators
	inline constexpr Transformator operator+(Transformator lhs, const Transformator &rhs) {
		return lhs += rhs;
	}
	inline constexpr Transformator operator+(Transformator lhs, const Vector &rhs) {
		return lhs += rhs;
	}
	inline constexpr Transformator operator+(Transformator lhs, const Direction &rhs) {
		return lhs += rhs;
	}
	inline constexpr Transformator operator-(const Transformator &lhs, const Transformator &rhs) {
		return lhs + -rhs;
	}
	inline std::ostream &operator<<(std::ostream &os, const Transformator &t) {
		return os << '(' << t.vector.x << ',' << t.vector.y << ')' << "->" << t.direction.to_degrees();
	}
}

#endif // UTILITY_H
