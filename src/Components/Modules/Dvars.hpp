#pragma once

namespace Dvars
{
	// Test
	//extern Game::dvar_s* test_dvar_int;
	//extern Game::dvar_s* test_dvar_bool;
	//extern Game::dvar_s* test_dvar_string;
	//extern Game::dvar_s* test_dvar_enum;
	//extern Game::dvar_s* test_dvar_float;
	//extern Game::dvar_s* test_dvar_vec2;
	//extern Game::dvar_s* test_dvar_vec3;
	//extern Game::dvar_s* test_dvar_vec4;
	//extern Game::dvar_s* test_dvar_bool_savedflag;

	extern Game::dvar_s* ui_language;
	extern Game::dvar_s* language_first_setting;
	extern Game::dvar_s* r_noborder;
	extern Game::dvar_s* r_videomode;
	extern Game::dvar_s* ui_nativeCursor;
	extern Game::dvar_s* safeArea_horizontal;
	extern Game::dvar_s* safeArea_vertical;

	// d3d9
	extern Game::dvar_s* r_d3d9ex;

	// Field of view dvars
	extern Game::dvar_s* viewmodelFovOffsetEnabled;
	extern Game::dvar_s* cg_fovMenu;
	extern Game::dvar_s* cg_fovScale;
	extern Game::dvar_s* cg_fovMin;
	extern Game::dvar_s* cg_fov;
	extern Game::dvar_s* cg_drawViewpos;

	extern Game::dvar_s* developer_mode;
	extern Game::dvar_s* ui_debugMode;
	extern Game::dvar_s* console_menu_style;

	// Player settings dvars
	extern Game::dvar_s* player_allowFire;
	extern Game::dvar_s* player_sprintUnlimited;
	extern Game::dvar_s* paintball_mode;

	extern Game::dvar_s* external_console;
	extern Game::dvar_s* allowCompactFolderForMod;

	extern Game::dvar_s* M_RawInput;

	// Gamepad
	extern Game::dvar_s* gpad_enabled;
	extern Game::dvar_s* gpad_debug;
	extern Game::dvar_s* gpad_present;
	extern Game::dvar_s* gpad_in_use;
	extern Game::dvar_s* gpad_style;
	extern Game::dvar_s* gpad_sticksConfig;
	extern Game::dvar_s* gpad_buttonConfig;
	extern Game::dvar_s* gpad_menu_scroll_delay_first;
	extern Game::dvar_s* gpad_menu_scroll_delay_rest;
	extern Game::dvar_s* gpad_stick_pressed_hysteresis;
	extern Game::dvar_s* gpad_stick_pressed;
	extern Game::dvar_s* gpad_stick_deadzone_min;
	extern Game::dvar_s* gpad_stick_deadzone_max;
	extern Game::dvar_s* gpad_button_deadzone;
	extern Game::dvar_s* gpad_button_lstick_deflect_max;
	extern Game::dvar_s* gpad_button_rstick_deflect_max;
	extern Game::dvar_s* gpad_use_hold_time;
	extern Game::dvar_s* gpad_lockon_enabled;
	extern Game::dvar_s* gpad_slowdown_enabled;
	extern Game::dvar_s* gpad_autoaim_enabled;
	extern Game::dvar_s* gpad_aimassist;
	extern Game::dvar_s* input_viewSensitivity;
	extern Game::dvar_s* input_invertPitch;
	extern Game::dvar_s* aim_aimAssistRangeScale;
	extern Game::dvar_s* aim_autoAimRangeScale;
	extern Game::dvar_s* aim_lockon_pitch_strength;

	extern Game::dvar_s* damagefeedback;
	//extern Game::dvar_s* chaos_mode;
	extern Game::dvar_s* specialops;
	extern Game::dvar_s* survival;
	extern Game::dvar_s* zombiemode;
	extern Game::dvar_s* viewmodelAnimatedJumps;
	extern Game::dvar_s* meleeAttackType;
	extern Game::dvar_s* gameMode;
	extern Game::dvar_s* gameModeSP;
	extern Game::dvar_s* gameModeSO;
	extern Game::dvar_s* gameModeZM;
	extern Game::dvar_s* perks_enabled;
	extern Game::dvar_s* bulletpenetration;
	extern Game::dvar_s* marathon;
	extern Game::dvar_s* fastreload;
	extern Game::dvar_s* longersprint;
	extern Game::dvar_s* extralethal;
	extern Game::dvar_s* quickdraw;
	extern Game::dvar_s* armorvest;
	extern Game::dvar_s* armorvest2;
	extern Game::dvar_s* twoprimaries;
	extern Game::dvar_s* finalstand;
	extern Game::dvar_s* cg_previewMode;
	extern Game::dvar_s* cg_weaponInspection;
	//extern Game::dvar_s* cg_weaponSwitchTransition;
	extern Game::dvar_s* cg_useLegacyAnims;
	extern Game::dvar_s* cg_crosshairVerticalOffset;
	extern Game::dvar_s* scr_physWeaponDrop;
	extern Game::dvar_s* phys_weapon_delete_time;

