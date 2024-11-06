#include "STDInc.hpp"

namespace Components
{
	const char* CL_GetMaxXP()
	{
		const Game::StringTable* scoreInfoTable;
		Game::StringTable_GetAsset("sp/specopstable.csv", &scoreInfoTable);
		const char* maxrank = Game::StringTable_Lookup(scoreInfoTable, 1, "dev_so_test", 2);
		return Utils::String::VA("%s", maxrank);
	}

// Currently unused.
/*
	void CommonPatch::SV_CheatsStub()
	{
		Dvars::Override::DvarBoolOverride("sv_cheats",
			Dvars::Functions::Dvar_FindVar("sv_allowCheats")->current.enabled ? true : false,
			Dvars::Functions::Dvar_FindVar("sv_allowCheats")->current.enabled ? Game::none : Game::cheat_protected);
	}
*/

	void CommonPatch::DB_LoadCommonFastFiles()
	{
		int i = 0;
		Game::XZoneInfo XZoneInfoStack[7];// 6

		XZoneInfoStack[i].name = "code_post_gfx";
		XZoneInfoStack[i].allocFlags = Game::XZONE_FLAGS::XZONE_POST_GFX;
		XZoneInfoStack[i].freeFlags = Game::XZONE_FLAGS::XZONE_POST_GFX_FREE;
		++i;

		XZoneInfoStack[i].name = "code_pre_gfx";
		XZoneInfoStack[i].allocFlags = Game::XZONE_FLAGS::XZONE_POST_GFX;
		XZoneInfoStack[i].freeFlags = Game::XZONE_FLAGS::XZONE_POST_GFX_FREE;
		++i;

		// Not used.
		/*
		const char* languageName = Game::SEH_GetLanguageName(Dvars::Functions::Dvar_FindVar("loc_language")->current.unsignedInt);
		if (FastFiles::Exists(Utils::String::VA("%s_common_patch", languageName)))
		{
			XZoneInfoStack[i].name = Utils::String::VA("%s_common_patch", languageName);
			XZoneInfoStack[i].allocFlags = Game::XZONE_FLAGS::XZONE_MOD;
			XZoneInfoStack[i].freeFlags = Game::XZONE_FLAGS::XZONE_MOD_FREE;
			++i;

			//override the exists the fonts from code_post_gfx
			if (Language::GetCurrentLanguage() == "russian")
			{
				Game::DB_LoadXAssets(XZoneInfoStack, i, false);
				Game::R_BeginRemoteScreenUpdate();
				WaitForSingleObject(Game::databaseCompletedEvent, 0xFFFFFFFF);
				Game::R_EndRemoteScreenUpdate();
				i = 0;

				Utils::Hook::Set<const char*>(0x42033E, "decode_characters_64px");
				Utils::Hook::Set<const char*>(0x420365, "decode_characters_64px_glow");
			}
			else
			{
				Utils::Hook::Set<const char*>(0x42033E, "decode_characters");
				Utils::Hook::Set<const char*>(0x420365, "decode_characters_glow");
			}
		}

		if (FastFiles::Exists("modernops_mod"))
		{
			XZoneInfoStack[i].name = "modernops_mod";
			XZoneInfoStack[i].allocFlags = Game::XZONE_FLAGS::XZONE_MOD;
			XZoneInfoStack[i].freeFlags = Game::XZONE_FLAGS::XZONE_MOD_FREE;
			++i;
		}
		*/

		auto mod_exists = Utils::Hook::Call<char()>(0x45CC00)(); //DB_ModFileExists 0x45C920 from 1.0
		if (mod_exists)
		{
			// Unloading our mod except "loc_mod" file when mod.ff exists in mods/modName folder
			XZoneInfoStack[2].name = nullptr;
			XZoneInfoStack[2].allocFlags = Game::XZONE_FLAGS::XZONE_MOD;
			XZoneInfoStack[2].freeFlags = Game::XZONE_FLAGS::XZONE_MOD_FREE;


			XZoneInfoStack[i].name = "mod";
			XZoneInfoStack[i].allocFlags = Game::XZONE_FLAGS::XZONE_MOD;
			XZoneInfoStack[i].freeFlags = Game::XZONE_FLAGS::XZONE_MOD_FREE;
			++i;

			Game::DB_LoadXAssets(XZoneInfoStack, i, true);

			Game::R_BeginRemoteScreenUpdate();
			WaitForSingleObject(Game::databaseCompletedEvent, 0xFFFFFFFF);
			Game::R_EndRemoteScreenUpdate();
			i = 0;
		}

		XZoneInfoStack[i].name = "ui";
		XZoneInfoStack[i].allocFlags = Game::XZONE_FLAGS::XZONE_UI;
		XZoneInfoStack[i].freeFlags = Game::XZONE_FLAGS::XZONE_UI_FREE;
		++i;

		if (FastFiles::Exists("patch_common"))
		{
			XZoneInfoStack[i].name = "patch_common";
			XZoneInfoStack[i].allocFlags = Game::XZONE_FLAGS::XZONE_COMMON;
			XZoneInfoStack[i].freeFlags = Game::XZONE_FLAGS::XZONE_COMMON_FREE;
			++i;
		}

		XZoneInfoStack[i].name = "common";
		XZoneInfoStack[i].allocFlags = Game::XZONE_FLAGS::XZONE_COMMON;
		XZoneInfoStack[i].freeFlags = Game::XZONE_FLAGS::XZONE_COMMON_FREE;
		++i;

		Game::DB_LoadXAssets(XZoneInfoStack, i, true);

		Game::R_BeginRemoteScreenUpdate();
		WaitForSingleObject(Game::databaseCompletedEvent, 0xFFFFFFFF);
		Game::R_EndRemoteScreenUpdate();

		Game::Com_Printf(0, "Common .ff files loaded.\n");


		// CoD2R dvars.
		#ifdef CoD2R
			Command::Execute("cg_drawFriendlyFireCrosshair 1");
			Command::Execute("friendlynamefontcolor 0 1 0 1");
			Command::Execute("friendlynamefontglowcolor 0 0 0 0");
			Command::Execute("hostilenamefontcolor 1 0 0 1");
			Command::Execute("hostilenamefontglowcolor 0 0 0 0");
		#endif

		Command::Add("dev_discord_page", [](Command::Params*)
		{
			Utils::OpenUrl("https://discord.gg/28sARKGhcK");
		});

		Command::Add("dev_moddb_page", [](Command::Params*)
		{
			Utils::OpenUrl("https://www.moddb.com/members/bogdan1997");
		});

		Command::Add("dev_youtube_page", [](Command::Params*)
		{
			Utils::OpenUrl("https://youtube.com/@bogdankonstantinou");
		});


		// test CMD
		// Currently not used.
		/*
		Command::Add("RemoveClipByIndex", [](Command::Params*)
		{
			std::vector<Game::cbrush_t*> brush_list(Game::cm->numBrushes);
			for (auto num = 0u; num < Game::cm->numBrushes; num++)
			{
				brush_list[num] = &Game::cm->brushes[num];
			}
			brush_list[20685]->contents = 0x2000;
		});
		*/


		// Not used.
		/*
		Command::Add("test_scr_error", [](Command::Params*)
		{
			Game::Scr_Error("ScrError() test\n");
			return;
		});
		*/

		// Not used.
		/*
		Command::Add("test_cmd", [](Command::Params*)
		{
			Game::Com_Printf(0, "Test print message\n");
		});
		*/
		
		// Not used.
		/*
		Command::Add("test_url_cmd", [](Command::Params*)
		{
			Utils::OpenUrl("https://youtu.be/dQw4w9WgXcQ?t=85");
		});
		*/

		Command::Add("laserforceon", [](Command::Params*)
		{
			// Empty
		});

		Command::Add("laserforceoff", [](Command::Params*)
		{
			// Empty
		});

		Command::Add("toggle_timer", [](Command::Params*)
		{
			// Empty
		});

		#ifdef DISABLE_CONSOLE
			// Disable Console
			Utils::Hook::Set<BYTE>(0x4388A3, 0x0);
		#endif

		#ifdef MODERN_OPS
			// Disable Lean Left/Right
			/*Game::Cmd_RemoveCommand("-leanleft");
			Game::Cmd_RemoveCommand("+leanleft");
			Game::Cmd_RemoveCommand("-leanright");
			Game::Cmd_RemoveCommand("+leanright");
			
			Command::Add("-leanleft", [](Command::Params*)
			{
				// Empty.
			});
			
			Command::Add("+leanleft", [](Command::Params*)
			{
				// Empty.
			});
			
			Command::Add("-leanright", [](Command::Params*)
			{
				// Empty.
			});
			
			Command::Add("+leanright", [](Command::Params*)
			{
				// Empty.
			});*/
		#endif
		
		#ifdef CoD2R
			// Disable Nightvision
			Game::Cmd_RemoveCommand("-nightvision");
			Game::Cmd_RemoveCommand("+nightvision");
		#endif

		#ifdef DEVELOPER_MODE
			const char* res = CL_GetMaxXP();
			//Game::Com_Printf(0, "string: ^2%s \n", res);
			
			const auto& dvar = Dvars::Functions::Dvar_FindVar("version");
			//Game::Com_Printf(0, "Dvar name: ^2%s \n", dvar->name);
			
			Command::Execute("developer 2");
			Command::Execute("fs_debug 2");
			Command::Execute("logfile 2");
		#endif

		// Not used.
		/*
		Command::Add("unlockall", [](Command::Params*)
		{
			Command::Execute("seta mis_01 50");
			//Command::Execute("seta mis_difficulty 4444444444444444444440000000000000000000");
			//Command::Execute("seta cheat_points 30");
			//Command::Execute("seta cheat_items_set1 1073741823");
			Game::Com_Printf(0, "Game [^3INFO^7]: missions and cheats has been unlocked on your game profile.\n");
		});
		*/

		// Not used.
		/*
		Command::Add("resetstats", [](Command::Params*)
		{
			Command::Execute("seta mis_01 0");
			Command::Execute("seta mis_difficulty 0000000000000000000000000000000000000000");
			Command::Execute("seta cheat_points 0");
			Command::Execute("seta cheat_items_set1 0");
			Game::Com_Printf(0, "Game [^3INFO^7]: missions and cheats has been resets on your game profile.\n");
		});
		*/

		// Not used.
		/*
		Command::Add("applyCheat", [](Command::Params*)
		{
			if (Dvars::Functions::Dvar_FindVar("sv_allowCheats")->current.enabled)
			{
				Command::Execute("seta thereisacow 1337", false);

				Utils::Hook::Set<BYTE>(0x587872, 0xEB); // Read only.
				Utils::Hook::Set<BYTE>(0x58788F, 0xEB); // Write protected.
				Utils::Hook::Set<BYTE>(0x5878AD, 0xEB); // Cheat protected.
			}
			else
			{
				Command::Execute("seta thereisacow 0", false);

				Utils::Hook::Set<BYTE>(0x587872, 0x74); // Read only.
				Utils::Hook::Set<BYTE>(0x58788F, 0x74); // Write protected.
				Utils::Hook::Set<BYTE>(0x5878AD, 0x75); // Cheat protected.
			}

			Dvars::Override::DvarBoolOverride("sv_cheats",
				Dvars::Functions::Dvar_FindVar("sv_allowCheats")->current.enabled ? true : false,
				Dvars::Functions::Dvar_FindVar("sv_allowCheats")->current.enabled ? Game::none : Game::cheat_protected);
		});
		*/

		// Not used.
		/*
		ServerCommand::Add("server_test_cmd", []()
		{
			const char* arg1 = Command::Cmd_Argv(1);
			Game::Com_Printf(0, "^1Custom server command with argument %s\n", arg1);
		});
		*/

		// Not used.
		/*
		Command::Add("test_send_custom_server_cmd", [](Command::Params*)
		{
			Game::SV_GameSendServerCommand(-1, Utils::String::VA("%s %d", "server_test_cmd", 5));
			//Game::SV_GameSendServerCommand(-1, Utils::String::VA("%s", "function_not_exists"));
		});
		*/

		//Dvars::Override::DvarBoolOverride("developer_script", Game::none);
		//Dvars::Override::DvarBoolOverride("fs_usedevdir", true, Game::read_only);// Enable the dev directions by default.
		Dvars::Override::DvarBoolOverride("r_ignorehwgamma", Game::saved);

		// set to 0 and make them read only.
		Dvars::Override::DvarBoolOverride("r_fastskin", false, Game::read_only);
		Dvars::Override::DvarBoolOverride("r_skinCache", false, Game::read_only);

		LocalizedStrings::OverrideLocalizeStrings();

		// Not used.
		/*
		if (Flags::HasFlag("dump"))
		{
			Command::Add("dumpLocStrings", [](Command::Params* params)
			{
				if (params->size() < 2)
				{
					Game::Com_Printf(0, "dump_localize_strings <menu_name>\n");
					return;
				}

				std::vector<Game::LocalizeEntry*> localize;
				std::ofstream dumpFile;

				FastFiles::EnumAssets(Game::ASSET_TYPE_LOCALIZE_ENTRY, [&localize](Game::XAssetHeader header)
				{
					localize.push_back(header.localize);
				}, false);

				// sort localize strings
				std::sort(localize.begin(), localize.end(), [](Game::LocalizeEntry* localize_start, Game::LocalizeEntry* localize_end)
				{
					return std::string_view(localize_start->name) < std::string_view(localize_end->name);
				});

				if (const auto& fs_basepath = Dvars::Functions::Dvar_FindVar("fs_basepath"); fs_basepath)
				{
					std::string base_path = fs_basepath->current.string;
					const char* languageName = Game::SEH_GetLanguageName(Dvars::Functions::Dvar_FindVar("loc_language")->current.unsignedInt);
					base_path += Utils::String::VA("\\dump\\%s\\localizedstrings\\", languageName);

					if (std::filesystem::create_directories(base_path))
					{
						Game::Com_Printf(0, "Created directory \"dump/%s/localizedstrings\"\n", languageName);
					}

					const std::string file_name = base_path + Utils::String::VA("%s.str", params->get(1));
					dumpFile.open(file_name.c_str());
					if (dumpFile.is_open())
					{
						dumpFile << "// dumped by Modern Operations " << __TIMESTAMP__ << "\n";
						dumpFile << "\n";

						dumpFile << "// Note to translators:\n";
						dumpFile << "// If a sentence is the same in your language then please change it to ""#same""\n";
						dumpFile << "//\n";
						dumpFile << "// eg:\n";
						dumpFile << "// LANG_ENGLISH  \"HALT\"\n";
						dumpFile << "// LANG_GERMAN  \"#same\"\n";
						dumpFile << "//\n";
						dumpFile << "// (This is so we can tell which strings have been signed-off as ok to be the same words for QA\n";
						dumpFile << "//  and because we do not store duplicate strings, which will then get exported again next time\n";
						dumpFile << "//  as being untranslated.)\n";
						dumpFile << "//\n";
						dumpFile << "VERSION             \"1\"\n";
						dumpFile << "CONFIG				 \"C:\\trees\\cod3\\cod3\\bin\\StringEd.cfg\"\n";
						dumpFile << "FILENOTES			 \"\"\n";
						dumpFile << "\n";

						for (std::size_t i = 0; i < localize.size(); i++)
						{
							if (Utils::String::StartsWith(localize[i]->name, Utils::String::ToUpper(params->get(1))))
							{
								//Utils::String::
								std::string localize_name = Utils::String::VA("%s", localize[i]->name);
								Utils::String::Replace(localize_name, Utils::String::ToUpper(params->get(1)) + "_", "");
								dumpFile << "REFERENCE           " << localize_name.c_str() << std::endl;
								dumpFile << "LANG_" << Utils::String::ToUpper(languageName) << "        " << Utils::String::VA("\"%s\"", localize[i]->value) << std::endl;
								dumpFile << "\n";
							}
						}

						dumpFile << "ENDMARKER\n";

						Game::Com_Printf(0, Utils::String::VA("Dumped localized strings to: %s", file_name.c_str()));
						dumpFile.close();
					}
				}
			});
		}
		*/

		TextRenderer::UpdateGameFontsAndText();
	}

