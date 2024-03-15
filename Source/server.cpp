#pragma once
#include "stdafx.h"
using std::string;
#define URL L"https://diablogta.com/authserver/server.php"
void DoGetVase()
{
	net::requests m_request(L"authserver", false);
	std::wstring answer = m_request.Post(false, URL, "willkommen.php");

	if (answer == L"rt") {

		UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("~g~Server Conected");
		UI::_SET_NOTIFICATION_MESSAGE_CLAN_TAG_2("CHAR_EPSILON", "CHAR_EPSILON", 1, 8, "~HUD_COLOUR_GOLD~Saphir", "~HUD_COLOUR_GOLD~Server", 1, "notification", 9, 0);
		UI::_DRAW_NOTIFICATION(FALSE, FALSE);
	}
	else if (answer != L"rt")
	{
		UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("~g~Auth Server Down");
		UI::_SET_NOTIFICATION_MESSAGE_CLAN_TAG_2("CHAR_EPSILON", "CHAR_EPSILON", 1, 8, "~HUD_COLOUR_GOLD~Saphir", "~HUD_COLOUR_GOLD~Server", 1, "notification", 9, 0);
		UI::_DRAW_NOTIFICATION(FALSE, FALSE);
	}

}