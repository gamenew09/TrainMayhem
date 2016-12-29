#include "cbase.h"
#include "usermessages.h"
#include "shake.h"
#include "voice_gamemgr.h"

// NVNT include to register in haptic user messages
#include "haptics/haptic_msgs.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// (const char* name, int size)
#define RegisterUserMessage usermessages->Register

void RegisterUserMessages(void)
{
	RegisterUserMessage("Geiger", 1);
	RegisterUserMessage("Train", 1);
	RegisterUserMessage("HudText", -1);
	RegisterUserMessage("SayText", -1);
	RegisterUserMessage("SayText2", -1);
	RegisterUserMessage("TextMsg", -1);
	RegisterUserMessage("HudMsg", -1);
	RegisterUserMessage("ResetHUD", 1);		// called every respawn
	RegisterUserMessage("GameTitle", 0);
	RegisterUserMessage("ItemPickup", -1);
	RegisterUserMessage("ShowMenu", -1);
	RegisterUserMessage("Shake", 13);
	RegisterUserMessage("Fade", 10);
	RegisterUserMessage("VGUIMenu", -1);	// Show VGUI menu
	RegisterUserMessage("Rumble", 3);	// Send a rumble to a controller
	RegisterUserMessage("Battery", 2);
	RegisterUserMessage("Damage", 18);		// BUG: floats are sent for coords, no variable bitfields in hud & fixed size Msg
	RegisterUserMessage("VoiceMask", VOICE_MAX_PLAYERS_DW * 4 * 2 + 1);
	RegisterUserMessage("RequestState", 0);
	RegisterUserMessage("CloseCaption", -1); // Show a caption (by string id number)(duration in 10th of a second)
	RegisterUserMessage("HintText", -1);	// Displays hint text display
	RegisterUserMessage("KeyHintText", -1);	// Displays hint text display
	RegisterUserMessage("SquadMemberDied", 0);
	RegisterUserMessage("AmmoDenied", 2);
	RegisterUserMessage("CreditsMsg", 1);
	RegisterUserMessage("LogoTimeMsg", 4);
	RegisterUserMessage("AchievementEvent", -1);
	RegisterUserMessage("UpdateJalopyRadar", -1);

#ifndef _X360
	// NVNT register haptic user messages
	RegisterHapticMessages();
#endif
}

#undef RegisterUserMessage