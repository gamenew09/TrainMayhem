//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef TRAINMAYHEM_PLAYERLOCALDATA_H
#define TRAINMAYHEM_PLAYERLOCALDATA_H
#ifdef _WIN32
#pragma once
#endif

#include "networkvar.h"

#include "trainmayhem_movedata.h"

//-----------------------------------------------------------------------------
// Purpose: Player specific data for HL2 ( sent only to local player, too )
//-----------------------------------------------------------------------------
class CTrainMayhemPlayerLocalData
{
public:
	// Save/restore
	DECLARE_SIMPLE_DATADESC();
	DECLARE_CLASS_NOBASE(CTrainMayhemPlayerLocalData);
	DECLARE_EMBEDDED_NETWORKVAR();

	CTrainMayhemPlayerLocalData();

	CNetworkVar(float, m_flSuitPower);
	CNetworkVar(bool, m_bZooming);
	CNetworkVar(int, m_bitsActiveDevices);
	CNetworkVar(int, m_iSquadMemberCount);
	CNetworkVar(int, m_iSquadMedicCount);
	CNetworkVar(bool, m_fSquadInFollowMode);
	CNetworkVar(bool, m_bWeaponLowered);
	CNetworkVar(EHANDLE, m_hAutoAimTarget);
	CNetworkVar(Vector, m_vecAutoAimPoint);
	CNetworkVar(bool, m_bDisplayReticle);
	CNetworkVar(bool, m_bStickyAutoAim);
	CNetworkVar(bool, m_bAutoAimTarget);

	// Ladder related data
	CNetworkVar(EHANDLE, m_hLadder);
	LadderMove_t			m_LadderMove;
};

EXTERN_SEND_TABLE(DT_TMLocal);


#endif // HL2_PLAYERLOCALDATA_H
