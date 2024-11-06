#include "STDInc.hpp"

namespace Dvars
{
	// Test
	//Game::dvar_s* test_dvar_int = nullptr;
	//Game::dvar_s* test_dvar_bool = nullptr;
	//Game::dvar_s* test_dvar_string = nullptr;
	//Game::dvar_s* test_dvar_enum = nullptr;
	//Game::dvar_s* test_dvar_float = nullptr;
	//Game::dvar_s* test_dvar_vec2 = nullptr;
	//Game::dvar_s* test_dvar_vec3 = nullptr;
	//Game::dvar_s* test_dvar_vec4 = nullptr;
	//Game::dvar_s* test_dvar_bool_savedflag = nullptr;

	Game::dvar_s* ui_language = nullptr;
	Game::dvar_s* language_first_setting = nullptr;
	Game::dvar_s* r_noborder = nullptr;
	Game::dvar_s* r_videomode = nullptr;
	Game::dvar_s* ui_nativeCursor = nullptr;
	Game::dvar_s* safeArea_horizontal = nullptr;
	Game::dvar_s* safeArea_vertical = nullptr;

	// d3d9
	Game::dvar_s* r_d3d9ex = nullptr;

	// Field of view dvars
	Game::dvar_s* viewmodelFovOffsetEnabled;
	Game::dvar_s* cg_fovMenu = nullptr;
	Game::dvar_s* cg_fovScale = nullptr;
	Game::dvar_s* cg_fovMin = nullptr;
	Game::dvar_s* cg_fov = nullptr;
	Game::dvar_s* cg_drawViewpos = nullptr;

	Game::dvar_s* developer_mode = nullptr;
	Game::dvar_s* ui_debugMode = nullptr;
	Game::dvar_s* console_menu_style = nullptr;

	// Player settings dvars
	Game::dvar_s* player_allowFire = nullptr;
	Game::dvar_s* player_sprintUnlimited = nullptr;
	Game::dvar_s* paintball_mode = nullptr;

	Game::dvar_s* external_console = nullptr;
	Game::dvar_s* allowCompactFolderForMod = nullptr;

	Game::dvar_s* M_RawInput = nullptr;

	// Gamepad
	Game::dvar_s* gpad_enabled = nullptr;
	Game::dvar_s* gpad_debug = nullptr;
	Game::dvar_s* gpad_present = nullptr;
	Game::dvar_s* gpad_in_use = nullptr;
	Game::dvar_s* gpad_style = nullptr;
	Game::dvar_s* gpad_sticksConfig = nullptr;
	Game::dvar_s* gpad_buttonConfig = nullptr;
	Game::dvar_s* gpad_menu_scroll_delay_first = nullptr;
	Game::dvar_s* gpad_menu_scroll_delay_rest = nullptr;
	Game::dvar_s* gpad_stick_pressed_hysteresis = nullptr;
	Game::dvar_s* gpad_stick_pressed = nullptr;
	Game::dvar_s* gpad_stick_deadzone_min = nullptr;
	Game::dvar_s* gpad_stick_deadzone_max = nullptr;
	Game::dvar_s* gpad_button_deadzone = nullptr;
	Game::dvar_s* gpad_button_lstick_deflect_max = nullptr;
	Game::dvar_s* gpad_button_rstick_deflect_max = nullptr;
	Game::dvar_s* gpad_use_hold_time = nullptr;
	Game::dvar_s* gpad_lockon_enabled = nullptr;
	Game::dvar_s* gpad_slowdown_enabled = nullptr;
	Game::dvar_s* gpad_autoaim_enabled = nullptr;
	Game::dvar_s* gpad_aimassist = nullptr;
	Game::dvar_s* input_viewSensitivity = nullptr;
	Game::dvar_s* input_invertPitch = nullptr;
	Game::dvar_s* aim_aimAssistRangeScale = nullptr;
	Game::dvar_s* aim_autoAimRangeScale = nullptr;
	Game::dvar_s* aim_lockon_pitch_strength = nullptr;

