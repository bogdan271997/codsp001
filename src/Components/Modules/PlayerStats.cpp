#include "STDInc.hpp"

namespace Components
{
	void PlayerStats::ChangeItemDefText(const char* menuName, const char* itemDefName, const char* value)
	{
		Game::menuDef_t* menu = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MENU, menuName).menu;
		if (!menu) return;

		for (auto i = 0; i < menu->itemCount; i++)
		{
			if (menu->items[i] && menu->items[i]->window.name)
			{
				if (!_stricmp(menu->items[i]->window.name, itemDefName))
				{
					menu->items[i]->text = value;
				}
			}
		}
	}

	const char* PlayerStats::getHighestCompletedDifficulty(const float stat_easy, const float stat_regular, const float stat_hardened, const float stat_veteran)
	{
		if (stat_easy == 0.0f || stat_regular == 0.0f || stat_hardened == 0.0f || stat_veteran == 0.0f)
			return "MENU_NOT_STARTED";
		if (stat_veteran == 100.0f) 
			return "MENU_VETERAN";
		else if (stat_hardened == 100.0f)
			return "MENU_HARDENED";
		else if (stat_regular == 100.0f)
			return "MENU_REGULAR";
		else if (stat_easy == 100.0f)
			return "MENU_RECRUIT";
		else
			return "MENU_IN_PROGRESS";
	}

	double PlayerStats::CalculateAccuracy(uint32_t bullets_fired, uint32_t bullets_hit)
	{
		if (bullets_fired != 0)
			return static_cast<double>(bullets_hit) / bullets_fired * 100.0;
		else
			return 0.0;
	}

	float PlayerStats::getStatProgression(const std::string& difficulty_string, int difficulty)
	{
		int levels = 0;
		size_t max_missions = std::min(difficulty_string.size(), static_cast<size_t>(21));

		for (size_t i = 0; i < max_missions; ++i) {
			if (static_cast<int>(difficulty_string[i] - '0') >= difficulty) {
				levels++;
			}
		}

		float completion = static_cast<float>(levels) / max_missions * 100;
		return completion;
	}

	float PlayerStats::getStatEasy(const std::string& difficulty_string)
	{
		return getStatProgression(difficulty_string, 1);
	}

	float PlayerStats::getStatRegular(const std::string& difficulty_string)
	{
		return getStatProgression(difficulty_string, 2);
	}

	float PlayerStats::getStatHardened(const std::string& difficulty_string)
	{
		return getStatProgression(difficulty_string, 3);
	}

	float PlayerStats::getStatVeteran(const std::string& difficulty_string)
	{
		return getStatProgression(difficulty_string, 4);
	}

	float PlayerStats::getStatIntel(int intelCount, int totalIntelItems)
	{
		return static_cast<float>(intelCount) / totalIntelItems * 100;
	}

	int PlayerStats::getTotalPercentCompleteSP()
	{
		float stat_easy = getStatEasy(Dvars::Functions::Dvar_FindVar("mis_difficulty")->current.string);
		float stat_regular = getStatRegular(Dvars::Functions::Dvar_FindVar("mis_difficulty")->current.string);
		float stat_hardened = getStatHardened(Dvars::Functions::Dvar_FindVar("mis_difficulty")->current.string);
		float stat_veteran = getStatVeteran(Dvars::Functions::Dvar_FindVar("mis_difficulty")->current.string);
		float stat_intel = getStatIntel(GetStruct("career", "intel"), 30);
		Achievements::achievement_file_t file{};
		Achievements::GetAchievementsData(&file);
		int achievements_earned = Achievements::GetEarnedAchievementCount(&file);

		// Calculate completion progress based on various factors
		float total_progress = 0.0;

		// Completion progress based on difficulty completion
		total_progress += (0.1 / 1) * stat_easy;
		total_progress += (0.1 / 1) * stat_regular;
		total_progress += (0.1 / 1) * stat_hardened;
		total_progress += (0.5 / 1) * stat_veteran;

		// Intel completion progress
		total_progress += (0.15 / 1) * stat_intel;

		// Achievement completion progress
		total_progress += (0.15 / 38) * achievements_earned;

		// Clamp total progress to 100%
		total_progress = std::min(total_progress, 100.0f);

		// Convert total progress to integer
		return static_cast<int>(total_progress);
	}

	uint32_t PlayerStats::CalculateDays(uint32_t totalMilliseconds) {
		return totalMilliseconds / (1000 * 60 * 60 * 24);
	}

	uint32_t PlayerStats::CalculateHours(uint32_t totalMilliseconds) {
		totalMilliseconds %= (1000 * 60 * 60 * 24);
		return totalMilliseconds / (1000 * 60 * 60);
	}

	uint32_t PlayerStats::CalculateMinutes(uint32_t totalMilliseconds) {
		totalMilliseconds %= (1000 * 60 * 60);
		return totalMilliseconds / (1000 * 60);
	}

	uint32_t PlayerStats::CalculateSeconds(uint32_t totalMilliseconds) {
		totalMilliseconds %= (1000 * 60);
		return totalMilliseconds / 1000;
	}

	void PlayerStats::DefaultStats()
	{
		const auto path = GetPlayerStatsPath();
		Utils::IO::RemoveFile(path);
	}

	std::string PlayerStats::GetPlayerStatsPath()
	{
		auto fileBuffer = Utils::IO::ReadFile("players/profiles/active.txt");
		if (fileBuffer.empty())
			return "";

		std::string game_folder = std::format("{}\\players\\profiles\\{}\\stats.json", Dvars::Functions::Dvar_FindVar("fs_basepath")->current.string, fileBuffer.c_str());
		return game_folder;
	}

	nlohmann::json PlayerStats::ReadPlayerStats()
	{
		const auto path = GetPlayerStatsPath();
		if (!Utils::IO::FileExists(path))
		{
			return {};
		}

		try
		{
			const auto data = Utils::IO::ReadFile(path);
			return nlohmann::json::parse(data);
		}
		catch (const std::exception& e)
		{
			Game::Com_Printf(0, "Failed to parse config file: %s\n", e.what());
			Utils::IO::WriteFile(path, "{}", false);
		}

		return {};
	}

	void PlayerStats::ResetData()
	{
		PlayerStats::DefaultStats();
	}

	void PlayerStats::WriteData(const nlohmann::json& json)
	{
		try
		{
			const auto path = GetPlayerStatsPath();
			const auto str = json.dump(4);
			Utils::IO::WriteFile(path, str, false);
		}
		catch (const std::exception& e)
		{
			Game::Com_Printf(0, "Failed to write config file: %s\n", e.what());
		}
	}

	void PlayerStats::PlayTimeStart()
	{
		Scheduler::Loop([]
		{
		    static uint32_t lastUpdateTime = timeGetTime();
		
		    uint32_t currentTime = timeGetTime();
		    uint32_t elapsedTime = currentTime - lastUpdateTime;
		
		    uint32_t playTimeSP = GetStruct("career", "playTimeSP");
		    playTimeSP += elapsedTime;
		    SetStruct("career", "playTimeSP", playTimeSP);
		
		    lastUpdateTime = currentTime;
		}, Scheduler::Pipeline::MAIN, 1000ms);
	}

	void __declspec(naked) PlayerStats::Com_Init_Try_Block_Function_Stub()
	{
		const static uint32_t retn_addr = 0x535220;
		__asm
		{
			mov     dword ptr ds:[0xF8E004], ebx;
			call	PlayTimeStart;
			jmp		retn_addr;
		}
	}

	PlayerStats::PlayerStats()
	{
		Command::Add("reset_stats", [](Command::Params*)
		{
			ResetData();
		});

		GSC::AddFunction("getStatsFromStruct", []
		{
			const auto json_struct = Game::Scr_GetString(0);
			const auto field = Game::Scr_GetString(1);
			const auto value = GetStruct(json_struct, field);

			return Game::Scr_AddInt(value);
		}, false);

		GSC::AddFunction("setStatsFromStruct", []
		{
			const auto json_struct = Game::Scr_GetString(0);
			const auto field = Game::Scr_GetString(1);
			const auto value = Game::Scr_GetInt(2);
			SetStruct(json_struct, field, value);
		}, false);

		UIScript::Add("get_career_info", []([[maybe_unused]] const UIScript::Token& token, [[maybe_unused]] const Game::uiInfo_s* info)
		{
			//stats vars
			uint32_t var_bullets_fired = GetStruct("career", "bullets_fired");
			uint32_t var_bullets_hit = GetStruct("career", "bullets_hit");
			uint32_t var_deaths = GetStruct("career", "deaths");
			uint32_t var_headshots = GetStruct("career", "headshots");
			uint32_t var_kills = GetStruct("career", "kills");
			uint32_t var_kills_melee = GetStruct("career", "kills_melee");
			uint32_t var_kills_explosives = GetStruct("career", "kills_explosives");
			uint32_t var_playTimeSP = GetStruct("career", "playTimeSP");
			uint32_t var_intel = GetStruct("career", "intel");
			uint32_t var_days = CalculateDays(var_playTimeSP);
			uint32_t var_hours = CalculateHours(var_playTimeSP);
			uint32_t var_minutes = CalculateMinutes(var_playTimeSP);
			uint32_t var_seconds = CalculateSeconds(var_playTimeSP);

			float stat_easy = getStatEasy(Dvars::Functions::Dvar_FindVar("mis_difficulty")->current.string);
			float stat_regular = getStatRegular(Dvars::Functions::Dvar_FindVar("mis_difficulty")->current.string);
			float stat_hardened = getStatHardened(Dvars::Functions::Dvar_FindVar("mis_difficulty")->current.string);
			float stat_veteran = getStatVeteran(Dvars::Functions::Dvar_FindVar("mis_difficulty")->current.string);

			Achievements::achievement_file_t file{};
			Achievements::GetAchievementsData(&file);
			int var_achievements_earned = Achievements::GetEarnedAchievementCount(&file);

			auto var_accuracy = CalculateAccuracy(var_bullets_fired, var_bullets_hit);

			int total_progress = getTotalPercentCompleteSP();

			// For menu showcase
			const char* kills = Utils::String::VA_NEW("%d", var_kills);
			const char* kills_melee = Utils::String::VA_NEW("%d", var_kills_melee);
			const char* kills_explosives = Utils::String::VA_NEW("%d", var_kills_explosives);
			const char* headshots = Utils::String::VA_NEW("%d", var_headshots);
			const char* deaths = Utils::String::VA_NEW("%d", var_deaths);
			const char* bullets_fired = Utils::String::VA_NEW("%d", var_bullets_fired);
			const char* bullets_hit = Utils::String::VA_NEW("%d", var_bullets_hit);
			const char* intel = Utils::String::VA_NEW("%d/%d", var_intel, 30);
			const char* timestamp = Utils::String::VA_NEW("%u:%02u:%02u:%02u", var_days, var_hours, var_minutes, var_seconds);
			const char* achievements_earned = Utils::String::VA_NEW("%d/%d", var_achievements_earned, Achievements::ACHIEVEMENT_TOTAL_COUNT);
			const char* accuracy = Utils::String::VA_NEW("%.0f%%", var_accuracy);
			const char* difficulty = getHighestCompletedDifficulty(stat_easy, stat_regular, stat_hardened, stat_veteran);
			const char* total_percent_sp = Utils::String::VA_NEW("%d%%", total_progress);

			ChangeItemDefText("stats", "kills", kills);
			ChangeItemDefText("stats", "headshots", headshots);
			ChangeItemDefText("stats", "kills_melee", kills_melee);
			ChangeItemDefText("stats", "kills_explosives", kills_explosives);
			ChangeItemDefText("stats", "deaths", deaths);
			ChangeItemDefText("stats", "accuracy", accuracy);
			//-----------------------------------------------------------
			ChangeItemDefText("stats", "playedtime", timestamp);
			//-----------------------------------------------------------
			ChangeItemDefText("stats", "difficulty", Game::UI_SafeTranslateString(difficulty));
			ChangeItemDefText("stats", "intels", intel);
			ChangeItemDefText("stats", "achievements", achievements_earned);
			//-----------------------------------------------------------
			ChangeItemDefText("stats", "campaign_progress", total_percent_sp);
			//-----------------------------------------------------------
		});
		
		Utils::Hook::Nop(0x53521A, 6);
		Utils::Hook(0x53521A, Com_Init_Try_Block_Function_Stub, HOOK_JUMP).install()->quick();
	}

	PlayerStats::~PlayerStats()
	{
	}
}