#pragma once
#include "../Utils/Function.hpp"

namespace Game
{
	//defines
	constexpr auto KEY_NAME_COUNT = 95;
	constexpr auto LOCALIZED_KEY_NAME_COUNT = 95;
	
	const static HWND__* hWnd = reinterpret_cast<HWND__*>(0x13E39A8); // Splash screen (cod.bmp)
	const static HWND* hWndParent = reinterpret_cast<HWND*>(0x13E39B0); // External console
	const static HWND* hWndBuffer = reinterpret_cast<HWND*>(0x13E39B4); // External console buffer

	extern IDirect3DDevice9** dx9_device_ptr;

	// Game data
	extern Game::AimAssistGlobals* aaGlobArray;
	extern Game::cg_s* cgs;
	extern Game::cgMedia_t* cgMedia;
	extern Game::cgs_t* cgsArray;
	extern Game::centity_s* cg_entitiesArray;
	extern Game::clientActive_t* clients;
	extern Game::clientUIActive_t* clientUIActive;
	extern Game::CinematicGlob* cinematicGlob;
	extern Game::clipMap_t* cm;
	extern Game::gclient_s* g_clients;
	extern Game::gentity_s* g_entities;
	extern Game::GfxDrawMethod* gfxDrawMethod;
	extern Game::itemDef_s* my_item;
	extern Game::keyname_t* keyNames;
	extern Game::keyname_t* localizedKeyNames;
	//extern Game::languageInfo_t* g_languages;
	extern Game::PlayerKeyState* playerKeys;
	extern Game::playerState_s* ps;
	extern Game::pmove_t* pmove;
	extern Game::ScreenPlacement* scrPlace;
	extern Game::ScreenPlacement* scrPlaceFull;
	extern Game::ScreenPlacement* scrPlaceFullUnsafe;
	extern Game::searchpath_s* fs_searchpaths;
	extern Game::sharedUiInfo_t* sharedUiInfo;
	extern Game::snd_local_t* g_snd;
	extern Game::uiInfo_s* uiInfo;
	extern Game::WeaponDef_s** bg_weaponDefs;
	extern Game::weaponInfo_s* cg_weaponsArray;
	extern Game::WinMouseVars_t* s_wmv;
	extern Game::DxGlobals* dx;
	extern int* g_currentMenuType;

	extern Game::server_t* sv;
	extern Game::serverStatic_t* svs;

	extern int* g_waitingForKey;
	extern bool* isDvarSystemActive;
	extern int* level_initializing;

	typedef int(__cdecl* Cmd_RemoveCommand_t)(const char* name);
	extern Cmd_RemoveCommand_t Cmd_RemoveCommand;

	typedef void(*longjmp_internal_t)(jmp_buf env, int status);
	extern longjmp_internal_t longjmp_internal;

	static Utils::function<void()> R_BeginRemoteScreenUpdate = 0x5DC550;
	static Utils::function<void()> R_EndRemoteScreenUpdate = 0x5DC5A0;

	//-------------------------------------------------
	typedef void(*UpdateViewWeaponAnim_t)();
	extern UpdateViewWeaponAnim_t UpdateViewWeaponAnim;

	typedef int(__cdecl* SND_GetEntChannelFromName_t)(const char* channelName);
	extern SND_GetEntChannelFromName_t SND_GetEntChannelFromName;

	extern snd_alias_list_t* Com_FindSoundAlias(const char* name);

	static Utils::function<snd_alias_list_t* (const char* soundname)>
		Com_PickSoundAliasFromList_t = 0x581500;

	inline snd_alias_list_t* Com_PickSoundAliasFromList(const char* soundname) {
		return Com_PickSoundAliasFromList_t(soundname);
	}

	extern bool CL_IsCgameInitialized();

	extern Game::Font_s* R_RegisterFont(char const* fontName, int imageTrack);
	extern const char* SL_ConvertToString(unsigned int idx);

	typedef int(__cdecl* StringTable_LookupRowNumForValue_t)(const StringTable* table, int comparisonColumn, const char* value);
	extern StringTable_LookupRowNumForValue_t StringTable_LookupRowNumForValue;

	extern const char* StringTable_Lookup(const StringTable* table /*<ebp>*/, const int comparisonColumn /*<esi>*/, const char* value/*<eax>*/, const int valueColumn /*<edi>*/);
	extern void StringTable_GetAsset(const char* filename, const StringTable** tablePtr);

