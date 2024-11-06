#include "STDInc.hpp"

namespace Components
{
	//Rename function in future
	void TextRenderer::UpdateGameFontsAndText()
	{
		Utils::Hook::Set<const char*>(0x42937B, "EXE_GAMESAVED");
		// Forced setup the stock fonts
		Utils::Hook::Set<const char*>(0x564783, "fonts/objectivefont");
		Utils::Hook::Set<const char*>(0x56475C, "fonts/extrabigfont");
		Utils::Hook::Set<const char*>(0x564736, "fonts/normalfont");
		// Changing the font in video cinematic from 'normalFont' to 'extraBigFont'.
		Utils::Hook::Set<DWORD>(0x56502B, 0x6ABEC4);
		Utils::Hook::Set<const char*>(0x56470F, "fonts/boldfont");
		Utils::Hook::Set<const char*>(0x5646C2, "fonts/smallfont");
		Utils::Hook::Set<const char*>(0x56469B, "fonts/bigfont");

		// DO NOT APPLYING CHANGES FOR ANY MODS!!!
		if (Game::HasLoadedMod())
			return;

		std::string language = Language::GetCurrentLanguage();
		if (language == "english" || language == "french" || language == "german" || language == "italian" || language == "spanish")
		{
			if (Dvars::console_menu_style->current.enabled)
			{
				Utils::Hook::Set<const char*>(0x42937B, "EXE_CHECKPOINT_REACHED");
				Utils::Hook::Set<const char*>(0x564783, "fonts/objectivefontxenon");
				Utils::Hook::Set<const char*>(0x56475C, "fonts/extrabigfontxenon");
				Utils::Hook::Set<const char*>(0x564736, "fonts/normalfontxenon");
				Utils::Hook::Set<const char*>(0x56502B, "fonts/normalfontxenon");
				Utils::Hook::Set<const char*>(0x56470F, "fonts/boldfontxenon");
				Utils::Hook::Set<const char*>(0x5646C2, "fonts/smallfontxenon");
				Utils::Hook::Set<const char*>(0x56469B, "fonts/bigfontxenon");
			}
		}
	}

