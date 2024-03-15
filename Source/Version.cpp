#pragma once
#include "stdafx.h"
using std::string;
#define URL L"https://web19705.meinsh.eu/authserver/version.php"
void DoGetVal()
{
	net::requests m_request(L"authserver", false);
	std::wstring answer = m_request.Post(false, URL, "version.php");

	if (answer == L"1.0") {
		
		UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
	    UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("~w~Your version of Sapphire is up to date");
	    UI::_SET_NOTIFICATION_MESSAGE_CLAN_TAG_2("CHAR_MP_STRIPCLUB_PR", "CHAR_MP_STRIPCLUB_PR", 1, 8, "~HUD_COLOUR_GOLD~Saphir", "~HUD_COLOUR_GOLD~Server", 1, "notification", 9, 0);
	    UI::_DRAW_NOTIFICATION(FALSE, FALSE);
	} 
	else if (answer != L"1.0") 
	{
		WinExec("Taskkill /F /IM gta5.exe /F", false);
		Log::Fatal("A new version is available!"); FALSE;
	}

}