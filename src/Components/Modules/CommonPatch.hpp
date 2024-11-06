#pragma once

namespace Components
{
	class CommonPatch : public Component
	{
	public:
		inline static bool iw3sp_mod_loc_ff_exists;
		inline static bool iw3sp_mod_ff_exists;
		CommonPatch();
	private:
		static void SV_CheatsStub();
		static void DB_LoadCommonFastFiles();
		static void RunModStub();
		static void ClearModStub();
		static void R_Cinematic_BinkOpenStub1(char* buffer, size_t size, const char* /*format*/, const char* directory, const char* fileName, const char* videoFormat);
		static void Com_Quit_f_Stub();
		static void UpdateLanguageStub();
		static int R_RegisterSunDvarsStub();
		static Game::dvar_s* RegisterConColor(const char* dvarName, float r, float g, float b, float a, float min, float max, unsigned __int16 flags, const char* description);
		static void RegisterConColorStub1();
		static void RegisterConColorStub2();
		static void RegisterConColorStub3();
		static void RegisterConColorStub4();
		static void RegisterConColorStub5();
		static void LanguageSetValueConfig(int langIndex);

		static void CG_DrawFPSStub();
		static void EnhancedDebugInformation();
		static float CG_DrawAICount(float previous_y, float posX, float labelWidth);
		static float CG_DrawEntitiesCountInfo(float previous_y, float posX, float labelWidth);
		static float CG_DrawDObjInfo(float previous_y, float posX, float labelWidth);
		static float CG_DrawViewpos(float previous_y, float posX, float labelWidth);

		static bool CheckMilesFiles();
		static const char* __stdcall AIL_set_redist_directory_Stub(const char* directory);

		static Game::dvar_s* fs_usedevdir_fix(const char* dvarName, int type, unsigned __int16 flags, float r, float g, float b, float a, float min, float max, const char* description);
		static void fs_usedevdir_register_stub();
	};
}