/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "src_proxies.h"
#include "netvarutil.h"

#include "global.h"

#include "src_lib.h"

NAMESPACE_CORE(Hooks)

RecvVarProxyFn g_angOldX;
RecvVarProxyFn g_angOldY;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void ProxyEyeAngleX(const CRecvProxyData* pData, SrcEntity* pEntity, float* fpOut)
////////////////////////////////////////////////////////////////////////////////////////////////////

void ProxyEyeAngleX(const CRecvProxyData* pData, SrcEntity* pEntity, float* fpOut)
{
	static SymConVar<short>* aim_aaa = dynamic_cast<SymConVar<short>*>(Class::Console->FindEntry("aim_aaa"));
	
	float value = pData->m_Value.m_Float;

	if (aim_aaa && pEntity != SrcLib::GetLocalPlayer())
	{
		if (aim_aaa->GetValue() == 1)
		{
			if (value >= 90.0f)
			{
				// Common value
				value = 181.0f;
			}
			else if (value <= -90.0f)
			{
				// Common value
				value = -181.0f;
			}
		}
	}

	*fpOut = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void ProxyEyeAngleY(const CRecvProxyData* pData, SrcEntity* pEntity, float* fpOut)
////////////////////////////////////////////////////////////////////////////////////////////////////

void ProxyEyeAngleY(const CRecvProxyData* pData, SrcEntity* pEntity, float* fpOut)
{
	float value = pData->m_Value.m_Float;
	// Nothing here yet
	*fpOut = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SrcProxies::Hook()
////////////////////////////////////////////////////////////////////////////////////////////////////

void SrcProxies::Hook()
{
	RecvTable* table = NetVarUtil::FindTable("DT_HL2MPNonLocalPlayerExclusive");

	if (table)
	{
		RecvProp* ang0 = NetVarUtil::FindProp(table, "m_angEyeAngles[0]");
		RecvProp* ang1 = NetVarUtil::FindProp(table, "m_angEyeAngles[1]");

		if (ang0)
		{
			g_angOldX = ang0->GetProxyFn();
			ang0->SetProxyFn((RecvVarProxyFn)ProxyEyeAngleX);
		}

		if (ang1)
		{
			g_angOldY = ang1->GetProxyFn();
			ang1->SetProxyFn((RecvVarProxyFn)ProxyEyeAngleY);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SrcProxies::UnHook()
////////////////////////////////////////////////////////////////////////////////////////////////////

void SrcProxies::UnHook()
{
	RecvTable* table = NetVarUtil::FindTable("DT_HL2MPNonLocalPlayerExclusive");

	if (table && g_angOldX && g_angOldY)
	{
		RecvProp* angX = NetVarUtil::FindProp(table, "m_angEyeAngles[0]");
		RecvProp* angY = NetVarUtil::FindProp(table, "m_angEyeAngles[1]");

		if (angX)
			angX->SetProxyFn(g_angOldX);

		if (angY)
			angY->SetProxyFn(g_angOldY);
	}
}

NAMESPACE_END2