	bool CommonPatch::CheckMilesFiles()
	{
		// Not used.
		/*
		if (!Utils::IO::DirectoryExists("modern_ops_data/miles"))
		{
			MessageBoxA(nullptr, "The 'modern_ops_data/miles' folder does not exist.", nullptr, MB_ICONERROR);
			Utils::Library::Terminate();
			return false;
		}

		std::vector<std::string> requiredFiles = 
		{
			"milesEq.flt",
			"mssds3d.flt",
			"mssdsp.flt",
			"msseax.flt",
			"mssmp3.asi",
			"mssvoice.asi"
		};

		std::filesystem::path basePath = std::filesystem::path(Dvars::Functions::Dvar_FindVar("fs_basepath")->current.string) / "modern_ops_data" / "miles";
		const auto path = (basePath).generic_string();
		const auto files = Utils::IO::ListFiles(path);

		std::vector<std::string> unexpectedFiles;
		for (const auto& file : files)
		{
			auto fileName = std::filesystem::path(file).filename().string();
			if (std::find(requiredFiles.begin(), requiredFiles.end(), fileName) == requiredFiles.end())
			{
				unexpectedFiles.push_back(file);
			}
		}

		if (!unexpectedFiles.empty())
		{
			std::string errorMessage = "In COD4 ROOT modern_ops_data/miles folder was spotted custom .asi files:\n";
			for (const auto& file : unexpectedFiles)
			{
				errorMessage += file + "\n";
			}
			errorMessage += "Remove the following .asi files before running the client.";
			MessageBoxA(nullptr, errorMessage.c_str(), nullptr, MB_ICONERROR);
			Utils::Library::Terminate();
			return false;
		}
		*/

		return true;
	}

