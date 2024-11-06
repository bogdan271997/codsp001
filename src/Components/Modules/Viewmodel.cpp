#include "STDInc.hpp"

namespace Components
{
	// PM_ResetWeaponState( playerState_s )
	void PM_ResetWeaponState(Game::playerState_s *ps)
	{
		ps->weapFlags &= ~2u;
		ps->pm_flags &= ~0x200u;

		// Reset weapon state variables
		ps->weaponTime = 0;
		ps->weaponDelay = 0;
		ps->weaponstate = Game::WEAPON_READY;

		if (ps->pm_type < 4)
			ps->weapAnim = ~ps->weapAnim & 0x200;
	}

	// PM_StartWeaponAnim( playerState_s, weapAnim )
	void PM_StartWeaponAnim(Game::playerState_s *ps, int weapAnim)
	{
		if ( ps->pm_type < 4 )
			ps->weapAnim = weapAnim | ~ps->weapAnim & 0x200;
	}

	// ViewModelOverride( playerState_s, pml ) Overrides ForceViewmodelAnim
	bool ViewModelOverride(Game::playerState_s *ps, Game::pml_t* pml)
	{
		int weapFlags = ps->weapFlags;
		
		if ((weapFlags & 0x400) == 0)
			return false;
		
		const auto weapon = Game::bg_weaponDefs[Game::g_clients->ps.forcedViewAnimWeaponIdx];
		int forcedViewAnimWeaponState = ps->forcedViewAnimWeaponState;

		if (ps->weaponstate == forcedViewAnimWeaponState && ps->weapon == ps->forcedViewAnimWeaponIdx)
		{
			ps->weaponTime -= pml->msec;
			
			if (ps->weaponTime <= 0)
			{
				unsigned int forcedViewAnimOriginalWeaponIdx = ps->forcedViewAnimOriginalWeaponIdx;
				ps->weapFlags = weapFlags & 0xFFFFFBFF;
				ps->weapon = forcedViewAnimOriginalWeaponIdx;
				ps->weaponTime = 0;
				PM_StartWeaponAnim(ps, 0);
			}
			return true;
		}
		else
		{
			switch (forcedViewAnimWeaponState)
			{
			case 1:// New case. Added 10.09.2024.
				ps->forcedViewAnimWeaponState = 0;
				PM_StartWeaponAnim(ps, 20);
				ps->weaponTime = weapon->quickRaiseTime;
				ps->events[ps->eventSequence & 3] = 0;
				break;
			case 3:// New case. Added 10.09.2024.
				ps->forcedViewAnimWeaponState = 0;
				PM_StartWeaponAnim(ps, 10);
				ps->weaponTime = weapon->iDropTime;
				ps->events[ps->eventSequence & 3] = 0;
				break;
			case 5:
				PM_StartWeaponAnim(ps, 2);
				ps->weaponTime = weapon->iFireTime;
				ps->events[ps->eventSequence & 3] = 26;
				break;
			case 6:// New case. Added 10.09.2024. For secondary weapon inspection.
				ps->forcedViewAnimWeaponState = 0;// hack for secondary weapon inspection
				PM_StartWeaponAnim(ps, 7);
				ps->weaponTime = weapon->iReloadEmptyTime;
				ps->events[ps->eventSequence & 3] = 0;
				break;
			case 7:
				PM_StartWeaponAnim(ps, 13);
				ps->weaponTime = weapon->iReloadTime;
				ps->events[ps->eventSequence & 3] = 15;
				break;
			case 17:// New case. Added 18.06.2024. For breachRaiseAnim.
				ps->forcedViewAnimWeaponState = 0;// hack for breachRaise
				PM_StartWeaponAnim(ps, 26);
				ps->weaponTime = weapon->iHoldFireTime;
				ps->events[ps->eventSequence & 3] = 0;
				break;
			case 21:// New case. Added 10.09.2024. For primary weapon inspection.
				ps->forcedViewAnimWeaponState = 0;// hack for primary weapon inspection
				PM_StartWeaponAnim(ps, 27);
				ps->weaponTime = weapon->iDetonateTime;
				ps->events[ps->eventSequence & 3] = 0;
				break;
			case 22:// New case. Added 18.06.2024. For sprintIn.
				ps->forcedViewAnimWeaponState = 0;
				PM_StartWeaponAnim(ps, 23);
				ps->weaponTime = weapon->sprintInTime;
				ps->events[ps->eventSequence & 3] = 0;
				break;
			case 23:// New case. Added 18.06.2024. For sprintLoop.
				ps->forcedViewAnimWeaponState = 0;
				PM_StartWeaponAnim(ps, 24);
				ps->weaponTime = weapon->sprintLoopTime;
				ps->events[ps->eventSequence & 3] = 0;
				break;
			case 24:// New case. Added 18.06.2024. For sprintOut.
				ps->forcedViewAnimWeaponState = 0;
				PM_StartWeaponAnim(ps, 25);
				ps->weaponTime = weapon->sprintOutTime;
				ps->events[ps->eventSequence & 3] = 0;
				break;
			case 25:
				PM_StartWeaponAnim(ps, 28);
				ps->weaponTime = weapon->nightVisionWearTime;
				ps->events[ps->eventSequence & 3] = 68;
				break;
			case 26:
				PM_StartWeaponAnim(ps, 29);
				ps->weaponTime = weapon->nightVisionRemoveTime;
				ps->events[ps->eventSequence & 3] = 69;
				break;
			default:
				Game::Com_Printf(0, "^3Trying to force viewmodel to play an animation not supported by code: %u.\n", ps->forcedViewAnimWeaponState);
				ps->weapFlags &= ~0x400u;
				return false;
			}
			
			ps->eventParms[ps->eventSequence & 3] = 0;
			++ps->eventSequence;
			ps->forcedViewAnimOriginalWeaponIdx = ps->weapon;
			ps->weapon = ps->forcedViewAnimWeaponIdx;
			ps->weaponstate = ps->forcedViewAnimWeaponState;
		}
		return true;
	}

