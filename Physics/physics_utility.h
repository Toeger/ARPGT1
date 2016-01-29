#ifndef UTILITY_H
#define UTILITY_H

#include <array>
#include <cmath>
#include <limits>
#include <SFML/System/Vector2.hpp>
#include <ostream>

namespace Physical {
	//A Vector defines a position through its x and y coordinate
	//has nothing to do with std::vector
	struct Vector{
		Vector(float x = 0, float y = 0) :
			x(x),
			y(y)
		{}
		Vector(const Vector &) = default;

		//compatibility with sf::Vector2f
		Vector(const sf::Vector2f &v) :
			x(v.x),
			y(v.y)
		{}
		operator sf::Vector2f(){
			return {x, y};
		}
		float length() const{
			return std::sqrt(x*x + y*y);
		}
		Vector &operator *= (float scale){
			x *= scale;
			y *= scale;
			return *this;
		}

		//data
		float x, y;
	};
	inline std::ostream &operator << (std::ostream &os, const Vector &v){
		return os << v.x << '\n' << v.y << '\n';
	}
	inline Vector operator -(const Vector &v){
		return {-v.x, -v.y};
	}
	inline Vector operator -(const Vector &lhs, const Vector &rhs){
		return {lhs.x - rhs.x, lhs.y - rhs.y};
	}

	//A Direction defines the direction an object is oriented towards
	//A direction of (x=1, y=0) means the object is rotated by 0 degrees
	//A direction of (x=sqrt(0.5), y=sqrt(0.5)) means the object is rotated by 45 degrees counter clockwise
	//Direction automatically normalizes its values
	struct Direction{
		Direction() :
			x(1),
			y(0)
		{}
		Direction(float x, float y) :
			x(x),
			y(y)
		{
			normalize();
		}
		float to_radians() const{
			return std::atan2(y, x);
		}
		float to_degrees() const{
			return std::atan2(y, x) * 180 / M_PI;
		}
		float get_x() const{
			return x;
		}
		float get_y() const{
			return y;
		}
		Direction &operator +=(const Direction &other){
			auto newx = x * other.x - y * other.y;
			y = y * other.x + x * other.y;
			x = newx;
			normalize();
			return *this;
		}
		Direction &operator -=(const Direction &other){
			auto newx = x * other.x + y * other.y;
			y = y * other.x - x * other.y;
			x = newx;
			normalize();
			return *this;
		}
		Direction operator -() const{
			return {x, -y};
		}
		static Direction from_radians(float radians){
			return {std::cos(radians), std::sin(radians)};
		}
		static Direction from_degrees(float degrees){
			return from_radians(degrees * M_PI / 180);
		}
		Direction(const Direction &) = default;
		Direction &operator =(const Direction &) = default;
	private:
		void normalize(){
			auto sq = x * x + y * y;
			if (sq < std::numeric_limits<float>::epsilon()){
				x = 1;
				y = 0;
				return;
			}
			float q = std::sqrt(sq);
			x /= q;
			y /= q;
		}
		float x, y;
	};
	inline Direction operator +(Direction lhs, const Direction &rhs){
		return lhs += rhs;
	}
	inline Direction operator -(Direction lhs, const Direction &rhs){
		return lhs -= rhs;
	}

	//a Transformator contains a position vector and a direction vector
	//applying a Transformator will translate by the position vector and then turn it by the direction vector
	struct Transformator{
		Vector vector;
		Direction direction;
		Transformator()
		{}
		Transformator(const Direction &direction) :
			direction(direction)
		{}
		Transformator(const Vector &vector) :
			vector(vector)
		{}
		Transformator(const Vector &vector, const Direction &direction) :
			vector(vector),
			direction(direction)
		{}
		//operators
		Transformator &operator += (const Vector &offset){
			vector.x += direction.get_x() * offset.x - direction.get_y() * offset.y;
			vector.y += direction.get_y() * offset.x + direction.get_x() * offset.y;
			return *this;
		}
		Transformator &operator -= (const Vector &offset){
			return *this += -offset;
		}
		Transformator &operator += (const Direction &dir){
			direction += dir;
			return *this;
		}
		Transformator &operator -= (const Direction &dir){
			direction -= dir;
			return *this;
		}
		Transformator &operator += (const Transformator &other){
			*this += other.vector; //order of evaluation matters
			direction += other.direction;
			return *this;
		}
		Transformator &operator = (const Transformator &other) = default;
		//inverting transformations
		Transformator operator -() const{
			Transformator t;
			t -= direction;
			t -= vector;
			return t;
		}
	};
	//Transformator operators
	inline Transformator operator +(Transformator lhs, const Transformator &rhs){
		return lhs += rhs;
	}
	inline Transformator operator +(Transformator lhs, const Vector &rhs){
		return lhs += rhs;
	}
	inline Transformator operator +(Transformator lhs, const Direction &rhs){
		return lhs += rhs;
	}
	inline Transformator operator -(const Transformator &lhs, const Transformator &rhs){
		return lhs + -rhs;
	}
}

#endif // UTILITY_H