	/*
	const char* __stdcall CommonPatch::AIL_set_redist_directory_Stub(const char* directory)
	{
		return Utils::Hook::Call<const char* (__stdcall)(const char*)>(0x63F7EA)("modern_ops_data/miles");
	}
	*/

	void CommonPatch::RunModStub()
	{
		char mod[260];
		
		if (Game::sharedUiInfo->modList[Game::sharedUiInfo->modIndex].modName)
		{
			sprintf_s(mod, "mods/%s", Game::sharedUiInfo->modList[Game::sharedUiInfo->modIndex].modName);

			if (Dvars::Functions::Dvar_FindVar("useFastFile")->current.enabled)
			{
				Game::R_BeginRemoteScreenUpdate();
				WaitForSingleObject(Game::databaseCompletedEvent, 0xFFFFFFFF);
				Game::R_EndRemoteScreenUpdate();
			}

			Dvars::Functions::Dvar_SetStringByName("fs_game", mod);
			Language::VideoSubtitlesStub();
			Config::GameWithMod = true;
			Config::CallExecFromCFG(true);
		}
	}

	void CommonPatch::ClearModStub()
	{
		Dvars::Functions::Dvar_Reset(0, Dvars::Functions::Dvar_FindVar("fs_game"));
		Language::VideoSubtitlesStub();
		Config::GameWithMod = false;
		Config::CallExecFromCFG(false);
	}

	void CommonPatch::R_Cinematic_BinkOpenStub1(char* buffer, size_t size, const char* /*format*/, const char* directory, const char* fileName, const char* videoFormat)
	{
		const char* languageName = Game::SEH_GetLanguageName(Dvars::Functions::Dvar_FindVar("loc_language")->current.unsignedInt);

		// Not used.
		/*if (Utils::IO::FileExists(Utils::String::VA("%s\\modern_ops_data\\video\\%s.%s", directory, fileName, videoFormat)))
		{
			_snprintf_s(buffer, size, _TRUNCATE, "%s\\modern_ops_data\\video\\%s.%s", directory, fileName, videoFormat);
			return;
		}
		else */if (Utils::IO::FileExists(Utils::String::VA("%s\\main\\video\\%s.%s", directory, fileName, videoFormat)))
		{
			_snprintf_s(buffer, size, _TRUNCATE, "%s\\main\\video\\%s.%s", directory, fileName, videoFormat);
			return;
		}
		else if (Utils::IO::FileExists(Utils::String::VA("%s\\main\\%s_video\\%s.%s", directory, languageName, fileName, videoFormat)))
		{
			_snprintf_s(buffer, size, _TRUNCATE, "%s\\main\\%s_video\\%s.%s", directory, languageName, fileName, videoFormat);
			return;
		}
		else if (Utils::IO::FileExists(Utils::String::VA("%s\\%s\\main\\video\\%s.%s", directory, Dvars::Functions::Dvar_FindVar("fs_game")->current.string, fileName, videoFormat)))
		{
			_snprintf_s(buffer, size, _TRUNCATE, "%s\\%s\\main\\video\\%s.%s", directory, Dvars::Functions::Dvar_FindVar("fs_game")->current.string, fileName, videoFormat);
			return;
		}
		else
		{
			_snprintf_s(buffer, size, _TRUNCATE, "%s\\%s\\main\\video\\%s\\%s.%s", directory, Dvars::Functions::Dvar_FindVar("fs_game")->current.string, languageName, fileName, videoFormat);
			return;
		}
	}

	void CommonPatch::Com_Quit_f_Stub()
	{
		Utils::Library::Terminate();
	}

	void CommonPatch::LanguageSetValueConfig(int langIndex)
	{
		Config::Set<std::string>("language", Game::SEH_GetLanguageName(langIndex));
	}

	void __declspec(naked) CommonPatch::UpdateLanguageStub()
	{
		const static uint32_t retn_addr = 0x5674FD;
		__asm
		{
			push	eax; // value
			call	Dvars::Functions::Dvar_SetIntByName;
			call	LanguageSetValueConfig;
			add		esp, 4;
			jmp		retn_addr;
		}
	}

	// It may need to be redone as 'Event' module, but i don't have much dvars with latched flag :<
	int CommonPatch::R_RegisterSunDvarsStub()
	{
		Dvars::console_menu_style = Dvars::Register::Dvar_RegisterBool("console_menu_style", "Enable/Disable menu style from console version", false, Game::saved | Game::latched);
		return Utils::Hook::Call<int()>(0x61D050)();
	}

	Game::dvar_s* CommonPatch::RegisterConColor(const char* dvarName, float r, float g, float b, float a, float min, float max, unsigned __int16 flags, const char* description)
	{
		static struct
		{
			const char* name;
			float color[4];
		} patchedColors[] =
		{
			{ "con_inputBoxColor",     { 0.15f, 0.15f, 0.15f, 1.00f } },
			{ "con_inputHintBoxColor", { 0.10f, 0.10f, 0.10f, 1.00f } },
			{ "con_outputWindowColor", { 0.10f, 0.10f, 0.10f, 0.75f } },
			{ "con_outputBarColor",    { 0.10f, 0.10f, 0.10f, 1.00f } },
			{ "con_outputSliderColor", { 0.10f, 0.10f, 0.10f, 1.00f } },
		};

		for (std::size_t i = 0; i < ARRAYSIZE(patchedColors); ++i)
		{
			if (std::strcmp(dvarName, patchedColors[i].name) == 0)
			{
				r = patchedColors[i].color[0];
				g = patchedColors[i].color[1];
				b = patchedColors[i].color[2];
				a = patchedColors[i].color[3];
				break;
			}
		}

		return Dvars::Register::Dvar_RegisterVec4(dvarName, description, r, g, b, a, min, max, flags);
	}