	void __declspec(naked) ViewModelOverrideStub()
	{
		const static uint32_t retn_addr = 0x5C29F8;
		__asm
		{
			push     esi;
			call    ViewModelOverride;
			add     esp, 4;
			jmp     retn_addr;
		}
	}


	bool inspecting = false;
	bool weaponLower = false;

	float weaponInspectionTime;

	int inspectAnimIndex;

	const int secondaryWeaponInspectAnimIndex = Game::WEAP_ANIM_ADS_RECHAMBER;// adsRechamberAnim
	const int primaryWeaponInspectAnimIndex = Game::WEAP_ANIM_DETONATE;// detonateAnim


	// get weapon inspection animIndex
	int getWeaponInspectionAnimation()
	{
		const auto weapon = Game::bg_weaponDefs[Game::g_clients->ps.weapon];
		if ( weapon->weapType == 1 )
			inspectAnimIndex = secondaryWeaponInspectAnimIndex;
		else
			inspectAnimIndex = primaryWeaponInspectAnimIndex;
		
		return inspectAnimIndex;
	}

	// get weapon inspection time
	float getWeaponInspectionTime()
	{
		const auto weapon = Game::bg_weaponDefs[Game::g_clients->ps.weapon];
		if ( weapon->weapType == 1 )
			weaponInspectionTime = weapon->iRechamberBoltTime;
		else
			weaponInspectionTime = weapon->iDetonateTime;
		
		return weaponInspectionTime;
	}


