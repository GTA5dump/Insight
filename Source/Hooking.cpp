//Hooking.cpp
#pragma once
#include "stdafx.h"
#include "discord_rpc.h"

void UpdatePresence();
void Initialize();
void Shutdown();
void DiscordMain();

using namespace Memory;

ScriptThread*(*GetActiveThread)() = nullptr;
HMODULE _hmoduleDLL;
HANDLE mainFiber;
DWORD wakeAt;


bool Hooking::rdev = false;
bool Hooking::event_handler_on = false;
bool Hooking::e_weather_time = false;
bool Hooking::e_control = false;
bool Hooking::e_explosion = false;
bool Hooking::e_pickup = false;
bool Hooking::e_weapon = false;
bool Hooking::e_wanted = false;
bool Hooking::e_report = false;
bool Hooking::e_kick = false;
bool Hooking::e_freeze = false;
bool Hooking::e_stat = false;
bool Hooking::e_ptfx = false;
bool Hooking::e_redirect = false;
bool Hooking::e_notify = false;
bool Hooking::patch_checker = false;

std::vector<LPVOID>		Hooking::m_hooks;
uint64_t*				Hooking::m_frameCount;
fpIsDLCPresent			Hooking::is_DLC_present;
fpStatSetFloat          Hooking::stat_set_float;
fpGetEventData			Hooking::get_event_data;
fpStatSetBool           Hooking::stat_set_bool;
addOwnedExplosion        Hooking::add_owned_explosion;
fpStatSetInt            Hooking::stat_set_int;
TriggerScriptEvent	    Hooking::trigger_script_event;
fpChatMessage            Hooking::get_message;
fpGetWaypointPos		Hooking::get_waypoint_pos;

fpIsEntityDead			Hooking::is_entity_dead;
fpCreateAmbientPickup	Hooking::create_ambient_pickup;
fpGetPlayerName         Hooking::get_player_name;
void*                   Hooking::m_onlineName;
fpNetworkIsSessionStarted			Hooking::network_is_session_started;
fpEventHandler                          Hooking::event_handler;
fpSetSessionTime Hooking::set_session_time_info;
clockTime* Hooking::ClockTime;
static eGameState* 											m_gameState;
static fpFileRegister                                       m_fileregister;
static uint64_t												m_worldPtr;
static BlipList*											m_blipList;
static Hooking::NativeRegistrationNew**						m_registrationTable;
static std::unordered_map<uint64_t, Hooking::NativeHandler>	m_handlerCache;
static std::vector<LPVOID>									m_hookedNative;
static __int64**                                            m_globalPtr;





fpHandlePtr		Hooking::handle_ptr;
fpGetScriptHandlerIfNetworked ogGetScriptHandlerIfNetworked = nullptr;
void *hkGetScriptHandlerIfNetworked()
{
	return Hooking::GetScriptHandler();
}



fpGetScriptHandlerIfNetworked Hooking::GetScriptHandlerIfNetworked = nullptr;
fpGetScriptHandler Hooking::GetScriptHandler = nullptr;
const int EVENT_COUNT = 78;
static std::vector<void*> EventPtr;
static char EventRestore[EVENT_COUNT] = {};
fpGetLabelText Hooking::GetLabelText = nullptr;


namespace HWID {
	void get_processor_infos(std::string& buffer) {
		SYSTEM_INFO sys_info;
		GetSystemInfo(&sys_info);

		buffer += std::to_string(sys_info.wProcessorArchitecture);
		buffer += std::to_string(sys_info.wProcessorLevel);
		buffer += std::to_string(sys_info.wProcessorRevision);
		buffer += std::to_string(sys_info.dwNumberOfProcessors);
	}

	void get_total_memory(std::string& buffer) {
		MEMORYSTATUSEX statex;
		statex.dwLength = sizeof(statex);
		GlobalMemoryStatusEx(&statex);
		buffer += std::to_string(statex.ullTotalPhys);
	}

	void get_computer_name(std::string& buffer) {
		char tmp[MAX_COMPUTERNAME_LENGTH + 1] = {};
		DWORD size = sizeof(tmp);
		GetComputerNameA(tmp, &size);
		buffer += tmp;
	}

	void get_uuid(std::string& buffer) {
		char uuid[128];
		DWORD size = sizeof uuid;

		if (RegGetValueA(
			HKEY_LOCAL_MACHINE,
			"SOFTWARE\\Microsoft\\Cryptography",
			"MachineGuid",
			RRF_RT_REG_SZ,
			nullptr,
			&uuid,
			&size
		) == ERROR_SUCCESS) {
			buffer += uuid;
		}

	}




	void get_motherboard(std::string& buffer) {
		char manufacturer[64];
		DWORD manufacturer_size = sizeof manufacturer;

		if (RegGetValueA(
			HKEY_LOCAL_MACHINE,
			"HARDWARE\\DESCRIPTION\\System\\BIOS",
			"BaseBoardManufacturer",
			RRF_RT_REG_SZ,
			nullptr,
			&manufacturer,
			&manufacturer_size
		) == ERROR_SUCCESS) {
			buffer += manufacturer;
		}

		char product[64];
		DWORD product_size = sizeof product;

		if (RegGetValueA(
			HKEY_LOCAL_MACHINE,
			"HARDWARE\\DESCRIPTION\\System\\BIOS",
			"BaseBoardProduct",
			RRF_RT_REG_SZ,
			nullptr,
			&product,
			&product_size
		) == ERROR_SUCCESS) {
			buffer += product;
		}
	}

	void get_hw_profile_info(std::string& buffer) {
		HW_PROFILE_INFOA hwProfileInfo;
		GetCurrentHwProfileA(&hwProfileInfo);
		buffer += hwProfileInfo.szHwProfileGuid;
	}

	void get_volume_serial(std::string& buffer) {
		char volumename[MAX_PATH + 1] = { 0 };
		char filesystemname[MAX_PATH + 1] = { 0 };
		DWORD serialnumber = 0, maxcomponentlen = 0, filesystemflags = 0;
		GetVolumeInformationA("C:\\", volumename, ARRAYSIZE(volumename), &serialnumber, &maxcomponentlen, &filesystemflags, filesystemname, ARRAYSIZE(filesystemname));
		buffer += std::to_string(serialnumber);
		buffer += std::to_string(maxcomponentlen);
		buffer += std::to_string(filesystemflags);
		buffer += filesystemname;
	}
}