	void __declspec(naked) CommonPatch::RegisterConColorStub1()
	{
		const static uint32_t retn_addr = 0x439948;
		__asm
		{
			push	edi;
			call	RegisterConColor;
			add		esp, 4;
			jmp		retn_addr;
		}
	}

	void __declspec(naked) CommonPatch::RegisterConColorStub2()
	{
		const static uint32_t retn_addr = 0x43998B;
		__asm
		{
			push	edi;
			call	RegisterConColor;
			add		esp, 4;
			jmp		retn_addr;
		}
	}

	void __declspec(naked) CommonPatch::RegisterConColorStub3()
	{
		const static uint32_t retn_addr = 0x4399CE;
		__asm
		{
			push	edi;
			call	RegisterConColor;
			add		esp, 4;
			jmp		retn_addr;
		}
	}

	void __declspec(naked) CommonPatch::RegisterConColorStub4()
	{
		const static uint32_t retn_addr = 0x439A17;
		__asm
		{
			push	edi;
			call	RegisterConColor;
			add		esp, 4;
			jmp		retn_addr;
		}
	}

	void __declspec(naked) CommonPatch::RegisterConColorStub5()
	{
		const static uint32_t retn_addr = 0x439A60;
		__asm
		{
			push	edi;
			call	RegisterConColor;
			add		esp, 4;
			jmp		retn_addr;
		}
	}

	float CommonPatch::CG_DrawViewpos(float previous_y, float posX, float labelWidth)
	{
		const float color[4] = { 1.00f, 1.00f, 1.00f, 1.00f };
		const char* mapname = Dvars::Functions::Dvar_FindVar("mapname")->current.string;
		const char* text = Utils::String::VA("%s (%.1f %.1f %.1f) %.1f %.1f", mapname, Game::g_clients->ps.origin[0], Game::g_clients->ps.origin[1], Game::g_clients->ps.origin[2], Game::cgs->refdefViewAngles[1], Game::cgs->refdefViewAngles[0]);
		float final_pos = Game::CG_CornerDebugPrint(color, text, Game::scrPlaceFull, posX, previous_y, labelWidth, " viewpos") + previous_y;
		return final_pos;
	}

	float CommonPatch::CG_DrawDObjInfo(float previous_y, float posX, float labelWidth)
	{
		int objFreeCount = *(int*)(0xFC1918);
		const float color[4] = { 1.00f, 1.00f, 1.00f, 1.00f };
		const char* text = Utils::String::VA("%d/%d", 2048 - objFreeCount, 2048);
		float final_pos = Game::CG_CornerDebugPrint(color, text, Game::scrPlaceFull, posX, previous_y, labelWidth, " used DObjs") + previous_y;
		return final_pos;
	}

	float CommonPatch::CG_DrawEntitiesCountInfo(float previous_y, float posX, float labelWidth)
	{
		int level_num_entities = *(int*)(0xE18E24);
		const float color[4] = { 1.00f, 1.00f, 1.00f, 1.00f };
		const char* text = Utils::String::VA("%d/%d", level_num_entities, 2174);
		float final_pos = Game::CG_CornerDebugPrint(color, text, Game::scrPlaceFull, posX, previous_y, labelWidth, " total ents") + previous_y;
		return final_pos;
	}

	float CommonPatch::CG_DrawAICount(float previous_y, float posX, float labelWidth)
	{
		int level_actorCount = *(int*)(0xE18E64);
		const float color[4] = { 1.00f, 1.00f, 1.00f, 1.00f };
		const char* text = Utils::String::VA("%d/%d", level_actorCount, 32);
		float final_pos = Game::CG_CornerDebugPrint(color, text, Game::scrPlaceFull, posX, previous_y, labelWidth, " total AI") + previous_y;
		return final_pos;
	}

	void CommonPatch::EnhancedDebugInformation()
	{
		float x = Game::scrPlaceFull->virtualViewableMax[0] - Game::scrPlaceFull->virtualViewableMin[0] + Dvars::Functions::Dvar_FindVar("cg_debugInfoCornerOffset")->current.value;
		float y = Dvars::Functions::Dvar_FindVar("cg_debugInfoCornerOffset")->current.vector[1];
		float labelWidth = Dvars::Functions::Dvar_FindVar("cg_small_dev_string_fontscale")->current.value * (Game::R_TextWidth(" cg ms/frame", 0, Game::cgMedia->smallDevFont) * 0.75f);

		if(Dvars::cg_drawViewpos->current.enabled)
			y = CG_DrawViewpos(y, x, labelWidth);
		if (Dvars::Functions::Dvar_FindVar("cg_drawFPS")->current.integer)
		{
			y = Utils::Hook::Call<float(float)>(0x411510)(y); //CG_DrawFPS
			if (Dvars::Functions::Dvar_FindVar("cg_drawFPS")->current.integer > 1)
			{
				y = CG_DrawDObjInfo(y, x, labelWidth);
				y = CG_DrawEntitiesCountInfo(y, x, labelWidth);
				y = CG_DrawAICount(y, x, labelWidth);
			}
		}
	}

	void __declspec(naked) CommonPatch::CG_DrawFPSStub()
	{
		const static uint32_t retn_addr = 0x411167;
		__asm
		{
			mov     dword ptr ds:[0x1628E68], 0;
			call	EnhancedDebugInformation;
			jmp		retn_addr;
		}
	}

// Doesn't work!
/*
	Game::dvar_s* CommonPatch::fs_usedevdir_fix(const char* dvarName, int type, unsigned __int16 flags, float r, float g, float b, float a, float min, float max, const char* description)
	{
		return Dvars::Register::Dvar_RegisterBool(dvarName, description, true, Game::saved);
	}

	void __declspec(naked) CommonPatch::fs_usedevdir_register_stub()
	{
		const static uint32_t retn_addr = 0x57AC09;
		__asm
		{
			push	edi;
			call	fs_usedevdir_fix;
			add		esp, 4;
			jmp		retn_addr;
		}
	}
*/
// Doesn't work!

	void BG_RegisterDvars_stub()
	{
		Utils::Hook::Call<void()>(0x5B3BE0)();

		// Override viewmodel, crosshair, death delay and sprint time dvars
		Dvars::Functions::Dvar_FindVar("bg_bobAmplitudeDucked")->flags = Game::saved_flag;
		Dvars::Functions::Dvar_FindVar("bg_bobAmplitudeProne")->flags = Game::saved_flag;
		Dvars::Functions::Dvar_FindVar("bg_bobAmplitudeStanding")->flags = Game::saved_flag;

		Dvars::Functions::Dvar_FindVar("cg_bobWeaponAmplitude")->flags = Game::saved_flag;
		
		Dvars::Functions::Dvar_FindVar("cg_gun_x")->flags = Game::saved_flag;
		Dvars::Functions::Dvar_FindVar("cg_gun_y")->flags = Game::saved_flag;
		Dvars::Functions::Dvar_FindVar("cg_gun_z")->flags = Game::saved_flag;
		
		Dvars::Functions::Dvar_FindVar("cg_gun_move_f")->flags = Game::saved_flag;
		Dvars::Functions::Dvar_FindVar("cg_gun_move_r")->flags = Game::saved_flag;
		Dvars::Functions::Dvar_FindVar("cg_gun_move_u")->flags = Game::saved_flag;
		
		Dvars::Functions::Dvar_FindVar("cg_gun_ofs_f")->flags = Game::saved_flag;
		Dvars::Functions::Dvar_FindVar("cg_gun_ofs_r")->flags = Game::saved_flag;
		Dvars::Functions::Dvar_FindVar("cg_gun_ofs_u")->flags = Game::saved_flag;
		
		Dvars::Functions::Dvar_FindVar("cg_gun_rot_r")->flags = Game::saved_flag;
		Dvars::Functions::Dvar_FindVar("cg_gun_rot_p")->flags = Game::saved_flag;
		Dvars::Functions::Dvar_FindVar("cg_gun_rot_y")->flags = Game::saved_flag;

		Dvars::Functions::Dvar_FindVar("cg_drawCrosshair")->flags = Game::saved_flag;
		//Dvars::Functions::Dvar_FindVar("cg_fov")->flags = Game::saved;
		Dvars::Functions::Dvar_FindVar("cg_weaponCycleDelay")->flags = Game::saved_flag;

		// Adjust the size of player compass icon.
		Dvars::Override::DvarFloatOverride("compassPlayerHeight", 18.75, Game::saved_flag);
		Dvars::Override::DvarFloatOverride("compassPlayerWidth", 18.75, Game::saved_flag);

		// Change the flag to saved.
		//Dvars::Override::DvarFloatOverride("hud_fade_ammodisplay", Game::saved);
		//Dvars::Override::DvarFloatOverride("hud_fade_compass", Game::saved);
		//Dvars::Override::DvarFloatOverride("hud_fade_healthbar", Game::saved);
		//Dvars::Override::DvarFloatOverride("hud_fade_offhand", Game::saved);
		//Dvars::Override::DvarFloatOverride("hud_fade_sprint", Game::saved);
		//Dvars::Override::DvarFloatOverride("hud_fade_stance", Game::saved);
		//Dvars::Override::DvarFloatOverride("hud_fadeout_speed", Game::saved);

		// Change the flag to saved_flag
		Dvars::Functions::Dvar_FindVar("g_deathDelay")->flags = Game::saved_flag;

		// Change the flag to saved_flag
		Dvars::Functions::Dvar_FindVar("player_sprintTime")->flags = Game::saved_flag;
	}

