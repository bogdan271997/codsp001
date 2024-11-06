#include "STDInc.hpp"

namespace Components
{
	void Weapons::BG_WeaponFixBurstMode(Game::playerState_s* ps)
	{
		Game::WeaponDef_s* weapon = Game::bg_weaponDefs[ps->weapon];

		if (weapon->fireType >= Game::WEAPON_FIRETYPE_BURSTFIRE2 && weapon->fireType <= Game::WEAPON_FIRETYPE_BURSTFIRE4 && !Game::ShotLimitReached(weapon, ps))
		{
			if (ps->ammo[weapon->iAmmoIndex] <= 0)
			{
				if (!ps->ammoclip[weapon->iClipIndex])
				{
					ps->weaponShotCount = 0;
				}
			}
		}
	}

	void Weapons::BG_FireWeaponStub(Game::playerState_s* playerState, int delayedAction)
	{
		Game::PM_Weapon_FireWeapon(playerState, delayedAction);
		Weapons::BG_WeaponFixBurstMode(playerState);
	}

	void __declspec(naked) Weapons::FireWeaponStub()
	{
		const static uint32_t offset_return = 0x5C2C96;

		__asm
		{
			push	eax;
			mov		eax, Dvars::player_allowFire;
			cmp		byte ptr[eax + 12], 1;
			pop		eax;

			// disableFire is false
			jne		DISABLE;

			// disableFire is true
			jmp		STOCK;
		DISABLE:
			jmp		offset_return;
		STOCK:
			push	eax;
			call	Weapons::BG_FireWeaponStub;
			add		esp, 4;
			jmp		offset_return;
		}
	}

	Weapons::Weapons()
	{
		Events::OnDvarInit([]
		{
			Dvars::player_allowFire = Dvars::Register::Dvar_RegisterBool("player_allowFire", "Allow the player to fire weapons.", true, Game::saved_flag);
		});



		// Level Functions.
		
		// Not used anywhere.
		/*
		GSC::AddFunction("getLevelWeaponDamage", []
		{
			if (Game::Scr_GetNumParam() != 1)
			{
				Game::Scr_Error("getWeaponDamage( <string> weaponName ) requires one parameter");
				return;
			}

			int weaponIndex = Game::G_GetWeaponIndexForName(Game::Scr_GetString(0));
			const auto weapon = Game::bg_weaponDefs[weaponIndex];
			return Game::Scr_AddInt(weapon->damage);
		}, false);

		GSC::AddFunction("setLevelWeaponDamage", []
		{
			if (Game::Scr_GetNumParam() != 2)
			{
				Game::Scr_Error("setWeaponDamage( <string> weaponName, <int> value ) requires two parameters");
				return;
			}

			int weaponIndex = Game::G_GetWeaponIndexForName(Game::Scr_GetString(0));
			const auto damage_value = Game::Scr_GetInt(1);

			const auto weapon = Game::bg_weaponDefs[weaponIndex];
			weapon->damage = damage_value;
			Game::Scr_AddInt(true);
		}, false);
		*/



		// Player Methods.
		
		GSC::AddMethod("playViewmodelFX", [](Game::scr_entref_t entref)
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

			if (Game::Scr_GetNumParam() != 2)
			{
				Game::Scr_Error("PlayViewmodelFX() called with wrong params.\n");
				return;
			}

			int fxId = Game::Scr_GetInt(0);
			int tagName = Game::Scr_GetConstString(1);

			unsigned int weaponIndex = Game::ps->weapon;
			const auto viewmodelInfo = &Game::cg_weaponsArray[weaponIndex];

			std::uint8_t bone = 254;
			if (!Game::DObjGetBoneIndex(viewmodelInfo->viewModelDObj, tagName, &bone))
			{
				const char* error_msg = Utils::String::VA("playViewmodelFX(): viewmodel weapon '%s', does not have bone '%s'", viewmodelInfo->translatedDisplayName, Game::SL_ConvertToString(tagName));
				Game::Scr_Error(error_msg);
				return;
			}

			Game::CG_PlayBoltedEffect(Game::cgs->localClientNum,
				tagName,
				weaponIndex + 2176, // <-- CG_WeaponDObjHandle
				Game::cgsArray->fxs[fxId]);
			Game::Scr_AddInt(true);

		}, false);

		GSC::AddMethod("viewmodelHideTag", [](Game::scr_entref_t entref)
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

			if (Game::Scr_GetNumParam() != 1)
			{
				Game::Scr_Error("viewmodelHideTag() called with wrong params.\n");
				return;
			}

			int tagName = Game::Scr_GetConstString(0);

			unsigned int weaponIndex = Game::ps->weapon;
			const auto viewmodelInfo = &Game::cg_weaponsArray[weaponIndex];

			std::uint8_t bone = 254;
			if (!Game::DObjGetBoneIndex(viewmodelInfo->viewModelDObj, tagName, &bone))
			{
				const char* error_msg = Utils::String::VA("viewmodelHideTag: No such bone tag (%s) for weapon (%s)", viewmodelInfo->translatedDisplayName, Game::SL_ConvertToString(tagName));
				Game::Scr_Error(error_msg);
				return;
			}

			viewmodelInfo->partBits[bone >> 5] |= 0x80000000 >> (bone & 0x1F);
			Game::ChangeViewmodelDobj(Game::g_clients->ps.weapon, Game::g_clients->ps.weaponmodels[Game::g_clients->ps.weapon], viewmodelInfo->handModel, viewmodelInfo->gogglesModel, viewmodelInfo->rocketModel, viewmodelInfo->knifeModel, 1);
			Game::Scr_AddInt(true);
		}, false);

		GSC::AddMethod("viewmodelShowTag", [](Game::scr_entref_t entref)
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

			if (Game::Scr_GetNumParam() != 1)
			{
				Game::Scr_Error("viewmodelShowTag() called with wrong params.\n");
				return;
			}

			int tagName = Game::Scr_GetConstString(0);

			unsigned int weaponIndex = Game::ps->weapon;
			const auto viewmodelInfo = &Game::cg_weaponsArray[weaponIndex];

			std::uint8_t bone = 254;
			if (!Game::DObjGetBoneIndex(viewmodelInfo->viewModelDObj, tagName, &bone))
			{
				const char* error_msg = Utils::String::VA("viewmodelShowTag: No such bone tag (%s) for weapon (%s)", viewmodelInfo->translatedDisplayName, Game::SL_ConvertToString(tagName));
				Game::Scr_Error(error_msg);
				return;
			}

			viewmodelInfo->partBits[bone >> 5] &= ~(0x80000000 >> (bone & 0x1F));
			Game::ChangeViewmodelDobj(Game::g_clients->ps.weapon, Game::g_clients->ps.weaponmodels[Game::g_clients->ps.weapon], viewmodelInfo->handModel, viewmodelInfo->gogglesModel, viewmodelInfo->rocketModel, viewmodelInfo->knifeModel, 1);
			Game::Scr_AddInt(true);
		}, false);
		
		Utils::Hook(0x5C2C91, Weapons::FireWeaponStub, HOOK_JUMP).install()->quick();
	}

	Weapons::~Weapons()
	{
	}
}