	Game::dvar_s* damagefeedback = nullptr;
	//Game::dvar_s* chaos_mode = nullptr;
	Game::dvar_s* specialops = nullptr;
	Game::dvar_s* survival = nullptr;
	Game::dvar_s* zombiemode = nullptr;
	Game::dvar_s* viewmodelAnimatedJumps = nullptr;
	Game::dvar_s* meleeAttackType = nullptr;
	Game::dvar_s* gameMode = nullptr;
	Game::dvar_s* gameModeSP = nullptr;
	Game::dvar_s* gameModeSO = nullptr;
	Game::dvar_s* gameModeZM = nullptr;
	Game::dvar_s* perks_enabled = nullptr;
	Game::dvar_s* bulletpenetration = nullptr;
	Game::dvar_s* marathon = nullptr;
	Game::dvar_s* fastreload = nullptr;
	Game::dvar_s* longersprint = nullptr;
	Game::dvar_s* extralethal = nullptr;
	Game::dvar_s* quickdraw = nullptr;
	Game::dvar_s* armorvest = nullptr;
	Game::dvar_s* armorvest2 = nullptr;
	Game::dvar_s* twoprimaries = nullptr;
	Game::dvar_s* finalstand = nullptr;
	Game::dvar_s* cg_previewMode = nullptr;
	Game::dvar_s* cg_weaponInspection = nullptr;
	//Game::dvar_s* cg_weaponSwitchTransition = nullptr;
	Game::dvar_s* cg_useLegacyAnims = nullptr;
	Game::dvar_s* cg_crosshairVerticalOffset = nullptr;
	Game::dvar_s* scr_physWeaponDrop = nullptr;
	Game::dvar_s* phys_weapon_delete_time = nullptr;

	Game::dvar_s* mis_02 = nullptr;
	Game::dvar_s* mis_03 = nullptr;
	Game::dvar_s* mis_04 = nullptr;
	Game::dvar_s* mis_05 = nullptr;
	Game::dvar_s* mis_06 = nullptr;
	Game::dvar_s* mis_07 = nullptr;
	Game::dvar_s* mis_08 = nullptr;
	Game::dvar_s* mis_09 = nullptr;
	Game::dvar_s* mis_10 = nullptr;

	Game::dvar_s* ui_level01 = nullptr;
	Game::dvar_s* ui_level02 = nullptr;
	Game::dvar_s* ui_level03 = nullptr;
	Game::dvar_s* ui_level04 = nullptr;
	Game::dvar_s* ui_level05 = nullptr;
	Game::dvar_s* ui_level_status = nullptr;

	Game::dvar_s* zombiemode_survivors = nullptr;

	Game::dvar_s* timer_enabled = nullptr;

	Game::dvar_s* dlc1 = nullptr;
	Game::dvar_s* dlc2 = nullptr;
	Game::dvar_s* dlc3 = nullptr;
	Game::dvar_s* dlc4 = nullptr;
	Game::dvar_s* dlc5 = nullptr;
	Game::dvar_s* dlc6 = nullptr;
	Game::dvar_s* dlc7 = nullptr;
	Game::dvar_s* dlc8 = nullptr;
	Game::dvar_s* dlc9 = nullptr;
	Game::dvar_s* dlc10 = nullptr;

	Game::dvar_s* demo_build = nullptr;
	Game::dvar_s* early_demo_build = nullptr;
	Game::dvar_s* limited_mode = nullptr;

	Game::dvar_s* hud_showStanceCarry = nullptr;

	//Game::dvar_s* g_speed = nullptr;
	//Game::dvar_s* g_speedScale = nullptr;

	namespace Functions
	{
		Dvar_FindVar_t Dvar_FindVar = (Dvar_FindVar_t)0x587B00;

		void Dvar_SetStringByName(const char* dvarName, const char* value)
		{
			const static uint32_t Dvar_SetStringByName_func = 0x589720;
			__asm
			{
				mov		eax, dvarName;
				push	value;
				call	Dvar_SetStringByName_func;
				add     esp, 4;
			}
		}

		void Dvar_SetIntByName(const char* dvarName, int value)
		{
			const static uint32_t Dvar_SetIntByName_func = 0x589630;
			__asm
			{
				mov		eax, dvarName;
				push	value;
				call	Dvar_SetIntByName_func;
				add     esp, 4;
			}
		}

