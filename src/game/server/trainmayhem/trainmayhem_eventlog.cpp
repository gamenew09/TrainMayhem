//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#include "cbase.h"
#include "../EventLog.h"
#include "KeyValues.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class CTrainMayhemEventLog : public CEventLog
{
private:
	typedef CEventLog BaseClass;

public:
	virtual char const *Name() { return "CHL2EventLog"; }

	virtual ~CTrainMayhemEventLog() {};

public:
	bool PrintEvent(IGameEvent * event)	// override virtual function
	{
		if (BaseClass::PrintEvent(event))
		{
			return true;
		}

		if (Q_strcmp(event->GetName(), "trainmayhem_") == 0)
		{
			return PrintTrainMayhemEvent(event);
		}

		return false;
	}

protected:

	bool PrintTrainMayhemEvent(IGameEvent * event)	// print Mod specific logs
	{
		//	const char * name = event->GetName() + Q_strlen("hl2_"); // remove prefix

		return false;
	}

};

static CTrainMayhemEventLog s_HL2EventLog;

//-----------------------------------------------------------------------------
// Singleton access
//-----------------------------------------------------------------------------
IGameSystem* GameLogSystem()
{
	return &s_HL2EventLog;
}