	// GetCurrentWeaponAnimIndex() returns the current player animIndex.
	int Viewmodel::GetCurrentWeaponAnimIndex()
	{
		int animIndex;
		int weapAnim = Game::ps->weapAnim;
		switch ( weapAnim & 0xFFFFFDFF )
		{
		case 0u:
		case 1u:
			if ( inspecting && !weaponLower )
			{
				animIndex = getWeaponInspectionAnimation();
			}
			else if ( !inspecting && weaponLower )
			{
				const auto weapon = Game::bg_weaponDefs[Game::g_clients->ps.weapon];
				if ( weapon->weapType == 1 )
					animIndex = Game::WEAP_ANIM_DROP;
				else
					animIndex = Game::WEAP_ANIM_SPRINT_IN;
			}
			else
			{
				animIndex = 1;
			}
			break;
		case 2u:
			animIndex = Game::WEAP_ANIM_FIRE;
			break;
		case 3u:
			animIndex = Game::WEAP_ANIM_LASTSHOT;
			break;
		case 4u:
			animIndex = Game::WEAP_ANIM_RECHAMBER;
			break;
		case 5u:
			animIndex = Game::WEAP_ANIM_ADS_FIRE;
			break;
		case 6u:
			animIndex = Game::WEAP_ANIM_ADS_LASTSHOT;
			break;
		case 7u:
			animIndex = Game::WEAP_ANIM_ADS_RECHAMBER;
			break;
		case 8u:
			animIndex = Game::WEAP_ANIM_MELEE;
			break;
		case 9u:
			animIndex = Game::WEAP_ANIM_MELEE_CHARGE;
			break;
		case 10u:
			animIndex = Game::WEAP_ANIM_DROP;
			break;
		case 11u:
			animIndex = Game::WEAP_ANIM_RAISE;
			break;
		case 12u:
			animIndex = Game::WEAP_ANIM_FIRST_RAISE;
			break;
		case 13u:
			animIndex = Game::WEAP_ANIM_RELOAD;
			break;
		case 14u:
			animIndex = Game::WEAP_ANIM_RELOAD_EMPTY;
			break;
		case 15u:
			animIndex = Game::WEAP_ANIM_RELOAD_START;
			break;
		case 16u:
			animIndex = Game::WEAP_ANIM_RELOAD_END;
			break;
		case 17u:
			animIndex = Game::WEAP_ANIM_ALT_DROP;
			break;
		case 18u:
			animIndex = Game::WEAP_ANIM_ALT_RAISE;
			break;
		case 19u:
			animIndex = Game::WEAP_ANIM_QUICK_DROP;
			break;
		case 20u:
			animIndex = Game::WEAP_ANIM_QUICK_RAISE;
			break;
		case 21u:
			animIndex = Game::WEAP_ANIM_EMPTY_DROP;
			break;
		case 22u:
			animIndex = Game::WEAP_ANIM_EMPTY_RAISE;
			break;
		case 23u:
			animIndex = Game::WEAP_ANIM_SPRINT_IN;
			break;
		case 24u:
			animIndex = Game::WEAP_ANIM_SPRINT_LOOP;
			break;
		case 25u:
			animIndex = Game::WEAP_ANIM_SPRINT_OUT;
			break;
		case 26u:
			animIndex = Game::WEAP_ANIM_HOLD_FIRE;
			break;
		case 27u:
			animIndex = Game::WEAP_ANIM_DETONATE;
			break;
		case 28u:
			animIndex = Game::WEAP_ANIM_NIGHTVISION_WEAR;
			break;
		case 29u:
			animIndex = Game::WEAP_ANIM_NIGHTVISION_REMOVE;
			break;
		default:
			animIndex = Game::WEAP_ANIM_IDLE;// Force default animation.
			Game::Com_Printf(19, "GetCurrentWeaponAnimIndex: Unknown weapon animation %i\n", Game::ps->weapAnim & 0xFFFFFDFF);
		}
		
		return animIndex;
	}

