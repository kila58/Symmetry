/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#pragma once

// Copy of source's Vector class
class Vector
{
public:
	// No hungarian notation because it's way easier for objects like these
	float x, y, z;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	Vector::Vector()
	///
	/// \brief	Default constructor.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	Vector()
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	Vector::Vector(float x, float y, float z)
	///
	/// \brief	Constructor.
	///
	/// \param	x	Pitch.
	/// \param	y	Yaw.
	/// \param	z	Roll.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	Vector(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	bool Vector::operator==(const Vector& v) const
	///
	/// \brief	Equality operator.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool operator==(const Vector& v) const
	{
		return v.z == this->z && v.y == this->y && v.z == this->z;
	}
};

// 16bit aligned vector class
class __declspec(align(16)) VectorAligned : public Vector
{
public:
	// Methods we don't need
	
public:
	// For alignment
	float w;
};