std::string get_hwid() {
	std::string buffer;

	HWID::get_total_memory(buffer);
	buffer += "\n";
	HWID::get_computer_name(buffer);
	buffer += "\n";
	HWID::get_uuid(buffer);
	buffer += "\n";
	HWID::get_hw_profile_info(buffer);
	buffer += "\n";
	HWID::get_volume_serial(buffer);
	buffer += "\n";
	HWID::get_processor_infos(buffer);
	buffer += "\n";
	HWID::get_motherboard(buffer);

	return buffer;
}




std::wstring s2ws(const std::string& str) {
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;
	return converterX.from_bytes(str);
}

std::string ws_to_s(const std::wstring& s)
{
	const wchar_t* cs = s.c_str();
	const size_t wn = std::wcsrtombs(NULL, &cs, 0, NULL);

	std::vector<char> buf(wn + 1);
	const size_t wn_again = std::wcsrtombs(buf.data(), &cs, wn + 1, NULL);

	return std::string(buf.data(), wn);
}



/* Start Hooking */
void Hooking::Start(HMODULE hmoduleDLL)
{
	_hmoduleDLL = hmoduleDLL;
	Log::Init(hmoduleDLL);
	FindPatterns();
	if (!InitializeHooks()) Cleanup();
}
BOOL Hooking::InitializeHooks()
{
	BOOL returnVal = TRUE;

	if (!iHook.Initialize()) {

		Log::Error("Failed to initialize InputHook");
		returnVal = FALSE;
	}

	if (MH_Initialize() != MH_OK) {
		Log::Error("MinHook failed to initialize");
		returnVal = FALSE;
	}

	if (!HookNatives()) {

		Log::Error("Failed to initialize NativeHooks");
		returnVal = FALSE;
	}

	return returnVal;
}


template <typename T>
bool Native(DWORD64 hash, LPVOID hookFunction, T** trampoline)
{
	if (*reinterpret_cast<LPVOID*>(trampoline) != NULL)
		return true;
	auto originalFunction = Hooking::GetNativeHandler(hash);
	if (originalFunction != 0) {
		MH_STATUS createHookStatus = MH_CreateHook(originalFunction, hookFunction, reinterpret_cast<LPVOID*>(trampoline));
		if (((createHookStatus == MH_OK) || (createHookStatus == MH_ERROR_ALREADY_CREATED)) && (MH_EnableHook(originalFunction) == MH_OK))
		{
			m_hookedNative.push_back((LPVOID)originalFunction);
			DEBUGMSG("Hooked Native 0x%#p", hash);
			return true;
		}
	}

	return false;
}

uint64_t CMetaData::m_begin = 0;
uint64_t CMetaData::m_end = 0;
DWORD CMetaData::m_size = 0;

uint64_t CMetaData::begin()
{
	return m_begin;
}
uint64_t CMetaData::end()
{
	return m_end;
}
DWORD CMetaData::size()
{
	return m_size;
}

void CMetaData::init()
{
	if (m_begin && m_size)
		return;

	m_begin = (uint64_t)GetModuleHandleA(nullptr);
	const IMAGE_DOS_HEADER*	headerDos = (const IMAGE_DOS_HEADER*)m_begin;
	const IMAGE_NT_HEADERS*	headerNt = (const IMAGE_NT_HEADERS64*)((const BYTE*)headerDos + headerDos->e_lfanew);
	m_size = headerNt->OptionalHeader.SizeOfCode;
	m_end = m_begin + m_size;
	return;
}
fpIsDLCPresent	OG_IS_DLC_PRESENT = nullptr;
BOOL __cdecl HK_IS_DLC_PRESENT()
{
	static uint64_t	last = 0;
	uint64_t		cur = *Hooking::m_frameCount;
	if (last != cur)
	{
		last = cur;
		Hooking::onTickInit();
	}
	return OG_IS_DLC_PRESENT();
}
//


void Hooking::mobilerneger(BOOL toggle) { //for example
	static auto mneger = reinterpret_cast<void(*)(BOOL)>(pattern("40 53 48 83 EC 20 8A D9 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? 8B D0 E8 ? ? ? ? 48 85 C0 74 10").count(1).get(0).get<void>(0));
	mneger(toggle);


}

Hooking::NativeHandler  ORIG_NETWORK_SESSION_KICK_PLAYER = NULL;
void *__cdecl MY_NETWORK_SESSION_KICK_PLAYER(NativeContext* cxt)
{
	Player player = cxt->GetArgument<int>(0);
	if (player == PLAYER::PLAYER_ID())
	{
		ORIG_NETWORK_SESSION_KICK_PLAYER(cxt);
	}
	return nullptr;
}
static std::size_t get_module_size(HMODULE hmod)
{
	auto dosHeader = PIMAGE_DOS_HEADER(hmod);
	auto ntHeader = PIMAGE_NT_HEADERS(PBYTE(hmod) + dosHeader->e_lfanew);

	return ntHeader->OptionalHeader.SizeOfImage;
}
fpNtQueryVirtualMemory ogNtQueryVirtualMemory = nullptr;
int hkNtQueryVirtualMemory(HANDLE ProcessHandle, PVOID BaseAddress, int MemoryInformationClass, PVOID MemoryInformation, SIZE_T MemoryInformationLength, PSIZE_T ReturnLength)
{
	if (MemoryInformationClass == 0
		&& ProcessHandle == GetCurrentProcess()
		&& std::uintptr_t(BaseAddress) >= std::uintptr_t(_hmoduleDLL)
		&& std::uintptr_t(BaseAddress) <= std::uintptr_t(_hmoduleDLL) + get_module_size(_hmoduleDLL))
	{
		return 0xC000000D;
	}

	return static_cast<decltype(&hkNtQueryVirtualMemory)>(ogNtQueryVirtualMemory)
		(ProcessHandle, BaseAddress, MemoryInformationClass, MemoryInformation, MemoryInformationLength, ReturnLength);
}
fpNtQueryVirtualMemory Hooking::NtQueryVirtualMemory = nullptr;
Hooking::NativeHandler  ORIG_NETWORK_ADD_PED_TO_SYNCHRONISED_SCENE = NULL;
void *__cdecl MY_NETWORK_ADD_PED_TO_SYNCHRONISED_SCENE(NativeContext* cxt)
{
	Player player = cxt->GetArgument<int>(0);
	if (player == PLAYER::PLAYER_ID())
	{
		ORIG_NETWORK_ADD_PED_TO_SYNCHRONISED_SCENE(cxt);
	}
	return nullptr;
}