	// GetCurrentRealWeaponAnimIndex() returns the current real player animIndex.
	int Viewmodel::GetCurrentRealWeaponAnimIndex()
	{
		int animIndex;
		int weapAnim = Game::ps->weapAnim;
		switch ( weapAnim & 0xFFFFFDFF )
		{
		case 0u:
		case 1u:
			animIndex = 1;
			break;
		case 2u:
			animIndex = Game::WEAP_ANIM_FIRE;
			break;
		case 3u:
			animIndex = Game::WEAP_ANIM_LASTSHOT;
			break;
		case 4u:
			animIndex = Game::WEAP_ANIM_RECHAMBER;
			break;
		case 5u:
			animIndex = Game::WEAP_ANIM_ADS_FIRE;
			break;
		case 6u:
			animIndex = Game::WEAP_ANIM_ADS_LASTSHOT;
			break;
		case 7u:
			animIndex = Game::WEAP_ANIM_ADS_RECHAMBER;
			break;
		case 8u:
			animIndex = Game::WEAP_ANIM_MELEE;
			break;
		case 9u:
			animIndex = Game::WEAP_ANIM_MELEE_CHARGE;
			break;
		case 10u:
			animIndex = Game::WEAP_ANIM_DROP;
			break;
		case 11u:
			animIndex = Game::WEAP_ANIM_RAISE;
			break;
		case 12u:
			animIndex = Game::WEAP_ANIM_FIRST_RAISE;
			break;
		case 13u:
			animIndex = Game::WEAP_ANIM_RELOAD;
			break;
		case 14u:
			animIndex = Game::WEAP_ANIM_RELOAD_EMPTY;
			break;
		case 15u:
			animIndex = Game::WEAP_ANIM_RELOAD_START;
			break;
		case 16u:
			animIndex = Game::WEAP_ANIM_RELOAD_END;
			break;
		case 17u:
			animIndex = Game::WEAP_ANIM_ALT_DROP;
			break;
		case 18u:
			animIndex = Game::WEAP_ANIM_ALT_RAISE;
			break;
		case 19u:
			animIndex = Game::WEAP_ANIM_QUICK_DROP;
			break;
		case 20u:
			animIndex = Game::WEAP_ANIM_QUICK_RAISE;
			break;
		case 21u:
			animIndex = Game::WEAP_ANIM_EMPTY_DROP;
			break;
		case 22u:
			animIndex = Game::WEAP_ANIM_EMPTY_RAISE;
			break;
		case 23u:
			animIndex = Game::WEAP_ANIM_SPRINT_IN;
			break;
		case 24u:
			animIndex = Game::WEAP_ANIM_SPRINT_LOOP;
			break;
		case 25u:
			animIndex = Game::WEAP_ANIM_SPRINT_OUT;
			break;
		case 26u:
			animIndex = Game::WEAP_ANIM_HOLD_FIRE;
			break;
		case 27u:
			animIndex = Game::WEAP_ANIM_DETONATE;
			break;
		case 28u:
			animIndex = Game::WEAP_ANIM_NIGHTVISION_WEAR;
			break;
		case 29u:
			animIndex = Game::WEAP_ANIM_NIGHTVISION_REMOVE;
			break;
		default:
			animIndex = Game::WEAP_ANIM_IDLE;// Force default animation.
			Game::Com_Printf(19, "GetCurrentRealWeaponAnimIndex: Unknown weapon animation %i\n", Game::ps->weapAnim & 0xFFFFFDFF);
		}
		
		return animIndex;
	}


	// Returns the current Ads float value.
	float Viewmodel::GetCurrentWeaponAdsProgress()
	{
		return *reinterpret_cast<float*>(0x714BA8 + 208);
	}


	// weapInspectAnim is not allowed if one of the following conditions is true
	bool weapInspectionIsNotAllowed()
	{
		if (/* Game::g_clients->ps.pm_flags & Game::PMF_FROZEN causes crashes during restart
		|| Game::g_clients->ps.pm_flags & Game::PMF_PRONE returns true for both prone and jump(bug)
		||*/ Game::g_clients->ps.pm_flags & Game::PMF_SHELLSHOCKED 
		|| Game::g_clients->ps.pm_flags & Game::PMF_TOOK_DAMAGE )
			return true;
		
		return false;
	}

