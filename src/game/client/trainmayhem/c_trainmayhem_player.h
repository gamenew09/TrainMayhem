#ifndef TRAINMAYHEM_PLAYER_H
#define TRAINMAYHEM_PLAYER_H
#pragma once

class C_TrainMayhemPlayer;
#include "c_baseplayer.h"
#include "trainmayhem_player_shared.h"
#include "beamdraw.h"
#include "c_trainmayhem_playerlocaldata.h"

//=============================================================================
// >> HL2MP_Player
//=============================================================================
class C_TrainMayhemPlayer : public C_BasePlayer
{
public:
	DECLARE_CLASS(C_TrainMayhemPlayer, C_BasePlayer);

	DECLARE_CLIENTCLASS();
	DECLARE_PREDICTABLE();
	DECLARE_INTERPOLATION();

	LadderMove_t *C_TrainMayhemPlayer::GetLadderMove(void);

	C_TrainMayhemPlayer();
	~C_TrainMayhemPlayer(void);

	void ClientThink(void);

	static C_TrainMayhemPlayer* GetLocalHL2MPPlayer();

	virtual int DrawModel(int flags);
	virtual void AddEntity(void);

	QAngle GetAnimEyeAngles(void) { return m_angEyeAngles; }
	Vector GetAttackSpread(CBaseCombatWeapon *pWeapon, CBaseEntity *pTarget = NULL);

	virtual void Spawn(void);

	// Should this object cast shadows?
	virtual ShadowType_t		ShadowCastType(void);
	virtual C_BaseAnimating *BecomeRagdollOnClient();
	virtual const QAngle& GetRenderAngles();
	virtual bool ShouldDraw(void);
	virtual void OnDataChanged(DataUpdateType_t type);
	virtual float GetFOV(void);
	virtual CStudioHdr *OnNewModel(void);
	virtual void TraceAttack(const CTakeDamageInfo &info, const Vector &vecDir, trace_t *ptr, CDmgAccumulator *pAccumulator);
	virtual void ItemPreFrame(void);
	virtual void ItemPostFrame(void);
	virtual float GetMinFOV()	const { return 5.0f; }
	virtual Vector GetAutoaimVector(float flDelta);
	virtual void NotifyShouldTransmit(ShouldTransmitState_t state);
	virtual void CreateLightEffects(void) {}
	virtual bool ShouldReceiveProjectedTextures(int flags);
	virtual void PostDataUpdate(DataUpdateType_t updateType);
	virtual void PlayStepSound(Vector &vecOrigin, surfacedata_t *psurface, float fvol, bool force);
	virtual void PreThink(void);
	virtual void DoImpactEffect(trace_t &tr, int nDamageType);
	IRagdoll* GetRepresentativeRagdoll() const;
	virtual void CalcView(Vector &eyeOrigin, QAngle &eyeAngles, float &zNear, float &zFar, float &fov);
	virtual const QAngle& EyeAngles(void);


	bool	CanSprint(void);
	void	StartSprinting(void);
	void	StopSprinting(void);
	void	HandleSpeedChanges(void);
	void	UpdateLookAt(void);
	void	Initialize(void);
	int		GetIDTarget() const;
	void	UpdateIDTarget(void);
	void	PrecacheFootStepSounds(void);
	const char	*GetPlayerModelSoundPrefix(void);

	TrainMayhemPlayerState State_Get() const;

	// Walking
	void StartWalking(void);
	void StopWalking(void);
	bool IsWalking(void) { return m_fIsWalking; }

	virtual void PostThink(void);

	CNetworkVarEmbedded(C_TrainMayhemPlayerLocalData, m_LocalData);

private:

	C_TrainMayhemPlayer(const C_TrainMayhemPlayer &);

	CPlayerAnimState m_PlayerAnimState;

	QAngle	m_angEyeAngles;

	CInterpolatedVar< QAngle >	m_iv_angEyeAngles;

	EHANDLE	m_hRagdoll;

	int	m_headYawPoseParam;
	int	m_headPitchPoseParam;
	float m_headYawMin;
	float m_headYawMax;
	float m_headPitchMin;
	float m_headPitchMax;

	bool m_isInit;
	Vector m_vLookAtTarget;

	float m_flLastBodyYaw;
	float m_flCurrentHeadYaw;
	float m_flCurrentHeadPitch;

	int	  m_iIDEntIndex;

	//CountdownTimer m_blinkTimer;

	int	  m_iSpawnInterpCounter;
	int	  m_iSpawnInterpCounterCache;

	int	  m_iPlayerSoundType;

	void ReleaseFlashlight(void);
	Beam_t	*m_pFlashlightBeam;

	CNetworkVar(TrainMayhemPlayerState, m_iPlayerState);

	bool m_fIsWalking;
};

inline C_TrainMayhemPlayer *ToTrainMayhemPlayer(CBaseEntity *pEntity)
{
	if (!pEntity || !pEntity->IsPlayer())
		return NULL;

	return dynamic_cast<C_TrainMayhemPlayer*>(pEntity);
}


class C_TrainMayhemRagdoll : public C_BaseAnimatingOverlay
{
public:
	DECLARE_CLASS(C_TrainMayhemRagdoll, C_BaseAnimatingOverlay);
	DECLARE_CLIENTCLASS();

	C_TrainMayhemRagdoll();
	~C_TrainMayhemRagdoll();

	virtual void OnDataChanged(DataUpdateType_t type);

	int GetPlayerEntIndex() const;
	IRagdoll* GetIRagdoll() const;

	void ImpactTrace(trace_t *pTrace, int iDamageType, const char *pCustomImpactName);
	void UpdateOnRemove(void);
	virtual void SetupWeights(const matrix3x4_t *pBoneToWorld, int nFlexWeightCount, float *pFlexWeights, float *pFlexDelayedWeights);

private:

	C_TrainMayhemRagdoll(const C_TrainMayhemRagdoll &) {}

	void Interp_Copy(C_BaseAnimatingOverlay *pDestinationEntity);
	void CreateHL2MPRagdoll(void);

private:

	EHANDLE	m_hPlayer;
	CNetworkVector(m_vecRagdollVelocity);
	CNetworkVector(m_vecRagdollOrigin);
};

#endif //HL2MP_PLAYER_H
