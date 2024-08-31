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
#include "src_vector.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \struct	SrcTraceSettings
///
/// \brief	Source ray trace settings.
////////////////////////////////////////////////////////////////////////////////////////////////////

struct SrcTraceSettings
{
	VectorAligned m_vStart;
	VectorAligned m_vDelta;
	VectorAligned m_vExtends;

	bool m_bIsRay;
	bool m_bIsSwept;

	// Methods we don't need
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \struct	SrcPlane
///
/// \brief	Source plane structure.
////////////////////////////////////////////////////////////////////////////////////////////////////

struct SrcPlane
{
	Vector	m_vNormal;
	float	m_fDist;
	uchar	m_cType;
	uchar	m_cSignBits;
	uchar	pad[2];
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \struct	SrcSurface
///
/// \brief	Source surface structure.
////////////////////////////////////////////////////////////////////////////////////////////////////

struct SrcSurface
{
	const char*		m_cName;
	short			m_sName;
	uint16			m_iFlags;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class	SrcTraceBase
///
/// \brief	Base source trace results/settings.
////////////////////////////////////////////////////////////////////////////////////////////////////

class SrcTraceBase
{
public:

	// Methods we don't need
	
public:

	Vector		m_vStartPos;
	Vector		m_vEndPos;
	
	// DOTHIS
	SrcPlane*	m_srcPlane;

	float		m_fFraction;

	int			m_iContents;
	uint32		m_iDispFlags;

	bool		m_bAllSolid;
	bool		m_bStartSolid;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class	SrcTrace
///
/// \brief	Default source trace results/settings.
////////////////////////////////////////////////////////////////////////////////////////////////////

class SrcTrace : public SrcTraceBase
{
public:

	// Methods we could do with
	bool DidHitWorld() const;
	bool DidHitNonWorldEntity() const;
	int	 GetEntityIndex() const;
	bool DidHit();

public:
	
	float		m_fFractionLeftSolid;
	SrcSurface*	m_srcSurface;

	int			m_iHitGroup;
	short		m_sPhysicsBone;

	void*		m_ent;

	int			m_iHitBox;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \enum	SrcTraceType
///
/// \brief	Different source trace types
////////////////////////////////////////////////////////////////////////////////////////////////////

enum SrcTraceType
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY = 1,
	TRACE_ENTITIES_ONLY = 2,
	TRACE_EVERYTHING_FILTER_PROPS = 3
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class	SrcTraceFilter
///
/// \brief	Source trace filter.
////////////////////////////////////////////////////////////////////////////////////////////////////

class SrcTraceFilter
{
public:
	virtual bool			ShouldHitEntity(void* ent, int contentMask) = 0;
	virtual SrcTraceType	GetTraceType() const = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class	SrcTraceFilterDefault
///
/// \brief	Default source trace filter.
////////////////////////////////////////////////////////////////////////////////////////////////////

class SrcTraceFilterDefault : public SrcTraceFilter
{
public:
	virtual SrcTraceType GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class	SrcTraceFilterEntitiesOnly
///
/// \brief	Source trace filter, filters everything but entities.
////////////////////////////////////////////////////////////////////////////////////////////////////

class SrcTraceFilterEntitiesOnly : public SrcTraceFilter
{
public:

	virtual SrcTraceType GetTraceType() const
	{
		return TRACE_ENTITIES_ONLY;
	}

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class	SrcTraceFilterWorldOnly
///
/// \brief	Source trace filter, filters everything but the world.
////////////////////////////////////////////////////////////////////////////////////////////////////

class SrcTraceFilterWorldOnly : public SrcTraceFilter
{
public:

	bool ShouldHitEntity(void *pEnt, int iContentMask)
	{
		return false;
	}

	virtual SrcTraceType GetTraceType() const
	{
		return TRACE_WORLD_ONLY;
	}

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class	SrcTraceFilterHitAll
///
/// \brief	Source trace filter, doesn't filter anything.
////////////////////////////////////////////////////////////////////////////////////////////////////

class SrcTraceFilterHitAll : public SrcTraceFilterDefault
{
public:
	virtual bool ShouldHitEntity(void *pEnt, int iContentMask)
	{
		return true;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class	SrcEngineTrace
///
/// \brief	Source engine trace.
////////////////////////////////////////////////////////////////////////////////////////////////////

class SrcEngineTrace
{
public:
	// Alignment
	virtual void GetPointContents();
	virtual void GetPointContents_Collideable();
	virtual void ClipRayToEntity();
	virtual void ClipRayToCollideable();

	// Used
	virtual void TraceRay(const SrcTraceSettings &ray, uint32 iMask, SrcTraceFilter* tf, SrcTrace* srcTrace);

	// Alignment
	virtual void SetupLeafAndEntityListRay();
	virtual void SetupLeafAndEntityListBox();
	virtual void TraceRayAgainstLeafAndEntityList();
	virtual void SweepCollideable();
	virtual void EnumerateEntities();
	virtual void EnumerateEntities2(); // diff args
	virtual void GetCollideable();
	virtual void GetStatByIndex();
	virtual void GetBrushesInAABB();
	virtual void GetCollidableFromDisplacementsInAABB();
	virtual void GetBrushInfo();
	virtual void PointOutsideWorld();
	virtual void GetLeafContainingPoint();
};