	extern bool Key_IsCatcherActive(int mask);

	extern const char* SEH_GetCurrentLanguage();

	typedef Game::gentity_s* (*GetPlayerEntity_t)(Game::scr_entref_t entref);
	extern GetPlayerEntity_t GetPlayerEntity;

	Game::ScreenPlacement* ScrPlace_GetFullPlacement();
	Game::ScreenPlacement* ScrPlace_GetUnsafeFullPlacement();
	void R_AddCmdDrawTextASM(const char* text, int max_chars, void* font, float x, float y, float x_scale, float y_scale, float rotation, const float* color, int style);
	void R_AddCmdDrawStretchPic(Game::Material* material, float x, float y, float w, float h, float null1, float null2, float null3, float null4, float* color); // ASM

	const char* g_he_font[];
	void RelocateFontArray();
	int getFontIndex(int fontIndex);

	Game::Font_s* UI_GetFontHandle(int fontEnum, const ScreenPlacement* scrPlace, float scale);
	int R_TextHeight(Font_s* font);
	long double R_NormalizedTextScale(Font_s* font, float scale);

	int UI_TextWidth(const char* text, int maxChars, Font_s* font, float scale);
	int UI_TextHeight(Font_s* font, float scale);
	int R_TextWidth/*eax*/(const char* text /*eax*/, signed int maxChars, Game::Font_s* font/*ecx*/);


	void ScrPlace_ApplyRect(const ScreenPlacement* ScrPlace, float* x, float* y, float* w, float* h, int horzAlign, int vertAlign);
	void UI_DrawText(const ScreenPlacement* ScrPlace, const char* text, int maxChars, Font_s* font, float ix, float iy, int horzAlign, int vertAlign, float scale, const float* color, int style);
	
	extern int* g_entities_int;

	extern DWORD* dword_FDBDCC;
	extern __int16* word_13E45E8;
	extern __int16* word_13E45DC;

	extern int* g_animRateOffsets;

	void SV_SendServerCommand(int clientNum, const char* fmt);
	void SV_GameSendServerCommand(int clientNum, const char* fmt);

	static Utils::function<void(Game::pmove_t* pm, Game::pml_t* pml)> PM_AirMove = 0x5BF480;
	static Utils::function<void(Game::pmove_t* pm)> PM_UpdateSprint = 0x5B72F0;


	extern char* byte_14C80E0;

	typedef void(__cdecl* CG_GameMessage_t)(const char* message, int a2);
	extern CG_GameMessage_t CG_GameMessage;

	const char* SEH_GetLanguageName/*eax*/(unsigned int iLanguage /*eax*/);
	void SEH_GetLanguageIndexFromName(const char* name/*edi*/, int* langindex); // Redone via ASM wrapper.
	const char* UI_SafeTranslateString/*eax*/(const char* reference /*eax*/);

	typedef const char*(__cdecl* UI_ReplaceConversionString_t)(const char* reference, const char* value);
	extern UI_ReplaceConversionString_t UI_ReplaceConversionString;

	int I_stricmp/*eax*/(int a1/*eax*/, const char* a2 /*edx*/, const char* a3);

	static Utils::function<unsigned int()> G_RegisterWeapon = 0x4B6140;
	static Utils::function<unsigned int(const char* weaponName, void* registerWeaponFunction)> BG_GetWeaponIndexForName = 0x5BECE0;

	typedef unsigned int(__cdecl* BG_FindWeaponIndexForName_t)(const char* weaponName);
	extern BG_FindWeaponIndexForName_t BG_FindWeaponIndexForName;

	extern Game::localization_t* localization;

	extern game_hudelem_s* g_hudelems;
	extern game_hudelem_field_t* fields_0;

	void FS_DisplayPath(int bLanguageCull);

	typedef void(__cdecl* FS_AddIwdFilesForGameDirectory_t)(const char* path, char* pszGameFolder);
	extern FS_AddIwdFilesForGameDirectory_t FS_AddIwdFilesForGameDirectory;

	void FS_AddLocalizedGameDirectory(const char* dir/*edi*/, const char* path);
	int FS_FOpenFileWriteToDir(const char* a1/*eax*/, const char* a2/*esi*/, int a3);

