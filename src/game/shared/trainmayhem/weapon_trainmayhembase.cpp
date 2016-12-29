//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "in_buttons.h"
#include "takedamageinfo.h"
#include "ammodef.h"
#include "trainmayhem_gamerules.h"


#ifdef CLIENT_DLL
extern IVModelInfoClient* modelinfo;
#else
extern IVModelInfo* modelinfo;
#endif


#if defined( CLIENT_DLL )

#include "vgui/ISurface.h"
#include "vgui_controls/Controls.h"
#include "c_trainmayhem_player.h"
#include "hud_crosshair.h"

#else

#include "trainmayhem_player.h"
#include "vphysics/constraints.h"

#endif

#include "weapon_trainmayhembase.h"


// ----------------------------------------------------------------------------- //
// Global functions.
// ----------------------------------------------------------------------------- //

bool IsAmmoType(int iAmmoType, const char *pAmmoName)
{
	return GetAmmoDef()->Index(pAmmoName) == iAmmoType;
}

static const char * s_WeaponAliasInfo[] =
{
	"none",	//	WEAPON_NONE = 0,

	//Melee
	"shotgun",	//WEAPON_AMERKNIFE,

	NULL,		// end of list marker
};


// ----------------------------------------------------------------------------- //
// CWeaponTrainMayhemBase tables.
// ----------------------------------------------------------------------------- //

IMPLEMENT_NETWORKCLASS_ALIASED(WeaponTrainMayhemBase, DT_WeaponTrainMayhemBase)

BEGIN_NETWORK_TABLE(CWeaponTrainMayhemBase, DT_WeaponTrainMayhemBase)

#ifdef CLIENT_DLL

#else
// world weapon models have no aminations
//	SendPropExclude( "DT_AnimTimeMustBeFirst", "m_flAnimTime" ),
//	SendPropExclude( "DT_BaseAnimating", "m_nSequence" ),
//	SendPropExclude( "DT_LocalActiveWeaponData", "m_flTimeWeaponIdle" ),
#endif

END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA(CWeaponTrainMayhemBase)
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS(weapon_hl2mp_base, CWeaponTrainMayhemBase);


#ifdef GAME_DLL

BEGIN_DATADESC(CWeaponTrainMayhemBase)

END_DATADESC()

#endif

// ----------------------------------------------------------------------------- //
// CWeaponTrainMayhemBase implementation. 
// ----------------------------------------------------------------------------- //
CWeaponTrainMayhemBase::CWeaponTrainMayhemBase()
{
	SetPredictionEligible(true);
	AddSolidFlags(FSOLID_TRIGGER); // Nothing collides with these but it gets touches.

	m_flNextResetCheckTime = 0.0f;
}


bool CWeaponTrainMayhemBase::IsPredicted() const
{
	return true;
}

void CWeaponTrainMayhemBase::WeaponSound(WeaponSound_t sound_type, float soundtime /* = 0.0f */)
{
#ifdef CLIENT_DLL

	// If we have some sounds from the weapon classname.txt file, play a random one of them
	const char *shootsound = GetWpnData().aShootSounds[sound_type];
	if (!shootsound || !shootsound[0])
		return;

	CBroadcastRecipientFilter filter; // this is client side only
	if (!te->CanPredict())
		return;

	CBaseEntity::EmitSound(filter, GetPlayerOwner()->entindex(), shootsound, &GetPlayerOwner()->GetAbsOrigin());
#else
	BaseClass::WeaponSound(sound_type, soundtime);
#endif
}


CBasePlayer* CWeaponTrainMayhemBase::GetPlayerOwner() const
{
	return dynamic_cast< CBasePlayer* >(GetOwner());
}

CTrainMayhemPlayer* CWeaponTrainMayhemBase::GetTrainMayhemPlayerOwner() const
{
	return dynamic_cast< CTrainMayhemPlayer* >(GetOwner());
}

#ifdef CLIENT_DLL

void CWeaponTrainMayhemBase::OnDataChanged(DataUpdateType_t type)
{
	BaseClass::OnDataChanged(type);

	if (GetPredictable() && !ShouldPredict())
		ShutdownPredictable();
}


bool CWeaponTrainMayhemBase::ShouldPredict()
{
	if (GetOwner() && GetOwner() == C_BasePlayer::GetLocalPlayer())
		return true;

	return BaseClass::ShouldPredict();
}


#else

void CWeaponTrainMayhemBase::Spawn()
{
	BaseClass::Spawn();

	// Set this here to allow players to shoot dropped weapons
	SetCollisionGroup(COLLISION_GROUP_WEAPON);
}

