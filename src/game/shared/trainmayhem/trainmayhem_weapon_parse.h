//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef TRAINMAYHEM_WEAPON_PARSE_H
#define TRAINMAYHEM_WEAPON_PARSE_H
#ifdef _WIN32
#pragma once
#endif


#include "weapon_parse.h"
#include "networkvar.h"


//--------------------------------------------------------------------------------------------------------
class CTrainMayhemSWeaponInfo : public FileWeaponInfo_t
{
public:
	DECLARE_CLASS_GAMEROOT(CTrainMayhemSWeaponInfo, FileWeaponInfo_t);

	CTrainMayhemSWeaponInfo();

	virtual void Parse(::KeyValues *pKeyValuesData, const char *szWeaponName);


public:

	int m_iPlayerDamage;
};


#endif // HL2MP_WEAPON_PARSE_H
