#include "STDInc.hpp"

namespace Game
{
	//d3d9
	IDirect3DDevice9** dx9_device_ptr = reinterpret_cast<IDirect3DDevice9**>(0x1623F88);

	// Game data
	Game::AimAssistGlobals* aaGlobArray = reinterpret_cast<Game::AimAssistGlobals*>(0x6E1788);
	Game::WeaponDef_s** bg_weaponDefs = reinterpret_cast<Game::WeaponDef_s**>(0x14227A8);
	Game::cg_s* cgs = reinterpret_cast<Game::cg_s*>(0x6FA590);
	Game::cgMedia_t* cgMedia = reinterpret_cast<Game::cgMedia_t*>(0x729400);
	Game::cgs_t* cgsArray = reinterpret_cast<Game::cgs_t*>(0x6FA130);
	Game::centity_s* cg_entitiesArray = reinterpret_cast<Game::centity_s*>(0x72B6E8);
	Game::clientActive_t* clients = reinterpret_cast<Game::clientActive_t*>(0x85BD98);
	Game::clientUIActive_t* clientUIActive = reinterpret_cast<Game::clientUIActive_t*>(0x85BD70);
	Game::CinematicGlob* cinematicGlob = reinterpret_cast<Game::CinematicGlob*>(0x1621600);
	Game::clipMap_t* cm = reinterpret_cast<Game::clipMap_t*>(0xF788C8);
	Game::game_hudelem_field_t* fields_0 = reinterpret_cast<Game::game_hudelem_field_t*>(0x6814B8);
	Game::game_hudelem_s* g_hudelems = reinterpret_cast<Game::game_hudelem_s*>(0xC74818);
	Game::gclient_s* g_clients = reinterpret_cast<Game::gclient_s*>(0xE0DA00);
	Game::gentity_s* g_entities = reinterpret_cast<Game::gentity_s*>(0xC81418);
	Game::GfxDrawMethod* gfxDrawMethod = reinterpret_cast<Game::GfxDrawMethod*>(0x1D9E0A0);
	//Game::languageInfo_t* g_languages = reinterpret_cast<Game::languageInfo_t*>(0x6DD360);
	Game::keyname_t* keyNames = reinterpret_cast<Game::keyname_t*>(0x6DFB30);
	Game::keyname_t* localizedKeyNames = reinterpret_cast<Game::keyname_t*>(0x6DFE30);
	Game::GraphFloat* aaInputGraph = reinterpret_cast<Game::GraphFloat*>(0x6E25F0);
	Game::localization_t* localization = reinterpret_cast<Game::localization_t*>(0x13E0700);
	Game::PlayerKeyState* playerKeys = reinterpret_cast<Game::PlayerKeyState*>(0x816764);
	Game::playerState_s* ps = reinterpret_cast<Game::playerState_s*>(0x714BA8);
	Game::pmove_t* pmove = reinterpret_cast<Game::pmove_t*>(0x7FDE88);
	Game::ScreenPlacement* scrPlace = reinterpret_cast<Game::ScreenPlacement*>(0x8F8C68);
	Game::ScreenPlacement* scrPlaceFull = reinterpret_cast<Game::ScreenPlacement*>(0x8F8C20);
	Game::ScreenPlacement* scrPlaceFullUnsafe = reinterpret_cast<Game::ScreenPlacement*>(0x8F8CB0);
	Game::searchpath_s* fs_searchpaths = reinterpret_cast<Game::searchpath_s*>(0x1E209E4);
	Game::sharedUiInfo_t* sharedUiInfo = reinterpret_cast<Game::sharedUiInfo_t*>(0x129AD80);
	Game::snd_local_t* g_snd = reinterpret_cast<Game::snd_local_t*>(0x14C30E0);
	Game::uiInfo_s* uiInfo = reinterpret_cast<Game::uiInfo_s*>(0x1290F50);
	Game::weaponInfo_s* cg_weaponsArray = reinterpret_cast<Game::weaponInfo_s*>(0x6F7C88);
	Game::WinMouseVars_t* s_wmv = reinterpret_cast<Game::WinMouseVars_t*>(0x13E06F0);
	Game::DxGlobals* dx = reinterpret_cast<Game::DxGlobals*>(0x1623F84);
	int* g_currentMenuType = reinterpret_cast<int*>(0x1E209B4);

	Game::server_t* sv = reinterpret_cast<Game::server_t*>(0x14B10F8);
	Game::serverStatic_t* svs = reinterpret_cast<Game::serverStatic_t*>(0x14C2B98);

	longjmp_internal_t longjmp_internal = longjmp_internal_t(0x64285C);

	int* g_waitingForKey = reinterpret_cast<int*>(0x1E209C4);
	bool* isDvarSystemActive = reinterpret_cast<bool*>(0xE3AE5E);
	int* level_initializing = reinterpret_cast<int*>(0xE18E40);

	Cmd_RemoveCommand_t Cmd_RemoveCommand = (Cmd_RemoveCommand_t)0x530F80;

	bool CL_IsCgameInitialized()
	{
		return Game::clientUIActive->cgameInitialized ? true : false;
	}

	FS_AddIwdFilesForGameDirectory_t FS_AddIwdFilesForGameDirectory = FS_AddIwdFilesForGameDirectory_t(0x579FD0);

	snd_alias_list_t* __cdecl Com_FindSoundAlias_FastFile(const char* name)
	{
		XAssetHeader header;

		header = DB_FindXAssetHeader(ASSET_TYPE_SOUND, name);

		if (DB_IsXAssetDefault(ASSET_TYPE_SOUND, name))
		{
			Game::Com_Error(10, "Missing soundalias \"%s\".\n", name);
			return NULL;
		}
		return header.sound;
	}

	snd_alias_list_t* Com_FindSoundAlias(const char* name)
	{
		const auto com_useFastFiles = Dvars::Functions::Dvar_FindVar("useFastFile");

		if (com_useFastFiles && com_useFastFiles->current.enabled)
		{
			return Com_FindSoundAlias_FastFile(name);
		}
		return Com_PickSoundAliasFromList(name);
	}

	Game::Font_s* __cdecl R_RegisterFont_FastFile(const char* fontname)
	{
		Game::XAssetHeader header;

		header = Game::DB_FindXAssetHeader(ASSET_TYPE_FONT, fontname);

		if (Game::DB_IsXAssetDefault(ASSET_TYPE_FONT, fontname))
		{
			Game::Com_Error(10, "Missing font file \"%s\".\n", fontname);
			return 0;
		}
		return header.font;
	}

