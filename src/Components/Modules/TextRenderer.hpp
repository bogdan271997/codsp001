#pragma once

namespace Components
{
	constexpr unsigned int ColorRgba(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
	{
		return (r) | (g << 8) | (b << 16) | (a << 24);
	}

	constexpr unsigned int ColorRgb(const uint8_t r, const uint8_t g, const uint8_t b)
	{
		return ColorRgba(r, g, b, 0xFF);
	}

	class TextRenderer : public Component
	{
	public:
		TextRenderer();

		static void UpdateGameFontsAndText();
	private:
		static void R_TextWidth_Stub01();
		static void R_TextWidth_Stub02();
		static void DrawHudIconStub();
		static void UI_ReplaceConversionStringStub();
		static void ButtonAnimation(char* text);
		static void CG_DrawPlayerWeaponLowAmmoWarningStub();
		static const char* CG_DrawPlayerWeaponLowAmmoKey(const char* pszInputBuffer, const char* pszMessageType, int errType);
		static void UI_GetKeyBindingLocalizedStringStub();
		static int SubtitleCalculateWidth(int width);

		static void UI_GetFontHandleStub1();
		static void UI_GetFontHandleStub2();
		static void UI_GetFontHandleStub3();
		static void UI_GetFontHandleStub4();
		static void UI_GetFontHandleStub5();
		static void UI_GetFontHandleStub6();
		static void UI_GetFontHandleStub7();
		static void UI_GetFontHandleStub8();
		static void UI_GetFontHandleStub9();
		static void UI_GetFontHandleStub10();

		static void CG_SubtitlePrintStub1();
		static void CG_SubtitlePrintStub2();

		static int R_TextWidth_Hk(const char* text, int maxChars, Game::Font_s* font);
		static float DrawHudIcon(const char* text, const Game::Font_s* font, const float x, const float y, const float sinAngle, const float cosAngle, const float xScale, const float yScale, Game::GfxColor color);

		static void DrawClipAmmoMagazineStub();
		static void DrawClipAmmoShortMagazineStub();
		static void DrawClipAmmoShotgunShellsStub();
		static void DrawClipAmmoRocketsStub();
		static void DrawClipAmmoBeltfedStub();
		static void DrawClipAmmoMagazine(int weapIdx, float* base, Game::cg_s* cgameGlob, Game::WeaponDef_s* weapDef, float* color);
		static void DrawClipAmmoShortMagazine(int weapIdx, float* base, Game::cg_s* cgameGlob, Game::WeaponDef_s* weapDef, float* color);
		static void DrawClipAmmoShotgunShells(int weapIdx, float* base, Game::cg_s* cgameGlob, Game::WeaponDef_s* weapDef, float* color);
		static void DrawClipAmmoRockets(int weapIdx, float* base, Game::cg_s* cgameGlob, Game::WeaponDef_s* weapDef, float* color);
		static void DrawClipAmmoBeltfed(float* color, int weapIdx, Game::cg_s* cgameGlob, float* base, Game::WeaponDef_s* weapDef);
	};
}