	float TextRenderer::DrawHudIcon(const char* text, const Game::Font_s* font, const float x, const float y, const float sinAngle, const float cosAngle, const float xScale, const float yScale, Game::GfxColor color)
	{
		float s0, s1, t0, t1;

		if (*text == '\x01')
		{
			s0 = 0.0;
			t0 = 0.0;
			s1 = 1.0;
			t1 = 1.0;
		}
		else
		{
			s0 = 1.0;
			t0 = 0.0;
			s1 = 0.0;
			t1 = 1.0;
		}

		++text; //push to width

		if (*text == 0)
		{
			return 0.0f;
		}

		const auto v12 = font->pixelHeight * (*text - 16) + 16;
		const auto w = static_cast<float>((((v12 >> 24) & 0x1F) + v12) >> 5) * xScale;
		++text; //push to height

		if (*text == 0)
		{
			return 0.0f;
		}

		const auto h = static_cast<float>((font->pixelHeight * (*text - 16) + 16) >> 5) * yScale;
		++text; //push to nameLen

		if (*text == 0)
		{
			return 0.0f;
		}

		const auto materialNameLen = static_cast<uint8_t>(*text);
		++text; //push to materialName

		for (auto i = 0u; i < materialNameLen; i++)
		{
			if (text[i] == 0)
			{
				return 0.0f;
			}
		}

		const std::string materialName(text, materialNameLen);
		text += materialNameLen;

		auto* material = Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_MATERIAL, materialName.data()).material;
		if (material == nullptr || material->techniqueSet == nullptr || material->techniqueSet->name == nullptr || std::strcmp(material->techniqueSet->name, "2d") != 0)
		{
			material = Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_MATERIAL, "default").material;
		}

		const auto yy = y - (h + yScale * static_cast<float>(font->pixelHeight)) * 0.5f;

		Game::RB_DrawStretchPicRotate(material, x, yy, w, h, s0, t0, s1, t1, sinAngle, cosAngle, ColorRgba(255, 255, 255, color.array[3]));

		return w;
	}

	int TextRenderer::R_TextWidth_Hk(const char* text, int maxChars, Game::Font_s* font)
	{
		auto lineWidth = 0;
		auto maxWidth = 0;

		if (maxChars <= 0)
		{
			maxChars = std::numeric_limits<int>::max();
		}

		if (text == nullptr)
		{
			return 0;
		}

		auto count = 0;
		while (text && *text && count < maxChars)
		{
			const auto letter = Game::SEH_ReadCharFromString(&text);
			if (letter == '\r' || letter == '\n')
			{
				lineWidth = 0;
			}
			else
			{
				if (letter == '^' && text)
				{
					if (*text >= '0' && *text <= '9')
					{
						++text;
						continue;
					}

					if (*text >= '\x01' && *text <= '\x02' && text[1] != '\0' && text[2] != '\0' && text[3] != '\0')
					{
						const auto width = text[1];
						const auto materialNameLength = text[3];

						// This is how the game calculates width and height. Probably some 1 byte floating point number.
						// Details to be investigated if necessary.
						const auto v9 = font->pixelHeight * (width - 16) + 16;
						const auto w = ((((v9 >> 24) & 0x1F) + v9) >> 5);

						lineWidth += w;
						if (lineWidth > maxWidth)
						{
							maxWidth = lineWidth;
						}

						text += 4;
						for (auto currentLength = 0; currentLength < materialNameLength && *text; currentLength++)
						{
							++text;
						}
						continue;
					}
				}

				lineWidth += Game::R_GetCharacterGlyph(font, letter)->dx;
				if (lineWidth > maxWidth)
				{
					maxWidth = lineWidth;
				}

				++count;
			}
		}

		return maxWidth;
	}

	void TextRenderer::ButtonAnimation(char* text)
	{
		Game::UI_FilterStringForButtonAnimation(text, 1024);
	}

	const char* TextRenderer::CG_DrawPlayerWeaponLowAmmoKey(const char* pszInputBuffer, const char* pszMessageType, int errType)
	{
		char binding[256];
		if (!Game::UI_GetKeyBindingLocalizedString(0, "+usereload", binding, 0))
			strncpy_s(binding, sizeof(binding), "", _TRUNCATE);
		const char* displayString = Game::UI_SafeTranslateString(pszInputBuffer);
		const char* localizedString = Game::UI_ReplaceConversionString(displayString, binding);

		return localizedString;
	}

	__declspec(naked) void TextRenderer::CG_DrawPlayerWeaponLowAmmoWarningStub()
	{
		const static uint32_t jump_offset = 0x40A1F0;
		__asm
		{
			call	CG_DrawPlayerWeaponLowAmmoKey;
			jmp		jump_offset;
		}
	}

	__declspec(naked) void TextRenderer::R_TextWidth_Stub01()
	{
		const static uint32_t jump_offset = 0x56E618;
		__asm
		{
			push	eax;
			call	TextRenderer::R_TextWidth_Hk;
			add		esp, 4;
			jmp		jump_offset;
		}
	}

	__declspec(naked) void TextRenderer::R_TextWidth_Stub02()
	{
		const static uint32_t jump_offset = 0x564BB6;
		__asm
		{
			push	eax;
			call	TextRenderer::R_TextWidth_Hk;
			add		esp, 4;
			jmp		jump_offset;
		}
	}

	__declspec(naked) void TextRenderer::DrawHudIconStub()
	{
		const static uint32_t jump_offset = 0x5FE407;
		__asm
		{
			push	eax; //font
			push	ecx; //text
			call	TextRenderer::DrawHudIcon;
			add		esp, 0x8;
			jmp		jump_offset;
		}
	}

	__declspec(naked) void TextRenderer::UI_ReplaceConversionStringStub()
	{
		const static uint32_t UI_ReplaceConversions_func = 0x568240;
		const static uint32_t jump_offset = 0x568229;
		__asm
		{
			call	UI_ReplaceConversions_func;

			lea	 ecx, [esp + 34h];
			push	ecx;
			call	ButtonAnimation;
			add		esp, 4;

			jmp		jump_offset;
		}
	}

	__declspec(naked) void TextRenderer::UI_GetKeyBindingLocalizedStringStub()
	{
		const static uint32_t continue_function = 0x41AA92;
		const static uint32_t jump_end = 0x41AAF5;
		__asm
		{
			//stock op's
			add	 esp, 8;
			test	eax, eax;
			//mod
			jz		isNotKeyBinding;

			pushad;
			push	edi;
			call	ButtonAnimation;
			add		esp, 4;
			popad;

			jmp		jump_end;
		isNotKeyBinding:
			jmp		continue_function;
		}
	}


	void __declspec(naked) TextRenderer::UI_GetFontHandleStub1()
	{
		const static uint32_t retn_addr = 0x410E3A;
		__asm
		{
			push	ecx; //scrPlace
			push	eax; //fontEnum
			call	Game::UI_GetFontHandle;
			add		esp, 8;
			jmp		retn_addr;
		}
	}

	void __declspec(naked) TextRenderer::UI_GetFontHandleStub2()
	{
		const static uint32_t retn_addr = 0x41B649;
		__asm
		{
			push	ecx; //scrPlace
			push	eax; //fontEnum
			call	Game::UI_GetFontHandle;
			add		esp, 8;
			jmp		retn_addr;
		}
	}

	void __declspec(naked) TextRenderer::UI_GetFontHandleStub3()
	{
		const static uint32_t retn_addr = 0x43A21D;
		__asm
		{
			push	ecx; //scrPlace
			push	eax; //fontEnum
			call	Game::UI_GetFontHandle;
			add		esp, 8;
			jmp		retn_addr;
		}
	}

	void __declspec(naked) TextRenderer::UI_GetFontHandleStub4()
	{
		const static uint32_t retn_addr = 0x56DFAF;
		__asm
		{
			push	ecx; //scrPlace
			push	eax; //fontEnum
			call	Game::UI_GetFontHandle;
			add		esp, 8;
			jmp		retn_addr;
		}
	}

	void __declspec(naked) TextRenderer::UI_GetFontHandleStub5()
	{
		const static uint32_t retn_addr = 0x56E813;
		__asm
		{
			push	ecx; //scrPlace
			push	eax; //fontEnum
			call	Game::UI_GetFontHandle;
			add		esp, 8;
			jmp		retn_addr;
		}
	}

	void __declspec(naked) TextRenderer::UI_GetFontHandleStub6()
	{
		const static uint32_t retn_addr = 0x56ED1B;
		__asm
		{
			push	ecx; //scrPlace
			push	eax; //fontEnum
			call	Game::UI_GetFontHandle;
			add		esp, 8;
			jmp		retn_addr;
		}
	}

	void __declspec(naked) TextRenderer::UI_GetFontHandleStub7()
	{
		const static uint32_t retn_addr = 0x56EFCF;
		__asm
		{
			push	ecx; //scrPlace
			push	eax; //fontEnum
			call	Game::UI_GetFontHandle;
			add		esp, 8;
			jmp		retn_addr;
		}
	}

	void __declspec(naked) TextRenderer::UI_GetFontHandleStub8()
	{
		const static uint32_t retn_addr = 0x56F911;
		__asm
		{
			push	ecx; //scrPlace
			push	eax; //fontEnum
			call	Game::UI_GetFontHandle;
			add		esp, 8;
			jmp		retn_addr;
		}
	}

	void __declspec(naked) TextRenderer::UI_GetFontHandleStub9()
	{
		const static uint32_t retn_addr = 0x57056F;
		__asm
		{
			push	ecx; //scrPlace
			push	eax; //fontEnum
			call	Game::UI_GetFontHandle;
			add		esp, 8;
			jmp		retn_addr;
		}
	}

	void __declspec(naked) TextRenderer::UI_GetFontHandleStub10()
	{
		const static uint32_t retn_addr = 0x570805;
		__asm
		{
			push	ecx; //scrPlace
			push	eax; //fontEnum
			call	Game::UI_GetFontHandle;
			add		esp, 8;
			jmp		retn_addr;
		}
	}

	int TextRenderer::SubtitleCalculateWidth(int width)
	{
		if (Dvars::console_menu_style->current.enabled)
			return width - 100;
		return width;
	}

	void __declspec(naked) TextRenderer::CG_SubtitlePrintStub1()
	{
		const static uint32_t retn_addr = 0x420854;
		__asm
		{
			mov		eax, ds:7FDDD4h
			mov	 eax, [eax + 0Ch];

			push	eax;
			pushad;
			push	eax;
			call	SubtitleCalculateWidth;
			add		esp, 4;
			mov		[esp + 0x20], eax;
			popad
			pop eax

			mov		edx, eax;

			jmp		retn_addr;
		}
	}

	void __declspec(naked) TextRenderer::CG_SubtitlePrintStub2()
	{
		__asm
		{
			mov		edx, ds:7F777Ch
			mov	 edx, [edx + 0Ch];

			push	edx;
			pushad;
			push	edx;
			call	SubtitleCalculateWidth;
			add		esp, 4;
			mov		[esp + 0x20], edx;
			popad;
			pop		edx;

			retn;
		}
	}

	void TextRenderer::DrawClipAmmoMagazine(int weapIdx, float* base, Game::cg_s* cgameGlob, Game::WeaponDef_s* weapDef, float* color)
	{
		int width = Renderer::Width();
		int height = Renderer::Height();

		// Define reference resolution
		const int refWidth = 640;
		const int refHeight = 480;

		// Calculate scaling factor
		float scaleX = static_cast<float>(width) / refWidth;
		float scaleY = static_cast<float>(height) / refHeight;

		float bulletX = *base - 4.0f;
		float bulletY = base[1] - 4.0f;
		int clipCnt = cgameGlob->predictedPlayerState.ammoclip[Game::bg_weaponDefs[weapIdx]->iClipIndex];
		AmmoColor(cgameGlob, color, weapIdx);
		for (int clipIdx = 0; clipIdx < weapDef->iClipSize; ++clipIdx)
		{
			if (clipIdx == clipCnt)
			{
				*color = 0.30000001f;
				color[1] = 0.30000001f;
				color[2] = 0.30000001f;
			}
			float scaledBulletWidth = 4.0f * scaleX;
			float scaledBulletHeight = 8.0f * scaleY;
			Game::R_AddCmdDrawStretchPic(Game::cgMedia->ammoCounterBullet, bulletX, bulletY, scaledBulletWidth, scaledBulletHeight, 0.0f, 0.0f, 1.0f, 1.0f, color);
			bulletX -= 3.0f * scaleX;
		}
	}

	void TextRenderer::DrawClipAmmoShortMagazine(int weapIdx, float* base, Game::cg_s* cgameGlob, Game::WeaponDef_s* weapDef, float* color)
	{
		int width = Renderer::Width();
		int height = Renderer::Height();

		// Define reference resolution
		const int refWidth = 640;
		const int refHeight = 480;

		// Calculate scaling factor
		float scaleX = static_cast<float>(width) / refWidth;
		float scaleY = static_cast<float>(height) / refHeight;

		float bulletX = *base - 28.0f * scaleX;// 32.0f
		float bulletY = base[1] - 4.0f * scaleY;
		int clipCnt = cgameGlob->predictedPlayerState.ammoclip[Game::bg_weaponDefs[weapIdx]->iClipIndex];
		AmmoColor(cgameGlob, color, weapIdx);
		for (int clipIdx = 0; clipIdx < weapDef->iClipSize; ++clipIdx)
		{
			if (clipIdx == clipCnt)
			{
				*color = 0.30000001f;
				color[1] = 0.30000001f;
				color[2] = 0.30000001f;
			}
			float scaledBulletWidth = 28.0f * scaleX;// 32.0f
			float scaledBulletHeight = 8.0f * scaleY;
			Game::R_AddCmdDrawStretchPic(Game::cgMedia->ammoCounterRifleBullet, bulletX, bulletY, scaledBulletWidth, scaledBulletHeight, 0.0f, 0.0f, 1.0f, 1.0f, color);
			bulletX -= 38.0f * scaleX;// 40.0f
		}
	}

	void TextRenderer::DrawClipAmmoShotgunShells(int weapIdx, float* base, Game::cg_s* cgameGlob, Game::WeaponDef_s* weapDef, float* color)
	{
		int width = Renderer::Width();
		int height = Renderer::Height();

		// Define reference resolution
		const int refWidth = 640;
		const int refHeight = 480;

		// Calculate scaling factor
		float scaleX = static_cast<float>(width) / refWidth;
		float scaleY = static_cast<float>(height) / refHeight;

		float bulletX = *base - 16.0f * scaleX;
		float bulletY = base[1] - 4.0f * scaleY;
		int clipCnt = cgameGlob->predictedPlayerState.ammoclip[Game::bg_weaponDefs[weapIdx]->iClipIndex];
		AmmoColor(cgameGlob, color, weapIdx);
		for (int clipIdx = 0; clipIdx < weapDef->iClipSize; ++clipIdx)
		{
			if (clipIdx == clipCnt)
			{
				*color = 0.30000001f;
				color[1] = 0.30000001f;
				color[2] = 0.30000001f;
			}
			float scaledBulletWidth = 16.0f * scaleX;
			float scaledBulletHeight = 8.0f * scaleY;
			Game::R_AddCmdDrawStretchPic(Game::cgMedia->ammoCounterShotgunShell, bulletX, bulletY, scaledBulletWidth, scaledBulletHeight, 0.0f, 0.0f, 1.0f, 1.0f, color);
			bulletX -= 20.0f * scaleX;
		}
	}

	void TextRenderer::DrawClipAmmoRockets(int weapIdx, float* base, Game::cg_s* cgameGlob, Game::WeaponDef_s* weapDef, float* color)
	{
		int width = Renderer::Width();
		int height = Renderer::Height();

		// Define reference resolution
		const int refWidth = 640;
		const int refHeight = 480;

		// Calculate scaling factor
		float scaleX = static_cast<float>(width) / refWidth;
		float scaleY = static_cast<float>(height) / refHeight;

		float bulletX = *base - 55.0f * scaleX;// 64.0f
		float bulletY = base[1] - (13.0f * 0.5f) * scaleY;// 16.0f
		int clipCnt = cgameGlob->predictedPlayerState.ammoclip[Game::bg_weaponDefs[weapIdx]->iClipIndex];
		AmmoColor(cgameGlob, color, weapIdx);
		for (int clipIdx = 0; clipIdx < weapDef->iClipSize; ++clipIdx)
		{
			if (clipIdx == clipCnt)
			{
				*color = 0.30000001f;
				color[1] = 0.30000001f;
				color[2] = 0.30000001f;
			}
			float scaledBulletWidth = 55.0f * scaleX;// 64.0f
			float scaledBulletHeight = 13.0f * scaleY;// 16.0f
			Game::R_AddCmdDrawStretchPic(Game::cgMedia->ammoCounterRocket, bulletX, bulletY, scaledBulletWidth, scaledBulletHeight, 0.0f, 0.0f, 1.0f, 1.0f, color);
			bulletX -= 72.0f * scaleX;
		}
	}

	void TextRenderer::DrawClipAmmoBeltfed(float* color, int weapIdx, Game::cg_s* cgameGlob, float* base, Game::WeaponDef_s* weapDef)
	{
		// width 8.0
		// height 4.0

		int width = Renderer::Width();
		int height = Renderer::Height();

		// Define reference resolution
		const int refWidth = 640;
		const int refHeight = 480;

		// Calculate scaling factor
		float scaleX = static_cast<float>(width) / refWidth;
		float scaleY = static_cast<float>(height) / refHeight;

		float stepX = 8.0f * scaleX;
		float scaledBulletWidth = 8.0f * scaleX;
		float scaledBulletHeight = 4.0f * scaleY;

		float x = *base;
		float y = (weapDef->iClipSize / 20) * (4.0f * 0.25f * scaleY) + base[1];
		int clipCnt = cgameGlob->predictedPlayerState.ammoclip[Game::bg_weaponDefs[weapIdx]->iClipIndex];

		AmmoColor(cgameGlob, color, weapIdx);

		for (int clipIdx = 0; clipIdx < weapDef->iClipSize; ++clipIdx)
		{
			if (clipIdx == clipCnt)
			{
				*color = 0.30000001f;
				color[1] = 0.30000001f;
				color[2] = 0.30000001f;
			}

			if (!(clipIdx % 20)) // TEST_bullet_rowCnt
			{
				stepX = stepX * -1.0f;
				y += -2.0f * scaleY;
				x += stepX;
			}

			Game::R_AddCmdDrawStretchPic(Game::cgMedia->ammoCounterBeltBullet, x, y, scaledBulletWidth, scaledBulletHeight, 0.0f, 0.0f, 1.0f, 1.0f, color);
			x += stepX;
		}
	}


	void __declspec(naked) TextRenderer::DrawClipAmmoMagazineStub()
	{
		const static uint32_t retn_addr = 0x4092F0;
		__asm
		{
			push	ecx;
			push	eax;
			call	DrawClipAmmoMagazine;
			add		esp, 8;
			jmp		retn_addr;
		}
	}

	void __declspec(naked) TextRenderer::DrawClipAmmoShortMagazineStub()
	{
		const static uint32_t retn_addr = 0x409302;
		__asm
		{
			push	ecx;
			push	eax;
			call	DrawClipAmmoShortMagazine;
			add		esp, 8;
			jmp		retn_addr;
		}
	}

	void __declspec(naked) TextRenderer::DrawClipAmmoShotgunShellsStub()
	{
		const static uint32_t retn_addr = 0x409314;
		__asm
		{
			push	ecx;
			push	eax;
			call	DrawClipAmmoShotgunShells;
			add		esp, 8;
			jmp		retn_addr;
		}
	}

	void __declspec(naked) TextRenderer::DrawClipAmmoRocketsStub()
	{
		const static uint32_t retn_addr = 0x409336;
		__asm
		{
			push	ecx;
			push	eax;
			call	DrawClipAmmoRockets;
			add		esp, 8;
			jmp		retn_addr;
		}
	}

	void __declspec(naked) TextRenderer::DrawClipAmmoBeltfedStub()
	{
		const static uint32_t retn_addr = 0x409324;
		__asm
		{
			push	ecx;
			push	eax;
			call	DrawClipAmmoBeltfed;
			add		esp, 8;
			jmp		retn_addr;
		}
	}

	TextRenderer::TextRenderer()
	{
		Utils::Hook(0x56E613, R_TextWidth_Stub01, HOOK_JUMP).install()->quick();
		Utils::Hook(0x564BB1, R_TextWidth_Stub02, HOOK_JUMP).install()->quick();

		Utils::Hook::Set<BYTE>(0x5FE438, 0x12);
		Utils::Hook(0x5FE402, DrawHudIconStub, HOOK_JUMP).install()->quick();

		Utils::Hook(0x568224, UI_ReplaceConversionStringStub, HOOK_JUMP).install()->quick();
		// GetHudelemDirective
		Utils::Hook::Nop(0x41AA8B, 7);
		Utils::Hook(0x41AA8B, UI_GetKeyBindingLocalizedStringStub, HOOK_JUMP).install()->quick();

		// Converting a simple 'reload' text to a text with keybind localized
		Utils::Hook(0x40A1EB, CG_DrawPlayerWeaponLowAmmoWarningStub, HOOK_JUMP).install()->quick();

		// Replacing the original function on custom.
		Utils::Hook(0x410E35, UI_GetFontHandleStub1, HOOK_JUMP).install()->quick();
		Utils::Hook(0x41B644, UI_GetFontHandleStub2, HOOK_JUMP).install()->quick();
		Utils::Hook(0x43A218, UI_GetFontHandleStub3, HOOK_JUMP).install()->quick();
		Utils::Hook(0x56DFAA, UI_GetFontHandleStub4, HOOK_JUMP).install()->quick();
		Utils::Hook(0x56E80E, UI_GetFontHandleStub5, HOOK_JUMP).install()->quick();
		Utils::Hook(0x56ED16, UI_GetFontHandleStub6, HOOK_JUMP).install()->quick();
		Utils::Hook(0x56EFCA, UI_GetFontHandleStub7, HOOK_JUMP).install()->quick();
		Utils::Hook(0x56F90C, UI_GetFontHandleStub8, HOOK_JUMP).install()->quick();
		Utils::Hook(0x57056A, UI_GetFontHandleStub9, HOOK_JUMP).install()->quick();
		Utils::Hook(0x570800, UI_GetFontHandleStub10, HOOK_JUMP).install()->quick();

		Utils::Hook::Set<BYTE>(0x41B570, 0x7);
		Utils::Hook(0x41B579, Game::RelocateFontArray, HOOK_JUMP).install()->quick();
		Utils::Hook::Set<BYTE>(0x4B2EDC, 0x8);
		Utils::Hook::Set(0x4B2EE4, Game::g_he_font);
		Utils::Hook::Set(0x4B2F2E, Game::g_he_font);

		// Hooks the subtitle width
		Utils::Hook::Nop(0x420841, 8);
		Utils::Hook::Nop(0x42084B, 9);
		Utils::Hook(0x420841, CG_SubtitlePrintStub1, HOOK_JUMP).install()->quick();
		Utils::Hook(0x42084B, CG_SubtitlePrintStub2, HOOK_CALL).install()->quick();

		Utils::Hook(0x4092EB, DrawClipAmmoMagazineStub, HOOK_JUMP).install()->quick();
		Utils::Hook(0x4092FD, DrawClipAmmoShortMagazineStub, HOOK_JUMP).install()->quick();
		Utils::Hook(0x40930F, DrawClipAmmoShotgunShellsStub, HOOK_JUMP).install()->quick();
		Utils::Hook(0x409331, DrawClipAmmoRocketsStub, HOOK_JUMP).install()->quick();
		Utils::Hook(0x40931F, DrawClipAmmoBeltfedStub, HOOK_JUMP).install()->quick();
	}
}