	Game::Font_s* R_RegisterFont(const char* fontName, int imageTrack)
	{
		return Game::R_RegisterFont_FastFile(fontName);
	}

	StringTable_LookupRowNumForValue_t StringTable_LookupRowNumForValue = (StringTable_LookupRowNumForValue_t)0x586010;

	const char* StringTable_Lookup(const StringTable* table /*<ebp>*/, const int comparisonColumn /*<esi>*/, const char* value/*<eax>*/, const int valueColumn /*<edi>*/)
	{
		int row;
		int result;
		
		if (!table)
		{
			Game::Com_Error(13, "Unable to find the lookup table in the fastfile, aborting lookup\n");
			return "";
		}
		row = Game::StringTable_LookupRowNumForValue(table, comparisonColumn, value);
		if (valueColumn >= table->columnCount)
			return "";
		if (row >= table->rowCount)
			return "";
		if (row < 0)
			return "";
		if (valueColumn < 0)
			return "";
		result = valueColumn + row * table->columnCount;
		if (!table->values[result])
			return "";
		return table->values[result];
	}

	void StringTable_GetAsset(const char* filename, const StringTable** tablePtr)
	{
		const Game::StringTable* stringTable = 0;
		const Game::StringTable* oldTable = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_STRINGTABLE, filename).stringTable;
		*tablePtr = oldTable;
		return;
	}

	const char* SL_ConvertToString(unsigned int idx)
	{
		struct stringList
		{
			int unk;
			char string[1];
			int unk2;
		};

		return (*reinterpret_cast<stringList**>(0xFDBBC4))[idx & 0xFFFF].string;
	}

	bool Key_IsCatcherActive(int mask)
	{
		return (mask & Game::clientUIActive->keyCatchers) != 0;
	}

	const char* SEH_GetCurrentLanguage()
	{
		int language = Dvars::Functions::Dvar_FindVar("loc_language")->current.integer;
		const char* language_char;

		switch (language)
		{
			case 0: return language_char = "english"; break;
			case 1: return language_char = "french"; break;
			case 2: return language_char = "german"; break;
			case 3: return language_char = "italian"; break;
			case 4: return language_char = "spanish"; break;
			case 5: return language_char = "british"; break;
			case 6: return language_char = "russian"; break;
			case 7: return language_char = "polish"; break;
			case 8: return language_char = "korean"; break;
			case 9: return language_char = "taiwanese"; break;
			case 10: return language_char = "japanese"; break;
			case 11: return language_char = "chinese"; break;
			case 12: return language_char = "thai"; break;
			case 13: return language_char = "leet"; break;
			case 14: return language_char = "czech"; break;
			case 15: return language_char = "portuguese"; break;
			case 16: return language_char = "slovak"; break;
			default: return "english"; break;
		}
	}

	GetPlayerEntity_t GetPlayerEntity = GetPlayerEntity_t(0x4C75E0);

	ScreenPlacement* ScrPlace_GetFullPlacement()
	{
		return scrPlaceFull;
	}

	ScreenPlacement* ScrPlace_GetUnsafeFullPlacement()
	{
		return scrPlaceFullUnsafe;
	}

	SND_GetEntChannelFromName_t SND_GetEntChannelFromName = SND_GetEntChannelFromName_t(0x5CA4D0);

	//xoxor is very madness man! Thanks for code!
	void R_AddCmdDrawTextASM(const char* text, int max_chars, void* font, float x, float y, float x_scale, float y_scale, float rotation, const float* color, int style)
	{
		const static uint32_t R_AddCmdDrawText_func = 0x5DB7B0;
		__asm
		{
			push	style;
			sub     esp, 14h;

			fld		rotation;
			fstp[esp + 10h];

			fld		y_scale;
			fstp[esp + 0Ch];

			fld		x_scale;
			fstp[esp + 8];

			fld		y;
			fstp[esp + 4];

			fld		x;
			fstp[esp];

			push	font;
			push	max_chars;
			push	text;
			mov		ecx, [color];

			call	R_AddCmdDrawText_func;
			add		esp, 24h;
		}
	}

	void R_AddCmdDrawStretchPic(Game::Material* material, float x, float y, float w, float h, float null1, float null2, float null3, float null4, float* color)
	{
		const static uint32_t R_AddCmdDrawStretchPic_func = 0x5DB2A0;
		__asm
		{
			pushad;
			push	color;
			mov		eax, [material];
			sub		esp, 20h;

			fld		null4;
			fstp[esp + 1Ch];

			fld		null3;
			fstp[esp + 18h];

			fld		null2;
			fstp[esp + 14h];

			fld		null1;
			fstp[esp + 10h];

			fld		h;
			fstp[esp + 0Ch];

			fld		w;
			fstp[esp + 8h];

			fld		y;
			fstp[esp + 4h];

			fld		x;
			fstp[esp];

			call	R_AddCmdDrawStretchPic_func;
			add		esp, 24h;
			popad;
		}
	}

	const char* g_he_font[Game::HE_FONT_COUNT] =
	{
		"default",		//HE_FONT_DEFAULT
		"bigfixed",		//HE_FONT_BIGFIXED
		"smallfixed",	//HE_FONT_SMALLFIXED
		"objective",	//HE_FONT_OBJECTIVE
		"big",			//HE_FONT_BIG
		"small",		//HE_FONT_SMALL
		"hudbig",		//HE_FONT_HUDBIG
		"hudsmall",		//HE_FONT_HUDSMALL
	};

	Game::Font_s* UI_GetFontHandle(int fontEnum, const ScreenPlacement* ScrPlace, float scale)
	{
		Game::Font_s* bigFont = Game::R_RegisterFont("fonts/bigFont", sizeof("fonts/bigFont"));
		Game::Font_s* boldFont = Game::R_RegisterFont("fonts/boldFont", sizeof("fonts/boldFont"));
		Game::Font_s* consoleFont = Game::R_RegisterFont("fonts/consoleFont", sizeof("fonts/consoleFont"));
		Game::Font_s* extraBigFont = Game::R_RegisterFont("fonts/extraBigFont", sizeof("fonts/extraBigFont"));
		Game::Font_s* normalFont = Game::R_RegisterFont("fonts/normalFont", sizeof("fonts/normalFont"));
		Game::Font_s* objectiveFont = Game::R_RegisterFont("fonts/objectiveFont", sizeof("fonts/objectiveFont"));
		Game::Font_s* smallFont = Game::R_RegisterFont("fonts/smallFont", sizeof("fonts/smallFont"));
		//from mw2
		Game::Font_s* hudbigfont = Game::R_RegisterFont("fonts/hudbigfont", sizeof("fonts/hudbigfont"));
		Game::Font_s* hudsmallfont = Game::R_RegisterFont("fonts/hudsmallfont", sizeof("fonts/hudsmallfont"));

		switch (fontEnum)
		{
		case 1:
			return normalFont;
		case 2:
			return bigFont;
		case 3:
			return smallFont;
		// case 4:
		// 	return boldFont;
		case 5:
			return consoleFont;
		case 6:
			return objectiveFont;
		case 7:
			return hudbigfont;
		case 8:
			return hudsmallfont;
		}

		float fontscalea = 0.0f;
		switch (fontEnum)
		{
		case 3:
			return Game::sharedUiInfo->assets.smallFont;
		case 5:
			return Game::sharedUiInfo->assets.consoleFont;
		case 6:
			return Game::sharedUiInfo->assets.objectiveFont;
		}

		if (Game::HasLoadedMod())
			fontscalea = fontscalea = scrPlace->scaleVirtualToReal[1] * scale;
		else
			fontscalea = (Dvars::console_menu_style->current.enabled) ? fontscalea = 1.5f * scale : fontscalea = scrPlace->scaleVirtualToReal[1] * scale;

		const auto ui_smallFont = Dvars::Functions::Dvar_FindVar("ui_smallFont");
		const auto ui_extraBigFont = Dvars::Functions::Dvar_FindVar("ui_extraBigFont");
		const auto ui_bigFont = Dvars::Functions::Dvar_FindVar("ui_bigFont");

		if (fontEnum == 4)
		{
			if (fontscalea >= ui_smallFont->current.value)
			{
				if (fontscalea >= ui_bigFont->current.value)
					return Game::sharedUiInfo->assets.boldFont;
				return Game::sharedUiInfo->assets.textFont;
			}
			return Game::sharedUiInfo->assets.smallFont;
		}

		if (Dvars::console_menu_style->current.enabled && !Game::HasLoadedMod())
		{
			if (fontscalea <= 0.563)
				return Game::sharedUiInfo->assets.smallFont;
			if (fontscalea >= 1.0)
				return Game::sharedUiInfo->assets.extraBigFont;
			if (fontscalea >= 0.688)
				return Game::sharedUiInfo->assets.bigFont;
			return Game::sharedUiInfo->assets.textFont;
		}
		else
		{
			if (fontscalea <= ui_smallFont->current.value)
				return Game::sharedUiInfo->assets.smallFont;
			if (fontscalea >= ui_extraBigFont->current.value)
				return Game::sharedUiInfo->assets.extraBigFont;
			if (fontscalea >= ui_bigFont->current.value)
				return Game::sharedUiInfo->assets.bigFont;
			return Game::sharedUiInfo->assets.textFont;
		}
	}

	int getFontIndex(int fontIndex)
	{	
		if (fontIndex == Game::HE_FONT_HUDBIG)
			return 7;
		else if (fontIndex == Game::HE_FONT_HUDSMALL)
			return 8;
		else
			return 0;
	}

	__declspec(naked) void RelocateFontArray()
	{
		__asm
		{
			mov		[esp + 3Ch], edi;

			ja		goToDefaultCase;
			je		customFonts;

			// execute switch statement code
			push	0x41B57F;
			retn;

		goToDefaultCase:
			push	0x41B575;
			retn;

		customFonts:
			push    ecx;

			pushad;
			push	eax;
			call    getFontIndex;
			add		esp, 4;
			mov		[esp + 20h], eax;
			popad;

			pop		eax;
			mov		ecx, eax;
			push	0x41B5B7; //scale 0.25 (fixed)
			retn;
		}
	}

	int R_TextHeight(Font_s* font)
	{
		return font->pixelHeight;
	}

	long double R_NormalizedTextScale(Font_s* font, float scale)
	{
		return (float)((float)(48.0 * scale) / (float)R_TextHeight(font));
	}

	int UI_TextWidth(const char* text, int maxChars, Font_s* font, float scale)
	{
		float normscale = static_cast<float>(R_NormalizedTextScale(font, scale));
		return (int)((float)R_TextWidth(text, maxChars, font) * normscale);
	}

	int UI_TextHeight(Font_s* font, float scale)
	{
		float normscale = static_cast<float>(R_NormalizedTextScale(font, scale));
		return (int)((float)R_TextHeight(font) * normscale);
	}

	void ScrPlace_ApplyRect(const ScreenPlacement* ScrPlace, float* x, float* y, float* w, float* h, int horzAlign, int vertAlign)
	{
		float v7;
		float v8;
		float v9;
		float v10;

		switch (horzAlign)
		{
		case 7:
			v7 = *x * ScrPlace->scaleVirtualToReal[0];
			v8 = (float)(ScrPlace->realViewableMin[0] + ScrPlace->realViewableMax[0]) * 0.5f;
			*x = v7 + v8;
			*w = *w * ScrPlace->scaleVirtualToReal[0];
			break;
		case 6:
			*x = *x * ScrPlace->scaleRealToVirtual[0];
			*w = *w * ScrPlace->scaleRealToVirtual[0];
			break;
		case 5:
			break;
		case 4:
			*x = *x * ScrPlace->scaleVirtualToFull[0];
			*w = *w * ScrPlace->scaleVirtualToFull[0];
			break;
		case 3:
			*x = (float)(*x * ScrPlace->scaleVirtualToReal[0]) + ScrPlace->realViewableMax[0];
			*w = *w * ScrPlace->scaleVirtualToReal[0];
			break;
		case 2:
			v7 = *x * ScrPlace->scaleVirtualToReal[0];
			v8 = 0.5f * ScrPlace->realViewportSize[0];
			*x = v7 + v8;
			*w = *w * ScrPlace->scaleVirtualToReal[0];
			break;
		case 1:
			*x = (float)(*x * ScrPlace->scaleVirtualToReal[0]) + ScrPlace->realViewableMin[0];
			*w = *w * ScrPlace->scaleVirtualToReal[0];
			break;
		default:
			*x = (float)(*x * ScrPlace->scaleVirtualToReal[0]) + ScrPlace->subScreenLeft;
			*w = *w * ScrPlace->scaleVirtualToReal[0];
			break;
		}

		switch (vertAlign)
		{
		case 7:
			v9 = *y * ScrPlace->scaleVirtualToReal[1];
			v10 = (float)(ScrPlace->realViewableMin[1] + ScrPlace->realViewableMax[1]) * 0.5f;
			*y = v9 + v10;
			*h = *h * ScrPlace->scaleVirtualToReal[1];
		case 6:
			*y = *y * ScrPlace->scaleRealToVirtual[1];
			*h = *h * ScrPlace->scaleRealToVirtual[1];
			break;
		case 5:
			return;
		case 4:
			*y = *y * ScrPlace->scaleVirtualToFull[1];
			*h = *h * ScrPlace->scaleVirtualToFull[1];
			break;
		case 3:
			*y = (float)(*y * ScrPlace->scaleVirtualToReal[1]) + ScrPlace->realViewableMax[1];
			*h = *h * ScrPlace->scaleVirtualToReal[1];
			break;
		case 2:
			v9 = *y * ScrPlace->scaleVirtualToReal[1];
			v10 = 0.5f * ScrPlace->realViewportSize[1];
			*y = v9 + v10;
			*h = *h * ScrPlace->scaleVirtualToReal[1];
			break;
		case 1:
			*y = (float)(*y * ScrPlace->scaleVirtualToReal[1]) + ScrPlace->realViewableMin[1];
			*h = *h * ScrPlace->scaleVirtualToReal[1];
			break;
		default:
			*y = *y * ScrPlace->scaleVirtualToReal[1];
			*h = *h * ScrPlace->scaleVirtualToReal[1];
			break;
		}
	}

	void UI_DrawText(const ScreenPlacement* ScrPlace, const char* text, int maxChars, Font_s* font, float ix, float iy, int horzAlign, int vertAlign, float scale, const float* color, int style)
	{
		float xScale = scale * 48.0f / static_cast<float>(font->pixelHeight);
		float yScale = xScale;

		ScrPlace_ApplyRect(ScrPlace, &ix, &iy, &xScale, &yScale, horzAlign, vertAlign);
		float x = floor(ix + 0.5f);
		float y = floor(iy + 0.5f);
		R_AddCmdDrawTextASM(text, maxChars, font, x, y, xScale, yScale, 0.0, color, style);
	}

	//Command which doesn't work in 1.5 version because __usercall
	//R_TextWidth_t R_TextWidth = R_TextWidth_t(0x5D5250);
	int R_TextWidth/*eax*/(const char* text /*eax*/, signed int maxChars, Game::Font_s* font/*ecx*/)
	{
		int result = 0;
		const static uint32_t R_TextWidth_func = 0x5D5250;
		__asm
		{
			push	font;
			push	maxChars;
			mov		eax, [text];
			call    R_TextWidth_func;
			add		esp, 0x8;
			mov		result, eax;
		}
		return result;
	}

	void SV_SendServerCommand(int clientNum, const char* fmt)
	{
		const static uint32_t SV_GameSendCommand_func = 0x5C7F00;
		__asm
		{
			mov		ecx, clientNum;
			push	fmt;
			call    SV_GameSendCommand_func;
			add     esp, 4;
		}
	}

	void SV_GameSendServerCommand(int clientNum, const char* fmt)
	{
		if (!Game::CL_IsCgameInitialized())
		{
			Game::Com_Printf(0, "Cannot activate this command while in the main menu.\n");
			return;
		}

		if (clientNum == -1)
			Game::SV_SendServerCommand(0, fmt);
	}

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

	CG_GameMessage_t CG_GameMessage = CG_GameMessage_t(0x41F560);

	const char* SEH_GetLanguageName/*eax*/(unsigned int iLanguage /*eax*/)
	{
		const static uint32_t SEH_GetLanguageName_func = 0x55C4D0;
		const char* result;
		__asm
		{
			mov     eax, iLanguage;
			call	SEH_GetLanguageName_func;
			mov		result, eax;
		}
		return result;
	}

	// Redone via ASM wrapper.
	void SEH_GetLanguageIndexFromName(const char* name/*edi*/, int* langindex)
	{
		const static uint32_t SEH_GetLanguageIndexFromName_func = 0x55C4F0;
		__asm
		{
			mov		edi, name;
			push	langindex;
			call	SEH_GetLanguageIndexFromName_func;
			add		esp, 4;
		}
	}

	const char* UI_SafeTranslateString/*eax*/(const char* reference /*eax*/)
	{
		const static uint32_t UI_SafeTranslateString_func = 0x5680C0;
		const char* result;
		__asm
		{
			mov     eax, reference;
			call	UI_SafeTranslateString_func;
			mov		result, eax;
		}
		return result;
	}

	UI_ReplaceConversionString_t UI_ReplaceConversionString = UI_ReplaceConversionString_t(0x5681D0);

	int I_stricmp/*eax*/(int a1/*eax*/, const char* a2 /*edx*/, const char* a3)
	{
		const static uint32_t I_stricmp_func = 0x58FFD0;
		int result;
		__asm
		{
			push	a3;
			mov		eax, a1;
			mov		edx, a2;
			call	I_stricmp_func;
			add		esp, 4;
			mov		result, eax;
		}
		return result;
	}

	BG_FindWeaponIndexForName_t BG_FindWeaponIndexForName = BG_FindWeaponIndexForName_t(0x5BEC90);

	void FS_DisplayPath(int bLanguageCull)
	{
		const static uint32_t FS_DisplayPath_func = 0x579C80;
		__asm
		{
			mov		eax, bLanguageCull;
			call	FS_DisplayPath_func;
		}
	}

	void FS_AddLocalizedGameDirectory(const char* dir/*edi*/, const char* path)
	{
		const static uint32_t FS_AddLocalizedGameDirectory_func = 0x57A6D0;
		__asm
		{
			push	path;
			mov		edi, dir;
			call	FS_AddLocalizedGameDirectory_func;
			add		esp, 4;
		}
	}

	int FS_FOpenFileWriteToDir(const char* a1/*eax*/, const char* a2/*esi*/, int a3)
	{
		const static uint32_t FS_FOpenFileWriteToDir_func = 0x577E30;
		int result;
		__asm
		{
			push	a3;
			mov		eax, a1;
			mov		esi, a2;
			call	FS_FOpenFileWriteToDir_func;
			add		esp, 4;
			mov		result, eax;
		}
		return result;
	}

	void PM_Weapon_FireWeapon(Game::playerState_s* playerState, int delayedAction)
	{
		const static uint32_t PM_Weapon_FireWeapon_func = 0x5C18D0;
		__asm
		{
			push	delayedAction;
			mov		eax, playerState;
			call	PM_Weapon_FireWeapon_func;
			add		esp, 4;
		}
	}

	bool ShotLimitReached(Game::WeaponDef_s* weaponDef, Game::playerState_s* playerState)
	{
		bool result;
		const static uint32_t ShotLimitReached_func = 0x5C1020;
		__asm
		{
			mov		eax, weaponDef;
			mov		ecx, ps;
			call	ShotLimitReached_func;
			mov		result, al;
		}
		return result;
	}

	void StartWeaponAnim(int weaponIndex /*eax*/, Game::DObj_s* obj /*edi*/, int animIndex, float transitionTime)
	{
		const static uint32_t StartWeaponAnim_func = 0x4307C0;
		__asm
		{
			pushad;
			mov		eax, weaponIndex;
			mov		edi, obj;
			push	transitionTime;
			push	animIndex;
			call	StartWeaponAnim_func;
			add		esp, 8;
			popad;
		}
	}

	void PM_Weapon_BeginWeaponRaise(Game::playerState_s* playerState, int time, int anim, float aim, int altSwitch)
	{
		const static uint32_t PM_Weapon_BeginWeaponRaise_func = 0x5C03C0;
		__asm
		{
			pushad;
			mov		eax, playerState;
			mov		edx, time;
			push	altSwitch;
			push	aim;
			push	anim;
			call	PM_Weapon_BeginWeaponRaise_func;
			add		esp, 0xC;
			popad;
		}
	}

	unsigned int G_GetWeaponIndexForName(const char* name)
	{
		if(level_initializing)
			return Game::BG_GetWeaponIndexForName(name, Game::G_RegisterWeapon);
		else
			return Game::BG_FindWeaponIndexForName(name);
	}

	void CL_DrawStretchPic(const Game::ScreenPlacement* ScrPlace, float x, float y, float w, float h, int horzAlign, int vertAlign, float s1, float t1, float s2, float t2, float* color, Game::Material* material)
	{
		ScrPlace_ApplyRect(ScrPlace, &x, &y, &w, &h, horzAlign, vertAlign);
		R_AddCmdDrawStretchPic(material, x, y, w, h, s1, t1, s2, t2, color);
	}

	int XModelGetBoneIndex(Game::XModel* model, unsigned int name, unsigned int offset, unsigned __int8* index)
	{
		unsigned int numBones;
		unsigned int localBoneIndex;
		unsigned __int16* boneNames;

		boneNames = model->boneNames;
		numBones = model->numBones;

		for (localBoneIndex = 0; ; ++localBoneIndex)
		{
			if (localBoneIndex >= numBones)
				return 0;
			if (name == boneNames[localBoneIndex])
				break;
		}
		*index = static_cast<unsigned char>(localBoneIndex + offset);
		return 1;
	}

	int DObjGetBoneIndex(const DObj_s* obj/*ecx*/, unsigned int name, unsigned __int8* index)
	{
		int result;
		const static uint32_t DObjGetBoneIndex_func = 0x59A090;
		__asm
		{
			push	index;
			push	name;
			mov		ecx, obj;
			call	DObjGetBoneIndex_func;
			add		esp, 8;
			mov		result, eax;
		}
		return result;
	}

	void ChangeViewmodelDobj(int weapIndex /*eax*/, unsigned __int8 weaponModel /*cl*/, Game::XModel* newHands, Game::XModel* newGoggles, Game::XModel* newRocket, Game::XModel* newKnife, char updateClientInfo)
	{
		const static uint32_t ChangeViewmodelDobj_func = 0x431400;
		__asm
		{
			pushad;
			mov		eax, weapIndex;
			mov		cl, weaponModel;
			push	1;
			push	newKnife;
			push	newRocket;
			push	newGoggles;
			push	newHands;
			call	ChangeViewmodelDobj_func;
			add		esp, 0x14;
			popad;
		}
	}

	float Vec2Normalize(float* v)
	{
		/*i'm not sure that this code return the correct result*/
		float result;
		const static uint32_t Vec2Normalize_func = 0x57BA10;
		__asm
		{
			mov		esi, [v];
			call	Vec2Normalize_func;
			fstp	[result];
		}

		return result;
	}

	void Cbuf_InsertText(int localClientNum /*eax*/, const char* text)
	{
		const static uint32_t Cbuf_InsertText_func = 0x5303E0;
		__asm
		{
			pushad;
			mov		eax, localClientNum;
			push	text;
			call	Cbuf_InsertText_func;
			add		esp, 4;
			popad;
		}
	}

	//(int a1@<ecx>, int a2@<edi>, int a3, int a4)
	void UI_KeyEvent(int down /*edi*/, int localClientNum, int key)
	{
		const static uint32_t UI_KeyEvent_func = 0x567C80;
		__asm
		{
			mov		edi, down;
			push	key;
			push	localClientNum;
			call	UI_KeyEvent_func;
			add		esp, 8;
		}
	}

	void Key_WriteBindings(int localClientNum /*eax*/, char* buffer)
	{
		const static uint32_t Key_WriteBindings_func = 0x443B00;
		__asm
		{
			pushad;
			mov		eax, localClientNum;
			push	buffer;
			call	Key_WriteBindings_func;
			add		esp, 4;
			popad;
		}
	}

	FS_Printf_t FS_Printf = (FS_Printf_t)0x578A90;
	UI_ReplaceConversions_t UI_ReplaceConversions = UI_ReplaceConversions_t(0x568240);

	int UI_GetActiveMenu()
	{
		return *(int*)(0x1E209B4);
	}

	void updateButtonString(char* str, unsigned int strMaxSize, const char* findStr, const char* replaceStr) {
		auto location = strstr(str, findStr);

		if (location) {
			std::size_t replaceOffset = location - str;
			std::size_t findStrLen = strlen(findStr);
			std::size_t replaceStrLen = strlen(replaceStr);

			if (replaceOffset + replaceStrLen < strMaxSize) {
				memmove(location + replaceStrLen,
					location + findStrLen,
					strMaxSize - replaceOffset - findStrLen);

				for (int i = 0; i < static_cast<int>(replaceStrLen); ++i) {
					location[i] = replaceStr[i];
				}
			}
		}
	}

	void UI_FilterStringForButtonAnimation(char* str, unsigned int strMaxSize)
	{
		const auto remainder = Game::Sys_MilliSeconds() % 1000;
		if (remainder <= 800)
		{
			return;
		}

		// Sony Icons
		if (std::strstr(str, "button_sony_09"))
		{
			updateButtonString(str, strMaxSize, "button_sony_09", "button_sony_17");
			if (std::strstr(str, "button_sony_10"))
				updateButtonString(str, strMaxSize, "button_sony_10", "button_sony_18");
		}
		else if (std::strstr(str, "button_sony_17"))
		{
			updateButtonString(str, strMaxSize, "button_sony_17", "button_sony_09");
			if (std::strstr(str, "button_sony_18"))
				updateButtonString(str, strMaxSize, "button_sony_18", "button_sony_10");
		}
		else if (std::strstr(str, "button_sony_10"))
		{
			updateButtonString(str, strMaxSize, "button_sony_10", "button_sony_18");
			if (std::strstr(str, "button_sony_10"))
				updateButtonString(str, strMaxSize, "button_sony_10", "button_sony_18");
		}
		else if (std::strstr(str, "button_sony_18"))
		{
			updateButtonString(str, strMaxSize, "button_sony_18", "button_sony_10");
			if (std::strstr(str, "button_sony_18"))
				updateButtonString(str, strMaxSize, "button_sony_18", "button_sony_10");
		}

		// Xbox Icons
		if (std::strstr(str, "button_xbox_09"))
		{
			updateButtonString(str, strMaxSize, "button_xbox_09", "button_xbox_17");
			if (std::strstr(str, "button_xbox_10"))
				updateButtonString(str, strMaxSize, "button_xbox_10", "button_xbox_18");
		}
		else if (std::strstr(str, "button_xbox_17"))
		{
			updateButtonString(str, strMaxSize, "button_xbox_17", "button_xbox_09");
			if (std::strstr(str, "button_xbox_18"))
				updateButtonString(str, strMaxSize, "button_xbox_18", "button_xbox_10");
		}
		else if (std::strstr(str, "button_xbox_10"))
		{
			updateButtonString(str, strMaxSize, "button_xbox_10", "button_xbox_18");
			if (std::strstr(str, "button_xbox_10"))
				updateButtonString(str, strMaxSize, "button_xbox_10", "button_xbox_18");
		}
		else if (std::strstr(str, "button_xbox_18"))
		{
			updateButtonString(str, strMaxSize, "button_xbox_18", "button_xbox_10");
			if (std::strstr(str, "button_xbox_18"))
				updateButtonString(str, strMaxSize, "button_xbox_18", "button_xbox_10");
		}
	}

	UI_SetActiveMenu_t UI_SetActiveMenu = UI_SetActiveMenu_t(0x567E00);

	void Key_SetBinding(int localClientNum /*eax*/, int keyNum /*ecx*/, const char* binding)
	{
		const static uint32_t Key_SetBinding_func = 0x4437B0;
		__asm
		{
			pushad;
			mov		ecx, keyNum;
			mov		eax, localClientNum;
			push	binding;
			call	Key_SetBinding_func;
			add		esp, 4;
			popad;
		}
	}

	void AimAssist_UpdateTweakables(const AimInput* input /*eax*/)
	{
		const static uint32_t AimAssist_UpdateTweakables_func = 0x401170;
		__asm
		{
			mov		eax, input;
			call	AimAssist_UpdateTweakables_func;
		}
	}

	void AimAssist_UpdateAdsLerp(const AimInput* input)
	{
		auto& aaGlob = Game::aaGlobArray[input->localClientNum];
		aaGlob.adsLerp = input->ps->fWeaponPosFrac;
		if ((input->ps->eFlags & 0x300) != 0 && (input->buttons & Game::CMD_BUTTON_ADS) != 0)
		{
			aaGlob.adsLerp = 1.0;
		}
	}

	AimAssist_ApplyAutoMelee_t AimAssist_ApplyAutoMelee = AimAssist_ApplyAutoMelee_t(0x402690);

	void AimAssist_ApplyMeleeCharge(const AimInput* input /*eax*/, const AimOutput* output)
	{
		const static uint32_t AimAssist_ApplyMeleeCharge_func = 0x402880;
		__asm
		{
			mov		eax, input;
			push	output;
			call	AimAssist_ApplyMeleeCharge_func;
			add		esp, 4;
		}
	}

	float GraphGetValueFromFraction(const int knotCount, const float(*knots)[2], const float fraction)
	{
		for (auto knotIndex = 1; knotIndex < knotCount; ++knotIndex)
		{
			if (knots[knotIndex][0] >= fraction)
			{
				const auto adjustedFraction = (fraction - knots[knotIndex - 1][0]) / (knots[knotIndex][0] - knots[knotIndex - 1][0]);

				return (knots[knotIndex][1] - knots[knotIndex - 1][1]) * adjustedFraction + knots[knotIndex - 1][1];
			}
		}

		return -1.0f;
	}

	float GraphFloat_GetValue(const GraphFloat* graph, const float fraction)
	{
		return GraphGetValueFromFraction(graph->knotCount, graph->knots, fraction) * graph->scale;
	}

	bool BG_PlayerEverHadWeapon(const Game::playerState_s* playerState, int weaponIndex)
	{
		return Com_BitCheckAssert(ps->weaponold, weaponIndex, 16);
	}

	bool BG_PlayerHasWeapon(const Game::playerState_s* playerState, int weaponIndex)
	{
		return Com_BitCheckAssert(ps->weapons, weaponIndex, 16);
	}

	int BG_WeaponAmmo(int weaponIndex /*eax*/, const Game::playerState_s* playerState /*ecx*/)
	{
		int result;
		const static uint32_t BG_WeaponAmmo_func = 0x5BFA70;
		__asm
		{
			mov		eax, weaponIndex;
			mov		ecx, playerState;
			call	BG_WeaponAmmo_func;
			mov		result, eax;
		}

		return result;
	}

	void vectoangles(const float* vec /*esi*/, float* angles /*edi*/)
	{
		const static uint32_t vectoangles_func = 0x57BFE0;
		__asm
		{
			mov		esi, [vec];
			mov		edi, [angles];
			call	vectoangles_func;
		}
	}

	float AngleNormalize360(float angle)
	{
		return (360.0f / 65536.0f) * ((int)(angle * (65536.0f / 360.0f)) & 65535);
	}

	float AngleNormalize180(float angle) {
		angle = AngleNormalize360(angle);
		if (angle > 180.0) {
			angle -= 360.0;
		}
		return angle;
	}

	DiffTrackAngle_t DiffTrackAngle = DiffTrackAngle_t(0x57B670);
	AngleSubtract_t AngleSubtract = AngleSubtract_t(0x57DCB0);

	bool sub_55C330(int a1, int a2) {
		return a1 - 176 <= 0x18 && a2 - 161 <= 0x5D;
	}

	bool Taiwanese_ValidBig5Code(uint16_t uiCode) {
		return ((((uiCode >> 8) >= 0xA1 && (uiCode >> 8) <= 0xC6) || (((uiCode >> 8) + 55) <= 0x30)) && (((uiCode & 0xFF) >= 0x40 && (uiCode & 0xFF) <= 0x7E) || ((uiCode & 0xFF) >= 0xA1 && (uiCode & 0xFF) != 0xFF)));
	}

	bool sub_55C380(uint16_t a1, uint16_t a2) {
		return ((a1 >= 0x81 && a1 <= 0x9F) || a1 - 224 <= 0xF) && ((a2 >= 0x40 && a2 <= 0x7E) || a2 - 128 <= 0x7C);
	}

	bool sub_55C3B0(uint16_t a1) {
		return (static_cast<uint8_t>(a1 >> 8) + 127) <= 0x7D && (static_cast<uint8_t>(a1) > 0x40) && (static_cast<uint8_t>(a1) != 0xFF);
	}

	unsigned int SEH_DecodeLetter(byte firstChar, byte secondChar, int* usedCount)
	{
		int g_currentAsian = *(int*)0x128F9AC;
		unsigned int v7;

		if (g_currentAsian)
		{
			switch (Dvars::Functions::Dvar_FindVar("loc_language")->current.integer)
			{
			case 8:
				if (sub_55C330(firstChar, secondChar)) 
				{
					*usedCount = 2;
					return secondChar + (firstChar << 8);
				}
				break;
			case 9:
				v7 = secondChar + (firstChar << 8);
				if (Taiwanese_ValidBig5Code(static_cast<uint16_t>(v7))) 
				{
					*usedCount = 2;
					return v7;
				}
				break;
			case 10:
				if (sub_55C380(static_cast<uint16_t>(firstChar), static_cast<uint16_t>(secondChar)))
				{
					*usedCount = 2;
					return secondChar + (firstChar << 8);
				}
				break;
			case 11:
				v7 = secondChar + (firstChar << 8);
				if (sub_55C3B0(static_cast<uint16_t>(v7)))
				{
					*usedCount = 2;
					return v7;
				}
				break;
			}
		}

		*usedCount = 1;
		return firstChar;
	}

	unsigned int SEH_ReadCharFromString(const char** text)
	{
		int usedCount;
		unsigned int letter;
		letter = Game::SEH_DecodeLetter(*(unsigned char*)*text, *((unsigned char*)*text + 1), &usedCount);
		*text += usedCount;
		return letter;
	}

	Glyph* R_GetCharacterGlyph(Font_s* font, unsigned int letter /*edi*/)
	{
		Game::Glyph* result;
		const static uint32_t R_GetCharacterGlyph_func = 0x5D50D0;
		__asm
		{
			mov		edi, letter;
			push	font;
			call	R_GetCharacterGlyph_func;
			add		esp, 4;
			mov		result, eax;
		}
		return result;
	}


	CG_CreateWeaponViewModelXAnim_t CG_CreateWeaponViewModelXAnim = CG_CreateWeaponViewModelXAnim_t(0x430EA0);

	__declspec(naked) void RB_DrawStretchPicRotate(Material* /*material*/, float /*x*/, float /*y*/, float /*w*/, float /*h*/, float /*s0*/, float /*t0*/, float /*s1*/, float /*t1*/, float /*sinAngle*/, float /*cosAngle*/, unsigned int /*color*/)
	{
		__asm
		{
			pushad

			mov eax, [esp + 0x4 + 0x20] // material
			push[esp + 0x30 + 0x20] // color
			push[esp + 0x30 + 0x20] // cosAngle
			push[esp + 0x30 + 0x20] // sinAngle
			push[esp + 0x30 + 0x20] // t1
			push[esp + 0x30 + 0x20] // s1
			push[esp + 0x30 + 0x20] // t0
			push[esp + 0x30 + 0x20] // s0
			push[esp + 0x30 + 0x20] // h
			push[esp + 0x30 + 0x20] // w
			push[esp + 0x30 + 0x20] // y
			push[esp + 0x30 + 0x20] // x
			mov ebx, 0x5FB310
			call ebx
			add esp, 0x2C

			popad
			ret
		}
	}

	void Sys_SnapVector()
	{
		Game::g_clients->ps.velocity[0] = rint(Game::g_clients->ps.velocity[0]);
		Game::g_clients->ps.velocity[1] = rint(Game::g_clients->ps.velocity[1]);
		Game::g_clients->ps.velocity[2] = rint(Game::g_clients->ps.velocity[2]);
	}

	Game::vec_t Vec3Normalize(Game::vec3_t v) 
	{
		float length, ilength;

		length = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
		length = sqrt(length);

		if (length) {
			ilength = 1 / length;
			v[0] *= ilength;
			v[1] *= ilength;
			v[2] *= ilength;
		}

		return length;
	}

	SV_SetConfigstring_t SV_SetConfigstring = SV_SetConfigstring_t(0x5C6780);

	void CG_PlayBoltedEffect(int localClientNum/*eax*/, unsigned int boneName/*edx*/, int dobjHandle/*edi*/, const Game::FxEffectDef* fxDef)
	{
		const static uint32_t CG_PlayBoltedEffect_func = 0x418690;
		__asm
		{
			mov		eax, localClientNum;
			mov		edx, boneName;
			mov		edi, dobjHandle;
			push	fxDef;
			call	CG_PlayBoltedEffect_func;
			add		esp, 4;
		}
	}

	void FX_ThroughWithEffect(Game::FxEffectDef* fxDef /*esi*/)
	{
		const static uint32_t FX_ThroughWithEffect_func = 0x4721F0;
		__asm
		{
			mov		esi, fxDef;
			call	FX_ThroughWithEffect_func;
		}
	}


	IN_RecenterMouse_t IN_RecenterMouse = IN_RecenterMouse_t(0x594570);
	IN_MouseMove_t IN_MouseMove = IN_MouseMove_t(0x594730);
	IN_Init_t IN_Init = IN_Init_t(0x5947F0);
	UI_MouseEvent_t UI_MouseEvent = UI_MouseEvent_t(0x567D30);

	void UI_DrawHandlePic(const float* color /*ecx*/, Game::ScreenPlacement* ScrPlace /*edx*/, float x, float y, float w, float h, int horzAlign, int vertAlign, Game::Material* material)
	{
		const static uint32_t UI_DrawHandlePic_func = 0x55F3D0;
		__asm
		{
			pushad;
			push	material;
			push	vertAlign;
			push	horzAlign;

			sub     esp, 10h;

			fld		h;
			fstp	[esp + 0Ch];

			fld		w;
			fstp	[esp + 8h];

			fld		y;
			fstp	[esp + 4h];

			fld		x;
			fstp	[esp];

			mov		ecx, [color];
			mov		edx, [ScrPlace];

			call	UI_DrawHandlePic_func;
			add		esp, 1Ch;
			popad;
		}
	}

	RemoveRefToObject_t RemoveRefToObject = RemoveRefToObject_t(0x54EB90);

	void SV_ShutdownGameVM(int clearScripts/*edi*/)
	{
		const uint32_t SV_ShutdownGameVM_func = 0x5C66A0;
		__asm
		{
			pushad;
			mov		edi, clearScripts;
			call	SV_ShutdownGameVM_func;
			popad;
		}
	}

	void UI_DrawTextWithGlow(int vertAlign/*eax*/, int horzAlign/*ecx*/, const Game::ScreenPlacement* ScrPlace, const char* text, Game::Font_s* font, float x, float y, float scale, const float* color, int style, const float* glowColor, int subtitle, int cinematic)
	{
		const static uint32_t UI_DrawTextWithGlow_func = 0x564D20;
		__asm
		{
			pushad;
			push	cinematic;
			push	subtitle;
			push	glowColor;
			push	style;
			push	color;

			sub     esp, 0Ch;

			fld		scale;
			fstp	[esp + 8h];

			fld		y;
			fstp	[esp + 4h];

			fld		x;
			fstp	[esp];

			push	font;
			push	text
			push	ScrPlace;

			mov		eax, [vertAlign];
			mov		ecx, [horzAlign];

			call	UI_DrawTextWithGlow_func;
			add		esp, 2Ch;

			popad;
		}
	}

	int String_Parse/*eax*/(const char** p/*eax*/, char* char_out, int len)
	{
		int result;
		const uint32_t String_Parse_func = 0x5C66A0;
		__asm
		{
			pushad;
			push	len;
			push	char_out;
			mov		eax, [p];
			call	String_Parse_func;
			add		esp, 8;
			mov		result, eax;
			popad;
		}
		return result;
	}

	int UI_GetKeyBindingLocalizedString/*eax*/(int localClientNum/*eax*/, const char* command/*ecx*/, char* keys, int bindNum)
	{
		int result;
		const uint32_t UI_GetKeyBindingLocalizedString_func = 0x56F1E0;
		__asm
		{
			pushad;
			mov		eax, [localClientNum];
			mov		ecx, [command];
			push	bindNum;
			push	keys;
			call	UI_GetKeyBindingLocalizedString_func;
			add		esp, 8;
			mov		result, eax;
			popad;
		}
		return result;
	}

	void DrawWrappedText(const rectDef_s* rect /*edi*/, Game::Font_s* font /*esi*/, const Game::ScreenPlacement* ScrPlace, const char* text, float x, float y, float scale, const float* color, int style, int textAlignMode, Game::rectDef_s* textRect, const float* glowColor, int subtitle, int cinematic)
	{
		const uint32_t DrawWrappedText_func = 0x56E440;
		__asm
		{
			pushad;
			push	cinematic;
			push	subtitle;
			push	glowColor;
			push	textRect;
			push	textAlignMode;
			push	style;
			push	color; //28

			sub     esp, 0Ch;

			fld		scale;
			fstp	[esp + 8h];

			fld		y;
			fstp	[esp + 4h];

			fld		x;
			fstp	[esp];

			push	text
			push	ScrPlace;

			mov		edi, [rect];
			mov		esi, [font];

			call	DrawWrappedText_func;
			add		esp, 30h;

			popad;
		}
	}

	void CL_StopLogoOrCinematic(int localClientNum /*edi*/)
	{
		const uint32_t CL_StopLogoOrCinematic_func = 0x445CE0;
		__asm
		{
			pushad;
			mov		edi, localClientNum;
			call	CL_StopLogoOrCinematic_func;
			popad;
		}
	}

	bool R_Cinematic_IsStarted()
	{
		return !Game::cinematicGlob->cinematicFinished && Game::cinematicGlob->currentCinematicName[0];
	}

	Game::FxEffect* FX_SpawnOrientedEffect/*eax*/(int axis/*edx*/, int markEntnum/*ecx*/, const Game::FxEffectDef* def, int msecBegin, const float* origin)
	{
		Game::FxEffect* result;
		const uint32_t FX_SpawnOrientedEffect_func = 0x471E30;
		__asm
		{
			mov		edx, [axis];
			mov		ecx, markEntnum;
			push	[origin];
			push	msecBegin;
			push	def;
			call	FX_SpawnOrientedEffect_func;
			add		esp, 0xC;
			mov		result, eax;
		}
		return result;
	}

	int CG_PlayClientSoundAlias/*eax*/(Game::snd_alias_list_t* aliasList/*eax*/, int localClientNum, float* origin)
	{
		int result;
		const uint32_t CG_PlayClientSoundAlias_func = 0x4208A0;
		__asm
		{
			mov		eax, aliasList;
			push	[origin];
			push	localClientNum;
			call	CG_PlayClientSoundAlias_func;
			add		esp, 8;
			mov		result, eax;
		}
		return result;
	}

	void DynEntCl_PlayEventFx(const float* origin/*edx*/, int axis/*ecx*/, const Game::FxEffectDef* def, int time)
	{
		const uint32_t DynEntCl_PlayEventFx_func = 0x471EA0;
		__asm
		{
			mov		edx, [origin];
			mov		ecx, [axis];
			push	time;
			push	def;
			call	DynEntCl_PlayEventFx_func;
			add		esp, 8;
		}
	}

	Game::centity_s* CG_GetEntity(int entityIndex)
	{
		return &Game::cg_entitiesArray[384 * entityIndex];
	}

	int CL_ControllerIndexFromClientNum()
	{
		return 0;
	}

	float CG_CornerDebugPrint/*<st0>*/(const float* color/*eax*/, const char* text/*ecx*/, const Game::ScreenPlacement* ScrPlace, float posX, float posY, float labelWidth, const char* label)
	{
		float result;
		const uint32_t CG_CornerDebugPrint_func = 0x4113A0;
		__asm
		{
			push	label;

			sub     esp, 0Ch;

			fld		labelWidth;
			fstp	[esp + 8h];

			fld		posY;
			fstp	[esp + 4h];

			fld		posX;
			fstp	[esp];

			push	ScrPlace;

			mov		eax, [color];
			mov		ecx, [text];

			call	CG_CornerDebugPrint_func;
			add		esp, 14h;
			fstp    dword ptr[result];
		}
		return result;
	}

	Com_ClientDObjCreate_t Com_ClientDObjCreate = Com_ClientDObjCreate_t(0x535F30);


	void AmmoColor(cg_s* cgameGlob/*edi*/, float* color/*esi*/, int weapIndex/*eax*/)
	{
		const uint32_t AmmoColor_func = 0x408CA0;
		__asm
		{
			pushad;
			mov		edi, [cgameGlob];
			mov		esi, [color];
			mov		eax, weapIndex;
			call	AmmoColor_func;
			popad;
		}
	}
}