	// check if not playing any other animation except weapInspectAnim on a secondary weapon
	bool resetWeaponInspectionAnimation()
	{
		if (Game::g_clients->ps.weaponstate == Game::WEAPON_RAISING 
		|| Game::g_clients->ps.weaponstate == Game::WEAPON_RAISING_ALTSWITCH 
		|| Game::g_clients->ps.weaponstate == Game::WEAPON_DROPPING 
		|| Game::g_clients->ps.weaponstate == Game::WEAPON_DROPPING_QUICK 
		|| Game::g_clients->ps.weaponstate == Game::WEAPON_FIRING 
		|| Game::g_clients->ps.weaponstate == Game::WEAPON_RECHAMBERING
		|| Game::g_clients->ps.weaponstate == Game::WEAPON_RELOAD_START 
		|| Game::g_clients->ps.weaponstate == Game::WEAPON_RELOADING 
		|| Game::g_clients->ps.weaponstate == Game::WEAPON_MELEE_INIT 
		|| Game::g_clients->ps.weaponstate == Game::WEAPON_MELEE_FIRE 
		|| Game::g_clients->ps.weaponstate == Game::WEAPON_OFFHAND_INIT
		|| Game::g_clients->ps.weaponstate == Game::WEAPON_OFFHAND_PREPARE 
		|| Game::g_clients->ps.weaponstate == Game::WEAPON_OFFHAND_HOLD 
		|| Game::g_clients->ps.weaponstate == Game::WEAPON_OFFHAND_START
		|| Game::g_clients->ps.weaponstate == Game::WEAPON_DETONATING
		|| Game::g_clients->ps.weaponstate == Game::WEAPON_SPRINT_RAISE 
		|| Game::g_clients->ps.weaponstate == Game::WEAPON_SPRINT_LOOP 
		|| Game::g_clients->ps.weaponstate == Game::WEAPON_SPRINT_DROP 
		|| Game::g_clients->ps.weaponstate == Game::WEAPON_NIGHTVISION_WEAR 
		|| Game::g_clients->ps.weaponstate == Game::WEAPON_NIGHTVISION_REMOVE)
			return true;
		
		return false;
	}

	void DisableInspectIfNotAllowed()
	{
		if (!Game::CL_IsCgameInitialized())
			return;
		
		if (inspecting && resetWeaponInspectionAnimation())// Disable if playing another animation.
			inspecting = false;
		
		float weaponPosFrac = Viewmodel::GetCurrentWeaponAdsProgress();
		if (inspecting && (weaponPosFrac > 0.0f || weapInspectionIsNotAllowed()))// Disable if aiming, going prone, frozen, shellshocked or took damage.
		{
			int current_animation = Viewmodel::GetCurrentWeaponAnimIndex();
			
			// restore to idle animation.
			unsigned int weap_index = Game::ps->offHandIndex;
			Game::DObj_s* obj = Game::cg_weaponsArray[Game::g_clients->ps.weapon].viewModelDObj;
			StartWeaponAnim(weap_index, obj, 1, 0.1f);
			inspecting = false;
		}
	}

	bool isUsingWeaponLowerAnimation(int weaponType, int current_animation)
	{
		if ( weaponType == 1 && current_animation == Game::WEAP_ANIM_DROP )
			return true;
		else if ( weaponType != 1 && current_animation == Game::WEAP_ANIM_SPRINT_IN )
			return true;
		
		return false;
	}



