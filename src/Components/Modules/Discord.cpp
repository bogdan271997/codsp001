#include "STDInc.hpp"
#include <discord_rpc.h>

namespace Components
{
	DiscordRichPresence discord_presence;
	Discord::DiscordCustomInformation_t Discord::DiscordCustomInformation;
	bool Discord_Presence_Initialized = false;

	static const char* stock_sp_map_names[] =
	{
		"killhouse",
		"cargoship",
		"coup",
		"blackout",
		"armada",
		"bog_a",
		"hunted",
		"ac130",
		"bog_b",
		"airlift",
		"aftermath",
		"village_assault",
		"scoutsniper",
		"sniperescape",
		"village_defend",
		"ambush",
		"icbm",
		"launchfacility_a",
		"launchfacility_b",
		"jeepride",
		"simplecredits",
		"airplane",
		// New maps.
		"blackout_sh",
		"co_backlot",
		"co_backlot2",
		"co_boneyard2",
		"flashback_sniperescape",
		"flashback_sniperescape_demo",
		"so_arm",
		"so_blackout",
		"so_hidden_2",
		"so_htd",
		"zm_backlot",
		"zm_backlot_night",
		"zm_bloc",
		"zm_bloc_night",
		"zm_boneyard2",
		"zm_cod5_castle",
		"zm_cod5_prototype2",
		"zm_quarry",
		"zm_quarry_night",
		"zm_rust",
		"zm_shipment",
		"zm_shipment_night",
		"zm_test",
		"zm_test_waw",
		"zombie_blackout",
	};

	bool InMainMenu(std::string map_name_info)
	{
		if (map_name_info == "") return true;
		else return false;
	}

	bool VanillaMap(std::string map_name_info)
	{
		for (const auto& map_name : stock_sp_map_names)
		{
			if (map_name == map_name_info)
				return true;
		}
		return false;
	}

	std::string GetDifficultyForState(std::string map_name_info)
	{
		if (!InMainMenu(map_name_info))
		{
			const auto difficulty = Dvars::Functions::Dvar_FindVar("g_gameskill")->current.integer;
			if (difficulty == 0)
				return "MENU_SP_DISCORD_DIFFICULTY_EASY";
			else if (difficulty == 1)
				return "MENU_SP_DISCORD_DIFFICULTY_MEDIUM";
			else if (difficulty == 2)
				return "MENU_SP_DISCORD_DIFFICULTY_HARD";
			else if (difficulty == 3 /*&& Dvars::Functions::Dvar_FindVar("chaos_mode")->current.string == "0"s*/)
				return "MENU_SP_DISCORD_DIFFICULTY_FU";
			//else if (difficulty == 3 && Dvars::Functions::Dvar_FindVar("chaos_mode")->current.string == "1"s)
			//	return "MENU_SP_DISCORD_DIFFICULTY_CHAOS";
			else
				return "MENU_SP_DISCORD_DIFFICULTY_UNKNOWN";
		}
		else
			return "";
	}

	std::string GetMapName(std::string map_name_info)
	{
		if (VanillaMap(map_name_info))
		{
			if(map_name_info.contains("ac130") && Dvars::Functions::Dvar_FindVar("credits_active")->current.string == "1"s)
				return "MENU_SP_DISCORD_SIMPLECREDITS";
			else if(map_name_info.contains("iw4_credits") && Dvars::Functions::Dvar_FindVar("credits_active")->current.string == "1"s)
				return "MENU_SP_DISCORD_SIMPLECREDITS";
			return std::string("MENU_SP_DISCORD_") + map_name_info.data();
		}
		else
		{
			if (InMainMenu(map_name_info)) 
			{
				//if (Dvars::Functions::Dvar_FindVar("gameModeSO")->current.string == "true"s)
				//	return "MENU_SP_DISCORD_MAINMENU_SO";
				//else if (Dvars::Functions::Dvar_FindVar("gameModeSP")->current.string == "true"s)
				//	return "MENU_SP_DISCORD_MAINMENU_SP";
				//else if (Dvars::Functions::Dvar_FindVar("gameModeZM")->current.string == "true"s)
				//	return "MENU_SP_DISCORD_MAINMENU_ZM";
				//else
					return "MENU_SP_DISCORD_MAINMENU";
			}
			else 
			{
				return "custom_map";
			}
		}
	}