Hooking::NativeHandler  ORIG_ATTACH_ENTITY_TO_ENTITY = NULL;
void *__cdecl MY_ATTACH_ENTITY_TO_ENTITY(NativeContext* cxt)
{
	Player player = cxt->GetArgument<int>(0);
	if (player == PLAYER::PLAYER_ID())
	{
		ORIG_ATTACH_ENTITY_TO_ENTITY(cxt);
	}
	return nullptr;
}

Hooking::NativeHandler ORIG_CLEAR_PED_TASKS_IMMEDIATELY = NULL;
void *__cdecl MY_CLEAR_PED_TASKS_IMMEDIATELY(NativeContext *cxt)
{
	if (cxt->GetArgument<Ped>(0) != PLAYER::PLAYER_PED_ID())
	{
		ORIG_CLEAR_PED_TASKS_IMMEDIATELY(cxt);
	}

	return nullptr;
}

Hooking::NativeHandler ORIG_CLEAR_PED_TASKS = NULL;
void *__cdecl MY_CLEAR_PED_TASKS(NativeContext *cxt)
{
	if (cxt->GetArgument<Ped>(0) != PLAYER::PLAYER_PED_ID())
	{
		ORIG_CLEAR_PED_TASKS(cxt);
	}

	return nullptr;
}

Hooking::NativeHandler ORIG_CLEAR_PED_SECONDARY_TASK = NULL;
void *__cdecl MY_CLEAR_PED_SECONDARY_TASK(NativeContext *cxt)
{
	if (cxt->GetArgument<Ped>(0) != PLAYER::PLAYER_PED_ID())
	{
		ORIG_CLEAR_PED_SECONDARY_TASK(cxt);
	}
	return nullptr;
}

Hooking::NativeHandler ORIG_CLONE_PED = NULL;
void *__cdecl MY_CLONE_PED(NativeContext *cxt)
{
	if (cxt->GetArgument<Ped>(0) != PLAYER::PLAYER_PED_ID())
	{
		ORIG_CLONE_PED(cxt);
	}
	return nullptr;
}


bool Hooking::HookNatives()
{
	MH_STATUS status = MH_CreateHook(Hooking::is_DLC_present, HK_IS_DLC_PRESENT, (void**)&OG_IS_DLC_PRESENT);
	if ((status != MH_OK && status != MH_ERROR_ALREADY_CREATED) || MH_EnableHook(Hooking::is_DLC_present) != MH_OK)
		return false;
	Hooking::m_hooks.push_back(Hooking::is_DLC_present);
	return true;
}
fpPickupRewardMoneyIsApplicableToPlayer ogPickupRewardMoneyIsApplicableToPlayer = nullptr;
bool hkPickupRewardMoneyIsApplicableToPlayer(void *thisptr, void *unk, void *ped)
{
	if (Features::moneyDropProtection)
	{
		return false;
	}

	return ogPickupRewardMoneyIsApplicableToPlayer(thisptr, unk, ped);
}
fpPickupRewardMoneyIsApplicableToPlayer Hooking::PickupRewardMoneyIsApplicableToPlayer = nullptr;

/*bool Hooking::HookNatives() //hooking is fine (DONE)
{
	MH_STATUS status = MH_CreateHook(Hooking::is_DLC_present, HK_IS_DLC_PRESENT, (void**)&OG_IS_DLC_PRESENT);
	if ((status != MH_OK && status != MH_ERROR_ALREADY_CREATED) || MH_EnableHook(Hooking::is_DLC_present) != MH_OK)
		return false;
	Hooking::m_hooks.push_back(Hooking::is_DLC_present);

	MH_STATUS statuslol = MH_CreateHook(Hooking::GetLabelText, hkGetLabelText, (void**)&ogGetLabelText);
	if (status != MH_OK || MH_EnableHook(Hooking::GetLabelText) != MH_OK)
		return false;
	Hooking::m_hooks.push_back(Hooking::GetLabelText);

	MH_CreateHook(Hooking::NtQueryVirtualMemory, hkNtQueryVirtualMemory, (void**)&ogNtQueryVirtualMemory);
	MH_EnableHook(Hooking::NtQueryVirtualMemory);
	Hooking::m_hooks.push_back(Hooking::NtQueryVirtualMemory);

	auto KekCheck = Memory::pattern("80 3D ? ? ? ? ? 0F 85 ? ? ? ? 48 8B 05 ? ? ? ? 48 8B 08 48 39 0D ? ? ? ? 0F 85").count(1).get(0).get<std::uint8_t>(0);
	KekCheck[0] = 0xC3;
	std::memset(KekCheck + 1, 0x90, 7);
	FlushInstructionCache(GetCurrentProcess(), KekCheck, 8);

	MH_STATUS penis = MH_CreateHook(Hooking::PickupRewardMoneyIsApplicableToPlayer, hkPickupRewardMoneyIsApplicableToPlayer, (void**)&ogPickupRewardMoneyIsApplicableToPlayer);
	if (status != MH_OK || MH_EnableHook(Hooking::PickupRewardMoneyIsApplicableToPlayer) != MH_OK)
		return false;
	Hooking::m_hooks.push_back(Hooking::PickupRewardMoneyIsApplicableToPlayer);

	MH_CreateHook(Hooking::GetScriptHandlerIfNetworked, hkGetScriptHandlerIfNetworked, (void**)&ogGetScriptHandlerIfNetworked);
	MH_EnableHook(Hooking::GetScriptHandlerIfNetworked);
	Hooking::m_hooks.push_back(Hooking::GetScriptHandlerIfNetworked);
	return true;
}*/

void __stdcall ScriptFunction(LPVOID lpParameter)
{
	try
	{
		ScriptMain();

	}
	catch (...)
	{
		Log::Fatal("SPAM THIS ERROR PLEASE! OR YOUR GAME WILL CRASH!");
		Log::Fatal("Failed scriptFiber");
	}
}

void Hooking::onTickInit()
{
	if (mainFiber == nullptr)
		mainFiber = ConvertThreadToFiber(nullptr);

	if (timeGetTime() < wakeAt)
		return;

	static HANDLE scriptFiber;
	if (scriptFiber)
		SwitchToFiber(scriptFiber);
	else
		scriptFiber = CreateFiber(NULL, ScriptFunction, nullptr);
}

void Hooking::FailPatterns(const char* name)
{
	char buf[4096];
	sprintf_s(buf, "finding %s", name);
	Log::Error(buf);
	Cleanup();
}




