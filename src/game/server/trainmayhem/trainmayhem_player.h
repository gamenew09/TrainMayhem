#ifndef H_TRAINMAYHEM_PLAYER
#define H_TRAINMAYHEM_PLAYER
#pragma once

#include "basemultiplayerplayer.h"
#include "trainmayhem_playerlocaldata.h"
#include "player.h"
#include "simtimer.h"
#include "soundenvelope.h"
#include "trainmayhem_player_shared.h"
#include "trainmayhem_gamerules.h"
#include "utldict.h"

class CTrainMayhemPlayer;

class CTrainMayhemPlayerStateInfo
{
public:
	TrainMayhemPlayerState m_iPlayerState;
	const char *m_pStateName;

	void (CTrainMayhemPlayer::*pfnEnterState)();	// Init and deinit the state.
	void (CTrainMayhemPlayer::*pfnLeaveState)();

	void (CTrainMayhemPlayer::*pfnPreThink)();	// Do a PreThink() in this state.
};

class CTrainMayhemPlayer : public CBasePlayer
{
public:
	DECLARE_CLASS(CTrainMayhemPlayer, CBasePlayer);

	CTrainMayhemPlayer();
	~CTrainMayhemPlayer();

	static CTrainMayhemPlayer *CreatePlayer(const char* className, edict_t *ed)
	{
		CTrainMayhemPlayer::s_PlayerEdict = ed;
		return (CTrainMayhemPlayer*)CreateEntityByName(className);
	}

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	virtual void Precache(void);
	virtual void Spawn(void);
	virtual void PostThink(void);
	virtual void PreThink(void);
	virtual void PlayerDeathThink(void);
	virtual void SetAnimation(PLAYER_ANIM playerAnim);
	virtual bool HandleCommand_JoinTeam(int team);
	virtual bool ClientCommand(const CCommand &args);
	virtual void CreateViewModel(int viewmodelindex = 0);
	virtual bool BecomeRagdollOnClient(const Vector &force);
	virtual void Event_Killed(const CTakeDamageInfo &info);
	virtual int OnTakeDamage(const CTakeDamageInfo &inputInfo);
	virtual bool WantsLagCompensationOnEntity(const CBasePlayer *pPlayer, const CUserCmd *pCmd, const CBitVec<MAX_EDICTS> *pEntityTransmitBits) const;
	virtual void FireBullets(const FireBulletsInfo_t &info);
	virtual bool Weapon_Switch(CBaseCombatWeapon *pWeapon, int viewmodelindex = 0);
	virtual bool BumpWeapon(CBaseCombatWeapon *pWeapon);
	virtual void ChangeTeam(int iTeam);
	virtual void PickupObject(CBaseEntity *pObject, bool bLimitMassAndSize);
	virtual void PlayStepSound(Vector &vecOrigin, surfacedata_t *psurface, float fvol, bool force);
	virtual void Weapon_Drop(CBaseCombatWeapon *pWeapon, const Vector *pvecTarget = NULL, const Vector *pVelocity = NULL);
	virtual void UpdateOnRemove(void);
	virtual void DeathSound(const CTakeDamageInfo &info);
	virtual CBaseEntity* EntSelectSpawnPoint(void);

	virtual LadderMove_t *GetLadderMove();

	int FlashlightIsOn(void);
	void FlashlightTurnOn(void);
	void FlashlightTurnOff(void);
	void	PrecacheFootStepSounds(void);
	bool	ValidatePlayerModel(const char *pModel);

	QAngle GetAnimEyeAngles(void) { return m_angEyeAngles.Get(); }

	Vector GetAttackSpread(CBaseCombatWeapon *pWeapon, CBaseEntity *pTarget = NULL);

	void CheatImpulseCommands(int iImpulse);
	void CreateRagdollEntity(void);
	void GiveAllItems(void);
	void GiveDefaultItems(void);

	void NoteWeaponFired(void);

	void ResetAnimation(void);
	void SetPlayerModel(void);
	void SetPlayerTeamModel(void);
	Activity TranslateTeamActivity(Activity ActToTranslate);

	float GetNextModelChangeTime(void) { return m_flNextModelChangeTime; }
	float GetNextTeamChangeTime(void) { return m_flNextTeamChangeTime; }
	void  PickDefaultSpawnTeam(void);
	void  SetupPlayerSoundsByModel(const char *pModelName);
	const char *GetPlayerModelSoundPrefix(void);
	int	  GetPlayerModelType(void) { return m_iPlayerSoundType; }

	//void  DetonateTripmines(void);

	void Reset();

	//bool IsReady();
	//void SetReady(bool bReady);

	void CheckChatText(char *p, int bufsize);

	void State_Transition(TrainMayhemPlayerState newState);
	void State_Enter(TrainMayhemPlayerState newState);
	void State_Leave();
	void State_PreThink();
	CTrainMayhemPlayerStateInfo *State_LookupInfo(TrainMayhemPlayerState state);

	void State_Enter_ACTIVE();
	void State_PreThink_ACTIVE();
	void State_Enter_OBSERVER_MODE();
	void State_PreThink_OBSERVER_MODE();


	virtual bool StartObserverMode(int mode);
	virtual void StopObserverMode(void);


	Vector m_vecTotalBulletForce;	//Accumulator for bullet force in a single frame

									// Tracks our ragdoll entity.
	CNetworkHandle(CBaseEntity, m_hRagdoll);	// networked entity handle 

	virtual bool	CanHearAndReadChatFrom(CBasePlayer *pPlayer);

	CNetworkVarEmbedded(CTrainMayhemPlayerLocalData, m_LocalData);

private:

	CNetworkQAngle(m_angEyeAngles);
	CPlayerAnimState   m_PlayerAnimState;

	int m_iLastWeaponFireUsercmd;
	int m_iModelType;
	CNetworkVar(int, m_iSpawnInterpCounter);
	CNetworkVar(int, m_iPlayerSoundType);

	float m_flNextModelChangeTime;
	float m_flNextTeamChangeTime;

	float m_flSlamProtectTime;

	TrainMayhemPlayerState m_iPlayerState;
	CTrainMayhemPlayerStateInfo *m_pCurStateInfo;

	bool ShouldRunRateLimitedCommand(const CCommand &args);

	// This lets us rate limit the commands the players can execute so they don't overflow things like reliable buffers.
	CUtlDict<float, int>	m_RateLimitLastCommandTimes;

	bool m_bEnterObserver;
	bool m_bReady;

	friend class CTrainMayhemGameMovement;
};

inline CTrainMayhemPlayer *ToTrainMayhemPlayer(CBaseEntity *pEntity)
{
	if (!pEntity || !pEntity->IsPlayer())
		return NULL;

	return dynamic_cast<CTrainMayhemPlayer*>(pEntity);
}

#endif