	CommonPatch::CommonPatch()
	{
		Events::OnDvarInit([]
		{
			Dvars::external_console = Dvars::Register::Dvar_RegisterBool("external_console", "Turn on/off the external console", false, Game::saved);

			//TESTING DVARS
			// Bool dvar register test
			//Dvars::test_dvar_bool = Dvars::Register::Dvar_RegisterBool("test_dvar_bool", "Test Bool Dvar", false, Game::none);
			// Int dvar register test
			//Dvars::test_dvar_int = Dvars::Register::Dvar_RegisterInt("test_dvar_int", "Test Int Dvar", 0, 0, 3, Game::none);
			// Enum dvar register test
			//static std::vector <const char*> r_enum_test_values = { "0", "1", "2", "3", };
			//Dvars::test_dvar_enum = Dvars::Register::Dvar_RegisterEnum("test_dvar_enum", "Test Enum Dvar", 0, r_enum_test_values.size(), r_enum_test_values.data(), Game::none);
			// Float dvar register test
			//Dvars::test_dvar_float = Dvars::Register::Dvar_RegisterFloat("test_dvar_float", "Test Float Dvar", 0.0f, 0.0f, 100.0f, Game::none);
			// Vectors dvars register test
			//Dvars::test_dvar_vec2 = Dvars::Register::Dvar_RegisterVec2("test_dvar_vec2", "Test Vector2 Dvar", 0.0f, 0.0f, 0.0f, 25.0f, Game::none);
			//Dvars::test_dvar_vec3 = Dvars::Register::Dvar_RegisterVec3("test_dvar_vec3", "Test Vector3 Dvar", 0.0f, 0.0f, 0.0f, 0.0f, 50.0f, Game::none);
			//Dvars::test_dvar_vec4 = Dvars::Register::Dvar_RegisterVec4("test_dvar_vec4", "Test Vector4 Dvar", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 100.0f, Game::none);
			// String DVAR Test
			//Dvars::test_dvar_string = Dvars::Register::Dvar_RegisterString("test_dvar_string", "Test String Dvar.", "default", Game::saved);
			// Saved flag dvar register test (works on one map only)
			//Dvars::test_dvar_bool_savedflag = Dvars::Register::Dvar_RegisterBool("test_dvar_savedbool", "Test Saved Bool Dvar", false, Game::saved_flag);
			// New fov control dvars
			Dvars::cg_fov = Dvars::Register::Dvar_RegisterFloat("cg_fov", "The field of view angle in degrees", 65.0f, 1.0f, 160.0f, Game::saved);
			Dvars::cg_fovScale = Dvars::Register::Dvar_RegisterFloat("cg_fovScale", "Scale applied to the field of view", 1.0f, 0.2f, 2.0f, Game::saved);
			Dvars::cg_fovMenu = Dvars::Register::Dvar_RegisterFloat("cg_fovMenu", "Menu option scale applied to the field of view", 1.0f, 0.2f, 2.0f, Game::saved);
			Dvars::cg_fovMin = Dvars::Register::Dvar_RegisterFloat("cg_fovMin", "The minimum possible field of view", 10.0f, 1.0f, 160.0f, Game::saved);

			Dvars::developer_mode = Dvars::Register::Dvar_RegisterBool("developer_mode", "Unlock development levels.", 0, 4u);

			// UI Debug without (developer 1 and CTRL+F11)
			Dvars::ui_debugMode = Dvars::Register::Dvar_RegisterBool("ui_debugMode", "Displays UI debug information on the screen.", 0, 4u);
			// Misc
			//Game::dvar_s* ui_skipMainLockout = Dvars::Register::Dvar_RegisterBool("ui_skipMainLockout", "", false, Game::none);
			//Game::dvar_s* profile_unlock_all = Dvars::Register::Dvar_RegisterBool("profile_unlock_all", "", false, Game::saved);
			Game::dvar_s* cg_drawClientVersion = Dvars::Register::Dvar_RegisterBool("cg_drawClientVersion", "Show/Hide the client version on screen.", false, Game::saved);
			//Game::dvar_s* cg_drawDamageFeedbackOption = Dvars::Register::Dvar_RegisterBool("cg_drawDamageFeedbackOption", "Display a marker when hitting enemies", false, Game::saved);
			//Game::dvar_s* snd_hitsoundDisabled = Dvars::Register::Dvar_RegisterBool("snd_hitsoundDisabled", "Disable the hit indicator sound", true, Game::saved);
			//Game::dvar_s* sv_allowCheats = Dvars::Register::Dvar_RegisterBool("sv_allowCheats", "Enable/Disable the game cheats", false, Game::saved);
			Dvars::allowCompactFolderForMod = Dvars::Register::Dvar_RegisterBool("fs_compactModDirectory", "Enable/Disable compact mod directory.", false, Game::saved);
			
			// Damage Feedback.
			Dvars::damagefeedback = Dvars::Register::Dvar_RegisterBool("scr_damagefeedback", "Damage Feedback", false, Game::saved);
			
			// Chaos Mode.
			//Dvars::chaos_mode = Dvars::Register::Dvar_RegisterBool("chaos_mode", "chaos mode difficulty", false, Game::user_created);
			
			// Special Ops.
			Dvars::specialops = Dvars::Register::Dvar_RegisterBool("specialops", "special ops", false, Game::none);
			
			// Survival.
			Dvars::survival = Dvars::Register::Dvar_RegisterBool("so_survival", "special ops - survival mode", false, Game::none);
			
			// Zombie Mode.
			Dvars::zombiemode = Dvars::Register::Dvar_RegisterBool("zombiemode", "zombie mode", false, Game::none);
			
			// Animated Jumps.
			Dvars::viewmodelAnimatedJumps = Dvars::Register::Dvar_RegisterBool("cg_viewmodelAnimatedJumps", "Enables/Disable animated jumps for player's viewhands", true, Game::saved_flag);
			
			// Viewmodel Fov Offset.
			Dvars::viewmodelFovOffsetEnabled = Dvars::Register::Dvar_RegisterBool("cg_viewmodelFovOffsetEnabled", "Enables/Disable viewmodel fov offset", true, Game::saved_flag);
			
			// Player's Melee Attack Type.
			Dvars::meleeAttackType = Dvars::Register::Dvar_RegisterInt("player_meleeAttackType","Types: 0(regular), 1(melee_stab), 2(stealth takedown)", 1, 0, 3, Game::read_only);
			
			// Gamemode.
			static std::vector <const char*> r_enum_gamemode_ratio = { "none", "sp", "so", "mp", "zm", };
			Dvars::gameMode = Dvars::Register::Dvar_RegisterEnum( "gameMode", "Current gameMode", 0, r_enum_gamemode_ratio.size(), r_enum_gamemode_ratio.data(), Game::none);
			
			// Gamemode Single Player.
			Dvars::gameModeSP = Dvars::Register::Dvar_RegisterString("gameModeSP", "Game Mode Single Player.", "false", Game::none);
			
			// Gamemode Special Ops.
			Dvars::gameModeSO = Dvars::Register::Dvar_RegisterString("gameModeSO", "Game Mode Special Ops.", "false", Game::none);
			
			// Gamemode Zombiemode.
			Dvars::gameModeZM = Dvars::Register::Dvar_RegisterString("gameModeZM", "Game Mode Zombies.", "false", Game::none);
			
			// Perks.
			Dvars::perks_enabled = Dvars::Register::Dvar_RegisterBool("perks_enabled", "Enables/Disables player's perks", true, Game::saved_flag);
			
			// specialty_marathon
			Dvars::bulletpenetration = Dvars::Register::Dvar_RegisterBool("perk_bulletpenetration_enabled", "Enables/Disables 'Bullet Penetration' perk", false, Game::saved_flag);
			
			// specialty_marathon
			Dvars::marathon = Dvars::Register::Dvar_RegisterBool("perk_marathon_enabled", "Enables/Disables 'Marathon' perk", false, Game::saved_flag);
			
			// specialty_fastreload
			Dvars::fastreload = Dvars::Register::Dvar_RegisterBool("perk_fastreload_enabled", "Enables/Disables 'Fast Reload' perk", false, Game::saved_flag);
			
			// specialty_longersprint
			Dvars::longersprint = Dvars::Register::Dvar_RegisterBool("perk_longersprint_enabled", "Enables/Disables 'Longer Sprint' perk", false, Game::saved_flag);
			
			// specialty_extralethal
			Dvars::extralethal = Dvars::Register::Dvar_RegisterBool("perk_extralethal_enabled", "Enables/Disables 'Extra Lethal' perk", false, Game::saved_flag);
			
			// specialty_quickdraw
			Dvars::quickdraw = Dvars::Register::Dvar_RegisterBool("perk_quickdraw_enabled", "Enables/Disables 'Quick Draw' perk", false, Game::saved_flag);
			
			// specialty_armorvest
			Dvars::armorvest = Dvars::Register::Dvar_RegisterBool("perk_armorvest_enabled", "Enables/Disables 'Armor Vest' perk", false, Game::saved_flag);
			
			// specialty_armorvest2
			Dvars::armorvest2 = Dvars::Register::Dvar_RegisterBool("perk_armorvest2_enabled", "Enables/Disables 'Armor Vest 2' perk", false, Game::saved_flag);
			
			// specialty_twoprimaries
			Dvars::twoprimaries = Dvars::Register::Dvar_RegisterBool("perk_twoprimaries_enabled", "Enables/Disables 'Two Primaries' perk", false, Game::saved_flag);
			
			// specialty_finalstand
			Dvars::finalstand = Dvars::Register::Dvar_RegisterBool("perk_finalstand_enabled", "Enables/Disables 'Final Stand' perk", false, Game::saved_flag);
			
			#ifdef PREVIEW_VERSION
				// Preview Mode.
				Dvars::cg_previewMode = Dvars::Register::Dvar_RegisterBool("cg_previewMode", "Enable Preview Cutscenes", true, Game::saved_flag);
			#else
				// Preview Mode.
				Dvars::cg_previewMode = Dvars::Register::Dvar_RegisterBool("cg_previewMode", "Enable Preview Cutscenes", false, Game::read_only);
			#endif
			
			// Weapon Inspection
			Dvars::cg_weaponInspection = Dvars::Register::Dvar_RegisterBool("cg_weaponInspection", "Enables/Disables weapon inspection", true, Game::saved_flag);
			
			// Weapon Switch Transition
			//Dvars::cg_weaponSwitchTransition = Dvars::Register::Dvar_RegisterBool("cg_weaponSwitchTransition", "Enables/Disables weapon switch transition", true, Game::saved_flag);
			
			// Legacy Animations
			Dvars::cg_useLegacyAnims = Dvars::Register::Dvar_RegisterBool("cg_useLegacyAnims", "Enables/Disables legacy animations", false, Game::saved);
			
			// Crosshair vertical offset
			Dvars::cg_crosshairVerticalOffset = Dvars::Register::Dvar_RegisterFloat("cg_crosshairVerticalOffset","Amount to vertically offset the crosshair from the center.", 0.0f, -1.0f, 1.0f, Game::saved_flag);
			
			// Non-pickable physics weapon drops
			Dvars::scr_physWeaponDrop = Dvars::Register::Dvar_RegisterBool("scr_physWeaponDrop", "Enables/Disables non-pickable physics weapon drops.", false, Game::saved_flag);
			
			// Time before a physics weapon is deleted
			Dvars::phys_weapon_delete_time = Dvars::Register::Dvar_RegisterFloat("phys_weapon_delete_time", "Sets the time (in seconds) before a dropped physics weapon is deleted.", 60.0f, 1.0f, 6000.0f, Game::saved_flag);
			
			// Missions.
			Dvars::mis_02 = Dvars::Register::Dvar_RegisterInt("mis_02","Mission 2", 0, 0, 50, Game::saved);
			Dvars::mis_03 = Dvars::Register::Dvar_RegisterInt("mis_03","Mission 3", 0, 0, 50, Game::saved);
			Dvars::mis_04 = Dvars::Register::Dvar_RegisterInt("mis_04","Mission 4", 0, 0, 50, Game::saved);
			Dvars::mis_05 = Dvars::Register::Dvar_RegisterInt("mis_05","Mission 5", 0, 0, 50, Game::saved);
			Dvars::mis_06 = Dvars::Register::Dvar_RegisterInt("mis_06","Mission 6", 0, 0, 50, Game::saved);
			Dvars::mis_07 = Dvars::Register::Dvar_RegisterInt("mis_07","Mission 7", 0, 0, 50, Game::saved);
			Dvars::mis_08 = Dvars::Register::Dvar_RegisterInt("mis_08","Mission 8", 0, 0, 50, Game::saved);
			Dvars::mis_09 = Dvars::Register::Dvar_RegisterInt("mis_09","Mission 9", 0, 0, 50, Game::saved);
			Dvars::mis_10 = Dvars::Register::Dvar_RegisterInt("mis_10","Mission 10", 0, 0, 50, Game::saved);
			
			// UI Level.
			Dvars::ui_level01 = Dvars::Register::Dvar_RegisterInt("ui_level01","UI Level 1", 0, 0, 50, Game::saved);
			Dvars::ui_level02 = Dvars::Register::Dvar_RegisterInt("ui_level02","UI Level 2", 0, 0, 50, Game::saved);
			Dvars::ui_level03 = Dvars::Register::Dvar_RegisterInt("ui_level03","UI Level 3", 0, 0, 50, Game::saved);
			Dvars::ui_level04 = Dvars::Register::Dvar_RegisterInt("ui_level04","UI Level 4", 0, 0, 50, Game::saved);
			Dvars::ui_level05 = Dvars::Register::Dvar_RegisterInt("ui_level05","UI Level 5", 0, 0, 50, Game::saved);
			Dvars::ui_level_status = Dvars::Register::Dvar_RegisterInt("ui_level_status","UI Level Status", 0, 0, 50, Game::saved);

			// Zombie Survivors.
			Dvars::zombiemode_survivors = Dvars::Register::Dvar_RegisterBool("zombiemode_survivors", "Zombie Mode Choice", true, Game::none);
			
			// Timer.
			Dvars::timer_enabled = Dvars::Register::Dvar_RegisterBool("timer_enabled", "Enables/Disables timer", false, Game::saved_flag);
			
			// Stance Carry.
			Dvars::hud_showStanceCarry = Dvars::Register::Dvar_RegisterBool("hud_showStanceCarry", "When true, the player's stance indicator changes to stance carry.", false, Game::saved_flag);

			// Player Speed Dvars.
			//Dvars::g_speed = Dvars::Register::Dvar_RegisterFloat("g_speed", "Maximum player speed", 190.0f, -3.4028235e38.0f, 3.4028235e38.0f, Game::saved_flag);
			//Dvars::g_speedScale = Dvars::Register::Dvar_RegisterFloat("g_speedScale", "Scale applied to the player speed", 1.0f, 0.0f, 2.0f, Game::saved);

		// DLCs
		#ifdef DLC1
			Dvars::dlc1 = Dvars::Register::Dvar_RegisterBool("dlc1_installed", "DLC 1", true, Game::read_only);
		#else
			Dvars::dlc1 = Dvars::Register::Dvar_RegisterBool("dlc1_installed", "DLC 1", false, Game::read_only);
		#endif
		
		#ifdef DLC2
			Dvars::dlc2 = Dvars::Register::Dvar_RegisterBool("dlc2_installed", "DLC 2", true, Game::read_only);
		#else
			Dvars::dlc2 = Dvars::Register::Dvar_RegisterBool("dlc2_installed", "DLC 2", false, Game::read_only);
		#endif
		
		#ifdef DLC3
			Dvars::dlc3 = Dvars::Register::Dvar_RegisterBool("dlc3_installed", "DLC 3", true, Game::read_only);
		#else
			Dvars::dlc3 = Dvars::Register::Dvar_RegisterBool("dlc3_installed", "DLC 3", false, Game::read_only);
		#endif
		
		#ifdef DLC4
			Dvars::dlc4 = Dvars::Register::Dvar_RegisterBool("dlc4_installed", "DLC 4", true, Game::read_only);
		#else
			Dvars::dlc4 = Dvars::Register::Dvar_RegisterBool("dlc4_installed", "DLC 4", false, Game::read_only);
		#endif
		
		#ifdef DLC5
			Dvars::dlc5 = Dvars::Register::Dvar_RegisterBool("dlc5_installed", "DLC 5", true, Game::read_only);
		#else
			Dvars::dlc5 = Dvars::Register::Dvar_RegisterBool("dlc5_installed", "DLC 5", false, Game::read_only);
		#endif
		
		#ifdef DLC6
			Dvars::dlc6 = Dvars::Register::Dvar_RegisterBool("dlc6_installed", "DLC 6", true, Game::read_only);
		#else
			Dvars::dlc6 = Dvars::Register::Dvar_RegisterBool("dlc6_installed", "DLC 6", false, Game::read_only);
		#endif
		
		#ifdef DLC7
			Dvars::dlc7 = Dvars::Register::Dvar_RegisterBool("dlc7_installed", "DLC 7", true, Game::read_only);
		#else
			Dvars::dlc7 = Dvars::Register::Dvar_RegisterBool("dlc7_installed", "DLC 7", false, Game::read_only);
		#endif
		
		#ifdef DLC8
			Dvars::dlc8 = Dvars::Register::Dvar_RegisterBool("dlc8_installed", "DLC 8", true, Game::read_only);
		#else
			Dvars::dlc8 = Dvars::Register::Dvar_RegisterBool("dlc8_installed", "DLC 8", false, Game::read_only);
		#endif
		
		#ifdef DLC9
			Dvars::dlc9 = Dvars::Register::Dvar_RegisterBool("dlc9_installed", "DLC 9", true, Game::read_only);
		#else
			Dvars::dlc9 = Dvars::Register::Dvar_RegisterBool("dlc9_installed", "DLC 9", false, Game::read_only);
		#endif
		
		#ifdef DLC10
			Dvars::dlc10 = Dvars::Register::Dvar_RegisterBool("dlc10_installed", "DLC 10", true, Game::read_only);
		#else
			Dvars::dlc10 = Dvars::Register::Dvar_RegisterBool("dlc10_installed", "DLC 10", false, Game::read_only);
		#endif

		#ifdef DEMO_VERSION
			Dvars::demo_build = Dvars::Register::Dvar_RegisterBool("demo_build", "Demo build", true, Game::read_only);
		#else
			Dvars::demo_build = Dvars::Register::Dvar_RegisterBool("demo_build", "Demo build", false, Game::read_only);
		#endif

		#ifdef EARLY_DEMO_VERSION
			Dvars::early_demo_build = Dvars::Register::Dvar_RegisterBool("early_demo_build", "Early Demo build", true, Game::read_only);
		#else
			Dvars::early_demo_build = Dvars::Register::Dvar_RegisterBool("early_demo_build", "Early Demo build", false, Game::read_only);
		#endif

		#ifdef DEMO_VERSION
			Dvars::limited_mode = Dvars::Register::Dvar_RegisterBool("limited_mode", "Limited Mode", true, Game::read_only);
		#else
			Dvars::limited_mode = Dvars::Register::Dvar_RegisterBool("limited_mode", "Limited Mode", false, Game::read_only);
		#endif
			
			//if (Game::DebugModeEnabled())
			//{
				Dvars::cg_drawViewpos = Dvars::Register::Dvar_RegisterBool("cg_drawViewpos", "Draw viewpos", false, Game::saved);
			//}
		});

		// Change the folder for loading files from miles
		// Not used.
		//Utils::Hook(0x591D15, AIL_set_redist_directory_Stub, HOOK_CALL).install()->quick();

		// Hook for correct works the latched dvars
 		Utils::Hook(0x5F68DB, R_RegisterSunDvarsStub, HOOK_CALL).install()->quick();

		//Utils::Hook(0x4B83CF, SV_CheatsStub, HOOK_CALL).install()->quick();
		//Utils::Hook::Nop(0x5899E8, 5);

		Utils::Hook::Set<const char*>(0x445667, "ModernOps.exe"); //-startSingleplayer

		// console logo
		if (Utils::IO::FileExists("logo.bmp"))
			Utils::Hook::Set<const char*>(0x59647A, "logo.bmp");

		// splash logo
		if (Utils::IO::FileExists("splash.bmp"))
			Utils::Hook::Set<const char*>(0x595EC4, "splash.bmp");

		// g_mem limit expansion.
		Utils::Hook::Set<DWORD>(0x534B3F, 0x20000000); //528mb
		Utils::Hook::Set<DWORD>(0x534B70, 0x20000000); //528mb

		// Increase hunkTotal from 10mb to 15mb.
		Utils::Hook::Set<BYTE>(0x5800D9, 0xF0);

		// Increase g_copyInfo limit.
		#ifdef MW1CR
			Utils::Hook::Set<BYTE>(0x45B5C4, 0x32);
		#else
			Utils::Hook::Set<BYTE>(0x45B5C4, 0x16);
		#endif

		// Fastfiles Hooking The Function
		Utils::Hook(0x5D94D0, DB_LoadCommonFastFiles, HOOK_CALL).install()->quick();

		// RunMod fix
		Utils::Hook::Nop(0x567238, 5);
		Utils::Hook::Nop(0x566E87, 5);
		Utils::Hook(0x567238, RunModStub, HOOK_CALL).install()->quick();
		Utils::Hook(0x56727E, ClearModStub, HOOK_CALL).install()->quick();

		Utils::Hook::Set<BYTE>(0x57AB0C, Game::none);
		Utils::Hook::Set<const char*>(0x567241, "vid_restart\n");
		Utils::Hook::Set<const char*>(0x567284, "vid_restart\n"); //for ClearMods

		// Making the separate video folder
		Utils::Hook(0x5D70DA, R_Cinematic_BinkOpenStub1, HOOK_CALL).install()->quick();

		Utils::Hook(0x5674F8, UpdateLanguageStub, HOOK_JUMP).install()->quick();

		// Com_Quit_f_stub for auto-update.
		Utils::Hook(0x595324, Com_Quit_f_Stub, HOOK_CALL).install()->quick();

		// Register mission dvars.
		Utils::Hook(0x41F0CE, BG_RegisterDvars_stub, HOOK_CALL).install()->quick();

		// Increase fps cap to 250 for menus.
		Utils::Hook::Set<BYTE>(0x535881, 0xEB);
		Utils::Hook::Set<BYTE>(0x535884, 0xEB);

		// Fix fps on windows 10 (stuck at 500) :: sleep(1) to sleep(0) (xoxor4d)
		Utils::Hook::Set<BYTE>(0x53571F, 0x0);

		// Alt + Enter for switching from fullscreen2window mode or window2fullscreen without developer mode.
		Utils::Hook::Nop(0x596BBE, 2);

		// Changed function for notice that player activate godmode/notarget and etc.
		Utils::Hook::Set<const char*>(0x4AD009, "gm \"%s\""); //godmode ON/OFF
		Utils::Hook::Set<const char*>(0x4AD099, "gm \"%s\""); //demigod ON/OFF
		Utils::Hook::Set<const char*>(0x4AD129, "gm \"%s\""); //notarget ON/OFF
		Utils::Hook::Set<const char*>(0x4AD1C4, "gm \"%s\""); //noclip ON/OFF
		Utils::Hook::Set<const char*>(0x4AD264, "gm \"%s\""); //ufo ON/OFF

		// Replaced the original .cfg name file.
		// Not used.
		/*
		Utils::Hook::Set<const char*>(0x5309CE, CLIENT_CONFIG);
		Utils::Hook::Set<const char*>(0x532778, CLIENT_CONFIG);
		Utils::Hook::Set<const char*>(0x53544A, CLIENT_CONFIG);
		Utils::Hook::Set<const char*>(0x57B2BD, CLIENT_CONFIG);
		*/

		static float BuildVersionColor[] = { 0.00f, 0.80f, 0.00f, 1.00f };
		Utils::Hook::Set<float*>(0x447661, BuildVersionColor);
		Utils::Hook::Set<float*>(0x43A8D2, BuildVersionColor);

		Utils::Hook::Nop(0x4477A1, 4);

		Utils::Hook::Nop(0x44045E, 5); // MAX_PACKET_USERCMDS\n
		Utils::Hook::Nop(0x53A334, 5); // Ragdoll allocation failed, out of ragdoll bodies (obj %d)

		// ui_showList: changing the font.
		Utils::Hook::Set<DWORD>(0x57164F, 0x129ADB8);

		// Patch in-game console dvars
		Utils::Hook(0x439943, RegisterConColorStub1, HOOK_JUMP).install()->quick();
		Utils::Hook(0x439986, RegisterConColorStub2, HOOK_JUMP).install()->quick();
		Utils::Hook(0x4399C9, RegisterConColorStub3, HOOK_JUMP).install()->quick();
		Utils::Hook(0x439A12, RegisterConColorStub4, HOOK_JUMP).install()->quick();
		Utils::Hook(0x439A5B, RegisterConColorStub5, HOOK_JUMP).install()->quick();

		// Doesn't work!
		//Utils::Hook(0x57AC04, fs_usedevdir_register_stub, HOOK_JUMP).install()->quick();

		//if (Game::DebugModeEnabled())
		//{
			// Additional debug information
			Utils::Hook::Nop(0x411140, 10);
			Utils::Hook(0x411140, CG_DrawFPSStub, HOOK_JUMP).install()->quick();
		//}

		// Mouse fix
		Utils::Hook::Nop(0x59681C, 8);
		Scheduler::Loop([]
		{
			SetThreadExecutionState(ES_DISPLAY_REQUIRED);
		}, Scheduler::Pipeline::MAIN);

		Scheduler::Loop([]
		{
			Utils::Hook::Set<bool>(0x1E209D4, (Dvars::ui_debugMode->current.enabled) ? 1 : 0);
		}, Scheduler::Pipeline::MAIN, 50ms);

		#ifdef MODERN_OPS
			// FF File Header
			//*(DWORD*)0x449665 = (DWORD)("CODSP001");
		#endif

		#ifdef MW1CR
			// FF File Header
			//*(DWORD*)0x449665 = (DWORD)("CODSP002");
		#endif

		#ifdef CoD2R
			// FF File Header
			//*(DWORD*)0x449665 = (DWORD)("CODSP003");
		#endif

		// overrides the 'ammo_counter_bullet' material name.
		Utils::Hook::Set<const char*>(0x420DAD, "h1_ammo_counter_bullet");

		// overrides the 'ammo_counter_beltbullet' material name.
		Utils::Hook::Set<const char*>(0x420DD3, "h1_ammo_counter_beltbullet");

		// overrides the 'ammo_counter_riflebullet' material name.
		Utils::Hook::Set<const char*>(0x420DF8, "h1_ammo_counter_riflebullet");

		// overrides the 'ammo_counter_rocket' material name.
		Utils::Hook::Set<const char*>(0x420E1E, "h1_ammo_counter_rocket");

		// overrides the 'ammo_counter_shotgunshell' material name.
		Utils::Hook::Set<const char*>(0x420E44, "h1_ammo_counter_shotgunshell");

		// overrides the 'compassping_friendlyfiring' material name.
		Utils::Hook::Set<const char*>(0x4200FA, "h1_compassping_friendlyfiring");

		// overrides the 'compassping_friendlyyelling' material name.
		//Utils::Hook::Set<const char*>(0x420121, "h1_compassping_friendlyyelling"); Not added yet.

		// overrides the 'compassping_enemyfiring' material name.
		Utils::Hook::Set<const char*>(0x420147, "h1_compassping_enemyfiring");

		// overrides the 'compassping_enemyyelling' material name.
		Utils::Hook::Set<const char*>(0x42016E, "h1_compassping_enemyyelling");

		// overrides the 'hit_direction' material name.
		Utils::Hook::Set<const char*>(0x420012, "mo_hit_direction");

		// overrides the 'hud_grenadeicon' material name.
		Utils::Hook::Set<const char*>(0x4201E2, "h1_hud_grenadeicon");

		// overrides the 'hud_flashbangicon' material name.
		//Utils::Hook::Set<const char*>(0x420209, "h1_hud_flashbangicon"); Not added yet.

		// overrides the 'hud_icon_nvg' material name.
		Utils::Hook::Set<const char*>(0x41FDD0, "h1_hud_icon_nvg");

		// overrides the 'nightvision_overlay_goggles' material name.
		Utils::Hook::Set<const char*>(0x41FDAA, "h1_nightvision_overlay_goggles");

		// overrides the 'stance_stand' material name.
		Utils::Hook::Set<const char*>(0x41FF2A, "h1_stance_stand");

		// overrides the 'stance_crouch' material name.
		Utils::Hook::Set<const char*>(0x41FF51, "h1_stance_crouch");

		// overrides the 'stance_prone' material name.
		Utils::Hook::Set<const char*>(0x41FF77, "h1_stance_prone");

		// overrides the 'stance_flash' material name.
		//Utils::Hook::Set<const char*>(0x41FF9E, "h1_stance_flash"); Not added yet.

		// overrides the 'ui_cursor' material name.
		Utils::Hook::Set<const char*>(0x564674, "h1_ui_cursor");

		// Executable versions.
		//const char* build_text = "Modern Operations Build 156 17.06.2024 17:36 win-x86";
		//*(DWORD*)0x43CCC4 = (DWORD)(build_text);

		#ifdef CoD2R
			// overrides the 'ui_pulse_text_type' soundalias name.
			Utils::Hook::Set<const char*>(0x43CFF2, "cod2r_typewriter_text_type");
			// overrides the 'ui_pulse_text_delete' soundalias name.
			Utils::Hook::Set<const char*>(0x43CFC5, "cod2r_typewriter_text_delete");
			// Disable Subtitle Glow.
			Utils::Hook::Set<BYTE>(0x5FE196, 0x0);
			Utils::Hook::Set<BYTE>(0x5FE145, 0x5);
		#endif
	}
}