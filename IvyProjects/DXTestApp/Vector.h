#pragma once

#include <cmath>

#ifndef VECTOR_EPSILON
#define VECTOR_EPSILON 0.001
#endif


/// TVector3 template
///
/// A 3-dimentional vector class
///
struct Vector3
{
public:

    Vector3() { X = Y = Z = 1; }

	Vector3(float x, float y, float z)
    {
		X = x;
        Y = y;
        Z = z;
    }

	Vector3(const Vector3& orig){ *this = orig; }

	~Vector3(){};

    /// Assigns one vector to another of the same type
	Vector3& operator=(const Vector3& rhs);
	/// Sums vectors of the same type, returning the sum
	Vector3 operator+(const Vector3& rhs);
    /// Sums a vector of the same type and assigns it to the left-hand value
	Vector3& operator+=(const Vector3& rhs);
    /// Subtracts one vector from another of the same type, returning the difference
	Vector3 operator-(const Vector3& rhs);
    /// Subtracts one vector from another of the same type, assigning it to the left-hand value
	Vector3& operator-=(const Vector3& rhs);
    
    /// Compares two vectors. Returns true if they are equal, false otherwise
	bool operator==(const Vector3& rhs);
    
    /// Multiplies the vector's components by the scalar rhs, returning the computed vector
    Vector3 operator*(const float& rhs);
    /// Multiplies the vector's components by the scalar and stores the new values in the vector
	Vector3& operator*=(const float& rhs);
    /// Divides the vector's components by the scalar rhs, returning the computed vector
    Vector3 operator/(const float& rhs);
    /// Divides the vector's components by the scalar and stores the new values in the vector
	Vector3& operator/=(const float& rhs);

    /// Invert the sign on the vector components
	Vector3 operator-();

public: // useful methods

    /// Normalizes the vector
	void Normalize();
    
    /// Calculates and returns the magnitude
	float Magnitude();
    
    /// Calculates the magnitude squared
    /// This is useful for comparing magnitudes w/o the square-root calculation
	float MagnitudeSquared();
        
    /// Clamps vector components to zero if they are within some epsilon
    /// May not be necessary...
    void ClampToZero();

    float X, Y, Z;
};


 Vector3& Vector3::operator=(const Vector3& rhs)
{
	if(this == &rhs){ return *this; }

    memcpy(this, &rhs, sizeof(Vector3));

	return *this;
}

 Vector3 Vector3::operator+(const Vector3& rhs)
{
	Vector3 sum = *this;
	sum += rhs;
	return sum;
}
 Vector3& Vector3::operator+=(const Vector3& rhs)
{
	this->X += rhs.X;
	this->Y += rhs.Y;
	this->Z += rhs.Z;
	return *this;
}

 Vector3 Vector3::operator-(const Vector3& rhs)
{
	Vector3 difference = *this;
	difference -= rhs;
	return difference;
}
 Vector3& Vector3::operator-=(const Vector3& rhs)
{
	this->X -= rhs.X;
	this->Y -= rhs.Y;
	this->Z -= rhs.Z;
	return *this;
}

 Vector3 Vector3::operator*(const float& rhs)
{
    Vector3 product = *this;
	product *= rhs;
    return product;
}
 Vector3& Vector3::operator*=(const float& rhs)
{
	this->X *= rhs;
	this->Y *= rhs;
	this->Z *= rhs;
	return *this;
}

 Vector3 Vector3::operator/(const float& rhs)
{
    Vector3 quotient = *this;
	quotient /= rhs;
    return quotient;
}

Vector3& Vector3::operator/=(const float& rhs)
{
	this->X /= rhs;
	this->Y /= rhs;
	this->Z /= rhs;
	return *this;
}

 bool Vector3::operator==(const Vector3& rhs)
{
	return this->X == rhs.X &&
		   this->Y == rhs.Y &&
		   this->Z == rhs.Z;
}

Vector3 Vector3::operator-()
{
	return Vector3(
		-this->X,
		-this->Y,
		-this->Z
		);
}

void Vector3::Normalize()
{
	float magnitude = this->Magnitude();

	this->X = this->X / magnitude;
	this->Y = this->Y / magnitude;
	this->Z = this->Z / magnitude;

    ClampToZero();
}

float Vector3::Magnitude()
{ 
	return (float) std::sqrt(MagnitudeSquared());
}

float Vector3::MagnitudeSquared()
{
	return (this->X * this->X) + (this->Y * this->Y) + (this->Z * this->Z);
}

void Vector3::ClampToZero()
{
    if(-VECTOR_EPSILON < this->X && this->X < VECTOR_EPSILON)
    {
        this->X = 0;
    }
    if(-VECTOR_EPSILON < this->Y && this->Y < VECTOR_EPSILON)
    {
        this->Y = 0;
    }
    if(-VECTOR_EPSILON < this->Z && this->Z < VECTOR_EPSILON)
    {
        this->Z = 0;
    }
}

float Dot(const Vector3& v1, const Vector3& v2)
{
	float product = v1.X * v2.X +
				    v1.Y * v2.Y +
				    v1.Z * v2.Z;
    
	return product;
}

Vector3 Cross(const Vector3& v1, const Vector3& v2)
{
	Vector3 product;

	// (u.y * v.z - u.z * v.y)i
	product.X = v1.Y * v2.Z - v1.Z * v2.Y;
	// -(u.x * v.z - u.z * v.x)j
	product.Y = -1 * (v1.X * v2.Z - v1.Z * v2.X);
	// (u.x * v.y - u.y * v.x)k
	product.Z = v1.X * v2.Y - v1.Y * v2.X;

    product.ClampToZero();

	return product;
}
