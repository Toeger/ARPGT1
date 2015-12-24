#ifndef UTILITY_H
#define UTILITY_H

#include <array>
#include <math.h>
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
		{
		}
		Vector(const Vector &) = default;

		//compatibility with sf::Vector2f
		Vector(const sf::Vector2f &v) :
			x(v.x),
			y(v.y)
		{
		}
		 operator sf::Vector2f(){
			return {x, y};
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

	//A Direction defines the direction an object is oriented towards
	//A direction of (x=1, y=0) means the object is rotated by 0 degrees
	//A direction of (x=1, y=1) means the object is rotated by 45 degrees counter clockwise
	struct Direction{
		Direction(float x = 0, float y = 0) :
			x(x),
			y(y)
		{
		}
		void normalize(){
			auto sq = x * x + y * y;
			if (sq < std::numeric_limits<float>::epsilon()){
				x = 1;
				y = 0;
			}
			float q = sqrt(sq);
			x /= q;
			y /= q;
		}

		float x, y;
	};

	//a 3x2 matrix that can move and rotate objects (TODO: scale?)
	struct Transformator{
		/* Matrix indexes:
		 * 012
		 * 345
		 * 678
		*/
		void clear(){
			//set to identity matrix
			data[0] = 1;
			data[1] = 0;
			data[2] = 0;
			data[3] = 0;
			data[4] = 1;
			data[5] = 0;
		}
		//make the transformation move the object by given vector from 0/0
		void set_translation(const Vector &v){
			data[2] = v.x;
			data[5] = v.y;
		}
		void add_translation(const Vector &v){
			data[2] += v.x;
			data[5] += v.y;
		}
		void clear_translation(){
			data[2] = 0;
			data[5] = 0;
		}
		Vector get_translation() const{
			return {data[2], data[5]};
		}
		//make the transfromation rotate the object by angle given by direction
		void set_rotation(Direction d){
			d.normalize();
			//float phi = atan2(d.y, d.x);
			//data[4] = data[0] = cos(phi);
			//data[3] = sin(phi);
			//data[1] = -data[3]; //-sin(phi);
			data[4] = data[0] = d.x;
			data[1] = -d.y;
			data[3] = d.y;
		}
		//constructors and operators
		Transformator(const Transformator &other) :
			data(other.data){
		}
		Transformator(){
			clear();
		}
		Transformator &operator =(const Transformator &other){
			std::copy(begin(other.data), end(other.data), begin(data));
			return *this;
		}
		Transformator &assign(const Transformator &other){ //to assign through inheritance
			return *this = other;
		}
		Transformator &operator *=(const Transformator &other);
		Transformator(std::array<float, 6> &&data) :
			data(std::move(data)){
		}
		//static convinience constructors
		static Transformator get_translation_matrix(const Vector &v){
			return std::array<float, 6>{1, 0, v.x, 0, 1, v.y};
		}
		static Transformator get_rotation_matrix(Direction d){
			d.normalize();
			return std::array<float, 6>{d.x, -d.y, 0, d.y, d.x, 0};
		}
		static Transformator get_rotation_matrix(Direction d, const Vector &rotpoint);

		//data
		std::array<float, 6> data;
	};

	//mathmatical operations
	inline Transformator operator *(const Transformator &lhs, const Transformator &rhs){
		return Transformator({
								 lhs.data[0] * rhs.data[0] + lhs.data[1] * rhs.data[3],
								 lhs.data[0] * rhs.data[1] + lhs.data[1] * rhs.data[4],
								 lhs.data[0] * rhs.data[2] + lhs.data[1] * rhs.data[5] + lhs.data[2],
								 lhs.data[3] * rhs.data[0] + lhs.data[4] * rhs.data[3],
								 lhs.data[3] * rhs.data[1] + lhs.data[4] * rhs.data[4],
								 lhs.data[3] * rhs.data[2] + lhs.data[4] * rhs.data[5] + lhs.data[5],
							 });
	}
	inline Vector operator *(const Transformator &lhs, const Vector &rhs){
		return {lhs.data[0] * rhs.x + lhs.data[1] * rhs.y + lhs.data[2], lhs.data[3] * rhs.x + lhs.data[4] * rhs.y + lhs.data[5]};
	}

	inline Transformator &Transformator::operator *=(const Transformator &other)
	{
		//TODO: this can probably be optimized
		return *this = *this * other;
	}

	//printing
	inline std::ostream &operator << (std::ostream & os, const Transformator &t){
		return os << t.data[0] << ',' << t.data[1] << ',' << t.data[2] << '\n'
				  << t.data[3] << ',' << t.data[4] << ',' << t.data[5] << '\n';
	}

	//implementations for Transformator function:
	inline Transformator Transformator::get_rotation_matrix(Direction d, const Vector &r){
		d.normalize();
		return std::array<float, 6>{d.x, -d.y, r.x*(1 - d.x) + d.y * r.y, d.y, d.x, r.y * (1 - d.x) - d.y * r.x};
	}
}

#endif // UTILITY_H
