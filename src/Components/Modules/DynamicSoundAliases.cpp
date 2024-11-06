#include "STDInc.hpp"

namespace Components
{
	__declspec() void gear_rattle_plr_sprint_stub()
	{
		const auto gear_rattle_plr_sprint = Game::Com_FindSoundAlias("gear_rattle_plr_sprint");
		__asm
		{
			mov eax, gear_rattle_plr_sprint;
		}
	}

	__declspec() void leather_rattle_plr_sprint_stub()
	{
		const auto leather_rattle_plr_sprint = Game::Com_FindSoundAlias("leather_rattle_plr_sprint");
		__asm
		{
			mov eax, leather_rattle_plr_sprint;
		}
	}

	__declspec() void gear_rattle_plr_run_stub()
	{
		const auto gear_rattle_plr_run = Game::Com_FindSoundAlias("gear_rattle_plr_run");
		__asm
		{
			mov eax, gear_rattle_plr_run;
		}
	}

	__declspec() void leather_rattle_plr_run_stub()
	{
		const auto leather_rattle_plr_run = Game::Com_FindSoundAlias("leather_rattle_plr_run");
		__asm
		{
			mov eax, leather_rattle_plr_run;
		}
	}

	__declspec() void gear_rattle_plr_walk_stub()
	{
		const auto gear_rattle_plr_walk = Game::Com_FindSoundAlias("gear_rattle_plr_walk");
		__asm
		{
			mov eax, gear_rattle_plr_walk;
		}
	}

	__declspec() void leather_rattle_plr_walk_stub()
	{
		const auto leather_rattle_plr_walk = Game::Com_FindSoundAlias("leather_rattle_plr_walk");
		__asm
		{
			mov eax, leather_rattle_plr_walk;
		}
	}

	__declspec() void gear_rattle_sprint_stub()
	{
		const auto gear_rattle_sprint = Game::Com_FindSoundAlias("gear_rattle_sprint");
		__asm
		{
			mov eax, gear_rattle_sprint;
		}
	}

	__declspec() void leather_rattle_sprint_stub()
	{
		const auto leather_rattle_sprint = Game::Com_FindSoundAlias("leather_rattle_sprint");
		__asm
		{
			mov eax, leather_rattle_sprint;
		}
	}

	__declspec() void gear_rattle_run_stub()
	{
		const auto gear_rattle_run = Game::Com_FindSoundAlias("gear_rattle_run");
		__asm
		{
			mov eax, gear_rattle_run;
		}
	}

	__declspec() void leather_rattle_run_stub()
	{
		const auto leather_rattle_run = Game::Com_FindSoundAlias("leather_rattle_run");
		__asm
		{
			mov eax, leather_rattle_run;
		}
	}

	__declspec() void gear_rattle_walk_stub()
	{
		const auto gear_rattle_walk = Game::Com_FindSoundAlias("gear_rattle_walk");
		__asm
		{
			mov eax, gear_rattle_walk;
		}
	}

	__declspec() void leather_rattle_walk_stub()
	{
		const auto leather_rattle_walk = Game::Com_FindSoundAlias("leather_rattle_walk");
		__asm
		{
			mov eax, leather_rattle_walk;
		}
	}