	extern Game::dvar_s* mis_02;
	extern Game::dvar_s* mis_03;
	extern Game::dvar_s* mis_04;
	extern Game::dvar_s* mis_05;
	extern Game::dvar_s* mis_06;
	extern Game::dvar_s* mis_07;
	extern Game::dvar_s* mis_08;
	extern Game::dvar_s* mis_09;
	extern Game::dvar_s* mis_10;

	extern Game::dvar_s* ui_level01;
	extern Game::dvar_s* ui_level02;
	extern Game::dvar_s* ui_level03;
	extern Game::dvar_s* ui_level04;
	extern Game::dvar_s* ui_level05;
	extern Game::dvar_s* ui_level_status;

	extern Game::dvar_s* zombiemode_survivors;

	extern Game::dvar_s* timer_enabled;

	extern Game::dvar_s* dlc1;
	extern Game::dvar_s* dlc2;
	extern Game::dvar_s* dlc3;
	extern Game::dvar_s* dlc4;
	extern Game::dvar_s* dlc5;
	extern Game::dvar_s* dlc6;
	extern Game::dvar_s* dlc7;
	extern Game::dvar_s* dlc8;
	extern Game::dvar_s* dlc9;
	extern Game::dvar_s* dlc10;

	extern Game::dvar_s* demo_build;
	extern Game::dvar_s* early_demo_build;
	extern Game::dvar_s* limited_mode;

	extern Game::dvar_s* hud_showStanceCarry;

	//extern Game::dvar_s* g_speed;
	//extern Game::dvar_s* g_speedScale;

	namespace Functions
	{
		typedef Game::dvar_s* (__cdecl* Dvar_FindVar_t)(const char* dvarName);
		extern Dvar_FindVar_t Dvar_FindVar;

		void Dvar_SetStringByName(const char* dvarName, const char* value);
		void Dvar_SetIntByName(const char* dvarName, int value);
		void Dvar_Reset(int value/*eax*/, Game::dvar_s* dvar/*ecx*/);

		// Sets
		void SetRaw(const char* dvarName, bool value);
		void SetRaw(Game::dvar_s* dvar, bool value);
		void SetRaw(const char* dvarName, const char* value);
		void SetRaw(Game::dvar_s* dvar, const char* value);
	}

	namespace Register
	{
		// INTEGER
		static Utils::function<Game::dvar_s* (const char* dvarName, Game::dvar_type typeInt, std::uint16_t flags, const char* description, std::int32_t defaultValue, std::int32_t null1, std::int32_t null2, std::int32_t null3, std::int32_t minValue, std::int32_t maxValue)>
			Dvar_RegisterInt_r = 0x588750;
		static Utils::function<void(Game::dvar_s* dvar, const char* dvarName, Game::dvar_type typeInt, std::uint16_t flags, const char* description, std::int32_t defaultValue, std::int32_t null1, std::int32_t null2, std::int32_t null3, std::int32_t minValue, std::int32_t maxValue)>
			Dvar_ReregisterInt = 0x588610;

		inline Game::dvar_s* Dvar_RegisterInt(const char* dvar_name, const char* description, std::int32_t default_value, std::int32_t min_value, std::int32_t max_value, std::uint16_t flags)
		{
			if (const auto& dvar = Functions::Dvar_FindVar(dvar_name); dvar)
			{
				Dvar_ReregisterInt(dvar, dvar_name, Game::dvar_type::integer, flags, description, default_value, 0, 0, 0, min_value, max_value);
				return dvar;
			}
			return Dvar_RegisterInt_r(dvar_name, Game::dvar_type::integer, flags, description, default_value, 0, 0, 0, min_value, max_value);
		}

		// BOOL
		static Utils::function<Game::dvar_s* (const char* dvar_name, Game::dvar_type type_bool, std::uint16_t flags, const char* description, std::int32_t default_value, std::int32_t null1, std::int32_t null2, std::int32_t null3, std::int32_t null4, std::int32_t null5)>
			Dvar_RegisterNewBool = 0x588750;
		static Utils::function<void(Game::dvar_s* dvar, const char* dvar_name, Game::dvar_type type_bool, std::uint16_t flags, const char* description, std::int32_t default_value, std::int32_t null1, std::int32_t null2, std::int32_t null3, std::int32_t null4, std::int32_t null5)>
			Dvar_ReregisterBool = 0x588610;

