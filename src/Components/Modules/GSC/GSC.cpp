#include "STDInc.hpp"

namespace Components
{
	std::unordered_map<std::string, int> GSC::ScriptMainHandles;
	std::unordered_map<std::string, int> GSC::ScriptInitHandles;

	std::unordered_map<std::string, Game::scr_function_t> GSC::CustomScrFunctions;
	std::unordered_map<std::string, Game::scr_method_t> GSC::CustomScrMethods;

	void GSC::AddFunction(const char* name, Game::xfunction_t func, int type)
	{
		Game::scr_function_t toAdd;
		toAdd.function = func;
		toAdd.developer = type;

		GSC::CustomScrFunctions.insert_or_assign(Utils::String::ToLower(name), toAdd);
	}

	void GSC::AddMethod(const char* name, Game::xmethod_t method, int developer)
	{
		Game::scr_method_t toAdd;
		toAdd.function = method;
		toAdd.developer = developer;

		GSC::CustomScrMethods.insert_or_assign(Utils::String::ToLower(name), toAdd);
	}

	int weapInspectAnimIndex;

	// GSC Methods.
	void GSC::AddMethods()
	{
		// Player methods.

		GSC::AddMethod("allowFire", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				entityNum = entref.entnum;
				Game::gentity_s* gentity = &Game::g_entities[entityNum];

				if (!gentity->client)
				{
					Game::Scr_Error(Utils::String::VA("Entity: %i is not a player", entref.entnum));
					return;
				}
			}

			if (Game::Scr_GetInt(0))
			{
				Dvars::player_allowFire->current.enabled = true;
				Dvars::player_allowFire->latched.enabled = true;
			}
			else
			{
				Dvars::player_allowFire->current.enabled = false;
				Dvars::player_allowFire->latched.enabled = false;
			}
		}, false);

		GSC::AddMethod("forceMantle", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			#ifdef DEVELOPER_MODE
				Game::Scr_Error("'ForceMantle' is unfinished and not working.)\n");
			#endif
			//Command::Execute("+gostand");
		}, false);

		GSC::AddMethod("mantleDisable", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			Command::Execute("mantle_enable 0");
		}, false);

		GSC::AddMethod("mantleEnable", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			Command::Execute("mantle_enable 1");
		}, false);

		GSC::AddMethod("disableWeaponCycling", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			Command::Execute("cg_weaponCycleDelay 999999999");
		}, false);

		GSC::AddMethod("enableWeaponCycling", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			Command::Execute("cg_weaponCycleDelay 0");
		}, false);

		GSC::AddMethod("disableWeaponSwitch", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			Command::Execute("cg_weaponCycleDelay 999999999");
		}, false);

		GSC::AddMethod("enableWeaponSwitch", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			Command::Execute("cg_weaponCycleDelay 0");
		}, false);

		GSC::AddMethod("startWeaponInspection", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			Command::Execute("weapinspect");
		}, false);

		GSC::AddMethod("forceReloadWeapon", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			Command::Execute("+reload");
			Command::Execute("-reload");
		}, false);

		GSC::AddMethod("forceNightVisionSwitch", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			Command::Execute("+actionslot 1");
			Command::Execute("-actionslot 1");
		}, false);

		GSC::AddMethod("forceMelee", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			Command::Execute("+melee");
			Command::Execute("-melee");
		}, false);

		GSC::AddMethod("forceStanceCrouch", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			Command::Execute("gocrouch");
		}, false);

		GSC::AddMethod("forceStanceProne", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			Command::Execute("goprone");
		}, false);

		GSC::AddMethod("forceStanceStand", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			Command::Execute("+gostand");
			Command::Execute("-gostand");
		}, false);

		GSC::AddMethod("forceThrowSmoke", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			Command::Execute("+smoke");
			Command::Execute("-smoke");
		}, false);

		GSC::AddMethod("forceThrowFrag", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			Command::Execute("+frag");
			Command::Execute("-frag");
		}, false);
		
		GSC::AddMethod("setPerk", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			auto bulletpenetration = Game::GScr_AllocString("specialty_bulletpenetration");
			auto marathon = Game::GScr_AllocString("specialty_marathon");
			auto fastreload = Game::GScr_AllocString("specialty_fastreload");
			auto longersprint = Game::GScr_AllocString("specialty_longersprint");
			auto extralethal = Game::GScr_AllocString("specialty_extralethal");
			auto quickdraw = Game::GScr_AllocString("specialty_quickdraw");
			auto armorvest = Game::GScr_AllocString("specialty_armorvest");
			auto armorvest2 = Game::GScr_AllocString("specialty_armorvest2");
			auto twoprimaries = Game::GScr_AllocString("specialty_twoprimaries");
			auto finalstand = Game::GScr_AllocString("specialty_finalstand");

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
				Game::Scr_Error("USAGE: setPerk <perk>\n");
				return;
			}

			// Next - get const string and then the perk name.
			Game::scr_string_t perk = Game::Scr_GetConstString(0);
			if (perk == NULL)
			{
				Game::Scr_Error(Utils::String::VA("Unknown Perk %s in setPerk (valid perk names are: specialty_bulletpenetration, specialty_marathon, specialty_fastreload, specialty_longersprint, specialty_extralethal, specialty_quickdraw, specialty_armorvest, specialty_armorvest2, specialty_twoprimaries and specialty_finalstand)\n", perk));
				return;
			}

			if (perk == bulletpenetration)
			{
				Command::Execute("perk_bulletpenetration_enabled 1");
				return;
			}
			else if (perk == marathon)
			{
				Command::Execute("perk_marathon_enabled 1");
				return;
			}
			else if (perk == fastreload)
			{
				Command::Execute("perk_fastreload_enabled 1");
				return;
			}
			else if (perk == longersprint)
			{
				Command::Execute("perk_longersprint_enabled 1");
				return;
			}
			else if (perk == extralethal)
			{
				Command::Execute("perk_extralethal_enabled 1");
				return;
			}
			else if (perk == quickdraw)
			{
				Command::Execute("perk_quickdraw_enabled 1");
				return;
			}
			else if (perk == armorvest)
			{
				Command::Execute("perk_armorvest_enabled 1");
				return;
			}
			else if (perk == armorvest2)
			{
				Command::Execute("perk_armorvest2_enabled 1");
				return;
			}
			else if (perk == twoprimaries)
			{
				Command::Execute("perk_twoprimaries_enabled 1");
				return;
			}
			else if (perk == finalstand)
			{
				Command::Execute("perk_finalstand_enabled 1");
				return;
			}
			else
			{
				Game::Scr_Error(Utils::String::VA("Unknown Perk %s in setPerk (valid perk names are: specialty_bulletpenetration, specialty_marathon, specialty_fastreload, specialty_longersprint, specialty_extralethal, specialty_quickdraw, specialty_armorvest, specialty_armorvest2, specialty_twoprimaries and specialty_finalstand)\n", perk));
				return;
			}
		}, false);

		GSC::AddMethod("unsetPerk", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			auto bulletpenetration = Game::GScr_AllocString("specialty_bulletpenetration");
			auto marathon = Game::GScr_AllocString("specialty_marathon");
			auto fastreload = Game::GScr_AllocString("specialty_fastreload");
			auto longersprint = Game::GScr_AllocString("specialty_longersprint");
			auto extralethal = Game::GScr_AllocString("specialty_extralethal");
			auto quickdraw = Game::GScr_AllocString("specialty_quickdraw");
			auto armorvest = Game::GScr_AllocString("specialty_armorvest");
			auto armorvest2 = Game::GScr_AllocString("specialty_armorvest2");
			auto twoprimaries = Game::GScr_AllocString("specialty_twoprimaries");
			auto finalstand = Game::GScr_AllocString("specialty_finalstand");

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
				Game::Scr_Error("USAGE: unsetPerk <perk>\n");
				return;
			}

			// Next - get const string and then the perk name.
			Game::scr_string_t perk = Game::Scr_GetConstString(0);
			if (perk == NULL)
			{
				Game::Scr_Error(Utils::String::VA("Unknown Perk %s in unsetPerk (valid perk names are: specialty_bulletpenetration, specialty_marathon, specialty_fastreload, specialty_longersprint, specialty_extralethal, specialty_quickdraw, specialty_armorvest, specialty_armorvest2, specialty_twoprimaries and specialty_finalstand)\n", perk));
				return;
			}

			if (perk == bulletpenetration)
			{
				Command::Execute("perk_bulletpenetration_enabled 0");
				return;
			}
			else if (perk == marathon)
			{
				Command::Execute("perk_marathon_enabled 0");
				return;
			}
			else if (perk == fastreload)
			{
				Command::Execute("perk_fastreload_enabled 0");
				return;
			}
			else if (perk == longersprint)
			{
				Command::Execute("perk_longersprint_enabled 0");
				return;
			}
			else if (perk == extralethal)
			{
				Command::Execute("perk_extralethal_enabled 0");
				return;
			}
			else if (perk == quickdraw)
			{
				Command::Execute("perk_quickdraw_enabled 0");
				return;
			}
			else if (perk == armorvest)
			{
				Command::Execute("perk_armorvest_enabled 0");
				return;
			}
			else if (perk == armorvest2)
			{
				Command::Execute("perk_armorvest2_enabled 0");
				return;
			}
			else if (perk == twoprimaries)
			{
				Command::Execute("perk_twoprimaries_enabled 0");
				return;
			}
			else if (perk == finalstand)
			{
				Command::Execute("perk_finalstand_enabled 0");
				return;
			}
			else
			{
				Game::Scr_Error(Utils::String::VA("Unknown Perk %s in unsetPerk (valid perk names are: specialty_bulletpenetration, specialty_marathon, specialty_fastreload, specialty_longersprint, specialty_extralethal, specialty_quickdraw, specialty_armorvest, specialty_armorvest2, specialty_twoprimaries and specialty_finalstand)\n", perk));
				return;
			}
		}, false);

		/*
		GSC::AddMethod("setFloatZoom", [](Game::scr_entref_t entref) // gsc: level.player setFloatZoom();
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			const auto weapon = Game::bg_weaponDefs[Game::g_clients->ps.weapon];
			weapon->fAdsZoomFov = 160.0f;
			#ifdef DEVELOPER_MODE
				Game::Scr_Error(Utils::String::VA("level.player setFloatZoom: for current weapon %i \n", weapon->fAdsZoomFov));
			#endif
		}, false);
		*/

		/*
		GSC::AddMethod("setFastReloadForCurrentWeapon", [](Game::scr_entref_t entref) // gsc: level.player setFastReloadForCurrentWeapon();
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			const auto weapon = Game::bg_weaponDefs[Game::g_clients->ps.weapon];
			weapon->iReloadTime = (weapon->iReloadTime - 1500);
			weapon->iReloadEmptyTime = (weapon->iReloadEmptyTime - 1500);
			int iReloadTime = weapon->iReloadTime;
			int iReloadEmptyTime = weapon->iReloadEmptyTime;
			#ifdef DEVELOPER_MODE
				Game::Scr_Error(Utils::String::VA("Current reload time: %i\n", iReloadTime), 0);
				Game::Scr_Error(Utils::String::VA("Current empty reload time: %i\n", iReloadEmptyTime), 0);
			#endif
		}, false);
		*/

		GSC::AddMethod("laserForceOn", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			Command::Execute("laserforceon");
			
			Utils::Hook::Set<BYTE>(0x433AD1, 0x75);// Turns on the laser while not in nightvision
			Utils::Hook::Nop(0x433ADA, 2);
		});

		GSC::AddMethod("laserForceOff", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			Command::Execute("laserforceoff");
			
			Utils::Hook::Set<BYTE>(0x433AD1, 0x74);// Turns off the laser while not in nightvision(turns it on)
			Utils::Hook::Set<BYTE>(0x433ADA, 0x74);
			Utils::Hook::Set<BYTE>(0x433ADB, 0x13);
		});

		// Internal Methods. only called in animscripts\utility_code.gsc
		GSC::AddMethod("nvgLaserForceOn", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			Utils::Hook::Set<BYTE>(0x433AD1, 0x74);
		});

		GSC::AddMethod("nvgLaserForceOff", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			Utils::Hook::Set<BYTE>(0x433AD1, 0x75);
		});
		// Internal Methods. only called in animscripts\utility_code.gsc

		GSC::AddMethod("setWeaponHudIconOverride", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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
			// Only 2 are allowed.
			if (Game::Scr_GetNumParam() != 2)
			{
				Game::Scr_Error("GScr_setWeaponHudIconOverride( Actionslot, shader ) requires two parameters");
				return;
			}

			const auto actionslot = Game::Scr_GetString(0);
			if (actionslot == NULL)
			{
				Game::Scr_Error("GScr_setWeaponHudIconOverride: Illegal parameter! First parameter must be a string.\n");
				return;
			}

			const auto shader = Game::Scr_GetString(1);
			if (shader == NULL)
			{
				Game::Scr_Error("GScr_setWeaponHudIconOverride: Illegal parameter! Second parameter must be a string.\n");
				return;
			}

			//Command::Execute(Utils::String::VA("%s %s", "cg_weaponHudIconOverrideActionSlot", actionslot, false));
			//Command::Execute(Utils::String::VA("%s %s", "cg_weaponHudIconOverrideShader", shader, false));
		});

		/*
		GSC::AddMethod("enableFastReload", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			std::vector<Game::WeaponDef_s*> weapons;

			FastFiles::Enum_Assets(Game::ASSET_TYPE_WEAPON, [&weapons](Game::XAssetHeader header)
			{
				weapons.push_back(header.weapon);
			}, false);

			// sort weapons
			std::sort(weapons.begin(), weapons.end(), [](Game::WeaponDef_s* weapon1, Game::WeaponDef_s* weapon2)
			{
				return std::string_view(weapon1->szInternalName) <
					std::string_view(weapon2->szInternalName);
			});

			#ifdef DEVELOPER_MODE
				Game::Scr_Error("^2Fast reload enabled! \n");
			#endif

			for (std::size_t i = 0; i < weapons.size(); i++)
			{
				weapons[i]->iReloadTime = weapons[i]->iReloadTime / 1.75;
				weapons[i]->iReloadEmptyTime = weapons[i]->iReloadEmptyTime / 1.75;
				weapons[i]->iReloadAddTime = weapons[i]->iReloadAddTime / 1.75;
			}
		});
		*/

		/*
		GSC::AddMethod("disableFastReload", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			std::vector<Game::WeaponDef_s*> weapons;

			FastFiles::Enum_Assets(Game::ASSET_TYPE_WEAPON, [&weapons](Game::XAssetHeader header)
			{
				weapons.push_back(header.weapon);
			}, false);

			// sort weapons
			std::sort(weapons.begin(), weapons.end(), [](Game::WeaponDef_s* weapon1, Game::WeaponDef_s* weapon2)
			{
				return std::string_view(weapon1->szInternalName) <
					std::string_view(weapon2->szInternalName);
			});

			#ifdef DEVELOPER_MODE
				Game::Scr_Error("^1Fast reload disabled! \n");
			#endif

			for (std::size_t i = 0; i < weapons.size(); i++)
			{
				// Commented out.
				// Might cause bugs?
				//old_weapon_reload_stats.clear();
				//old_weapon_reload_empty_stats.clear();
				//old_weapon_reload_add_time_stats.clear();
				
				weapons[i]->iReloadTime = old_weapon_reload_stats[i];
				weapons[i]->iReloadEmptyTime = old_weapon_reload_empty_stats[i];
				weapons[i]->iReloadAddTime = old_weapon_reload_add_time_stats[i];
			}
		});
		*/

		/*
		GSC::AddMethod("resetReloadSpeed", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			std::vector<Game::WeaponDef_s*> weapons;

			FastFiles::Enum_Assets(Game::ASSET_TYPE_WEAPON, [&weapons](Game::XAssetHeader header)
			{
				weapons.push_back(header.weapon);
			}, false);

			// sort weapons
			std::sort(weapons.begin(), weapons.end(), [](Game::WeaponDef_s* weapon1, Game::WeaponDef_s* weapon2)
			{
				return std::string_view(weapon1->szInternalName) <
					std::string_view(weapon2->szInternalName);
			});

			old_weapon_reload_stats.clear();
			old_weapon_reload_empty_stats.clear();
			old_weapon_reload_add_time_stats.clear();

			for (std::size_t i = 0; i < weapons.size(); i++)
			{
				old_weapon_reload_stats.push_back(weapons[i]->iReloadTime);
				old_weapon_reload_empty_stats.push_back(weapons[i]->iReloadEmptyTime);
				old_weapon_reload_add_time_stats.push_back(weapons[i]->iReloadAddTime);
				#ifdef DEVELOPER_MODE
					Game::Scr_Error("%s %d \n", weapons[i]->szInternalName, old_weapon_reload_stats[i]);
					Game::Scr_Error("%s %d \n", weapons[i]->szInternalName, old_weapon_reload_empty_stats[i]);
					Game::Scr_Error("%s %d \n", weapons[i]->szInternalName, old_weapon_reload_add_time_stats[i]);
				#endif
			}
		});
		*/

		GSC::AddMethod("enableWeaponInspection", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			Command::Execute("cg_weaponInspection 1");
		});

		GSC::AddMethod("disableWeaponInspection", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			Command::Execute("cg_weaponInspection 0");
		});

		GSC::AddMethod("enableBreaching", [](Game::scr_entref_t entref)
		{
			//const auto* ent = Game::GetPlayerEntity(entref);

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

			//Command::Execute("breachraise");

			unsigned int weapIndex = Game::ps->offHandIndex;
			if ((Game::ps->weapFlags & 2) == 0)
			{
				weapIndex = Game::ps->weapon;
			}

			Game::DObj_s* obj = Game::cg_weaponsArray[Game::g_clients->ps.weapon].viewModelDObj;

			const auto weapon = Game::bg_weaponDefs[Game::g_clients->ps.weapon];
			float breachRaiseTime = weapon->iHoldFireTime;
			if ( breachRaiseTime != 0.0f )
			{
				Game::gentity_s *ent = Game::GetPlayerEntity(entref);
				ent->client->ps.weapFlags |= 0x400u;
				ent->client->ps.forcedViewAnimWeaponIdx = weapIndex;
				ent->client->ps.forcedViewAnimWeaponState = Game::WEAPON_OFFHAND_HOLD;// holdFireAnim
			}
		}, false);

		GSC::AddMethod("disableBreaching", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			// Currently Empty.
		}, false);

		GSC::AddMethod("enableOffhandWeapons", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			// Currently Empty.
		}, false);

		GSC::AddMethod("disableOffhandWeapons", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			// Currently Empty.
		}, false);

		GSC::AddMethod("enableWeaponSwitchTransition", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			//Command::Execute("cg_weaponSwitchTransition 1");
		}, false);

		GSC::AddMethod("disableWeaponSwitchTransition", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			//Command::Execute("cg_weaponSwitchTransition 0");
		}, false);

		GSC::AddMethod("playViewmodelAnimation", [](Game::scr_entref_t entref)
		{
			Game::gentity_s *ent = Game::GetPlayerEntity(entref);

			auto reload = Game::GScr_AllocString("reload");
			auto inspect = Game::GScr_AllocString("inspect");
			auto fire = Game::GScr_AllocString("fire");
			auto nvg_up = Game::GScr_AllocString("NVG_up");
			auto nvg_down = Game::GScr_AllocString("NVG_down");

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
			// Only 2 are allowed.
			if (Game::Scr_GetNumParam() != 2)
			{
				Game::Scr_Error("GScr_playViewmodelAnimation( Weapon, animation ) requires two parameters");
				return;
			}

			Game::scr_string_t weaponname = Game::Scr_GetConstString(0);
			if (weaponname == NULL)
			{
				Game::Scr_Error("GScr_playViewmodelAnimation: Illegal parameter! First parameter must be a string.\n");
				return;
			}

			Game::scr_string_t animation = Game::Scr_GetConstString(1);
			if (animation == NULL)
			{
				Game::Scr_Error("GScr_playViewmodelAnimation: Illegal parameter! Second parameter must be a string.\n");
				return;
			}

			unsigned int weapIndex = Game::G_GetWeaponIndexForName(Game::Scr_GetString(0));
			if (weapIndex != 0)
			{
				Game::DObj_s* obj = Game::cg_weaponsArray[weapIndex].viewModelDObj;
				
				const auto weapon = Game::bg_weaponDefs[weapIndex];
				
				if (animation == reload)
				{
					float time = weapon->iReloadTime;
					if ( time != 0.0f )
					{
						ent->client->ps.weapFlags |= 0x400u;
						ent->client->ps.forcedViewAnimWeaponIdx = weapIndex;
						ent->client->ps.forcedViewAnimWeaponState = Game::WEAPON_RELOADING;// reloadAnim
					}
					else
					{
						Game::Scr_Error("GScr_playViewmodelAnimation: 'reloadTime' is 0! Must be at least 0.05.\n");
					}
				}
				else if (animation == inspect)
				{
					float time = weapon->iDetonateTime;
					if ( time != 0.0f )
					{
						ent->client->ps.weapFlags |= 0x400u;
						ent->client->ps.forcedViewAnimWeaponIdx = weapIndex;
						
						if ( weapon->weapType == 1 )
							weapInspectAnimIndex = Game::WEAPON_RECHAMBERING;
						else
							weapInspectAnimIndex = Game::WEAPON_DETONATING;
						
						ent->client->ps.forcedViewAnimWeaponState = weapInspectAnimIndex;// detonateAnim
					}
					else
					{
						Game::Scr_Error("GScr_playViewmodelAnimation: 'reloadTime' is 0! Must be at least 0.05.\n");
					}
				}
				else if (animation == fire)
				{
					float time = weapon->iFireTime;
					if ( time != 0.0f )
					{
						ent->client->ps.weapFlags |= 0x400u;
						ent->client->ps.forcedViewAnimWeaponIdx = weapIndex;
						ent->client->ps.forcedViewAnimWeaponState = Game::WEAPON_FIRING;// fireAnim
					}
					else
					{
						Game::Scr_Error("GScr_playViewmodelAnimation: 'fireTime' is 0! Must be at least 0.05.\n");
					}
				}
				else if (animation == nvg_up)
				{
					float time = weapon->nightVisionWearTime;
					if ( time != 0.0f )
					{
						ent->client->ps.weapFlags |= 0x400u;
						ent->client->ps.forcedViewAnimWeaponIdx = weapIndex;
						ent->client->ps.forcedViewAnimWeaponState = Game::WEAPON_NIGHTVISION_WEAR;// nightVisionWearAnim
					}
					else
					{
						Game::Scr_Error("GScr_playViewmodelAnimation: 'nightVisionWearTime' is 0! Must be at least 0.05.\n");
					}
				}
				else if (animation == nvg_down)
				{
					float time = weapon->nightVisionRemoveTime;
					if ( time != 0.0f )
					{
						ent->client->ps.weapFlags |= 0x400u;
						ent->client->ps.forcedViewAnimWeaponIdx = weapIndex;
						ent->client->ps.forcedViewAnimWeaponState = Game::WEAPON_NIGHTVISION_REMOVE;// nightVisionRemoveAnim
					}
					else
					{
						Game::Scr_Error("GScr_playViewmodelAnimation: 'nightVisionRemoveTime' is 0! Must be at least 0.05.\n");
					}
				}
				else
				{
					Game::Scr_Error(Utils::String::VA("^3Animation name ''%s'' is not supported by this function.\n", animation));
				}
			}
			else
			{
				Game::Scr_Error(Utils::String::VA("^3Can't play ''%s'' on weapon 'none'.\n", animation));
			}
		});

		GSC::AddMethod("startSprinting", [](Game::scr_entref_t entref)
		{
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
				Game::Scr_Error("GScr_startSprinting( Weapon ) requires one parameter");
				return;
			}

			Game::scr_string_t weaponname = Game::Scr_GetConstString(0);
			if (weaponname == NULL)
			{
				Game::Scr_Error("GScr_startSprinting: Illegal parameter! First parameter must be a string.\n");
				return;
			}

			unsigned int weapIndex = Game::G_GetWeaponIndexForName(Game::Scr_GetString(0));
			if (weapIndex != 0)
			{
				Game::DObj_s* obj = Game::cg_weaponsArray[weapIndex].viewModelDObj;
				
				const auto weapon = Game::bg_weaponDefs[weapIndex];
				float sprintInTime = weapon->sprintInTime;
				if ( sprintInTime != 0.0f )
				{
					Game::gentity_s *ent = Game::GetPlayerEntity(entref);
					ent->client->ps.weapFlags |= 0x400u;
					ent->client->ps.forcedViewAnimWeaponIdx = weapIndex;
					ent->client->ps.forcedViewAnimWeaponState = Game::WEAP_ANIM_SPRINT_IN;
				}
			}
		}, false);

		GSC::AddMethod("loopSprinting", [](Game::scr_entref_t entref)
		{
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
				Game::Scr_Error("GScr_loopSprinting( Weapon ) requires one parameter");
				return;
			}

			Game::scr_string_t weaponname = Game::Scr_GetConstString(0);
			if (weaponname == NULL)
			{
				Game::Scr_Error("GScr_loopSprinting: Illegal parameter! First parameter must be a string.\n");
				return;
			}

			unsigned int weapIndex = Game::G_GetWeaponIndexForName(Game::Scr_GetString(0));
			if (weapIndex != 0)
			{
				Game::DObj_s* obj = Game::cg_weaponsArray[weapIndex].viewModelDObj;
				
				const auto weapon = Game::bg_weaponDefs[weapIndex];
				float sprintLoopTime = weapon->sprintLoopTime;
				if ( sprintLoopTime != 0.0f )
				{
					Game::gentity_s *ent = Game::GetPlayerEntity(entref);
					ent->client->ps.weapFlags |= 0x400u;
					ent->client->ps.forcedViewAnimWeaponIdx = weapIndex;
					ent->client->ps.forcedViewAnimWeaponState = Game::WEAP_ANIM_SPRINT_LOOP;
				}
			}
		}, false);

		GSC::AddMethod("endSprinting", [](Game::scr_entref_t entref)
		{
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
				Game::Scr_Error("GScr_endSprinting( Weapon ) requires one parameter");
				return;
			}

			Game::scr_string_t weaponname = Game::Scr_GetConstString(0);
			if (weaponname == NULL)
			{
				Game::Scr_Error("GScr_endSprinting: Illegal parameter! First parameter must be a string.\n");
				return;
			}

			unsigned int weapIndex = Game::G_GetWeaponIndexForName(Game::Scr_GetString(0));
			if (weapIndex != 0)
			{
				Game::DObj_s* obj = Game::cg_weaponsArray[weapIndex].viewModelDObj;
				
				const auto weapon = Game::bg_weaponDefs[weapIndex];
				float sprintOutTime = weapon->sprintOutTime;
				if ( sprintOutTime != 0.0f )
				{
					Game::gentity_s *ent = Game::GetPlayerEntity(entref);
					ent->client->ps.weapFlags |= 0x400u;
					ent->client->ps.forcedViewAnimWeaponIdx = weapIndex;
					ent->client->ps.forcedViewAnimWeaponState = Game::WEAP_ANIM_SPRINT_OUT;
				}
			}
		}, false);

		GSC::AddMethod("raiseWeapon", [](Game::scr_entref_t entref)
		{
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

			unsigned int weapIndex = Game::ps->offHandIndex;
			if ((Game::ps->weapFlags & 2) == 0)
			{
				weapIndex = Game::ps->weapon;
			}

			Game::DObj_s* obj = Game::cg_weaponsArray[Game::g_clients->ps.weapon].viewModelDObj;

			const auto weapon = Game::bg_weaponDefs[Game::g_clients->ps.weapon];
			float quickRaiseTime = weapon->quickRaiseTime;
			if ( quickRaiseTime != 0.0f )
			{
				Game::gentity_s *ent = Game::GetPlayerEntity(entref);
				ent->client->ps.weapFlags |= 0x400u;
				ent->client->ps.forcedViewAnimWeaponIdx = weapIndex;
				ent->client->ps.forcedViewAnimWeaponState = Game::WEAP_ANIM_QUICK_RAISE;
			}
		}, false);



		// Return player Methods

		GSC::AddMethod("allowMeleeStabAnimations", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

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

			//#ifdef MODERN_OPS
			//	#ifdef DEVELOPER_MODE
			//		Game::Scr_Error("Melee Stab Animations are disabled in 'AllowMeleeStabAnimations()'.\n");
			//	#endif
			//	return Game::Scr_AddInt(0);
			//#endif

			//#ifdef MW1CR
			//	#ifdef DEVELOPER_MODE
			//		Game::Scr_Error("Melee Stab Animations are disabled in 'AllowMeleeStabAnimations()'.\n");
			//	#endif
			//	return Game::Scr_AddInt(0);
			//#endif

			//#ifdef DEVELOPER_MODE
			//	Game::Scr_Error("Melee Stab Animations are enabled in 'AllowMeleeStabAnimations()'.\n");
			//#endif
			
			return Game::Scr_AddInt(1);
		}, false);

		GSC::AddMethod("hasPerk", [](Game::scr_entref_t entref)
		{
			auto bulletpenetration = Game::GScr_AllocString("specialty_bulletpenetration");
			auto marathon = Game::GScr_AllocString("specialty_marathon");
			auto fastreload = Game::GScr_AllocString("specialty_fastreload");
			auto longersprint = Game::GScr_AllocString("specialty_longersprint");
			auto extralethal = Game::GScr_AllocString("specialty_extralethal");
			auto quickdraw = Game::GScr_AllocString("specialty_quickdraw");
			auto armorvest = Game::GScr_AllocString("specialty_armorvest");
			auto armorvest2 = Game::GScr_AllocString("specialty_armorvest2");
			auto twoprimaries = Game::GScr_AllocString("specialty_twoprimaries");
			auto finalstand = Game::GScr_AllocString("specialty_finalstand");

			const auto* ent = Game::GetPlayerEntity(entref);

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
				Game::Scr_Error("USAGE: hasPerk <perk>\n");
				return;
			}

			// Next - get const string and then the perk name.
			Game::scr_string_t perk = Game::Scr_GetConstString(0);
			if (perk == NULL)
			{
				Game::Scr_Error(Utils::String::VA("Unknown Perk %s in hasPerk (valid perk names are: specialty_bulletpenetration, specialty_marathon, specialty_fastreload, specialty_longersprint, specialty_extralethal, specialty_quickdraw, specialty_armorvest, specialty_armorvest2, specialty_twoprimaries and specialty_finalstand)\n", perk));
				return;
			}

			if (perk == bulletpenetration)
			{
				const auto perk_enabled = Dvars::Functions::Dvar_FindVar("perk_bulletpenetration_enabled");
				if (perk_enabled->current.enabled)
				{
					return Game::Scr_AddInt(1);
				}
			}
			else if (perk == marathon)
			{
				const auto perk_enabled = Dvars::Functions::Dvar_FindVar("perk_marathon_enabled");
				if (perk_enabled->current.enabled)
				{
					return Game::Scr_AddInt(1);
				}
			}
			else if (perk == fastreload)
			{
				const auto perk_enabled = Dvars::Functions::Dvar_FindVar("perk_fastreload_enabled");
				if (perk_enabled->current.enabled)
				{
					return Game::Scr_AddInt(1);
				}
			}
			else if (perk == longersprint)
			{
				const auto perk_enabled = Dvars::Functions::Dvar_FindVar("perk_longersprint_enabled");
				if (perk_enabled->current.enabled)
				{
					return Game::Scr_AddInt(1);
				}
			}
			else if (perk == extralethal)
			{
				const auto perk_enabled = Dvars::Functions::Dvar_FindVar("perk_extralethal_enabled");
				if (perk_enabled->current.enabled)
				{
					return Game::Scr_AddInt(1);
				}
			}
			else if (perk == quickdraw)
			{
				const auto perk_enabled = Dvars::Functions::Dvar_FindVar("perk_quickdraw_enabled");
				if (perk_enabled->current.enabled)
				{
					return Game::Scr_AddInt(1);
				}
			}
			else if (perk == armorvest)
			{
				const auto perk_enabled = Dvars::Functions::Dvar_FindVar("perk_armorvest_enabled");
				if (perk_enabled->current.enabled)
				{
					return Game::Scr_AddInt(1);
				}
			}
			else if (perk == armorvest2)
			{
				const auto perk_enabled = Dvars::Functions::Dvar_FindVar("perk_armorvest2_enabled");
				if (perk_enabled->current.enabled)
				{
					return Game::Scr_AddInt(1);
				}
			}
			else if (perk == twoprimaries)
			{
				const auto perk_enabled = Dvars::Functions::Dvar_FindVar("perk_twoprimaries_enabled");
				if (perk_enabled->current.enabled)
				{
					return Game::Scr_AddInt(1);
				}
			}
			else if (perk == finalstand)
			{
				const auto perk_enabled = Dvars::Functions::Dvar_FindVar("perk_finalstand_enabled");
				if (perk_enabled->current.enabled)
				{
					return Game::Scr_AddInt(1);
				}
			}
			else
			{
				Game::Scr_Error(Utils::String::VA("Unknown Perk %s in hasPerk (valid perk names are: specialty_bulletpenetration, specialty_marathon, specialty_fastreload, specialty_longersprint, specialty_extralethal, specialty_quickdraw, specialty_armorvest, specialty_armorvest2, specialty_twoprimaries and specialty_finalstand)\n", perk));
				return Game::Scr_AddInt(0);
			}

			return Game::Scr_AddInt(0);
		}, false);

		GSC::AddMethod("isReloading", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				entityNum = entref.entnum;
				Game::gentity_s* gentity = &Game::g_entities[entityNum];

				if (!gentity->client)
				{
					Game::Scr_Error(Utils::String::VA("Entity: %i is not a player", entref.entnum));
					return;
				}
			}

			if (ent->client->ps.weaponstate >= Game::WEAPON_RELOADING && ent->client->ps.weaponstate <= Game::WEAPON_RELOAD_END)
				return Game::Scr_AddInt(true);
			else
				return Game::Scr_AddInt(false);
		}, false);

		GSC::AddMethod("isSprinting", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				entityNum = entref.entnum;
				Game::gentity_s* gentity = &Game::g_entities[entityNum];

				if (!gentity->client)
				{
					Game::Scr_Error(Utils::String::VA("Entity: %i is not a player", entref.entnum));
					return;
				}
			}

			if (ent->client->ps.pm_flags & Game::PMF_SPRINTING)
				return Game::Scr_AddInt(true);
			else
				return Game::Scr_AddInt(false);
		}, false);

		GSC::AddMethod("isWearingNightVision", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				entityNum = entref.entnum;
				Game::gentity_s* gentity = &Game::g_entities[entref.entnum];

				if (!gentity->client)
				{
					Game::Scr_Error(Utils::String::VA("Entity: %i is not a player", entref.entnum));
					return;
				}
				
				if (Game::g_clients->ps.weaponstate == 0x19)
					return Game::Scr_AddInt(true);
				else
					return Game::Scr_AddInt(false);
			}
		}, false);
		
		GSC::AddMethod("isRemovingNightVision", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				entityNum = entref.entnum;
				Game::gentity_s* gentity = &Game::g_entities[entref.entnum];

				if (!gentity->client)
				{
					Game::Scr_Error(Utils::String::VA("Entity: %i is not a player", entref.entnum));
					return;
				}
				
				if (Game::g_clients->ps.weaponstate == 0x1A)
					return Game::Scr_AddInt(true);
				else
					return Game::Scr_AddInt(false);
			}
		}, false);

		GSC::AddMethod("isUsingNightVision", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				entityNum = entref.entnum;
				Game::gentity_s* gentity = &Game::g_entities[entityNum];

				if (!gentity->client)
				{
					Game::Scr_Error(Utils::String::VA("Entity: %i is not a player", entref.entnum));
					return;
				}
			}

			if (ent->client->ps.weaponstate >= Game::WEAPON_NIGHTVISION_WEAR && ent->client->ps.weaponstate <= Game::WEAPON_NIGHTVISION_REMOVE)
				return Game::Scr_AddInt(true);
			else
				return Game::Scr_AddInt(false);
		}, false);

		GSC::AddMethod("isMantling", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				entityNum = entref.entnum;
				Game::gentity_s* gentity = &Game::g_entities[entityNum];

				if (!gentity->client)
				{
					Game::Scr_Error(Utils::String::VA("Entity: %i is not a player", entref.entnum));
					return;
				}
			}

			if (ent->client->ps.pm_flags & Game::PMF_MANTLE)
				return Game::Scr_AddInt(true);
			else
				return Game::Scr_AddInt(false);
		}, false);

		GSC::AddMethod("isJumping", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				entityNum = entref.entnum;
				Game::gentity_s* gentity = &Game::g_entities[entref.entnum];

				if (!gentity->client)
				{
					Game::Scr_Error(Utils::String::VA("Entity: %i is not a player", entref.entnum));
					return;
				}
				
				if (ent->client->ps.pm_flags & Game::PMF_JUMPING)
					return Game::Scr_AddInt(true);
				else
					return Game::Scr_AddInt(false);
			}
		}, false);

		GSC::AddMethod("isCrouching", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				entityNum = entref.entnum;
				Game::gentity_s* gentity = &Game::g_entities[entref.entnum];

				if (!gentity->client)
				{
					Game::Scr_Error(Utils::String::VA("Entity: %i is not a player", entref.entnum));
					return;
				}
				
				if (ent->client->ps.pm_flags & Game::PMF_DUCKED)
					return Game::Scr_AddInt(true);
				else
					return Game::Scr_AddInt(false);
			}
		}, false);

		GSC::AddMethod("isProning", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				entityNum = entref.entnum;
				Game::gentity_s* gentity = &Game::g_entities[entref.entnum];

				if (!gentity->client)
				{
					Game::Scr_Error(Utils::String::VA("Entity: %i is not a player", entref.entnum));
					return;
				}
				
				if (ent->client->ps.pm_flags & Game::PMF_PRONE)
					return Game::Scr_AddInt(true);
				else
					return Game::Scr_AddInt(false);
			}
		}, false);

		GSC::AddMethod("isTakingDamage", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				entityNum = entref.entnum;
				Game::gentity_s* gentity = &Game::g_entities[entref.entnum];

				if (!gentity->client)
				{
					Game::Scr_Error(Utils::String::VA("Entity: %i is not a player", entref.entnum));
					return;
				}
				
				if (ent->client->ps.pm_flags & Game::PMF_TOOK_DAMAGE)
					return Game::Scr_AddInt(true);
				else
					return Game::Scr_AddInt(false);
			}
		}, false);

		GSC::AddMethod("isOnLadder", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				entityNum = entref.entnum;
				Game::gentity_s* gentity = &Game::g_entities[entref.entnum];

				if (!gentity->client)
				{
					Game::Scr_Error(Utils::String::VA("Entity: %i is not a player", entref.entnum));
					return;
				}
				
				if (ent->client->ps.pm_flags & Game::PMF_LADDER)
					return Game::Scr_AddInt(true);
				else
					return Game::Scr_AddInt(false);
			}
		}, false);

		GSC::AddMethod("jumpedDownFromLadder", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				entityNum = entref.entnum;
				Game::gentity_s* gentity = &Game::g_entities[entref.entnum];

				if (!gentity->client)
				{
					Game::Scr_Error(Utils::String::VA("Entity: %i is not a player", entref.entnum));
					return;
				}
				
				if (ent->client->ps.pm_flags & Game::PMF_LADDER_DOWN)
					return Game::Scr_AddInt(true);
				else
					return Game::Scr_AddInt(false);
			}
		}, false);

		GSC::AddMethod("jumpButtonPressed", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				entityNum = entref.entnum;
				Game::gentity_s* gentity = &Game::g_entities[entref.entnum];

				if (!gentity->client)
				{
					Game::Scr_Error(Utils::String::VA("Entity: %i is not a player", entref.entnum));
					return;
				}
				
				if (ent->client->ps.pm_flags & Game::PMF_JUMPING)
					return Game::Scr_AddInt(true);
				else
					return Game::Scr_AddInt(false);
			}
		}, false);

		GSC::AddMethod("leaningAllowed", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
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

			#ifdef MODERN_OPS
				return Game::Scr_AddInt(false);
			#endif
			
			return Game::Scr_AddInt(true);
		}, false);

		GSC::AddMethod("isSwitchingWeapon", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				entityNum = entref.entnum;
				Game::gentity_s* gentity = &Game::g_entities[entityNum];

				if (!gentity->client)
				{
					Game::Scr_Error(Utils::String::VA("Entity: %i is not a player", entref.entnum));
					return;
				}
			}

			if (ent->client->ps.weaponstate >= Game::WEAPON_DROPPING && ent->client->ps.weaponstate <= Game::WEAPON_DROPPING_QUICK)
				return Game::Scr_AddInt(true);
			else
				return Game::Scr_AddInt(false);
		}, false);

		GSC::AddMethod("isRechamberingWeapon", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				entityNum = entref.entnum;
				Game::gentity_s* gentity = &Game::g_entities[entref.entnum];

				if (!gentity->client)
				{
					Game::Scr_Error(Utils::String::VA("Entity: %i is not a player", entref.entnum));
					return;
				}
				
				if (Game::g_clients->ps.weaponstate == 6)
					return Game::Scr_AddInt(true);
				else
					return Game::Scr_AddInt(false);
			}
		}, false);

		GSC::AddMethod("isHoldingOffhandWeapon", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				entityNum = entref.entnum;
				Game::gentity_s* gentity = &Game::g_entities[entref.entnum];

				if (!gentity->client)
				{
					Game::Scr_Error(Utils::String::VA("Entity: %i is not a player", entref.entnum));
					return;
				}
				
				if (Game::g_clients->ps.weaponstate >= 0xF && Game::g_clients->ps.weaponstate <= 0x14)
					return Game::Scr_AddInt(true);
				else
					return Game::Scr_AddInt(false);
			}
		}, false);

		GSC::AddMethod("isShooting", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				entityNum = entref.entnum;
				Game::gentity_s* gentity = &Game::g_entities[entref.entnum];

				if (!gentity->client)
				{
					Game::Scr_Error(Utils::String::VA("Entity: %i is not a player", entref.entnum));
					return;
				}
				
				if (Game::g_clients->ps.weaponstate == 5)
					return Game::Scr_AddInt(true);
				else
					return Game::Scr_AddInt(false);
			}
		}, false);

		GSC::AddMethod("isUsingDetonator", [](Game::scr_entref_t entref)
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				entityNum = entref.entnum;
				Game::gentity_s* gentity = &Game::g_entities[entref.entnum];

				if (!gentity->client)
				{
					Game::Scr_Error(Utils::String::VA("Entity: %i is not a player", entref.entnum));
					return;
				}
				
				if (Game::g_clients->ps.weaponstate == 0x15)
					return Game::Scr_AddInt(true);
				else
					return Game::Scr_AddInt(false);
			}
		}, false);

		GSC::AddMethod("getWeaponReloadTime", [](Game::scr_entref_t entref) // gsc: rate = level.player getWeaponReloadTime();
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
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

			const auto weapon = Game::bg_weaponDefs[Game::g_clients->ps.weapon];
			int iReloadTime = weapon->iReloadTime;
			if (iReloadTime == 0)
				return Game::Scr_AddInt(0);
			else
				iReloadTime = iReloadTime / 100;

			return Game::Scr_AddInt(iReloadTime);
		}, false);

		GSC::AddMethod("getWeaponReloadEmptyTime", [](Game::scr_entref_t entref) // gsc: rate = level.player getWeaponReloadEmptyTime();
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
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

			const auto weapon = Game::bg_weaponDefs[Game::g_clients->ps.weapon];
			int iReloadEmptyTime = weapon->iReloadEmptyTime;
			if (iReloadEmptyTime == 0)
				return Game::Scr_AddInt(0);
			else
				iReloadEmptyTime = iReloadEmptyTime / 100;

			return Game::Scr_AddInt(iReloadEmptyTime);
		}, false);

		GSC::AddMethod("getWeaponReloadAddTime", [](Game::scr_entref_t entref) // gsc: rate = level.player getWeaponReloadAddTime();
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
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

			const auto weapon = Game::bg_weaponDefs[Game::g_clients->ps.weapon];
			int iReloadAddTime = weapon->iReloadAddTime;
			if (iReloadAddTime == 0)
				return Game::Scr_AddInt(0);
			else
				iReloadAddTime = iReloadAddTime / 100;

			return Game::Scr_AddInt(iReloadAddTime);
		}, false);

		// Internal Methods. only called in animscripts\utility_code.gsc
		GSC::AddMethod("getLaserDuringNVGState", [](Game::scr_entref_t entref) // gsc: rate = level.player getLaserDuringNVGState();
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
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

				const auto weapon = Game::bg_weaponDefs[Game::g_clients->ps.weapon];
				int laserSightDuringNightvision = weapon->laserSightDuringNightvision;
				return Game::Scr_AddInt(laserSightDuringNightvision);
			}
		}, false);
		// Internal Methods. only called in animscripts\utility_code.gsc

		GSC::AddMethod("getWeaponDetonateAnimTime", [](Game::scr_entref_t entref) // gsc: rate = level.player getWeaponDetonateAnimTime();
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
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

			const auto weapon = Game::bg_weaponDefs[Game::g_clients->ps.weapon];
			int iDetonateTime = weapon->iDetonateTime;
			if (iDetonateTime == 0)
				return Game::Scr_AddInt(0);
			else
				iDetonateTime = iDetonateTime / 100;

			return Game::Scr_AddInt(iDetonateTime);
		}, false);

		GSC::AddMethod("getWeaponHoldFireAnimTime", [](Game::scr_entref_t entref) // gsc: rate = level.player getWeaponHoldFireAnimTime();
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
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

			const auto weapon = Game::bg_weaponDefs[Game::g_clients->ps.weapon];
			int iHoldFireTime = weapon->iHoldFireTime;
			if (iHoldFireTime == 0)
				return Game::Scr_AddInt(0);
			else
				iHoldFireTime = iHoldFireTime / 100;

			return Game::Scr_AddInt(iHoldFireTime);
		}, false);

		GSC::AddMethod("getWeaponRechamberAnimTime", [](Game::scr_entref_t entref) // gsc: rate = level.player getWeaponRechamberAnimTime();
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
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

			const auto weapon = Game::bg_weaponDefs[Game::g_clients->ps.weapon];
			int iRechamberTime = weapon->iRechamberTime;
			if (iRechamberTime == 0)
				return Game::Scr_AddInt(0);
			else
				iRechamberTime = iRechamberTime / 100;

			return Game::Scr_AddInt(iRechamberTime);
		}, false);

		GSC::AddMethod("getWeaponRechamberBoltAnimTime", [](Game::scr_entref_t entref) // gsc: rate = level.player getWeaponRechamberBoltAnimTime();
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
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

			const auto weapon = Game::bg_weaponDefs[Game::g_clients->ps.weapon];
			int iRechamberBoltTime = weapon->iRechamberBoltTime;
			if (iRechamberBoltTime == 0)
				return Game::Scr_AddInt(0);
			else
				iRechamberBoltTime = iRechamberBoltTime / 100;

			return Game::Scr_AddInt(iRechamberBoltTime);
		}, false);

		GSC::AddMethod("getWeaponDropAnimTime", [](Game::scr_entref_t entref) // gsc: rate = level.player getWeaponRechamberBoltAnimTime();
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
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

			const auto weapon = Game::bg_weaponDefs[Game::g_clients->ps.weapon];
			int iDropTime = weapon->iDropTime;
			if (iDropTime == 0)
				return Game::Scr_AddInt(0);
			else
				iDropTime = iDropTime / 100;

			return Game::Scr_AddInt(iDropTime);
		}, false);

		GSC::AddMethod("getWeaponQuickDropAnimTime", [](Game::scr_entref_t entref) // gsc: rate = level.player getWeaponRechamberBoltAnimTime();
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
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

			const auto weapon = Game::bg_weaponDefs[Game::g_clients->ps.weapon];
			int quickDropTime = weapon->quickDropTime;
			if (quickDropTime == 0)
				return Game::Scr_AddInt(0);
			else
				quickDropTime = quickDropTime / 100;

			return Game::Scr_AddInt(quickDropTime);
		}, false);

		GSC::AddMethod("isMeleeSlicing", [](Game::scr_entref_t entref) // gsc: rate = level.player isMeleeSlicing();
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
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

			int current_animation = Viewmodel::GetCurrentWeaponAnimIndex();
			if (current_animation == Game::WEAP_ANIM_MELEE)
				return Game::Scr_AddInt(1);

			return Game::Scr_AddInt(0);
		}, false);

		GSC::AddMethod("isMeleeStabbing", [](Game::scr_entref_t entref) // gsc: rate = level.player isMeleeStabbing();
		{
			const auto* ent = Game::GetPlayerEntity(entref);

			int entityNum = 0;
			// First - check that method tied to a player.
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

			int current_animation = Viewmodel::GetCurrentWeaponAnimIndex();
			if (current_animation == Game::WEAP_ANIM_MELEE_CHARGE)
				return Game::Scr_AddInt(1);

			return Game::Scr_AddInt(0);
		}, false);



		// Non player methods.

		// Doens't work correctly. Need to reverse the gentity structure.
		/*GSC::AddMethod("getAIDamage", [](Game::scr_entref_t entref)
		{
			//int numberref = entref.entnum;
			//Game::gentity_s* gentity = &Game::g_entities[numberref];
			//return Game::Scr_Error("current ai[%i] health: %i \n", numberref, gentity->health);
			#ifdef DEVELOPER_MODE
				Game::Scr_Error("This method is unfinished.)\n");
			#endif
			return Game::Scr_AddInt(0);
		}, false);*/

		GSC::AddMethod("freeEntitySentient", [](Game::scr_entref_t entref)
		{
			// Should be applied to an entity.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				Game::gentity_s* gentity = &Game::g_entities[entref.entnum];

				if (gentity->client)
				{
					Game::Scr_Error("Do not use freeEntitySentient on AI or player");
					return;
				}
			}

			// This function is currently empty.

			#ifdef DEVELOPER_MODE
				Game::Scr_Error("'freeEntitySentient' is unfinished and not working.\n");
			#endif

		}, false);

		GSC::AddMethod("makeEntitySentient", [](Game::scr_entref_t entref)
		{
			// Should be applied to an entity.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				Game::gentity_s* gentity = &Game::g_entities[entref.entnum];

				if (gentity->client)
				{
					Game::Scr_Error("Do not use makeEntitySentient on AI or player");
					return;
				}
			}

			// Check the amount of parameters for this function.
			// Only 2 is allowed.
			if (Game::Scr_GetNumParam() >= 3)
			{
				Game::Scr_Error("USAGE: makeEntitySentient <team> and <param2>\n");
				return;
			}

			// Next - get const string and then param2.
			Game::scr_string_t type = Game::Scr_GetConstString(0);
			if (type == NULL)
			{
				Game::Scr_Error("makeEntitySentient: Illegal parameter!\n");
				return;
			}

			// This function is currently empty.

			#ifdef DEVELOPER_MODE
				Game::Scr_Error("'makeEntitySentient' is unfinished and not working.\n");
			#endif

		}, false);

		GSC::AddMethod("setTurretModeChangeWait", [](Game::scr_entref_t entref)
		{
			// Should be applied to an entity.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				Game::gentity_s* gentity = &Game::g_entities[entref.entnum];

				if (gentity->client)
				{
					Game::Scr_Error("Do not use setTurretModeChangeWait on AI or player");
					return;
				}
			}

			// Check the amount of parameters for this function.
			// Only 1 is allowed.
			if (Game::Scr_GetNumParam() >= 2)
			{
				Game::Scr_Error("USAGE: setTurretModeChangeWait <bool>\n");
				return;
			}

			// This function is currently empty.

			#ifdef DEVELOPER_MODE
				Game::Scr_Error("'setTurretModeChangeWait' is unfinished and not working.\n");
			#endif

		}, false);

		GSC::AddMethod("makeTurretInoperable", [](Game::scr_entref_t entref)
		{
			// Should be applied to an entity.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				Game::gentity_s* gentity = &Game::g_entities[entref.entnum];

				if (gentity->client)
				{
					Game::Scr_Error("Do not use makeTurretInoperable on AI or player");
					return;
				}
			}

			// This function is currently empty.

			#ifdef DEVELOPER_MODE
				Game::Scr_Error("'makeTurretInoperable' is unfinished and not working.\n");
			#endif

		}, false);

		GSC::AddMethod("sentryPowerOn", [](Game::scr_entref_t entref)
		{
			// Should be applied to an entity.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				Game::gentity_s* gentity = &Game::g_entities[entref.entnum];

				if (gentity->client)
				{
					Game::Scr_Error("Do not use sentryPowerOn on AI or player");
					return;
				}
			}

			// This function is currently empty.

			#ifdef DEVELOPER_MODE
				Game::Scr_Error("'sentryPowerOn' is unfinished and not working.\n");
			#endif

		}, false);

		GSC::AddMethod("sentryPowerOff", [](Game::scr_entref_t entref)
		{
			// Should be applied to an entity.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				Game::gentity_s* gentity = &Game::g_entities[entref.entnum];

				if (gentity->client)
				{
					Game::Scr_Error("Do not use sentryPowerOff on AI or player");
					return;
				}
			}

			// This function is currently empty.

			#ifdef DEVELOPER_MODE
				Game::Scr_Error("'sentryPowerOff' is unfinished and not working.\n");
			#endif

		}, false);

		GSC::AddMethod("setSentryOwner", [](Game::scr_entref_t entref)
		{
			// Should be applied to an entity.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				Game::gentity_s* gentity = &Game::g_entities[entref.entnum];

				if (gentity->client)
				{
					Game::Scr_Error("Do not use setSentryOwner on AI or player");
					return;
				}
			}

			// Check the amount of parameters for this function.
			// Only 2 is allowed.
			if (Game::Scr_GetNumParam() >= 2)
			{
				Game::Scr_Error("USAGE: setSentryOwner <owner>\n");
				return;
			}

			// This function is currently empty.

			#ifdef DEVELOPER_MODE
				Game::Scr_Error("'setSentryOwner' is unfinished and not working.\n");
			#endif

		}, false);

		GSC::AddMethod("setTurretMinimapVisible", [](Game::scr_entref_t entref)
		{
			// Should be applied to an entity.
			if (entref.classnum)
			{
				Game::Scr_Error("Not an entity");
				return;
			}
			else
			{
				Game::gentity_s* gentity = &Game::g_entities[entref.entnum];

				if (gentity->client)
				{
					Game::Scr_Error("Do not use setTurretMinimapVisible on AI or player");
					return;
				}
			}

			// Check the amount of parameters for this function.
			// Only 1 is allowed.
			if (Game::Scr_GetNumParam() >= 2)
			{
				Game::Scr_Error("USAGE: setTurretMinimapVisible <bool>\n");
				return;
			}

			// This function is currently empty.

			#ifdef DEVELOPER_MODE
				Game::Scr_Error("'setTurretMinimapVisible' is unfinished and not working.\n");
			#endif

		}, false);
	}


	// GSC Functions.
	void GSC::AddFunctions()
	{
		GSC::AddFunction("executeCommand", []
		{
			const auto str = Game::Scr_GetString(0);

			if (str == nullptr)
			{
				Game::Scr_Error("^1Exec: Illegal parameter!\n");
				return;
			}

			Command::Execute(str);
		}, false);

		GSC::AddFunction("toUpper", []
		{
			const auto str = Game::Scr_GetString(0);

			if (str == nullptr)
			{
				Game::Scr_Error("^1Scr_GetString: Illegal parameter!\n");
				return;
			}

			return Game::Scr_AddString(Utils::String::ToUpper(str).c_str());
		}, false);

		GSC::AddFunction("getAILimit", []
		{
			const auto& ai_limit = Dvars::Functions::Dvar_FindVar("ai_count")->current.integer;
			return Game::Scr_AddInt(ai_limit);
		}, false);

		GSC::AddFunction("isMP", []
		{
			return Game::Scr_AddInt(0);
		}, false);

		GSC::AddFunction("isSP", []
		{
			return Game::Scr_AddInt(1);
		}, false);

		GSC::AddFunction("isCampaign", []
		{
			const auto specialops = Dvars::Functions::Dvar_FindVar("specialops");
			const auto so_survival = Dvars::Functions::Dvar_FindVar("so_survival");
			const auto zombiemode = Dvars::Functions::Dvar_FindVar("zombiemode");
			if (specialops->current.enabled)
			{
				return Game::Scr_AddInt(0);
			}
			else if (so_survival->current.enabled)
			{
				return Game::Scr_AddInt(0);
			}
			else if (zombiemode->current.enabled)
			{
				return Game::Scr_AddInt(0);
			}

			return Game::Scr_AddInt(1);
		}, false);

		GSC::AddFunction("isSpecialOps", []
		{
			const auto& specialops = Dvars::Functions::Dvar_FindVar("specialops")->current.integer;
			return Game::Scr_AddInt(specialops);
		}, false);

		GSC::AddFunction("isSurvivalMode", []
		{
			const auto& so_survival = Dvars::Functions::Dvar_FindVar("so_survival")->current.integer;
			return Game::Scr_AddInt(so_survival);
		}, false);

		GSC::AddFunction("isZombieMode", []
		{
			const auto& zombiemode = Dvars::Functions::Dvar_FindVar("zombiemode")->current.integer;
			return Game::Scr_AddInt(zombiemode);
		}, false);

		GSC::AddFunction("isEarlyDemoVersion", []
		{
			#ifdef EARLY_DEMO_VERSION
				return Game::Scr_AddInt(1);
			#endif

			return Game::Scr_AddInt(0);
		}, false);

		GSC::AddFunction("isDemoVersion", []
		{
			#ifdef DEMO_VERSION
				return Game::Scr_AddInt(1);
			#endif

			return Game::Scr_AddInt(0);
		}, false);

		GSC::AddFunction("isDevVersion", []
		{
			#ifdef DEVELOPER_MODE
				return Game::Scr_AddInt(1);
			#endif

			return Game::Scr_AddInt(0);
		}, false);

		GSC::AddFunction("isReleaseVersion", []
		{
			#ifdef DEMO_VERSION
				return Game::Scr_AddInt(0);
			#endif

			return Game::Scr_AddInt(1);
		}, false);

		GSC::AddFunction("setDevDvar", [] // gsc: setDevDvar(<dvar>, <value>);
		{
			// Check the amount of parameters for this function.
			// Only 2 are allowed.
			if (Game::Scr_GetNumParam() != 2)
			{
				Game::Scr_Error("GScr_SetDevDvar( dvarName, initialValue ) requires two parameters");
				return;
			}

			const auto dvar = Game::Scr_GetString(0);
			if (dvar == NULL)
			{
				Game::Scr_Error("GScr_SetDevDvar: Illegal parameter! First parameter must be a string.\n");
				return;
			}

			const auto value = Game::Scr_GetString(1);

			if (dvar)
			{
				const auto user_dvar = Dvars::Functions::Dvar_FindVar(dvar);

				if (!user_dvar)
				{
					Dvars::Register::Dvar_RegisterString(dvar, "External Dvar", "", Game::user_created);

					if (value)
					{
						Command::Execute(Utils::String::VA("%s %s", dvar, value, false));
					}
				}
				else
				{
					if (value)
					{
						Command::Execute(Utils::String::VA("%s %s", dvar, value, false));
					}
				}
			}
		}, false);

		GSC::AddFunction("setDvarIfUninitialized", [] // gsc: setDvarIfUninitialized(<dvar>, <value>);
		{
			// Check the amount of parameters for this function.
			// Only 2 are allowed.
			if (Game::Scr_GetNumParam() != 2)
			{
				Game::Scr_Error("GScr_SetDvarIfUninitialized( dvarName, initialValue ) requires two parameters");
				return;
			}

			const auto dvar = Game::Scr_GetString(0);
			if (dvar == NULL)
			{
				Game::Scr_Error("GScr_SetDvarIfUninitialized: Illegal parameter! First parameter must be a string.\n");
				return;
			}

			const auto value = Game::Scr_GetString(1);

			if (dvar)
			{
				const auto user_dvar = Dvars::Functions::Dvar_FindVar(dvar);

				if (!user_dvar)
				{
					Dvars::Register::Dvar_RegisterString(dvar, "External Dvar", "", Game::user_created);

					if (value)
					{
						Command::Execute(Utils::String::VA("%s %s", dvar, value, false));
					}
				}
			}
		}, false);

		GSC::AddFunction("setDevDvarIfUninitialized", [] // gsc: setDevDvarIfUninitialized(<dvar>, <value>);
		{
			// Check the amount of parameters for this function.
			// Only 2 are allowed.
			if (Game::Scr_GetNumParam() != 2)
			{
				Game::Scr_Error("GScr_SetDevDvarIfUninitialized( dvarName, initialValue ) requires two parameters");
				return;
			}

			const auto dvar = Game::Scr_GetString(0);
			if (dvar == NULL)
			{
				Game::Scr_Error("GScr_SetDevDvarIfUninitialized: Illegal parameter! First parameter must be a string.\n");
				return;
			}

			const auto value = Game::Scr_GetString(1);

			if (dvar)
			{
				const auto user_dvar = Dvars::Functions::Dvar_FindVar(dvar);

				if (!user_dvar)
				{
					Dvars::Register::Dvar_RegisterString(dvar, "External Dvar", "", Game::user_created);

					if (value)
					{
						Command::Execute(Utils::String::VA("%s %s", dvar, value, false));
					}
				}
			}
		}, false);

		// Not used.
		/*GSC::AddFunction("consolePrintLn", [] // gsc: consolePrintLn(<msg>);
		{
			// Check the amount of parameters for this function.
			// Only 1 is allowed.
			if (Game::Scr_GetNumParam() != 1)
			{
				Game::Scr_Error("GScr_ConsolePrintLn( msg ) requires one parameter");
				return;
			}

			const auto msg = Game::Scr_GetString(0);
			if (msg == NULL)
			{
				Game::Scr_Error("GScr_ConsolePrintLn: Illegal parameter! Parameter must be a string.\n");
				return;
			}

			Game::Com_Printf(0, "%s\n", msg);
		}, false);*/

		GSC::AddFunction("devConsolePrintLn", [] // gsc: devConsolePrintLn(<msg>);
		{
			// Check the amount of parameters for this function.
			// Only 1 is allowed.
			if (Game::Scr_GetNumParam() != 1)
			{
				Game::Scr_Error("GScr_ConsolePrintLn( msg ) requires one parameter");
				return;
			}

			const auto msg = Game::Scr_GetString(0);
			if (msg == NULL)
			{
				Game::Scr_Error("GScr_ConsolePrintLn: Illegal parameter! Parameter must be a string.\n");
				return;
			}

			const auto developer_enabled = Dvars::Functions::Dvar_FindVar("developer");
			if (developer_enabled->current.enabled)
			{
				Game::Com_Printf(0, "%s\n", msg);
			}
		}, false);

		GSC::AddFunction("debugConsolePrintLn", [] // gsc: debugConsolePrintLn(<msg>);
		{
			// Check the amount of parameters for this function.
			// Only 1 is allowed.
			if (Game::Scr_GetNumParam() != 1)
			{
				Game::Scr_Error("GScr_ConsolePrintLn( msg ) requires one parameter");
				return;
			}

			const auto msg = Game::Scr_GetString(0);
			if (msg == NULL)
			{
				Game::Scr_Error("GScr_ConsolePrintLn: Illegal parameter! Parameter must be a string.\n");
				return;
			}

			#ifdef DEVELOPER_MODE
				Game::Com_Printf(0, "%s\n", msg);
			#endif
		}, false);

		GSC::AddFunction("playPreviewScene", []
		{
			const auto preview_enabled = Dvars::Functions::Dvar_FindVar("cg_previewMode");
			if (preview_enabled->current.enabled)
			{
				return Game::Scr_AddInt(1);
			}

			return Game::Scr_AddInt(0);
		}, false);

		// Internal Functions. only called in animscripts\utility_code.gsc
		GSC::AddFunction("ai_LaserForceOn", []
		{
			Utils::Hook::Set<BYTE>(0x4087A8, 0x75);
			//Utils::Hook::Set<BYTE>(0x4087B4, 0x74);// turn on laser for all guns

			#ifdef DEVELOPER_MODE
				Game::Scr_Error(Utils::String::VA("AI Laser Force ^2ON"));
			#endif
		}, false);

		GSC::AddFunction("ai_LaserForceOff", []
		{
			Utils::Hook::Set<BYTE>(0x4087A8, 0x74);
			//Utils::Hook::Set<BYTE>(0x4087B4, 0x75);// turn off laser for all guns

			#ifdef DEVELOPER_MODE
				Game::Scr_Error(Utils::String::VA("AI Laser Force ^1OFF"));
			#endif
		}, false);
		// Internal Functions. only called in animscripts\utility_code.gsc

		// Not used anywhere.
		/*GSC::AddFunction("assetExists", []
		{
			const auto asset_type = Game::Scr_GetInt(0);
			const auto asset_name = Game::Scr_GetString(1);

			if (asset_type < 0 || asset_type >= Game::XAssetType::ASSET_TYPE_COUNT)
			{
				Game::Scr_Error(Utils::String::VA("Invalid asset pool passed must be between [0, %d]\n", Game::XAssetType::ASSET_TYPE_COUNT - 1));
				return;
			}

			Game::XAssetType xasset_type = static_cast<Game::XAssetType>(asset_type);

			const auto asset = Game::DB_FindXAssetHeader(xasset_type, asset_name).data;
			return Game::Scr_AddInt(asset != nullptr ? 1 : 0);
		}, false);*/

		// Added 25.10.2024 (13:21) by NAKSHATRA_12
		GSC::AddFunction("weaponBurstCount", [] 
		{                                    
			int weaponIndex = Game::G_GetWeaponIndexForName(Game::Scr_GetString(0));
			const auto weapon = Game::bg_weaponDefs[weaponIndex];
			Game::weapFireType_t weaponFireType = weapon->fireType;
			if ( ( weaponFireType < 2 ) || ( weaponFireType > 4 ) )                
				return Game::Scr_AddInt( 0 );
			else
				return Game::Scr_AddInt( weaponFireType );
		}, false);
	}

	Game::xmethod_t GSC::Player_GetMethod_Stub(const char** name)
	{
		auto got = GSC::CustomScrMethods.find(*name);

		if (got == GSC::CustomScrMethods.end())
			return Game::Player_GetMethod(name);

		return got->second.function;
	}

	Game::xfunction_t GSC::Scr_GetFunction_Stub(const char** name, int* isDev)
	{
		auto got = GSC::CustomScrFunctions.find(*name);

		if (got == GSC::CustomScrFunctions.end())
			return Game::Scr_GetFunction(name, isDev);

		*isDev = got->second.developer;
		return got->second.function;
	}

	void GSC::LoadScript(const std::string& path)
	{
		if (!Game::Scr_LoadScript(path.data()))
		{
			Game::Com_Printf(0, Utils::String::VA("The script %s encountered an error during the loading process.\n", path.data()));
			return;
		}

		Game::Com_Printf(0, Utils::String::VA("The script %s has been loaded successfully.\n", path.data()));

		const auto initHandle = Game::Scr_GetFunctionHandle(path.data(), "init");
		if (initHandle != 0)
		{
			ScriptInitHandles.insert_or_assign(path, initHandle);
		}

		const auto mainHandle = Game::Scr_GetFunctionHandle(path.data(), "main");
		if (mainHandle != 0)
		{
			ScriptMainHandles.insert_or_assign(path, mainHandle);
		}
	}

	void GSC::LoadScriptsFromFastFiles()
	{
		// Clear handles (from previous GSC loading session)
		ScriptMainHandles.clear();
		ScriptInitHandles.clear();

		//Support exclusively loading scripts from fast files.
		FastFiles::EnumAssets(Game::ASSET_TYPE_RAWFILE, [](Game::XAssetHeader header)
		{
			std::string name = header.rawfile->name;

			if (name.ends_with(".gsc") && name.starts_with("scripts/"))
			{
				// Scr_LoadScriptInternal will add the '.gsc' suffix so we remove it
				const std::string path = name.substr(0, name.size() - 4);
				std::string middlePath = path.substr(path.rfind('/', path.find_last_of('/') - 1) + 1, path.find_last_of('/') - path.rfind('/', path.find_last_of('/') - 1) - 1);

				// Scripts folder:
				// 'scripts/[name_script].gsc' - global scripts
				// 'scripts/[level_name]/[name_script].gsc' - scripts for the specific level.
				if (name.contains("scripts/"s + middlePath + "/"))
				{
					if (middlePath == Dvars::Functions::Dvar_FindVar("mapname")->current.string)
					{
						LoadScript(path);
						//Game::Com_Printf(0, "^2The script %s is intended for the current map.\n", path.data());
					}
					else
					{
						//Game::Com_Printf(0, "^3The script %s has been loaded in the ff file, but it is not intended for the current level.\n", path.data());
					}
				}
				else
				{
					//Game::Com_Printf(0, "^1The script %s is meant for global use.\n", name.data());
					LoadScript(path);
				}
			}

		}, false);
	}

	void __declspec(naked) GSC::GScr_LoadScripts_Stub()
	{
		const static uint32_t GScr_LoadScriptsForEntities_func = 0x4C6680;
		const static uint32_t retn_addr = 0x4C6AF8;
		__asm
		{
			pushad;
			call	GSC::LoadScriptsFromFastFiles;
			popad;

			call	GScr_LoadScriptsForEntities_func;
			jmp		retn_addr;
		}
	}

	void GSC::G_RunFrame_Stub(int extent, int timeCap)
	{
		for (const auto& handle : ScriptInitHandles)
		{
			const auto thread = Game::Scr_ExecThread(handle.second);
			Game::Scr_FreeThread(static_cast<std::uint16_t>(thread));
		}

		Utils::Hook::Call<int(int, int)>(0x4BAB40)(extent, timeCap); //G_RunFrame
	}

	void GSC::Scr_LoadLevel_Stub()
	{
		for (const auto& handle : ScriptMainHandles)
		{
			const auto thread = Game::Scr_ExecThread(handle.second);
			Game::Scr_FreeThread(static_cast<std::uint16_t>(thread));
		}

		Utils::Hook::Call<void()>(0x4F2F50)(); //Path_AutoDisconnectPaths
	}

	GSC::GSC()
	{
		Utils::Hook(0x4DB7D7, Player_GetMethod_Stub, HOOK_CALL).install()->quick(); // Player_GetMethod
		Utils::Hook(0x5435F1, Scr_GetFunction_Stub, HOOK_CALL).install()->quick();  // Scr_GetFunction

		// Additional the folder 'scripts' for .GSC
		// Load handles
		Utils::Hook(0x4C6AF3, GScr_LoadScripts_Stub, HOOK_JUMP).install()->quick();
		// Exec handles
		Utils::Hook(0x4BA564, Scr_LoadLevel_Stub, HOOK_CALL).install()->quick();
		Utils::Hook(0x4BA58A, G_RunFrame_Stub, HOOK_CALL).install()->quick();

		GSC::AddFunctions();
		GSC::AddMethods();
	}


	GSC::~GSC()
	{
		CustomScrMethods.clear();
		CustomScrFunctions.clear();
	}
}