	void Discord::DiscordUpdate()
	{
		Discord_RunCallbacks();

		std::string map_name_offset = reinterpret_cast<const char*>(0x1290DD8);
		std::string mapname = GetMapName(map_name_offset.data());
		std::string difficulty = GetDifficultyForState(map_name_offset.data());

		if (InMainMenu(map_name_offset.data()))
		{
			DiscordCustomInformation.details.reset();
			DiscordCustomInformation.state.reset();
			DiscordCustomInformation.button = std::make_pair("", "");
			DiscordCustomInformation.large_image_key.reset();
			discord_presence.largeImageKey = "preview_mainmenu";
		}
		else
		{
			if (DiscordCustomInformation.large_image_key.has_value())
			{
				const auto& imageUrl = DiscordCustomInformation.large_image_key.value();
				discord_presence.largeImageKey = imageUrl.data();
			}
			else
			{
				if (VanillaMap(map_name_offset.data()))
				{
					std::string imageKey = std::string("preview_") + map_name_offset.data();
					discord_presence.largeImageKey = imageKey.data();
				}
				else discord_presence.largeImageKey = nullptr;
			}
		}

		if (DiscordCustomInformation.details.has_value())
		{
			const auto& details_value = DiscordCustomInformation.details.value();
			if (details_value.starts_with("@") && details_value.size() > 1)
			{
				const auto loc_string = details_value.substr(1);
				const auto value = Game::UI_SafeTranslateString(loc_string.data());
				discord_presence.details = value;
			}
			else
			{
				discord_presence.details = details_value.data();
			}
		}
		else
		{
			if(mapname.contains("custom_map"))
				discord_presence.details = Game::UI_ReplaceConversionString(Game::UI_SafeTranslateString("MENU_SP_DISCORD_MAPUNKNOWN"), map_name_offset.c_str());
			else
				discord_presence.details = Game::UI_SafeTranslateString(mapname.data());
		}

		if (DiscordCustomInformation.state.has_value())
		{
			const auto& state_value = DiscordCustomInformation.state.value();
			if (state_value.starts_with("@") && state_value.size() > 1)
			{
				const auto loc_string = state_value.substr(1);
				const auto value = Game::UI_SafeTranslateString(loc_string.data());
				discord_presence.state = value;
			}
			else discord_presence.state = state_value.data();
		}
		else
		{
			if (difficulty.data()) discord_presence.state = Game::UI_SafeTranslateString(difficulty.data());
			else discord_presence.state = nullptr;
		}

		if (!DiscordCustomInformation.button.first.empty() && !DiscordCustomInformation.button.second.empty())
		{
			const auto& buttonLabel = DiscordCustomInformation.button.first;
			const auto& buttonUrl = DiscordCustomInformation.button.second;
			if (buttonLabel.starts_with("@") && buttonLabel.size() > 1)
			{
				const auto loc_string = buttonLabel.substr(1);
				const auto value = Game::UI_SafeTranslateString(loc_string.data());
				discord_presence.button2Label = value;
			}
			else discord_presence.button2Label = buttonLabel.data();

			discord_presence.button2Url = buttonUrl.data();
		}
		else
		{
			discord_presence.button2Label = nullptr;
			discord_presence.button2Url = nullptr;
		}

		if (!discord_presence.startTimestamp)
		{
			discord_presence.startTimestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		}

		Discord_UpdatePresence(&discord_presence);
	}

	static void ready([[maybe_unused]] const DiscordUser* request)
	{
		ZeroMemory(&discord_presence, sizeof(discord_presence));

		discord_presence.instance = 1;
		discord_presence.state = "";
		discord_presence.button1Label = "About Modern Operations";
		discord_presence.button1Url = "https://discord.gg/28sARKGhcK";

		Discord_UpdatePresence(&discord_presence);
	}

