#ifndef H_TRAINMAYHEM_GAMERULES
#define H_TRAINMAYHEM_GAMERULES

#include "gamerules.h"
#include "teamplay_gamerules.h"
#include "gamevars_shared.h"

#ifndef CLIENT_DLL
#include "trainmayhem_player.h"
#endif

#define VEC_CROUCH_TRACE_MIN	TrainMayhemRules()->GetHL2MPViewVectors()->m_vCrouchTraceMin
#define VEC_CROUCH_TRACE_MAX	TrainMayhemRules()->GetHL2MPViewVectors()->m_vCrouchTraceMax

enum
{
	TEAM_RED = 2,
	TEAM_BLUE,
};

enum
{
	GAMEMODE_CAPTUREPOINT = 0,
	NUM_GAMEMODES
};

#ifdef CLIENT_DLL
#define CTrainMayhemRules C_TrainMayhemRules
#define CTrainMayhemGameRulesProxy C_TrainMayhemGameRulesProxy
#endif

class CTrainMayhemGameRulesProxy : public CGameRulesProxy
{
public:
	DECLARE_CLASS(CTrainMayhemGameRulesProxy, CGameRulesProxy);
	DECLARE_NETWORKCLASS();
};

class HL2MPViewVectors : public CViewVectors
{
public:
	HL2MPViewVectors(
		Vector vView,
		Vector vHullMin,
		Vector vHullMax,
		Vector vDuckHullMin,
		Vector vDuckHullMax,
		Vector vDuckView,
		Vector vObsHullMin,
		Vector vObsHullMax,
		Vector vDeadViewHeight,
		Vector vCrouchTraceMin,
		Vector vCrouchTraceMax) :
		CViewVectors(
			vView,
			vHullMin,
			vHullMax,
			vDuckHullMin,
			vDuckHullMax,
			vDuckView,
			vObsHullMin,
			vObsHullMax,
			vDeadViewHeight)
	{
		m_vCrouchTraceMin = vCrouchTraceMin;
		m_vCrouchTraceMax = vCrouchTraceMax;
	}

	Vector m_vCrouchTraceMin;
	Vector m_vCrouchTraceMax;
};

// Use Teamplay Round Based Rules??
class CTrainMayhemRules : public CTeamplayRules
{
public:
	DECLARE_CLASS(CTrainMayhemRules, CTeamplayRules);

#ifdef CLIENT_DLL
	DECLARE_CLIENTCLASS_NOBASE(); // This makes datatables able to access our private vars.
#else
	DECLARE_SERVERCLASS_NOBASE(); // This makes datatables able to access our private vars.
#endif

	CTrainMayhemRules();
	virtual ~CTrainMayhemRules();

	virtual void Precache(void);
	virtual bool ShouldCollide(int collisionGroup0, int collisionGroup1);
	virtual bool ClientCommand(CBaseEntity *pEdict, const CCommand &args);

	virtual float FlWeaponRespawnTime(CBaseCombatWeapon *pWeapon);
	virtual float FlWeaponTryRespawn(CBaseCombatWeapon *pWeapon);
	virtual Vector VecWeaponRespawnSpot(CBaseCombatWeapon *pWeapon);
	virtual int WeaponShouldRespawn(CBaseCombatWeapon *pWeapon);
	virtual void Think(void);
	virtual void CreateStandardEntities(void);
	virtual void ClientSettingsChanged(CBasePlayer *pPlayer);
	virtual int PlayerRelationship(CBaseEntity *pPlayer, CBaseEntity *pTarget);
	virtual void GoToIntermission(void);
	virtual void DeathNotice(CBasePlayer *pVictim, const CTakeDamageInfo &info);
	virtual const char *GetGameDescription(void);
	// derive this function if you mod uses encrypted weapon info files
	virtual const unsigned char *GetEncryptionKey(void) { return (unsigned char *)"x9Ke0BY7"; }
	virtual const CViewVectors* GetViewVectors() const;
	const HL2MPViewVectors* GetHL2MPViewVectors() const;

	float GetMapRemainingTime();
	void CleanUpMap();
	void CheckRestartGame();
	void RestartGame();

#ifndef CLIENT_DLL // Server
	virtual Vector VecItemRespawnSpot(CItem *pItem);
	virtual QAngle VecItemRespawnAngles(CItem *pItem);
	virtual float	FlItemRespawnTime(CItem *pItem);
	virtual bool	CanHavePlayerItem(CBasePlayer *pPlayer, CBaseCombatWeapon *pItem);
	virtual bool FShouldSwitchWeapon(CBasePlayer *pPlayer, CBaseCombatWeapon *pWeapon);

	void	AddLevelDesignerPlacedObject(CBaseEntity *pEntity);
	void	RemoveLevelDesignerPlacedObject(CBaseEntity *pEntity);
	void	ManageObjectRelocation(void);
	void    CheckChatForReadySignal(CTrainMayhemPlayer *pPlayer, const char *chatmsg);
	const char *GetChatFormat(bool bTeamOnly, CBasePlayer *pPlayer);
#endif
	virtual void ClientDisconnected(edict_t *pClient);

	bool CheckGameOver(void);
	bool IsIntermission(void);

	void PlayerKilled(CBasePlayer *pVictim, const CTakeDamageInfo &info);


	bool	IsTeamplay(void) { return m_bTeamPlayEnabled; }

	virtual bool IsConnectedUserInfoChangeAllowed(CBasePlayer *pPlayer);

private:

	CNetworkVar(bool, m_bTeamPlayEnabled);
	CNetworkVar(float, m_flGameStartTime);
	CUtlVector<EHANDLE> m_hRespawnableItemsAndWeapons;
	float m_tmNextPeriodicThink;
	float m_flRestartGameTime;
	bool m_bCompleteReset;
	bool m_bAwaitingReadyRestart;
	bool m_bHeardAllPlayersReady;

#ifndef CLIENT_DLL // Server
	bool m_bChangelevelDone;
#endif
};

inline CTrainMayhemRules* TrainMayhemRules()
{
	return static_cast<CTrainMayhemRules*>(g_pGameRules);
}

#endif