#ifndef UTILITY_H
#define UTILITY_H

#include <array>
#include <math.h>
#include <limits>

namespace Physical {
	//A Vector defines a position through its x and y coordinate
	//has nothing to do with std::vector
	struct Vector{
		Vector(float x = 0, float y = 0) :
			x(x),
			y(y)
		{
		}
		float x, y;
	};

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

	//a 3x3 matrix that can move and rotate objects
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
			data[6] = 0;
			data[7] = 0;
			data[8] = 1;
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
			data[4] = data[0] = d.y;
			data[1] = -d.x;
			data[3] = d.x;
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
		Transformator &operator *=(const Transformator &other);
		Transformator(std::array<float, 9> &&data) :
			data(std::move(data)){
		}

		std::array<float, 9> data;
	};

	inline Transformator operator *(const Transformator &lhs, const Transformator &rhs){
		return Transformator({
								 lhs.data[0] * rhs.data[0] + lhs.data[1] * rhs.data[3] + lhs.data[2] * rhs.data[6],
								 lhs.data[0] * rhs.data[1] + lhs.data[1] * rhs.data[4] + lhs.data[2] * rhs.data[7],
								 lhs.data[0] * rhs.data[2] + lhs.data[1] * rhs.data[5] + lhs.data[2] * rhs.data[8],
								 lhs.data[3] * rhs.data[0] + lhs.data[4] * rhs.data[3] + lhs.data[5] * rhs.data[6],
								 lhs.data[3] * rhs.data[1] + lhs.data[4] * rhs.data[4] + lhs.data[5] * rhs.data[7],
								 lhs.data[3] * rhs.data[2] + lhs.data[4] * rhs.data[5] + lhs.data[5] * rhs.data[8],
								 lhs.data[6] * rhs.data[0] + lhs.data[7] * rhs.data[3] + lhs.data[8] * rhs.data[6],
								 lhs.data[6] * rhs.data[1] + lhs.data[7] * rhs.data[4] + lhs.data[8] * rhs.data[7],
								 lhs.data[6] * rhs.data[2] + lhs.data[7] * rhs.data[5] + lhs.data[8] * rhs.data[8],
							 });
	}
	inline Vector operator *(const Transformator &lhs, const Vector &rhs){
		return {lhs.data[0] * rhs.x + lhs.data[1] * rhs.y + lhs.data[2], lhs.data[3] * rhs.x + lhs.data[4] * rhs.y * lhs.data[5]};
	}

	inline Transformator &Transformator::operator *=(const Transformator &other)
	{
		//TODO: this can probably be optimized
		return *this = *this * other;
	}
}

#endif // UTILITY_H