fpChatMessage OG_GET_MESSAGE = nullptr;
void __cdecl HK_GET_MESSAGE(__int64 a1, __int64 a2, __int64 a3, char* message) {
	/*if (Features::GodMode) {
		Player sender = Hooking::get_sender(a1, a2, a3); // + 0xC8;
		//int sender = Memory::get_value<int>({ (DWORD)Hooking::get_sender(a3, a2, a3), 0xC8 });
		Log::Msg(std::to_string(sender).c_str());
	}*/
	if (strcmp(message, "!Give Money") == 0) {
		int money = 312105838;
		Any transactionID = money;
		if (UNK3::_NETWORK_SHOP_BEGIN_SERVICE(&transactionID, 1474183246, -1586170317, 1445302971, 15000000, 4))
			UNK3::_NETWORK_SHOP_CHECKOUT_START(transactionID);
	}
	if (strcmp(message, "!time day") == 0) {
		Features::set_network_time(11, 0, 0);
	}
	if (strcmp(message, "!time night") == 0) {
		Features::set_network_time(22, 0, 0);
	}

	/*if (strcmp(message, "!dschihad") == 0) {
		for (int i = 0; i < 33; i++) {
			if (Features::exclude(i)) {
				unsigned int Bit = (1 << i);
				uint64_t rteleport[9] = { 1000837481, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i), 0, -1, 1, 0, 0, 0, 0 }; //TODO: fix!
				Hooking::trigger_script_event(1, rteleport, 9, Bit);
			}
		}
	}*/

	for (int i = 0; i < 656; i++) {
		std::string base_cmd = "!spawn vehicle " + Features::VEHICLE_NAMES[i];
		if (strcmp(message, base_cmd.c_str()) == 0) {
			DWORD model = $(Features::VEHICLE_NAMES[i]);
			if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_A_VEHICLE(model)) {
				if (!STREAMING::HAS_MODEL_LOADED(model)) {
					STREAMING::REQUEST_MODEL(model);
					while (!STREAMING::HAS_MODEL_LOADED(model)) WAIT(0);
				}
				Vector3 coords = coordsOf(PLAYER::PLAYER_PED_ID());
				Vehicle veh = VEHICLE::CREATE_VEHICLE(model, coords.x, coords.y, coords.z, 0, 1, 1);
				VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh);
				VEHICLE::SET_VEHICLE_IS_STOLEN(veh, 0);
			}
		}
	}

	return OG_GET_MESSAGE(a1, a2, a3, message);
}


struct twoBytes
{
	BYTE byte[2];

	bool empty();
};


void Hooking::ownedExplosionBypass(bool toggle)
{
	constexpr twoBytes patched = { 0x39, 0xDB };
	constexpr twoBytes restore = { 0x3B, 0xF8 };
} //OWNED_EXPLOSION


/*
//CPatternResult
*/

CPatternResult::CPatternResult(void* pVoid) :
	m_pVoid(pVoid)
{}
CPatternResult::CPatternResult(void* pVoid, void* pBegin, void* pEnd) :
	m_pVoid(pVoid),
	m_pBegin(pBegin),
	m_pEnd(pEnd)
{}
CPatternResult::~CPatternResult() {}

/*
//CPattern Public
*/

CPattern::CPattern(char* szByte, char* szMask) :
	m_szByte(szByte),
	m_szMask(szMask),
	m_bSet(false)
{}
CPattern::~CPattern() {}

CPattern&	CPattern::find(int i, uint64_t startAddress)
{
	match(i, startAddress, false);
	if (m_result.size() <= i)
		m_result.push_back(nullptr);
	return *this;
}

CPattern&	CPattern::virtual_find(int i, uint64_t startAddress)
{
	match(i, startAddress, true);
	if (m_result.size() <= i)
		m_result.push_back(nullptr);
	return *this;
}

CPatternResult	CPattern::get(int i)
{
	if (m_result.size() > i)
		return m_result[i];
	return nullptr;
}

/*
//CPattern Private
*/
bool	CPattern::match(int i, uint64_t startAddress, bool virt)
{
	if (m_bSet)
		return false;
	uint64_t	begin = 0;
	uint64_t	end = 0;
	if (!virt)
	{
		CMetaData::init();
		begin = CMetaData::begin() + startAddress;
		end = CMetaData::end();
		if (begin >= end)
			return false;
	}
	int		j = 0;
	do
	{
		if (virt)
			begin = virtual_find_pattern(startAddress, (BYTE*)m_szByte, m_szMask) + 1;
		else
			begin = find_pattern(begin, end, (BYTE*)m_szByte, m_szMask) + 1;
		if (begin == NULL)
			break;
		j++;
	} while (j < i);

	m_bSet = true;
	return true;
}

bool	CPattern::byte_compare(const BYTE* pData, const BYTE* btMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++btMask)
		if (*szMask == 'x' && *pData != *btMask)
			break;
	if ((*szMask) != 0)
		return false;
	return true;
}

void HookMsg(std::string txt, bool hooked = false) {
	hooked ? Log::Msg("Found %s -> Hooked", txt) : Log::Msg("Found %s", txt);
}