		void Dvar_Reset(int value/*eax*/, Game::dvar_s* dvar/*ecx*/)
		{
			const static uint32_t Dvar_Reset_func = 0x5898E0;
			__asm
			{
				mov		eax, value;
				mov		ecx, dvar;
				call	Dvar_Reset_func;
			}
		}

		void SetRaw(const char* dvarName, bool value)
		{
			if (const auto& dvar = Functions::Dvar_FindVar(dvarName); dvar)
			{
				dvar->current.enabled = value;
				dvar->latched.enabled = value;
			}
		}

		void SetRaw(Game::dvar_s* dvar, bool value)
		{
			dvar->current.enabled = value;
			dvar->latched.enabled = value;
		}

		void SetRaw(const char* dvarName, const char* value)
		{
			if (const auto& dvar = Functions::Dvar_FindVar(dvarName); dvar)
			{
				dvar->current.string = value;
				dvar->latched.string = value;
			}
		}

		void SetRaw(Game::dvar_s* dvar, const char* value)
		{
			dvar->current.string = value;
			dvar->latched.string = value;
		}
	}

	namespace Register
	{

	}

	namespace Override
	{
		void DvarBoolOverride(const char* dvarName, const bool value, Game::dvar_flags flags)
		{
			if (const auto& dvar = Functions::Dvar_FindVar(dvarName); dvar)
			{
				dvar->current.enabled = value;
				dvar->latched.enabled = value;
				dvar->flags = flags;
				dvar->modified = false;
			}
		}

		void DvarBoolOverride(const char* dvarName, Game::dvar_flags flags)
		{
			if (const auto& dvar = Functions::Dvar_FindVar(dvarName); dvar)
			{
				dvar->flags = flags;
				dvar->modified = false;
			}
		}

		void DvarIntOverride(const char* dvarName, const int value, Game::dvar_flags flags)
		{
			if (const auto& dvar = Functions::Dvar_FindVar(dvarName); dvar)
			{
				dvar->current.integer = value;
				dvar->latched.integer = value;
				dvar->flags = flags;
				dvar->modified = false;
			}
		}

		void DvarIntOverride(const char* dvarName, Game::dvar_flags flags)
		{
			if (const auto& dvar = Functions::Dvar_FindVar(dvarName); dvar)
			{
				dvar->flags = flags;
				dvar->modified = false;
			}
		}

		void DvarVec4Override(const char* dvarName, const float* dvar_value)
		{
			if (const auto& dvar = Functions::Dvar_FindVar(dvarName); dvar)
			{
				for (auto i = 0; i < 4; i++)
				{
					dvar->current.vector[i] = dvar_value[i];
					dvar->latched.vector[i] = dvar_value[i];
					dvar->modified = false;
				}
			}
		}

		void DvarVec4OverrideDefaultValue(const char* dvarName, const float* dvar_value)
		{
			if (const auto& dvar = Functions::Dvar_FindVar(dvarName); dvar)
			{
				for (auto i = 0; i < 4; i++)
				{
					dvar->reset.vector[i] = dvar_value[i];
					dvar->modified = false;
				}
			}
		}

		void DvarFloatOverride(const char* dvarName, Game::dvar_flags flags)
		{
			if (const auto& dvar = Functions::Dvar_FindVar(dvarName); dvar)
			{
				dvar->flags = flags;
				dvar->modified = false;
			}
		}

		void DvarFloatOverride(const char* dvarName, const float value, Game::dvar_flags flags)
		{
			if (const auto& dvar = Functions::Dvar_FindVar(dvarName); dvar)
			{
				dvar->current.value = value;
				dvar->latched.value = value;
				dvar->flags = flags;
				dvar->modified = false;
			}
		}

		void DvarFloatOverride(const char* dvarName, const float value, Game::dvar_flags flags, float mins, float maxs)
		{
			if (const auto& dvar = Functions::Dvar_FindVar(dvarName); dvar)
			{
				dvar->current.value = value;
				dvar->latched.value = value;
				dvar->domain.value.min = mins;
				dvar->domain.value.max = maxs;
				dvar->flags = flags;
				dvar->modified = false;
			}
		}
	}
}