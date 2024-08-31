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
#include "generic.h"

// Copy of source's QAngle, except without shit that I don't need.
class QAngle
{
public:
	// No hungarian notation because it's way easier for objects like these
	float p, y, r;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	QAngle::QAngle()
	///
	/// \brief	Default constructor.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	QAngle()
	{
		this->p = 0;
		this->y = 0;
		this->r = 0;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	QAngle::QAngle(float p, float y, float r)
	///
	/// \brief	Constructor.
	///
	/// \param	p	Pitch.
	/// \param	y	Yaw.
	/// \param	r	Roll.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	QAngle(float p, float y, float r)
	{
		this->p = p;
		this->y = y;
		this->r = y;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	bool QAngle::operator==(const QAngle& v) const
	///
	/// \brief	Equality operator.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool operator==(const QAngle& v) const
	{
		return v.p == this->p && v.y == this->y && v.r == this->r;
	}
};