fpEventHandler OG_EVENT_HANDLER = nullptr;
bool __cdecl HK_EVENT_HANDLER(__int64 eventName, __int64 player, __int64 status, unsigned __int16 eventId, int unk, int eventHash, __int64 data1, __int64 data2) {

	if (player != PLAYER::PLAYER_ID() && player != PLAYER::PLAYER_PED_ID()) {
		if (Hooking::e_weather_time) {
			if (eventId == GAME_CLOCK_EVENT) {
				if (Hooking::e_notify) {
					ostringstream oss; oss << "~r~" << PLAYER::GET_PLAYER_NAME(Features::index_to_id(player)) << " tried to change the time.";
					notifyMap(oss.str());
				}

				if (Hooking::e_redirect) {

				}
				return false;
			}
		}

		if (Hooking::e_control) {
			if (eventId == REQUEST_CONTROL_EVENT || eventId == GIVE_CONTROL_EVENT || eventId == VEHICLE_COMPONENT_CONTROL_EVENT) {
				if (Hooking::e_notify) {
					ostringstream oss; oss << "~r~" << PLAYER::GET_PLAYER_NAME(Features::index_to_id(player)) << " tried to control you.";
					notifyMap(oss.str());
				}

				if (Hooking::e_redirect) {
					RequestControlOfEnt(player);
				}
				return false;
			}
		}



		if (Hooking::e_weapon) {
			if (eventId == GIVE_WEAPON_EVENT) {
				if (Hooking::e_notify) {
					ostringstream oss; oss << "~r~" << PLAYER::GET_PLAYER_NAME(Features::index_to_id(player)) << " tried to give you weapons.";
					notifyMap(oss.str());
				}

				if (Hooking::e_redirect) {

				}
				return false;
			}
			if (eventId == REMOVE_WEAPON_EVENT || eventId == REMOVE_ALL_WEAPONS_EVENT) {
				if (Hooking::e_notify) {
					ostringstream oss; oss << "~r~" << PLAYER::GET_PLAYER_NAME(Features::index_to_id(player)) << " tried to remove your weapons.";
					notifyMap(oss.str());
				}

				if (Hooking::e_redirect) {
					WEAPON::REMOVE_ALL_PED_WEAPONS(player, true);
					WEAPON::REMOVE_ALL_PED_WEAPONS(player, false);
				}
				return false;
			}
		}



		if (Hooking::e_ptfx) {
			if (eventId == NETWORK_PTFX_EVENT) {
				if (Hooking::e_notify) {
					ostringstream oss; oss << "~r~" << PLAYER::GET_PLAYER_NAME(Features::index_to_id(player)) << " tried to show particles.";
					notifyMap(oss.str());
				}

				if (Hooking::e_redirect) {

				}
				return false;
			}
		}

		if (Hooking::e_stat) {
			if (eventId == NETWORK_BANK_REQUEST_EVENT || eventId == NETWORK_INCREMENT_STAT_EVENT) {
				if (Hooking::e_notify) {
					ostringstream oss; oss << "~r~" << PLAYER::GET_PLAYER_NAME(Features::index_to_id(player)) << " tried to change your stats.";
					notifyMap(oss.str());
				}

				if (Hooking::e_redirect) {

				}
				return false;
			}
		}

		if (Hooking::e_report) {
			if (eventId == PLAYER_CARD_STAT_EVENT || eventId == REPORT_MYSELF_EVENT || eventId == REPORT_CASH_SPAWN_EVENT) {
				if (Hooking::e_notify) {
					ostringstream oss; oss << "~r~" << PLAYER::GET_PLAYER_NAME(Features::index_to_id(player)) << " tried to report you.";
					notifyMap(oss.str());
				}

				if (Hooking::e_redirect) {

				}
				return false;
			}
		}

		if (Hooking::e_kick) {
			globalHandle(1377446).At(594).As<int>() = 594; //SP kick
			globalHandle(1377446).At(595).As<int>() = 595; //kick
			globalHandle(1382674).At(584).As<int>() = 584; //superkick IV
			if (eventId == REMOTE_SCRIPT_LEAVE_EVENT || eventId == KICK_VOTES_EVENT) {
				if (Hooking::e_notify) {
					ostringstream oss; oss << "~r~" << PLAYER::GET_PLAYER_NAME(Features::index_to_id(player)) << " tried to kick you.";
					notifyMap(oss.str());
				}

				if (Hooking::e_redirect) {
					Features::kick2(Features::index_to_id(player));
				}
				return false;
			}
		}

		if (Hooking::e_explosion) {
			if (eventId == FIRE_EVENT || eventId == EXPLOSION_EVENT || eventId == REQUEST_PHONE_EXPLOSION_EVENT || eventId == BLOW_UP_VEHICLE_EVENT) {
				if (Hooking::e_notify) {
					ostringstream oss; oss << "~r~" << PLAYER::GET_PLAYER_NAME(Features::index_to_id(player)) << " tried to explode you.";
					notifyMap(oss.str());
				}

				if (Hooking::e_redirect) {
					Vector3 crd = coordsOf(player);
					FIRE::ADD_EXPLOSION(crd.x, crd.y, crd.z, 0, 5.0f, true, false, 0.1f);
				}
				return false;
			}
		}

		if (Hooking::e_freeze) {
			if (eventId == RAGDOLL_REQUEST_EVENT || eventId == INCIDENT_ENTITY_EVENT || eventId == CLEAR_AREA_EVENT || eventId == CLEAR_RECTANGLE_AREA_EVENT || eventId == NETWORK_CLEAR_PED_TASKS_EVENT || eventId == NETWORK_SOUND_CAR_HORN_EVENT || eventId == NETWORK_PLAY_SOUND_EVENT || eventId == NETWORK_STOP_SOUND_EVENT || eventId == NETWORK_PLAY_AIRDEFENSE_FIRE_EVENT) {
				if (Hooking::e_notify) {
					ostringstream oss; oss << "~r~" << PLAYER::GET_PLAYER_NAME(Features::index_to_id(player)) << " tried to freeze you.";
					notifyMap(oss.str());
				}

				if (Hooking::e_redirect) {
					AI::CLEAR_PED_TASKS_IMMEDIATELY(player);
					AI::CLEAR_PED_TASKS(player);
					AI::CLEAR_PED_SECONDARY_TASK(player);
				}
				return false;
			}
		}

	}
}



uint64_t	CPattern::find_pattern(uint64_t address, uint64_t end, BYTE *btMask, char *szMask)
{
	size_t len = strlen(szMask) + 1;
	for (uint64_t i = 0; i < (end - address - len); i++)
	{
		BYTE*	ptr = (BYTE*)(address + i);
		if (byte_compare(ptr, btMask, szMask))
		{
			m_result.push_back(CPatternResult((void*)(address + i)));
			return address + i;
		}
	}
	return NULL;
}

uint64_t	CPattern::virtual_find_pattern(uint64_t address, BYTE *btMask, char *szMask)
{
	MEMORY_BASIC_INFORMATION mbi;
	char*	pStart = nullptr;
	char*	pEnd = nullptr;
	char*	res = nullptr;
	size_t	maskLen = strlen(szMask);

	while (res == nullptr && sizeof(mbi) == VirtualQuery(pEnd, &mbi, sizeof(mbi)))
	{
		pStart = pEnd;
		pEnd += mbi.RegionSize;
		if (mbi.Protect != PAGE_READWRITE || mbi.State != MEM_COMMIT)
			continue;

		for (int i = 0; pStart < pEnd - maskLen && res == nullptr; ++pStart)
		{
			if (byte_compare((BYTE*)pStart, btMask, szMask))
			{
				m_result.push_back(CPatternResult((void*)pStart, mbi.BaseAddress, pEnd));
				res = pStart;
			}
		}

		mbi = {};
	}
	return (uint64_t)res;
}

void	failPat(const char* name)
{
	Log::Fatal("Failed to find %s pattern.", name);
	exit(0);
}

void mem_nop2(void* ptr, int size)
{
	memset(ptr, 0x90, size);
}