	void PM_Weapon_FireWeapon(Game::playerState_s* ps, int delayedAction);

	void StartWeaponAnim(int weaponIndex /*eax*/, Game::DObj_s* obj /*edi*/, int animIndex, float transitionTime);

	bool ShotLimitReached(Game::WeaponDef_s* weaponDef, Game::playerState_s* playerState);

	void PM_Weapon_BeginWeaponRaise(Game::playerState_s* playerState, int time, int anim, float aim, int altSwitch);

	static const char* g_he_font_old[] =
	{
		"default",		// HE_FONT_DEFAULT
		"bigfixed",		// HE_FONT_BIGFIXED
		"smallfixed",	// HE_FONT_SMALLFIXED
		"objective",	// HE_FONT_OBJECTIVE
		"big",			// HE_FONT_BIG
		"small",		// HE_FONT_SMALL
	};

	unsigned int G_GetWeaponIndexForName(const char* name);

	int DObjGetBoneIndex(const DObj_s* obj/*ecx*/, unsigned int name, unsigned __int8* index);
	int XModelGetBoneIndex(Game::XModel* model, unsigned int name, unsigned int offset, unsigned __int8* index);

	void ChangeViewmodelDobj(int weapIndex /*eax*/, unsigned __int8 weaponModel /*cl*/, Game::XModel* newHands, Game::XModel* newGoggles, Game::XModel* newRocket, Game::XModel* newKnife, char updateClientInfo);
	float Vec2Normalize(float* v);
	void Cbuf_InsertText(int localClientNum /*eax*/, const char* text);
	void UI_KeyEvent(int down /*edi*/, int localClientNum, int key);
	void Key_WriteBindings(int localClientNum /*eax*/, char* buffer);

	typedef int(*FS_Printf_t)(int file, const char* fmt, ...);
	extern FS_Printf_t FS_Printf;

	void CL_DrawStretchPic(const Game::ScreenPlacement* ScrPlace, float x, float y, float w, float h, int horzAlign, int vertAlign, float s1, float t1, float s2, float t2, float* color, Game::Material* material);

	typedef void(*UI_ReplaceConversions_t)(const char* sourceString, ConversionArguments* arguments, char* outputString);
	extern UI_ReplaceConversions_t UI_ReplaceConversions;

	int UI_GetActiveMenu();

	void UI_FilterStringForButtonAnimation(char* str, unsigned int strMaxSize);

	typedef void(*UI_SetActiveMenu_t)(int localClientNum, int menu);
	extern UI_SetActiveMenu_t UI_SetActiveMenu;

	void Key_SetBinding(int localClientNum /*eax*/, int keyNum /*ecx*/, const char* binding);

	void AimAssist_UpdateTweakables(const AimInput* input);
	void AimAssist_UpdateAdsLerp(const AimInput* input);
	typedef void(*AimAssist_ApplyAutoMelee_t)(const AimInput* input, const AimOutput* output);
	extern AimAssist_ApplyAutoMelee_t AimAssist_ApplyAutoMelee;
	void AimAssist_ApplyMeleeCharge(const AimInput* input /*eax*/, const AimOutput* output);

	extern Game::GraphFloat* aaInputGraph;

	float GraphGetValueFromFraction(int knotCount, const float(*knots)[2], float fraction);
	float GraphFloat_GetValue(const GraphFloat* graph, const float fraction);
	bool BG_PlayerEverHadWeapon(const Game::playerState_s* ps, int weaponIndex);
	bool BG_PlayerHasWeapon(const Game::playerState_s* playerState, int weaponIndex);
	int BG_WeaponAmmo(int weaponIndex /*eax*/, const Game::playerState_s* ps /*ecx*/);
	void vectoangles(const float* vec /*esi*/, float* angles /*edi*/);
	float AngleNormalize360(float angle);
	float AngleNormalize180(float angle);

	typedef float(*DiffTrackAngle_t)(float tgt, float cur, float rate, float deltaTime);
	extern DiffTrackAngle_t DiffTrackAngle;

	typedef float(*AngleSubtract_t)(const float a1, const float a2);
	extern AngleSubtract_t AngleSubtract;

