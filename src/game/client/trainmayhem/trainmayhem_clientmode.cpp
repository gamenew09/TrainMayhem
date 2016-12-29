//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#include "cbase.h"
#include "ivmodemanager.h"
#include "clientmode_trainmayhemnormal.h"
#include "panelmetaclassmgr.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// default FOV for HL2
ConVar default_fov("default_fov", "90", FCVAR_CHEAT);

// The current client mode. Always ClientModeNormal in HL.
IClientMode *g_pClientMode = NULL;

#define SCREEN_FILE		"scripts/vgui_screens.txt"

class CTrainMayhemModeManager : public IVModeManager
{
public:
	CTrainMayhemModeManager(void);
	virtual		~CTrainMayhemModeManager(void);

	virtual void	Init(void);
	virtual void	SwitchMode(bool commander, bool force);
	virtual void	OverrideView(CViewSetup *pSetup);
	virtual void	CreateMove(float flInputSampleTime, CUserCmd *cmd);
	virtual void	LevelInit(const char *newmap);
	virtual void	LevelShutdown(void);
};

CTrainMayhemModeManager::CTrainMayhemModeManager(void)
{
}

CTrainMayhemModeManager::~CTrainMayhemModeManager(void)
{
}

void CTrainMayhemModeManager::Init(void)
{
	g_pClientMode = GetClientModeNormal();
	PanelMetaClassMgr()->LoadMetaClassDefinitionFile(SCREEN_FILE);
}

void CTrainMayhemModeManager::SwitchMode(bool commander, bool force)
{
}

void CTrainMayhemModeManager::OverrideView(CViewSetup *pSetup)
{
}

void CTrainMayhemModeManager::CreateMove(float flInputSampleTime, CUserCmd *cmd)
{
}

void CTrainMayhemModeManager::LevelInit(const char *newmap)
{
	g_pClientMode->LevelInit(newmap);
}

void CTrainMayhemModeManager::LevelShutdown(void)
{
	g_pClientMode->LevelShutdown();
}


static CTrainMayhemModeManager g_HLModeManager;
IVModeManager *modemanager = &g_HLModeManager;