template <typename T>
void	setPat
(
	const char*	name,
	char*		pat,
	char*		mask,
	T**			out,
	bool		rel,
	int			offset = 0,
	int			deref = 0,
	int			skip = 0
)
{
	T*	ptr = nullptr;

	CPattern pattern(pat, mask);
	pattern.find(1 + skip);
	if (rel)
		ptr = pattern.get(skip).get_rel<T>(offset);
	else
		ptr = pattern.get(skip).get<T>(offset);

	while (true)
	{
		if (ptr == nullptr)
			failPat(name);

		if (deref <= 0)
			break;
		ptr = *(T**)ptr;
		--deref;
	}

	*out = ptr;
	return;
}

template <typename T>
void	setFn
(
	const char*	name,
	char*		pat,
	char*		mask,
	T*			out,
	int			skip = 0
)
{
	char*	ptr = nullptr;

	CPattern pattern(pat, mask);
	pattern.find(1 + skip);
	ptr = pattern.get(skip).get<char>(0);

	if (ptr == nullptr)
		failPat(name);

	*out = (T)ptr;
	return;
}

void Hooking::FindPatterns()
{
	HANDLE steam = GetModuleHandleA("steam_api64.dll");
	Hooking::PickupRewardMoneyIsApplicableToPlayer = static_cast<fpPickupRewardMoneyIsApplicableToPlayer>(Memory::pattern("49 8B 80 ? ? ? ? 48 85 C0 74 0C F6 80 ? ? ? ? ? 75 03").count(1).get(0).get<void>(0));

	Hooking::NtQueryVirtualMemory = (fpNtQueryVirtualMemory)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtQueryVirtualMemory");
	auto p_activeThread = pattern("E8 ? ? ? ? 48 8B 88 10 01 00 00");
	auto p_blipList = pattern("4C 8D 05 ? ? ? ? 0F B7 C1");
	auto p_fixVector3Result = pattern("83 79 18 00 48 8B D1 74 4A FF 4A 18");
	auto p_gameLegals = pattern("72 1F E8 ? ? ? ? 8B 0D");
	auto p_gameLogos = pattern("70 6C 61 74 66 6F 72 6D 3A");
	auto p_gameState = pattern("83 3D ? ? ? ? ? 8A D9 74 0A");
	auto p_modelCheck = pattern("48 85 C0 0F 84 ? ? ? ? 8B 48 50");
	auto p_modelSpawn = pattern("48 8B C8 FF 52 30 84 C0 74 05 48");
	//auto p_modelCheck = pattern("\x48\x85\xC0\x0F\x84\x00\x00\x00\x00\x8B\x48\x50");
	//auto p_modelSpawn = pattern("48 8B C8 FF 52 30 84 C0 74 05 48");
	CPattern modelCheckPattern("\x48\x85\xC0\x0F\x84\x00\x00\x00\x00\x8B\x48\x50",
		"xxxxx????xxx");

	auto fickdiochopder = pattern("48 85 C0 0F 84 ? ? ? ? 8B 48 50");
	CPattern modelSpawnPattern("\x48\x8B\xC8\xFF\x52\x30\x84\xC0\x74\x05\x48",
		"xxxxxxxxxxx");
	auto p_nativeTable = pattern("76 32 48 8B 53 40 48 8D 0D");
	//auto p_nativeTable = pattern("76 ? ? 8b ? ? 48 8d 0d ? ? ? ? ? 8b ? ? ?");
	auto p_worldPtr = pattern("48 8B 05 ? ? ? ? 45 ? ? ? ? 48 8B 48 08 48 85 C9 74 07");
	auto p_globalPtr = pattern("4C 8D 05 ? ? ? ? 4D 8B 08 4D 85 C9 74 11");
	auto p_eventHook = pattern("48 83 EC 28 E8 ? ? ? ? 48 8B 0D ? ? ? ? 4C 8D 0D ? ? ? ? 4C 8D 05 ? ? ? ? BA 03");
	Hooking::handle_ptr = reinterpret_cast<decltype(handle_ptr)>(Memory::pattern("83 F9 FF 74 31 4C 8B 0D ? ? ? ?").count(1).get(0).get<char>(0));
	Hooking::GetScriptHandlerIfNetworked = static_cast<fpGetScriptHandlerIfNetworked>(Memory::pattern("40 53 48 83 EC 20 E8 ? ? ? ? 48 8B D8 48 85 C0 74 12 48 8B 10 48 8B C8").count(1).get(0).get<void>(0));
	Hooking::GetScriptHandler = static_cast<fpGetScriptHandler>(Memory::pattern("48 83 EC 28 E8 ? ? ? ? 33 C9 48 85 C0 74 0C E8 ? ? ? ? 48 8B 88 ? ? ? ?").count(1).get(0).get<void>(0));

	setPat<uint64_t>("frame count",
		"\x8B\x15\x00\x00\x00\x00\x41\xFF\xCF",
		"xx????xxx",
		&Hooking::m_frameCount,
		true,
		2);

	setFn<fpIsDLCPresent>("is_DLC_present",
		"\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x20\x81\xF9\x00\x00\x00\x00",
		"xxxx?xxxxxxx????",
		&Hooking::is_DLC_present);
	HookMsg("IDP", true);

	setFn<fpStatSetBool>("stat_set_bool",
		"\x48\x89\x5C\x24\x00\x88\x54\x24\x10\x57\x48\x83\xEC\x40",
		"xxxx?xxxxxxxxx",
		&Hooking::stat_set_bool);
	HookMsg("GSHIN", true);


	setFn<fpStatSetFloat>("stat_set_float",
		"\x48\x89\x5C\x24\x00\xF3\x0F\x11\x4C\x24\x00\x57\x48\x83\xEC\x40",
		"xxxx?xxxxx?xxxxx",
		&Hooking::stat_set_float);
	HookMsg("GSH");
	setFn<fpStatSetInt>("stat_set_int",
		"\x89\x54\x24\x10\x55\x53\x56\x57\x41\x57",
		"xxxxxxxxxx",
		&Hooking::stat_set_int);
	HookMsg("DCE", true);

	/*setPat<void>("name changer", "\x48\x8B\x05\x00\x00\x00\x00\xC3\x8A\xD1", "xxx????xxx", &Hooking::m_onlineName, true, 3, 1);
	setFn<fpGetPlayerName>("get_player_name",
		"\x40\x53\x48\x83\xEC\x20\x80\x3D\x00\x00\x00\x00\x00\x8B\xD9\x74\x22",
		"xxxxxxxx?????xxxx",
		&Hooking::get_player_name); */

	setFn<fpCreateAmbientPickup>("create_ambient_pickup",
		"\x40\x53\x48\x83\xEC\x20\x80\x3D\x00\x00\x00\x00\x00\x8B\xD9\x74\x22",
		"xxxxxxxx?????xxxx",
		&Hooking::create_ambient_pickup);
	HookMsg("OEB", true);

	setFn<fpGetEventData>("get_event_data",
		"\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x49\x8B\xF8\x4C\x8D\x05",
		"xxxx?xxxx?xxxxxx",
		&Hooking::get_event_data);
	HookMsg("TSE");


	setFn<fpGetWaypointPos>("get_waypoint_pos",
		"\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x8B\x15\x00\x00\x00\x00\x65\x48\x8B\x04\x25\x00\x00\x00\x00\x48\x8B\xF9\x48\x8B\x04\xD0\xB9\xB4",
		"xxxxxxxxxxxx????xxxxx????xxxxxxxxx",
		&Hooking::get_waypoint_pos);
	HookMsg("SLW");


	setFn<fpEventHandler>("scripted_game_event",
		"\x66\x41\x83\xF9\x00\x0F\x83\x00\x00\x00\x00",
		"xxxx?xx????",
		&Hooking::event_handler);
	HookMsg("SSI", true);

	//create_ambient_pickup
	/*setPat<fpCreateAmbientPickup>("create_ambient_pickup",
	"\x48\x8B\xC4\x48\x89\x58\x08\x48\x89\x68\x10\x48\x89\x70\x18\x48\x89\x78\x20\x41\x56\x48\x83\xEC\x60\x80\x3D",
	"xxxxxxxxxxxxxxxxxxxxxxxxxxx",
	"\x54\x58\x48\x89\x58\x08\x48\x89\x68\x10\x48\x89\x70\x18\x48\x89\x78\x20\x4C\x89\x74\x24\x00\x48\x8D\x64\x24\x00\x48\x83\xEC\x60",
	"xxxxxxxxxxxxxxxxxxxxxx?xxxx?xxxx",
	&Hooking::create_ambient_pickup);*/

	//get_player_ped
	/*setFn<fpGetPlayerPed>("get_player_ped",
	"\x40\x53\x48\x83\xEC\x20\x33\xDB\x83\xF9\xFF\x74\x09\xB2\x01\xE8\x00\x00\x00\x00\xEB\x16",
	"xxxxxxxxxxxxxxxx????xx",
	&Hooking::get_player_ped);*/

	//player_id
	/*setFn<fpPlayerId>("player_id",
	"\x48\x83\xEC\x28\x80\x3D\x00\x00\x00\x00\x00\x74\x12\x48\x8B\x0D\x00\x00\x00\x00\xE8",
	"xxxxxx?????xxxxx????x",
	&Hooking::player_id);*/
	Hooking::GetLabelText = static_cast<fpGetLabelText>(Memory::pattern("48 89 5C 24 ? 57 48 83 EC 20 48 8B DA 48 8B F9 48 85 D2 75 44 E8").count(1).get(0).get<void>(0));


	/*setFn<fpSetSessionTime>("session_time_set",
		"\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x8B\xF9\x48\x8B\x0D\x00\x00\x00\x00\x48\x8B\xDA\x33\xD2\xE9\x00\x00\x00\x00",
		"xxxxxxxxxxxxxxx????xxxxxx????",
		&Hooking::set_session_time_info);*/

	setPat<clockTime>("clock_time",
		"\x48\x8D\x0D\x00\x00\x00\x00\x8B\xFA\xE8\x00\x00\x00\x00\x44\x8D\x0C\x5B",
		"xxx????xxx????xxxx",
		&Hooking::ClockTime,
		true, // yes
		3);

	/*setFn<fpSetSessionTime>("session_time_set",
		"\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x8B\xF9\x48\x8B\x0D\x00\x00\x00\x00\x48\x8B\xDA\x33\xD2\xE9\x00\x00\x00\x00",
		"xxxxxxxxxxxxxxx????xxxxxx????",
		&Hooking::set_session_time_info);*/

		/*setFn<fp_CreateObject>("create_object",
			"\xE9\x00\x00\x00\x00\x48\x85\xC0\x55\x48\xBD\xD8\x00\x00\x00\x00\x00\x00\x00\x48\x87\x2C\x24\x53\x50\x48\x8B\x5C\x24\x10\x48\xB8\x00\x00\x00\x00\x00\x00\x00\x00\x48\x0F\x44\xD8\x48\x89\x5C\x24\x10\x58\x5B\xC3\xE9\x00\x00\x00\x00\x6E\x15\x00\x00\x00\x00\xE8", "x????xxxxxxx???????xxxxxxxxxxxxx????????xxxxxxxxxxxxx????xx????x",
			&Hooking::create__object);*/

			/*setFn<fpNetworkIsSessionStarted>("network_is_session_started",
				"\x48\x8B\x0D\x00\x00\x00\x00\x33\xC0\x48\x85\xC9\x74\x0E\x83\xB9\x00\x00\x00\x00\x00\x75\x05",
				"xxx????xxxxxxxxx?????xx",
				&Hooking::network_is_session_started);*/
	char * c_location = nullptr;
	void * v_location = nullptr;

	Sleep(5000);

	Log::Msg("Getting Game State...");
	c_location = p_gameState.count(1).get(0).get<char>(2);
	c_location == nullptr ? FailPatterns("gameState") : m_gameState = reinterpret_cast<decltype(m_gameState)>(c_location + *(int32_t*)c_location + 5);

	Log::Msg("Getting vector3 result fixer func...");
	v_location = p_fixVector3Result.count(1).get(0).get<void>(0);
	if (v_location != nullptr) scrNativeCallContext::SetVectorResults = (void(*)(scrNativeCallContext*))(v_location);

	Log::Msg("Getting native registration table...");
	c_location = p_nativeTable.count(1).get(0).get<char>(9);
	c_location == nullptr ? FailPatterns("native registration table") : m_registrationTable = reinterpret_cast<decltype(m_registrationTable)>(c_location + *(int32_t*)c_location + 4);

	Log::Msg("Getting World Pointer...");
	c_location = p_worldPtr.count(1).get(0).get<char>(0);
	c_location == nullptr ? FailPatterns("world Pointer") : m_worldPtr = reinterpret_cast<uint64_t>(c_location) + *reinterpret_cast<int*>(reinterpret_cast<uint64_t>(c_location) + 3) + 7;

	Log::Msg("Getting Blip List...");
	c_location = p_blipList.count(1).get(0).get<char>(0);
	c_location == nullptr ? FailPatterns("blip List") : m_blipList = (BlipList*)(c_location + *reinterpret_cast<int*>(c_location + 3) + 7);


	Log::Msg("Bypassing Object restrictions..");

	// Online model bypass


	if (p_modelCheck.size() > 0) {

		v_location = p_modelCheck.count(1).get(0).get<void>(0);
		Memory::nop(v_location, 24);
	}

	if (p_modelSpawn.size() > 0) {

		v_location = p_modelSpawn.count(1).get(0).get<void>(8);
		Memory::nop(v_location, 2);
	}

	Log::Msg("Getting active script thread...");
	c_location = p_activeThread.count(1).get(0).get<char>(1);
	c_location == nullptr ? FailPatterns("Active Script Thread") : GetActiveThread = reinterpret_cast<decltype(GetActiveThread)>(c_location + *(int32_t*)c_location + 4);

	Log::Msg("Getting World Pointer...");
	c_location = p_globalPtr.count(1).get(0).get<char>(0);
	__int64 patternAddr = NULL;
	c_location == nullptr ? FailPatterns("globalTable") : patternAddr = reinterpret_cast<decltype(patternAddr)>(c_location);
	m_globalPtr = (__int64**)(patternAddr + *(int*)(patternAddr + 3) + 7);

	Log::Msg("Getting Event Hook...");
	if ((c_location = p_eventHook.count(1).get(0).get<char>(0)))
	{
		int i = 0, j = 0, matches = 0, found = 0;
		char* pattern = "\x4C\x8D\x05";
		while (found != EVENT_COUNT)
		{
			if (c_location[i] == pattern[j])
			{
				if (++matches == 3)
				{
					EventPtr.push_back((void*)(reinterpret_cast<uint64_t>(c_location + i - j) + *reinterpret_cast<int*>(c_location + i + 1) + 7));
					found++;
					j = matches = 0;
				}
				j++;
			}
			else
			{
				matches = j = 0;
			}
			i++;
		}
	}

	Log::Msg("Initializing natives...");
	CrossMapping::initNativeMap();

	Log::Msg("Checking if GTA V is ready...");
	while (*m_gameState != GameStatePlaying) {
		Sleep(200);
	}
	Log::Msg("GTA V ready!");


}