	unsigned int SEH_DecodeLetter(char firstChar, char secondChar, int* usedCount);
	unsigned int SEH_ReadCharFromString(const char** text);
	Glyph* R_GetCharacterGlyph(Font_s* font, unsigned int letter);

	typedef XAnimTree_s*(*CG_CreateWeaponViewModelXAnim_t)(Game::WeaponDef_s* weaponDef);
	extern CG_CreateWeaponViewModelXAnim_t CG_CreateWeaponViewModelXAnim;

	void RB_DrawStretchPicRotate(Game::Material* material, float x, float y, float w, float h, float s0, float t0, float s1, float t1, float sinAngle, float cosAngle, unsigned int color);

	void Sys_SnapVector();
	Game::vec_t Vec3Normalize(Game::vec3_t v);

	typedef void(*SV_SetConfigstring_t)(int index, const char* string);
	extern SV_SetConfigstring_t SV_SetConfigstring;

	void CG_PlayBoltedEffect(int localClientNum/*eax*/, unsigned int boneName/*edx*/, int dobjHandle/*edi*/, const Game::FxEffectDef* fxDef);
	void FX_ThroughWithEffect(Game::FxEffectDef* fxDef /*esi*/);

	typedef void(*IN_RecenterMouse_t)();
	extern IN_RecenterMouse_t IN_RecenterMouse;

	typedef void(*IN_MouseMove_t)();
	extern IN_MouseMove_t IN_MouseMove;

	typedef void(*IN_Init_t)();
	extern IN_Init_t IN_Init;

	typedef void(*UI_MouseEvent_t)(int x, int y);
	extern UI_MouseEvent_t UI_MouseEvent;

	void UI_DrawHandlePic(const float* color /*ecx*/, Game::ScreenPlacement* ScrPlace /*edx*/, float x, float y, float w, float h, int horzAlign, int vertAlign, Game::Material* material);

	typedef int(__cdecl* RemoveRefToObject_t)(unsigned int id);
	extern RemoveRefToObject_t RemoveRefToObject;

	void SV_ShutdownGameVM(int clearScripts/*edi*/);
	void UI_DrawTextWithGlow(int vertAlign/*eax*/, int horzAlign/*ecx*/, const Game::ScreenPlacement* ScrPlace, const char* text, Game::Font_s* font, float x, float y, float scale, const float* color, int style, const float* glowColor, int subtitle, int cinematic);
	int String_Parse/*eax*/(const char** p/*eax*/, char* char_out, int len);

	int UI_GetKeyBindingLocalizedString/*eax*/(int localClientNum/*eax*/, const char* command/*ecx*/, char* keys, int bindNum);
	void DrawWrappedText(const rectDef_s* rect /*edi*/, Game::Font_s* font /*esi*/, const Game::ScreenPlacement* ScrPlace, const char* text, float x, float y, float scale, const float* color, int style, int textAlignMode, Game::rectDef_s* textRect, const float* glowColor, int subtitle, int cinematic);
	void CL_StopLogoOrCinematic(int localClientNum /*edi*/);

	bool R_Cinematic_IsStarted();
	Game::FxEffect* FX_SpawnOrientedEffect/*eax*/(int axis/*edx*/, int markEntnum/*ecx*/, const Game::FxEffectDef* def, int msecBegin, const float* origin);

	int CG_PlayClientSoundAlias/*eax*/(Game::snd_alias_list_t* aliasList/*eax*/, int localClientNum, float* origin);
	void DynEntCl_PlayEventFx(const float* origin/*edx*/, int axis/*ecx*/, const Game::FxEffectDef* def, int time);

	Game::centity_s* CG_GetEntity(int entityIndex);

	//rumble
	int CL_ControllerIndexFromClientNum();

	float CG_CornerDebugPrint/*<st0>*/(const float* color/*eax*/, const char* text/*ecx*/, const Game::ScreenPlacement* ScrPlace, float posX, float posY, float labelWidth, const char* label);

	typedef Game::DObj_s* (*Com_ClientDObjCreate_t)(Game::DObjModel_s* dobjModels, std::uint16_t numModels, Game::XAnimTree_s* tree, int handle);
	extern Com_ClientDObjCreate_t Com_ClientDObjCreate;

	void AmmoColor(Game::cg_s* cgameGlob/*edi*/, float* color/*esi*/, int weapIndex/*eax*/);
}