	DynamicSoundAliases::DynamicSoundAliases()
	{
		GSC::AddMethod("SetPlayerGearType", [](Game::scr_entref_t entref)
		{
			auto light = Game::GScr_AllocString("light");
			auto heavy = Game::GScr_AllocString("heavy");

			// Check if entity is a player.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				Game::gentity_s* gentity = &Game::g_entities[entref.entnum];

				if (!gentity->client)
				{
					Game::Scr_Error(Utils::String::VA("Entity: %i is not a player", entref.entnum));
					return;
				}
			}

			// Check the amount of parameters for this function.
			// Only 1 is allowed.
			if (Game::Scr_GetNumParam() != 1)
			{
				Game::Scr_Error("USAGE: SetPlayerGearType <type>\n");
				return;
			}

			// Next - get const string and then the gear type.
			Game::scr_string_t type = Game::Scr_GetConstString(0);
			if (type == NULL)
			{
				Game::Scr_Error("SetPlayerGearType: Illegal parameter!\n");
				return;
			}

			if (type == light)
			{
				Utils::Hook(0x4185FB, leather_rattle_plr_sprint_stub, HOOK_CALL).install()->quick();
				Utils::Hook(0x418617, leather_rattle_plr_run_stub, HOOK_CALL).install()->quick();
				Utils::Hook(0x4185DF, leather_rattle_plr_walk_stub, HOOK_CALL).install()->quick();
				//#ifdef DEVELOPER_MODE
				//	Game::Com_Printf(0, "Current Player Gear Type is [^2Light^7]\n");
				//#endif
				return;
			}
			else if (type == heavy)
			{
				Utils::Hook(0x4185FB, gear_rattle_plr_sprint_stub, HOOK_CALL).install()->quick();
				Utils::Hook(0x418617, gear_rattle_plr_run_stub, HOOK_CALL).install()->quick();
				Utils::Hook(0x4185DF, gear_rattle_plr_walk_stub, HOOK_CALL).install()->quick();
				//#ifdef DEVELOPER_MODE
				//	Game::Com_Printf(0, "Current Player Gear Type is [^1Heavy^7]\n");
				//#endif
				return;
			}
			else if ( type != light && type != heavy )
			{
				Game::Scr_Error("Uknown Gear Type\n Gear Type must be: ^2light^7 or ^1heavy^7.");
			}
			else
			{
				Game::Scr_Error("Gear type is undefined\n Gear Type must be: ^2light^7 or ^1heavy^7.");
				return;
			}
		}, false);

		GSC::AddMethod("EnableGearSounds", [](Game::scr_entref_t entref)
			{
				Utils::Hook(0x4185FB, gear_rattle_plr_sprint_stub, HOOK_CALL).install()->quick();
				Utils::Hook(0x418617, gear_rattle_plr_run_stub, HOOK_CALL).install()->quick();
				Utils::Hook(0x4185DF, gear_rattle_plr_walk_stub, HOOK_CALL).install()->quick();
				//#ifdef DEVELOPER_MODE
				//	Game::Com_Printf(0, "Game [^2DEBUG^7]: Player's Gear Sounds Enabled.\n");
				//#endif
			}, false);

		GSC::AddMethod("DisableGearSounds", [](Game::scr_entref_t entref)
			{
				Utils::Hook(0x4185FB, leather_rattle_plr_sprint_stub, HOOK_CALL).install()->quick();
				Utils::Hook(0x418617, leather_rattle_plr_run_stub, HOOK_CALL).install()->quick();
				Utils::Hook(0x4185DF, leather_rattle_plr_walk_stub, HOOK_CALL).install()->quick();
				//#ifdef DEVELOPER_MODE
				//	Game::Com_Printf(0, "Game [^2DEBUG^7]: Player's Gear Sounds Disabled.\n");
				//#endif
			}, false);

		GSC::AddFunction("AIEnableGearSounds", []
			{
				Utils::Hook(0x418659, gear_rattle_sprint_stub, HOOK_CALL).install()->quick();
				Utils::Hook(0x418675, gear_rattle_run_stub, HOOK_CALL).install()->quick();
				Utils::Hook(0x41863D, gear_rattle_walk_stub, HOOK_CALL).install()->quick();
				//#ifdef DEVELOPER_MODE
				//	Game::Com_Printf(0, "Game [^2DEBUG^7]: AI Gear Sounds Enabled.\n");
				//#endif
			}, false);

		GSC::AddFunction("AIDisableGearSounds", []
			{
				Utils::Hook(0x418659, leather_rattle_sprint_stub, HOOK_CALL).install()->quick();
				Utils::Hook(0x418675, leather_rattle_run_stub, HOOK_CALL).install()->quick();
				Utils::Hook(0x41863D, leather_rattle_walk_stub, HOOK_CALL).install()->quick();
				//#ifdef DEVELOPER_MODE
				//	Game::Com_Printf(0, "Game [^2DEBUG^7]: AI Gear Sounds Disabled.\n");
				//#endif
			}, false);
	}
}