static Hooking::NativeHandler _Handler(uint64_t origHash)
{
	uint64_t newHash = CrossMapping::MapNative(origHash);
	if (newHash == 0)
	{
		return nullptr;
	}

	Hooking::NativeRegistrationNew * table = m_registrationTable[newHash & 0xFF];

	for (; table; table = table->getNextRegistration())
	{
		for (uint32_t i = 0; i < table->getNumEntries(); i++)
		{
			if (newHash == table->getHash(i))
			{
				return table->handlers[i];
			}
		}
	}
	return nullptr;
}

Hooking::NativeHandler Hooking::GetNativeHandler(uint64_t origHash)
{
	auto& handler = m_handlerCache[origHash];

	if (handler == nullptr)
	{
		handler = _Handler(origHash);
	}

	return handler;
}

eGameState Hooking::GetGameState()
{
	return *m_gameState;
}

BlipList* Hooking::GetBlipList()
{
	return m_blipList;
}

uint64_t Hooking::getWorldPtr()
{
	return m_worldPtr;
}
void WAIT(DWORD ms)
{
	wakeAt = timeGetTime() + ms;
	SwitchToFiber(mainFiber);
}

void Hooking::Cleanup()
{
	Log::Msg("Cleaning up hooks");
	for (int i = 0; i < m_hooks.size(); i++)
		if (MH_DisableHook(m_hooks[i]) != MH_OK && MH_RemoveHook(m_hooks[i]) != MH_OK)
			Log::Error("Failed to unhook %p", (void*)m_hooks[i]);
	MH_Uninitialize();

	FreeLibraryAndExitThread(static_cast<HMODULE>(_hmoduleDLL), 1);
}