void CWeaponTrainMayhemBase::Materialize(void)
{
	if (IsEffectActive(EF_NODRAW))
	{
		// changing from invisible state to visible.
		EmitSound("AlyxEmp.Charge");

		RemoveEffects(EF_NODRAW);
		DoMuzzleFlash();
	}

	if (HasSpawnFlags(SF_NORESPAWN) == false)
	{
		VPhysicsInitNormal(SOLID_BBOX, GetSolidFlags() | FSOLID_TRIGGER, false);
		SetMoveType(MOVETYPE_VPHYSICS);

		TrainMayhemRules()->AddLevelDesignerPlacedObject(this);
	}

	if (HasSpawnFlags(SF_NORESPAWN) == false)
	{
		if (GetOriginalSpawnOrigin() == vec3_origin)
		{
			m_vOriginalSpawnOrigin = GetAbsOrigin();
			m_vOriginalSpawnAngles = GetAbsAngles();
		}
	}

	SetPickupTouch();

	SetThink(NULL);
}

int CWeaponTrainMayhemBase::ObjectCaps()
{
	return BaseClass::ObjectCaps() & ~FCAP_IMPULSE_USE;
}

#endif

void CWeaponTrainMayhemBase::FallInit(void)
{
#ifndef CLIENT_DLL
	SetModel(GetWorldModel());
	VPhysicsDestroyObject();

	if (HasSpawnFlags(SF_NORESPAWN) == false)
	{
		SetMoveType(MOVETYPE_NONE);
		SetSolid(SOLID_BBOX);
		AddSolidFlags(FSOLID_TRIGGER);

		UTIL_DropToFloor(this, MASK_SOLID);
	}
	else
	{
		if (!VPhysicsInitNormal(SOLID_BBOX, GetSolidFlags() | FSOLID_TRIGGER, false))
		{
			SetMoveType(MOVETYPE_NONE);
			SetSolid(SOLID_BBOX);
			AddSolidFlags(FSOLID_TRIGGER);
		}
		else
		{
#if !defined( CLIENT_DLL )
			// Constrained start?
			if (HasSpawnFlags(SF_WEAPON_START_CONSTRAINED))
			{
				//Constrain the weapon in place
				IPhysicsObject *pReferenceObject, *pAttachedObject;

				pReferenceObject = g_PhysWorldObject;
				pAttachedObject = VPhysicsGetObject();

				if (pReferenceObject && pAttachedObject)
				{
					constraint_fixedparams_t fixed;
					fixed.Defaults();
					fixed.InitWithCurrentObjectState(pReferenceObject, pAttachedObject);

					fixed.constraint.forceLimit = lbs2kg(10000);
					fixed.constraint.torqueLimit = lbs2kg(10000);

					IPhysicsConstraint *pConstraint = GetConstraint();

					pConstraint = physenv->CreateFixedConstraint(pReferenceObject, pAttachedObject, NULL, fixed);

					pConstraint->SetGameData((void *) this);
				}
			}
#endif //CLIENT_DLL
		}
	}

	SetPickupTouch();

	SetThink(&CBaseCombatWeapon::FallThink);

	SetNextThink(gpGlobals->curtime + 0.1f);

#endif
}

const CTrainMayhemSWeaponInfo &CWeaponTrainMayhemBase::GetTrainMayhemWpnData() const
{
	const FileWeaponInfo_t *pWeaponInfo = &GetWpnData();
	const CTrainMayhemSWeaponInfo *pHL2MPInfo;

#ifdef _DEBUG
	pHL2MPInfo = dynamic_cast< const CTrainMayhemSWeaponInfo* >(pWeaponInfo);
	Assert(pHL2MPInfo);
#else
	pHL2MPInfo = static_cast< const CTrainMayhemSWeaponInfo* >(pWeaponInfo);
#endif

	return *pHL2MPInfo;
}
void CWeaponTrainMayhemBase::FireBullets(const FireBulletsInfo_t &info)
{
	FireBulletsInfo_t modinfo = info;

	modinfo.m_iPlayerDamage = GetTrainMayhemWpnData().m_iPlayerDamage;

	BaseClass::FireBullets(modinfo);
}


#if defined( CLIENT_DLL )

#include "c_te_effect_dispatch.h"

#define NUM_MUZZLE_FLASH_TYPES 4

bool CWeaponTrainMayhemBase::OnFireEvent(C_BaseViewModel *pViewModel, const Vector& origin, const QAngle& angles, int event, const char *options)
{
	return BaseClass::OnFireEvent(pViewModel, origin, angles, event, options);
}


void UTIL_ClipPunchAngleOffset(QAngle &in, const QAngle &punch, const QAngle &clip)
{
	QAngle	final = in + punch;

	//Clip each component
	for (int i = 0; i < 3; i++)
	{
		if (final[i] > clip[i])
		{
			final[i] = clip[i];
		}
		else if (final[i] < -clip[i])
		{
			final[i] = -clip[i];
		}

		//Return the result
		in[i] = final[i] - punch[i];
	}
}

#endif