	void weaponLowerIfNotAllowed()
	{
		if (!Game::CL_IsCgameInitialized())
			return;
		
		int current_animation = Viewmodel::GetCurrentWeaponAnimIndex();
		float weaponPosFrac = Viewmodel::GetCurrentWeaponAdsProgress();

		const auto weapon = Game::bg_weaponDefs[Game::g_clients->ps.weapon];

		unsigned int weapIndex = Game::ps->offHandIndex;
		if ((Game::ps->weapFlags & 2) == 0)
		{
			weapIndex = Game::ps->weapon;
		}

		const auto player_allowFire = Dvars::Functions::Dvar_FindVar("player_allowFire");
		if (player_allowFire->current.enabled)
		{
			if ( weaponLower && weapIndex != 0 )
			{
				int current_real_animation = Viewmodel::GetCurrentRealWeaponAnimIndex();
				if (current_real_animation == 1)
				{
					Game::DObj_s* obj = Game::cg_weaponsArray[Game::g_clients->ps.weapon].viewModelDObj;
					StartWeaponAnim(weapIndex, obj, 1, 0.5f);
				}
				weaponLower = false;
			}
		}
		else
		{
			if (weapIndex != 0)
			{
				float weapLowerTime = weapon->sprintInTime;
				if (!inspecting && !weaponLower && !resetWeaponInspectionAnimation() && current_animation == 1 && weaponPosFrac == 0.0f && weapLowerTime != 0.0f)
				{
					weaponLower = true;
					Game::DObj_s* obj = Game::cg_weaponsArray[Game::g_clients->ps.weapon].viewModelDObj;
					if ( weapon->weapType == 1 )
						StartWeaponAnim(weapIndex, obj, Game::WEAP_ANIM_DROP, 0.5f);
					else
						StartWeaponAnim(weapIndex, obj, Game::WEAP_ANIM_SPRINT_IN, 0.5f);
				}
				else
				{
					if (weaponLower && (inspecting || resetWeaponInspectionAnimation() || !isUsingWeaponLowerAnimation(weapon->weapType, current_animation) || weaponPosFrac != 0.0f))
					{
						weaponLower = false;
						if (!inspecting && weaponPosFrac != 0.0f)
						{
							Game::DObj_s* obj = Game::cg_weaponsArray[Game::g_clients->ps.weapon].viewModelDObj;
							StartWeaponAnim(weapIndex, obj, 1, 0.5f);
						}
					}
				}
			}
		}
	}

	void weaponLowerCleanupOnRestart()
	{
		weaponLower = true;
	}


	Viewmodel::Viewmodel()
	{
		Utils::Hook(0x5C29F3, ViewModelOverrideStub, HOOK_JUMP).install()->quick();

		Scheduler::Loop(DisableInspectIfNotAllowed,Scheduler::Pipeline::MAIN);// Cancels the weapon inspection if a different weaponState is triggered.
		
		Utils::Hook(0x5C4EC0, weaponLowerCleanupOnRestart, HOOK_CALL).install()->quick();// reset the weaponLower animIndex if loadgame is triggered.
		Utils::Hook(0x5C50D0, weaponLowerCleanupOnRestart, HOOK_CALL).install()->quick();// reset the weaponLower animIndex if loadgame_continue is triggered.



		// Internal Methods. only called in animscripts\utility_code.gsc
		GSC::AddMethod("weaponLowerIfNotAllowed", [](Game::scr_entref_t entref)
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

			weaponLower = false;

			Scheduler::Loop(weaponLowerIfNotAllowed,Scheduler::Pipeline::MAIN,300ms);// Cancels the weapon inspection if a different weaponState is triggered.
		}, false);

