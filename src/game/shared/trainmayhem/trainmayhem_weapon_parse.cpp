#include "cbase.h"
#include <KeyValues.h>
#include "trainmayhem_weapon_parse.h"
#include "ammodef.h"

FileWeaponInfo_t* CreateWeaponInfo()
{
	return new CTrainMayhemSWeaponInfo;
}

CTrainMayhemSWeaponInfo::CTrainMayhemSWeaponInfo()
{
	m_iPlayerDamage = 0;
}


void CTrainMayhemSWeaponInfo::Parse(KeyValues *pKeyValuesData, const char *szWeaponName)
{
	BaseClass::Parse(pKeyValuesData, szWeaponName);

	m_iPlayerDamage = pKeyValuesData->GetInt("damage", 0);
}