		inline Game::dvar_s* Dvar_RegisterBool(const char* dvar_name, const char* description, std::int32_t default_value, std::uint16_t flags)
		{
			if (const auto& dvar = Functions::Dvar_FindVar(dvar_name); dvar)
			{
				Dvar_ReregisterBool(dvar, dvar_name, Game::dvar_type::boolean, flags, description, default_value, 0, 0, 0, 0, 0);
				return dvar;
			}
			return Dvar_RegisterNewBool(dvar_name, Game::dvar_type::boolean, flags, description, default_value, 0, 0, 0, 0, 0);
		}

		// ENUM
		static Utils::function<Game::dvar_s* (const char* dvar_name, Game::dvar_type type_enum, std::uint16_t flags, const char* description, std::int32_t default_index, std::int32_t null1, std::int32_t null2, std::int32_t null3, std::int32_t enumSize, const char** enum_data)>
			Dvar_RegisterNewEnum = 0x588750;
		static Utils::function<void(Game::dvar_s* dvar, const char* dvar_name, Game::dvar_type type_enum, std::uint16_t flags, const char* description, std::int32_t default_value, std::int32_t null1, std::int32_t null2, std::int32_t null3, std::int32_t enumSize, const char** enum_data)>
			Dvar_ReregisterEnum = 0x588610;

		inline Game::dvar_s* Dvar_RegisterEnum(const char* dvar_name, const char* description, std::int32_t default_value, std::int32_t enum_size, const char** enum_data, std::uint16_t flags)
		{
			if (const auto& dvar = Functions::Dvar_FindVar(dvar_name); dvar)
			{
				Dvar_ReregisterEnum(dvar, dvar_name, Game::dvar_type::enumeration, flags, description, default_value, 0, 0, 0, enum_size, enum_data);
				return dvar;
			}
			return Dvar_RegisterNewEnum(dvar_name, Game::dvar_type::enumeration, flags, description, default_value, 0, 0, 0, enum_size, enum_data);
		}

		// STRING
		static Utils::function<Game::dvar_s* (const char* dvar_name, Game::dvar_type type_string, std::uint16_t flags, const char* description, const char* default_value, std::int32_t null1, std::int32_t null2, std::int32_t null3, std::int32_t null4, std::int32_t null5)>
			Dvar_RegisterString_r = 0x588750;
		static Utils::function<void(Game::dvar_s* dvar, const char* dvar_name, Game::dvar_type type_string, std::uint16_t flags, const char* description, const char* default_value, std::int32_t null1, std::int32_t null2, std::int32_t null3, std::int32_t null4, std::int32_t null5)>
			Dvar_ReregisterString = 0x588610;

		inline Game::dvar_s* Dvar_RegisterString(const char* dvar_name, const char* description, const char* default_value, std::uint16_t flags)
		{
			if (const auto& dvar = Functions::Dvar_FindVar(dvar_name); dvar)
			{
				Dvar_ReregisterString(dvar, dvar_name, Game::dvar_type::string, flags, description, default_value, 0, 0, 0, 0, 0);
				return dvar;
			}
			return Dvar_RegisterString_r(dvar_name, Game::dvar_type::string, flags, description, default_value, 0, 0, 0, 0, 0);
		}

		// VEC2
		static Utils::function<Game::dvar_s* (const char* dvar_name, Game::dvar_type type_float2, std::uint16_t flags, const char* description, float x, float y, std::int32_t null1, std::int32_t null2, float min, float max)>
			Dvar_RegisterVec2_r = 0x588750;
		static Utils::function<void(Game::dvar_s* dvar, const char* dvar_name, Game::dvar_type type_float2, std::uint16_t flags, const char* description, float x, float y, std::int32_t null1, std::int32_t null2, float min, float max)>
			Dvar_ReregisterVec2 = 0x588610;

		inline Game::dvar_s* Dvar_RegisterVec2(const char* dvar_name, const char* description, float x, float y, float min_value, float max_value, std::uint16_t flags) 
		{
			if (const auto& dvar = Functions::Dvar_FindVar(dvar_name); dvar)
			{
				Dvar_ReregisterVec2(dvar, dvar_name, Game::dvar_type::vec2, flags, description, x, y, 0, 0, min_value, max_value);
				return dvar;
			}
			return Dvar_RegisterVec2_r(dvar_name, Game::dvar_type::vec2, flags, description, x, y, 0, 0, min_value, max_value);
		}