		GSC::AddMethod("stopWeaponInspectAnimation", [](Game::scr_entref_t entref)
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

			int current_animation = Viewmodel::GetCurrentWeaponAnimIndex();
			
			// restore to idle animation.
			unsigned int weap_index = Game::ps->offHandIndex;
			Game::DObj_s* obj = Game::cg_weaponsArray[Game::g_clients->ps.weapon].viewModelDObj;
			StartWeaponAnim(weap_index, obj, 1, 0.1f);
		}, false);

		GSC::AddMethod("endWeaponInspection", [](Game::scr_entref_t entref)
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

			inspecting = false;
		}, false);
		// Internal Methods. only called in animscripts\utility_code.gsc

		GSC::AddMethod("getWeaponInspectionTime", [](Game::scr_entref_t entref)
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
			
			float weapInspectTime;
			
			const auto weapon = Game::bg_weaponDefs[Game::g_clients->ps.weapon];
			if ( weapon->weapType == 1 )
			{
				weapInspectTime = weapon->iRechamberBoltTime;
			}
			else
			{
				weapInspectTime = weapon->iDetonateTime;
			}
			
			weapInspectTime = weapInspectTime / 1000;
			
			Game::Scr_AddFloat(weapInspectTime);
		}, false);

		GSC::AddMethod("isInspectingWeapon", [](Game::scr_entref_t entref)
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
			
			if ( inspecting )
			{
				return Game::Scr_AddInt(true);
			}
			else
			{
				return Game::Scr_AddInt(false);
			}
		}, false);



		// Commands.
		Command::Add("weapinspect", [](Command::Params*)
		{
			if (!Game::CL_IsCgameInitialized())
				return;

			const auto cg_weaponInspection = Dvars::Functions::Dvar_FindVar("cg_weaponInspection");
			if (cg_weaponInspection->current.enabled)
			{
				int current_animation = Viewmodel::GetCurrentWeaponAnimIndex();
				float weaponPosFrac = Viewmodel::GetCurrentWeaponAdsProgress();

				unsigned int weapIndex = Game::ps->offHandIndex;
				if ((Game::ps->weapFlags & 2) == 0)
				{
					weapIndex = Game::ps->weapon;
				}

				if ( !inspecting && weapIndex != 0 )
				{
					if ( !resetWeaponInspectionAnimation() && !weapInspectionIsNotAllowed() )
					{
						int animIndex = getWeaponInspectionAnimation();
						float weapInspectTime = getWeaponInspectionTime();
						
						// only idle and weaponLower animations are allowed to use this
						if (weaponPosFrac == 0.0f && ( current_animation == 1 || weaponLower ) && weapInspectTime != 0.0f )
						{
							Game::DObj_s* obj = Game::cg_weaponsArray[Game::g_clients->ps.weapon].viewModelDObj;
							StartWeaponAnim(weapIndex, obj, animIndex, 0.5f);
							inspecting = true;
						}
					}
				}
			}
		});

		#ifdef DEVELOPER_MODE
			Command::Add("vm_curanim", [](Command::Params*)
			{
				if (!Game::CL_IsCgameInitialized())
					return;

				int current_animation = Viewmodel::GetCurrentWeaponAnimIndex();
				Game::Com_Printf(0, "Current weapon animation index: %d\n", current_animation);
			});

			Command::Add("vm_weaponindex", [](Command::Params*)
			{
				if (!Game::CL_IsCgameInitialized())
					return;

				unsigned int weap_index = Game::ps->offHandIndex;
				if ((Game::ps->weapFlags & 2) == 0)
				{
					weap_index = Game::ps->weapon;
				}
				
				Game::Com_Printf(0, "Current weapon index: %d\n", weap_index);
			});

			Command::Add("vm_curevent", [](Command::Params*)
			{
				if (!Game::CL_IsCgameInitialized())
					return;

				int current_event = Game::ps->events[Game::ps->eventSequence & 3];
				Game::Com_Printf(0, "Current weapon event: %d\n", current_event);
			});

			Command::Add("vm_curweapontype", [](Command::Params*)
			{
				if (!Game::CL_IsCgameInitialized())
					return;

				const auto weapon = Game::bg_weaponDefs[Game::g_clients->ps.weapon];
				Game::Com_Printf(0, "Current weapon type: ^2%d \n", weapon->weapType);
			});

			Command::Add("vm_anim", [](Command::Params* p)
			{
				if (!Game::CL_IsCgameInitialized())
					return;

				unsigned int weap_index = Game::ps->offHandIndex;
				if ((Game::ps->weapFlags & 2) == 0)
				{
					weap_index = Game::ps->weapon;
				}
				
				Game::DObj_s* obj = Game::cg_weaponsArray[Game::g_clients->ps.weapon].viewModelDObj;

				StartWeaponAnim(weap_index, obj, std::atoi(p->get(1)), 0.5f);
			});
		#endif
	}
}