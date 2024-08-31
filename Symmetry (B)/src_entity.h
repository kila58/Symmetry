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

class SrcEntity
{
public:
	int GetHealth()
	{
		return *(int*)(this + 0x90); //m_iHealth
	}

	int GetMaxHealth()
	{
		return *(int*)(this + 0x94); //m_iMaxHealth
	}

	int GetTeamNumber()
	{
		return *(int*)(this + 0x9C); //m_iTeamNum
	}

	Vector EyePosition()
	{
		return GetAbsOrigin() + *(Vector*)((DWORD)this + 0xE8); //m_vecViewOffset
	}

	color32 GetRenderColor()
	{
		return *(color32*)(this + 0x58); //m_clrRender
	}

	virtual							~SrcEntity(); // 0
	virtual void					pad01();
	virtual void					pad02();
	virtual void					pad03();
	virtual void					pad04();
	virtual void					pad05();
	virtual void					pad06();
	virtual void					pad07();
	virtual void					pad08();
	virtual void					pad09();
	virtual Vector					GetAbsOrigin();
	virtual QAngle					GetAbsAngles();
	virtual void					pad12();
	virtual void					pad13();
	virtual void					pad14();
	virtual void					pad15();
	virtual ClientClass*			GetClientClass();
	virtual void					pad17();

	// C_BaseEntity starts here
	virtual void					FireBullets(const FireBulletsInfo_t &info);
	virtual void					ModifyFireBulletsDamage(CTakeDamageInfo* dmgInfo) {}
	virtual bool					ShouldDrawUnderwaterBulletBubbles();
	virtual bool					ShouldDrawWaterImpacts(void) { return true; }
	virtual bool					HandleShotImpactingWater(const FireBulletsInfo_t &info, const Vector &vecEnd, ITraceFilter *pTraceFilter, Vector *pVecTracerDest);
	virtual ITraceFilter*			GetBeamTraceFilter(void);
	virtual void					DispatchTraceAttack(const CTakeDamageInfo &info, const Vector &vecDir, trace_t *ptr, CDmgAccumulator *pAccumulator = NULL);
	virtual void					TraceAttack(const CTakeDamageInfo &info, const Vector &vecDir, trace_t *ptr, CDmgAccumulator *pAccumulator = NULL);
	virtual void					DoImpactEffect(trace_t &tr, int nDamageType);
	virtual void					MakeTracer(const Vector &vecTracerSrc, const trace_t &tr, int iTracerType);
	virtual int						GetTracerAttachment(void);
	virtual int						BloodColor();
	virtual const char*				GetTracerType();
	virtual void					Spawn(void);
	virtual void					SpawnClientEntity(void);
	virtual void					Precache(void);
	virtual void					Activate();
	virtual void					ParseMapData(CEntityMapData *mapData);
	virtual bool					KeyValue(const char *szKeyName, const char *szValue);
	virtual bool					KeyValue(const char *szKeyName, float flValue);
	virtual bool					KeyValue(const char *szKeyName, const Vector &vecValue);
	virtual bool					GetKeyValue(const char *szKeyName, char *szValue, int iMaxLen);
	virtual bool					Init(int entnum, int iSerialNum);
	virtual IClientUnknown*			GetIClientUnknown()	{}
	virtual C_BaseAnimating*		GetBaseAnimating() { return NULL; }
	virtual void					SetClassname(const char *className);
	virtual void					RecordToolMessage();
	virtual const Vector&			GetRenderOrigin(void);
	virtual const QAngle&			GetRenderAngles(void);
	virtual Vector					GetObserverCamOrigin(void) { return GetRenderOrigin(); }	// Return the origin for player observers tracking this target
	virtual const matrix3x4_t &		RenderableToWorldTransform();
	virtual bool					IsTransparent(void);
	virtual bool					IsTwoPass(void);
	virtual bool					UsesPowerOfTwoFrameBufferTexture();
	virtual bool					UsesFullFrameBufferTexture();
	virtual bool					IgnoresZBuffer(void) const;
	virtual const model_t			*GetModel(void) const;
	virtual int						DrawModel(int flags);
	virtual void					ComputeFxBlend(void);
	virtual int						GetFxBlend(void);
	virtual bool					LODTest() { return true; }   // NOTE: UNUSED
	virtual void					GetRenderBounds(Vector& mins, Vector& maxs);
	virtual IPVSNotify*				GetPVSNotifyInterface();
	virtual void					GetRenderBoundsWorldspace(Vector& absMins, Vector& absMaxs);
	virtual void					GetShadowRenderBounds(Vector &mins, Vector &maxs, ShadowType_t shadowType);
	virtual void					GetColorModulation(float* color);
	virtual void					OnThreadedDrawSetup() {}
	virtual bool					TestCollision(const Ray_t &ray, unsigned int fContentsMask, trace_t& tr);
	virtual bool					TestHitboxes(const Ray_t &ray, unsigned int fContentsMask, trace_t& tr);
	virtual float					GetAttackDamageScale(void);
	virtual void					NotifyShouldTransmit(ShouldTransmitState_t state);
	virtual void					PreDataUpdate(DataUpdateType_t updateType);
	virtual void					PostDataUpdate(DataUpdateType_t updateType);
	virtual void					ValidateModelIndex(void);
	virtual void					SetDormant(bool bDormant);
	virtual bool					IsDormant(void);
	virtual void					SetDestroyedOnRecreateEntities(void);
	virtual int						GetEFlags() const;
	virtual void					SetEFlags(int iEFlags);
	virtual int						entindex(void) const;
	virtual void					ReceiveMessage(int classID, bf_read &msg);
	virtual void*					GetDataTableBasePtr();
	virtual void					ClientThink();
	virtual ClientThinkHandle_t		GetThinkHandle();
	virtual void					SetThinkHandle(ClientThinkHandle_t hThink);
	virtual bool					ShouldSavePhysics();
	virtual void					OnSave();
	virtual void					OnRestore();
	virtual int						ObjectCaps(void);
	virtual int						Save(ISave &save);
	virtual int						Restore(IRestore &restore);
	virtual bool					CreateVPhysics();
	virtual void					VPhysicsDestroyObject(void);
	virtual void					GMOD_VPhysicsTest(IPhysicsObject *);
	virtual void					VPhysicsUpdate(IPhysicsObject *pPhysics);
	virtual int						VPhysicsGetObjectList(IPhysicsObject **pList, int listMax);
	virtual bool					VPhysicsIsFlesh(void);
	virtual bool					SetupBones(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime);
};