void Hooking::defuseEvent(RockstarEvent e, bool toggle)
{
	static const unsigned char retn = 0xC3;
	char* p = (char*)EventPtr[e];
	if (toggle)
	{
		if (EventRestore[e] == 0)
			EventRestore[e] = p[0];
		*p = retn;
	}
	else
	{
		if (EventRestore[e] != 0)
			*p = EventRestore[e];
	}
}

__int64** Hooking::getGlobalPtr()
{
	return m_globalPtr;
}
__int64 * Hooking::getGlobalPtr1(int index)
{
	return nullptr;
}
__int64 * Hooking::setTunable(int Tunable)
{
	return nullptr;
}



/*10 Mill Hooking by R!OT


bool Hooking::HookNatives()
{
auto p_framecount = pattern("8B 15 ? ? ? ? 41 FF CF");
char * frameCountPtr =
p_frameCount.count(1).get(0).get<char>(2);
int32_t rel = (int32_t)frameCountPtr;
frameCount = (unit32_t*)(frameCountPtr + rel +												//have to code alot dont touch this
sizeof(rel));
setFn<fpIsEntityDead>("is_entity_dead",
"\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\
xEC\x20\x40\xx8A\xFA\xE8\x00\x00\x00\x00\x33\xD8",
"xxxx?xxxx?xxxxxxxxx????xx",
&Hooking::is_entity_dead);
Log::Msg("fpIsEntityDead %p", is_entity_dead);

MH_STATUS status = MH_CreateHook(is_entity_dead,
HK_IS_ENTITY_DEAD, (void**)&OG_IS_ENTITY_DEAD);
if ((status != MH_OK || status !=
MH_ERROR_ALREAD_CREATED) && (MH_EnableHook(is - entity_dead) !=
MH_OK)) {
Log::Error("Failed to create hook %d", status) :
return false;
}
else {
m_hookNative.push_back(is_entity_dead);
return true;
}
}
ato p_beginService = pattern("48 89 5C 24 ? 48 89 6C 24 ? 48
89 74 24 ? 57 41 56 41 57 48 83 EC 50 45 8B F1")
fpNetworkShopBeginsService resolvedFunc1 = (fpNetworkShopBeginService)
p_beginService.count(1).get(0).get<void>(0);
Log::Msg("fpNetworkShaopBeginService %p", resolvedFunc1);

MH_STATUS status1 = MH_CreateHook(resolvedFunc1,
Hooking::HK_BEGIN_SERVICE, (void**)&OG_BEGIN_SERVICE);
if ((status1 != MH_OK && status !=
MH_ERROR_ALREDY_CREATED)) {
Log::Error("Failed to create hook %d", status1);
return false;
}*/





