		// VEC3
		static Utils::function<Game::dvar_s* (const char* dvar_name, Game::dvar_type type_float3, std::uint16_t flags, const char* description, float x, float y, float z, std::int32_t null, float min, float max)>
			Dvar_RegisterVec3_r = 0x588750;
		static Utils::function<void(Game::dvar_s* dvar, const char* dvar_name, Game::dvar_type type_float3, std::uint16_t flags, const char* description, float x, float y, float z, std::int32_t null, float min, float max)>
			Dvar_ReregisterVec3 = 0x588610;

		inline Game::dvar_s* Dvar_RegisterVec3(const char* dvar_name, const char* description, float x, float y, float z, float min_value, float max_value, std::uint16_t flags) 
		{
			if (const auto& dvar = Functions::Dvar_FindVar(dvar_name); dvar)
			{
				Dvar_ReregisterVec3(dvar, dvar_name, Game::dvar_type::vec3, flags, description, x, y, z, 0, min_value, max_value);
				return dvar;
			}
			return Dvar_RegisterVec3_r(dvar_name, Game::dvar_type::vec3, flags, description, x, y, z, 0, min_value, max_value);
		}

		// VEC4
		static Utils::function<Game::dvar_s* (const char* dvar_name, Game::dvar_type type_float4, std::uint16_t flags, const char* description, float x, float y, float z, float w, float min, float max)>
			Dvar_RegisterNewVec4 = 0x588750;
		static Utils::function<void(Game::dvar_s* dvar, const char* dvar_name, Game::dvar_type type_float4, std::uint16_t flags, const char* description, float x, float y, float z, float w, float min, float max)>
			Dvar_ReregisterVec4 = 0x588610;

		inline Game::dvar_s* Dvar_RegisterVec4(const char* dvar_name, const char* description, float x, float y, float z, float w, float min_value, float max_value, std::uint16_t flags) 
		{
			if (const auto& dvar = Functions::Dvar_FindVar(dvar_name); dvar)
			{
				Dvar_ReregisterVec4(dvar, dvar_name, Game::dvar_type::vec4, flags, description, x, y, z, w, min_value, max_value);
				return dvar;
			}
			return Dvar_RegisterNewVec4(dvar_name, Game::dvar_type::vec4, flags, description, x, y, z, w, min_value, max_value);
		}

		// FLOAT
		static Utils::function<Game::dvar_s* (const char* dvar_name, Game::dvar_type type_float2, std::uint16_t flags, const char* description, float x, float y, std::int32_t null1, std::int32_t null2, float min, float max)>
			Dvar_RegisterFloat_r = 0x588750;
		static Utils::function<void(Game::dvar_s* dvar, const char* dvar_name, Game::dvar_type type_float2, std::uint16_t flags, const char* description, float x, float y, std::int32_t null1, std::int32_t null2, float min, float max)>
			Dvar_ReregisterFloat = 0x588610;

		inline Game::dvar_s* Dvar_RegisterFloat(const char* dvar_name, const char* description, float default_value, float min_value, float max_value, std::uint16_t flags) 
		{
			if (const auto& dvar = Functions::Dvar_FindVar(dvar_name); dvar)
			{
				Dvar_ReregisterFloat(dvar, dvar_name, Game::dvar_type::value, flags, description, default_value, 0, 0, 0, min_value, max_value);
				return dvar;
			}
			return Dvar_RegisterFloat_r(dvar_name, Game::dvar_type::value, flags, description, default_value, 0, 0, 0, min_value, max_value);
		}
	}

	namespace Override
	{
		void DvarBoolOverride(const char* dvarName, const bool value, Game::dvar_flags flags);
		void DvarBoolOverride(const char* dvarName, Game::dvar_flags flags);
		void DvarVec4Override(const char* dvarName, const float* dvar_value);
		void DvarVec4OverrideDefaultValue(const char* dvarName, const float* dvar_value);
		void DvarFloatOverride(const char* dvarName, Game::dvar_flags flags);
		void DvarFloatOverride(const char* dvarName, const float value, Game::dvar_flags flags);
		void DvarFloatOverride(const char* dvarName, const float value, Game::dvar_flags flags, float mins, float maxs);
		void DvarIntOverride(const char* dvarName, const int value, Game::dvar_flags flags);
		void DvarIntOverride(const char* dvarName, Game::dvar_flags flags);
	}
}