//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
// $NoKeywords: $
//=============================================================================//
#if !defined( CLIENTMODE_TRAINMAYHEMNORMAL_H )
#define CLIENTMODE_TRAINMAYHEMNORMAL_H
#ifdef _WIN32
#pragma once
#endif

#include "clientmode_shared.h"
#include <vgui_controls/EditablePanel.h>
#include <vgui/Cursor.h>

class CHudViewport;

namespace vgui
{
	typedef unsigned long HScheme;
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class ClientModeTrainMayhemNormal : public ClientModeShared
{
public:
	DECLARE_CLASS(ClientModeTrainMayhemNormal, ClientModeShared);

	ClientModeTrainMayhemNormal();
	~ClientModeTrainMayhemNormal();

	virtual void	Init();
	virtual bool	ShouldDrawCrosshair(void);
};

extern IClientMode *GetClientModeNormal();
extern vgui::HScheme g_hVGuiCombineScheme;

#endif // CLIENTMODE_HLNORMAL_H
