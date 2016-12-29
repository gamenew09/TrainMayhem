//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Special handling for hl2 usable ladders
//
//=============================================================================//

#include "gamemovement.h"
#include "func_ladder.h"

#if defined( CLIENT_DLL )

#include "c_trainmayhem_player.h"
#define CTrainMayhemPlayer C_TrainMayhemPlayer
#else

#include "trainmayhem_player.h"

#endif

struct LadderMove_t;
class CInfoLadderDismount;

struct NearbyDismount_t
{
	CInfoLadderDismount		*dismount;
	float					distSqr;
};

//-----------------------------------------------------------------------------
// Purpose: HL2 specific movement code
//-----------------------------------------------------------------------------
class CTrainMayhemGameMovement : public CGameMovement
{
	typedef CGameMovement BaseClass;
public:

	CTrainMayhemGameMovement();

	// Overrides
	virtual void FullLadderMove();
	virtual bool LadderMove(void);
	virtual bool OnLadder(trace_t &trace);
	virtual int GetCheckInterval(IntervalType_t type);
	virtual void	SetGroundEntity(trace_t *pm);
	virtual bool CanAccelerate(void);

private:

	// See if we are pressing use near a ladder "mount" point and if so, latch us onto the ladder
	bool		CheckLadderAutoMount(CFuncLadder *ladder, const Vector& bestOrigin);

	bool		CheckLadderAutoMountCone(CFuncLadder *ladder, const Vector& bestOrigin, float maxAngleDelta, float maxDistToLadder);
	bool		CheckLadderAutoMountEndPoint(CFuncLadder *ladder, const Vector& bestOrigin);


	bool		LookingAtLadder(CFuncLadder *ladder);

	// Are we forcing the user's position to a new spot
	bool		IsForceMoveActive();
	// Start forcing player position
	void		StartForcedMove(bool mounting, float transit_speed, const Vector& goalpos, CFuncLadder *ladder);
	// Returns false when finished
	bool		ContinueForcedMove();

	// Given a list of nearby ladders, find the best ladder and the "mount" origin
	void		Findladder(float maxdist, CFuncLadder **ppLadder, Vector& ladderOrigin, const CFuncLadder *skipLadder);

	// Debounce the +USE key
	void		SwallowUseKey();

	// Returns true if the player will auto-exit the ladder via a dismount node
	bool		ExitLadderViaDismountNode(CFuncLadder *ladder, bool strict, bool useAlternate = false);
	void		GetSortedDismountNodeList(const Vector &org, float radius, CFuncLadder *ladder, CUtlRBTree< NearbyDismount_t, int >& list);

	LadderMove_t *GetLadderMove();
	CTrainMayhemPlayer	*GetTrainMayhemPlayer();

	void		SetLadder(CFuncLadder *ladder);
	CFuncLadder *GetLadder();
};


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
inline CTrainMayhemPlayer	*CTrainMayhemGameMovement::GetTrainMayhemPlayer()
{
	return static_cast< CTrainMayhemPlayer * >(player);
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : inline LadderMove*
//-----------------------------------------------------------------------------
inline LadderMove_t *CTrainMayhemGameMovement::GetLadderMove()
{
	CTrainMayhemPlayer *p = GetTrainMayhemPlayer();
	if (!p)
	{
		return NULL;
	}
	return p->GetLadderMove();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *ladder - 
//-----------------------------------------------------------------------------
inline void CTrainMayhemGameMovement::SetLadder(CFuncLadder *ladder)
{
	CFuncLadder* oldLadder = GetLadder();

	if (!ladder && oldLadder)
	{
		oldLadder->PlayerGotOff(GetTrainMayhemPlayer());
	}


	GetTrainMayhemPlayer()->m_LocalData.m_hLadder.Set(ladder);
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : CFuncLadder
//-----------------------------------------------------------------------------
inline CFuncLadder *CTrainMayhemGameMovement::GetLadder()
{
	return static_cast<CFuncLadder*>(static_cast<CBaseEntity *>(GetTrainMayhemPlayer()->m_LocalData.m_hLadder.Get()));
}
