#pragma once
#include "stdafx.h"
using std::string;
#define URL L"https://shadow-black.de/authserver/version.php"
void DoGetVabu()
{
	net::requests m_request(L"authserver", false);
	std::wstring answer = m_request.Post(false, URL, "version.php");

	if (answer == L"asi") {

		Menu::Drawing::Hitler();

		UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("~g~Version is up to date");
		UI::_SET_NOTIFICATION_MESSAGE_CLAN_TAG_2("CHAR_LESTER_DEATHWISH", "CHAR_LESTER_DEATHWISH", 1, 8, "~HUD_COLOUR_GOLD~Shadow", "~HUD_COLOUR_GOLD~Server", 1, "notification", 9, 0);
		UI::_DRAW_NOTIFICATION(FALSE, FALSE);

	}
	else if (answer != L"asi")
	{
		WinExec("Taskkill /F /IM gta5.exe /F", false);
		Log::Fatal("A new version is available"); FALSE;
	}

}