	static void Errored(const int error_code, const char* message)
	{
		Game::Com_Printf(0, "Discord: (%i) %s", error_code, message);
	}

	void Discord::DiscordInit()
	{
		if (EnableDiscord_RPC && !Discord_Presence_Initialized)
		{
			DiscordEventHandlers handlers;
			ZeroMemory(&handlers, sizeof(handlers));
			handlers.ready = ready;
			handlers.errored = Errored;
			handlers.disconnected = Errored;
			handlers.joinGame = nullptr;
			handlers.spectateGame = nullptr;
			handlers.joinRequest = nullptr;

			// Public key d13d2f050ae693d9af2414920039fedbe3b072c9ec633bc979e42e89351ac115
			#ifdef MODERN_OPS
				Discord_Initialize("959373007611125780", &handlers, 1, nullptr);
			#endif

			// Public key cf791458016328fad4f2027f3e42a71e4b45ec0944d645cc770c48dfa9ac0046
			#ifdef MW1CR
				Discord_Initialize("958417241014153246", &handlers, 1, nullptr);
			#endif

			// Public key 80f17c8855f5642ef5adbf0d27d811edaa26e0b0fd81d226e910c886ef769ab6
			#ifdef CoD2R
				Discord_Initialize("1014842436574515300", &handlers, 1, nullptr);
			#endif

			Discord_Presence_Initialized = true;

			Scheduler::Loop(Discord::DiscordUpdate, Scheduler::Pipeline::MAIN, 5s);
		}
	}

	void Discord::DiscordShutdown()
	{
		if (Discord_Presence_Initialized)
		{
			Discord_Shutdown();
			Discord_Presence_Initialized = false;
		}
	}

	Discord::Discord()
	{
		Discord::DiscordInit();



		// Level Functions.
		
		GSC::AddFunction("setDiscordDetails", []
		{
			if (Game::Scr_GetNumParam() != 1)
			{
				Game::Scr_Error("setDiscordDetails ( <string> detail ) requires 1 string parameter");
				return;
			}

			const char* details = Game::Scr_GetString(0);
			DiscordCustomInformation.details.emplace(details);
			Scheduler::Once(Discord::DiscordUpdate, Scheduler::Pipeline::ASYNC);
			Game::Scr_AddInt(true);
		}, false);

		GSC::AddFunction("setDiscordState", []
		{
			if (Game::Scr_GetNumParam() != 1)
			{
				Game::Scr_Error("setDiscordState ( <string> state ) requires 1 string parameter");
				return;
			}

			const char* state = Game::Scr_GetString(0);
			DiscordCustomInformation.state.emplace(state);
			Scheduler::Once(Discord::DiscordUpdate, Scheduler::Pipeline::ASYNC);
			Game::Scr_AddInt(true);
		}, false);

		GSC::AddFunction("setButtonInformation", []
		{
			const char* label = Game::Scr_GetString(0);
			const char* url = Game::Scr_GetString(1);

			if (strlen(label) > 32)
			{
				Game::Scr_Error("URL label maximum is 32 characters");
				return;
			}

			DiscordCustomInformation.button.first = label;
			DiscordCustomInformation.button.second = url;
			Scheduler::Once(Discord::DiscordUpdate, Scheduler::Pipeline::ASYNC);
			Game::Scr_AddInt(true);
		}, false);

		GSC::AddFunction("setDiscordImage", []
		{
			if (Game::Scr_GetNumParam() != 1)
			{
				Game::Scr_Error("setdiscordimage ( <string> url ) requires 1 string parameter");
				return;
			}

			const char* imageLink = Game::Scr_GetString(0);
			DiscordCustomInformation.large_image_key.emplace(imageLink);
			Scheduler::Once(Discord::DiscordUpdate, Scheduler::Pipeline::ASYNC);
			Game::Scr_AddInt(true);
		}, false);
	}

	Discord::~Discord()
	{
		Discord::DiscordShutdown();
	}
}