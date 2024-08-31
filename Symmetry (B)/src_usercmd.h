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
#include "src_qangle.h"

// Copy of source's CUserCmd class, with same member and function alignment.
class UserCmd
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	UserCmd::UserCmd()
	///
	/// \brief	Default constructor.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	UserCmd()
	{
		this->Reset();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	virtual UserCmd::~CUserCmd()
	///
	/// \brief	Destructor.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual ~UserCmd() { };

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	void UserCmd::Reset()
	///
	/// \brief	Resets this object.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void Reset()
	{
		this->m_iCommandNumber		= 0;
		this->m_iTickCount			= 0;
		this->m_fForwardMove		= 0.0f;
		this->m_fSideMove			= 0.0f;
		this->m_fUpMove				= 0.0f;
		this->m_iButtons			= 0;
		this->m_bImpulse			= 0;
		this->m_iWeaponSelect		= 0;
		this->m_iWeaponSubType		= 0;
		this->m_iRandomSeed			= 0;
		this->m_sMouseX				= 0;
		this->m_sMouseY				= 0;

		this->m_bHasBeenPredicted	= false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	UserCmd& UserCmd::operator=(const UserCmd& cmd)
	///
	/// \brief	Assignment operator.
	///
	/// \param	cmd	The command.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	UserCmd& operator=(const UserCmd& cmd)
	{
		if (this == &cmd)
			return *this;

		// Copy all data across
		memcpy(this, &cmd, sizeof(cmd));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	UserCmd::UserCmd(const UserCmd& cmd)
	///
	/// \brief	Copy constructor.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	UserCmd(const UserCmd& cmd)
	{
		*this = cmd;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	dword UserCmd::GetChecksum() const
	///
	/// \brief	Not implemented.
	///
	/// \return	.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	dword GetChecksum() const
	{
		// TODO: CRC32 checksum with CryptoPP
		return 0;
	}

	// Hungarian notation here
	int		m_iCommandNumber;
	int		m_iTickCount;

	QAngle	m_aViewAngles;

	float	m_fForwardMove;
	float	m_fSideMove;
	float	m_fUpMove;

	int		m_iButtons;

	uchar	m_bImpulse;

	int		m_iWeaponSelect;
	int		m_iWeaponSubType;

	int		m_iRandomSeed;

	short	m_sMouseX;
	short	m_sMouseY;

	// Apparently client only, we're on the client though.
	bool	m_bHasBeenPredicted;
};