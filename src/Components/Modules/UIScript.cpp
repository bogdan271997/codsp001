#include "STDInc.hpp"

namespace Components
{
	std::unordered_map<std::string, UIScript::UIScriptHandler> UIScript::UIScripts;
	std::unordered_map<int, UIScript::UIOwnerDrawHandler> UIScript::UIOwnerDraws;

	template<> int UIScript::Token::get() const
	{
		if (this->isValid())
		{
			return std::strtol(this->token, nullptr, 0);
		}

		return 0;
	}

	template<> const char* UIScript::Token::get() const
	{
		if (this->isValid())
		{
			return this->token;
		}

		return "";
	}

	template<> std::string UIScript::Token::get() const
	{
		return { this->get<const char*>() };
	}

	bool UIScript::Token::isValid() const
	{
		return (this->token && this->token[0]);
	}

	void UIScript::Token::parse(const char** args)
	{
		if (args)
		{
			this->token = Game::Com_Parse(args);
		}
	}

	Game::uiInfo_s* UIScript::UI_GetInfo()
	{
		return Game::uiInfo;
	}

	void UIScript::Add(const std::string& name, const UIScriptHandler& callback)
	{
		UIScripts[name] = callback;
	}

	void UIScript::AddOwnerDraw(int ownerdraw, const UIOwnerDrawHandler& callback)
	{
		UIOwnerDraws[ownerdraw] = callback;
	}

	bool UIScript::RunMenuScript(const char* name, const char** args)
	{
		if (const auto itr = UIScripts.find(name); itr != UIScripts.end())
		{
			const auto* info = UIScript::UI_GetInfo();
			itr->second(Token(args), info);
			return true;
		}

		return false;
	}

	void UIScript::OwnerDrawInit(int ownerDraw, float x, float y, float w, float h, int horzAlign, int vertAlign, float text_x, float text_y, float scale, Game::Font_s* font, const float* color,
		Game::Material* material, int textStyle, int textAlignMode)
	{
		for (auto i = UIOwnerDraws.begin(); i != UIOwnerDraws.end(); ++i)
		{
			if (i->first == ownerDraw)
			{
				i->second(x, y, w, h, horzAlign, vertAlign, text_x, text_y, scale, font, color, material, textStyle, textAlignMode);
			}	
		}
	}

	__declspec(naked) void UIScript::OwnerDrawStub()
	{
		const static uint32_t stock_scripts_addr = 0x56590E;

		__asm
		{
			pushad;
			push	[esp + 7Ch];			//textAlignMode
			push	[esp + 7Ch];			//textStyle
			push	[esp + 7Ch];			//material
			push	ebp;					//color
			push	[esp + 78h];			//font
			push	[esp + 80h];			//scale
			push	[esp + 7Ch];			//text_y
			push	[esp + 7Ch];			//text_x
			push    edi;					//vertAlign
			push    ebx;					//horzAlign
			push	[esp + 80h];			//h
			push	[esp + 80h];			//w
			push	[esp + 80h];			//y
			push	[esp + 80h];			//x
			push	esi;					//ownerDraw
			call	UIScript::OwnerDrawInit;
			add		esp, 0x3C;
			popad;

			//original function
			sub     esi, 0FAh;
			jmp		stock_scripts_addr;
		}
	}

	__declspec(naked) void UIScript::UI_RunMenuScript_stub()
	{
		const static char* overwritten_str = "clearError";
		const static uint32_t stock_scripts_addr = 0x566CC9;  // next op after hook spot
		const static uint32_t if_addon_return_addr = 0x5676CE; // jump to the valid return point if we had a valid match in addons
		__asm
		{
			lea     edx, [esp + 30h];

			pushad;
			push	esi;					//token
			push	edx;					//uiscript_name
			call	RunMenuScript;
			add		esp, 8;
			test	al, al;
			je		STOCK_FUNC;				// jump if UI_uiScriptsAddons was false
			popad;

			jmp		if_addon_return_addr;	// return to skip the stock function

		STOCK_FUNC:
			popad;
			push	overwritten_str;		// the original push we hooked at
			jmp		stock_scripts_addr;		// jump back and exec the original function
		}
	}

	UIScript::UIScript()
	{
		// UI_LOGGEDINUSER
		UIScript::AddOwnerDraw(272, []([[maybe_unused]] float x, [[maybe_unused]] float y, [[maybe_unused]] float w, [[maybe_unused]] float h, 
			[[maybe_unused]] int horzAlign, [[maybe_unused]] int vertAlign, [[maybe_unused]] float text_x, [[maybe_unused]] float text_y, [[maybe_unused]] float scale,
			[[maybe_unused]] Game::Font_s* font, [[maybe_unused]] const float* color, [[maybe_unused]] Game::Material* material, [[maybe_unused]] int textStyle,
			[[maybe_unused]] int textAlignMode)
		{			
			auto* const scrPlace = Game::ScrPlace_GetFullPlacement();
			std::string profileName = Utils::IO::ReadFile("players/profiles/active.txt");
			const char* displayString = Game::UI_SafeTranslateString("MODERN_OPS_LOC_MENU_SIGNEDINAS");
			const char* localizedString = Game::UI_ReplaceConversionString(displayString, profileName.data());
			Game::UI_DrawText(scrPlace, localizedString, 64, font, x, y, horzAlign, vertAlign, scale, color, textStyle);
		});

		// UI_GPAD_BUTTONS_SCHEME
		UIScript::AddOwnerDraw(198, []([[maybe_unused]] float x, [[maybe_unused]] float y, [[maybe_unused]] float w, [[maybe_unused]] float h, 
			[[maybe_unused]] int horzAlign, [[maybe_unused]] int vertAlign, [[maybe_unused]] float text_x, [[maybe_unused]] float text_y, [[maybe_unused]] float scale,
			[[maybe_unused]] Game::Font_s* font, [[maybe_unused]] const float* color, [[maybe_unused]] Game::Material* material, [[maybe_unused]] int textStyle,
			[[maybe_unused]] int textAlignMode)
		{
			Gamepad::GetButtonLayout();

			auto* const scrPlace = Game::ScrPlace_GetFullPlacement();
			auto fontHandle = Game::UI_GetFontHandle(0, scrPlace, 0.3750f);

			if (Dvars::gpad_style->current.enabled)
			{
				//SONY
				Game::rectDef_s textRect{};
				Game::rectDef_s rect{};
				rect.x = 462.0f;
				rect.y = 375.0f;
				rect.w = 200.0f;
				rect.horzAlign = 0;
				rect.vertAlign = 0;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonA), rect.x, rect.y + 20.0f, scale, color, textStyle, textAlignMode, &textRect, 0, 0, 0);
				rect.x = 452.0f; rect.y = 330.0f;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonB), rect.x, rect.y + 20.0f, scale, color, textStyle, textAlignMode, &textRect, 0, 0, 0);
				rect.x = 459.0f; rect.y = 350.0f;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonX), rect.x, rect.y + 24.0f, scale, color, textStyle, textAlignMode, &textRect, 0, 0, 0);
				rect.x = 450.0f; rect.y = 307.0f;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonY), rect.x, rect.y + 20.0f, scale, color, textStyle, textAlignMode, &textRect, 0, 0, 0);
				rect.x = 0.0f; rect.y = 248.0f;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonBlack), rect.x, rect.y + 15.0f, scale, color, textStyle, 6, &textRect, 0, 0, 0); //R1/L1
				rect.x = 442.0f; rect.y = 248.0f;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonWhite), rect.x + 7.0f, rect.y + 33.0f, scale, color, textStyle, textAlignMode, &textRect, 0, 0, 0); //R1/L1
				rect.x = 20.0f; rect.y = 298.0f;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonLStick), rect.x, rect.y + 70.0f, scale, color, textStyle, 6, &textRect, 0, 0, 0);
				rect.x = 7.0f; rect.y = 228.0f;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonLTrig), rect.x, rect.y + 15.0f, scale, color, textStyle, 6, &textRect, 0, 0, 0);
				rect.x = 435.0f; rect.y = 228.0f;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonRTrig), rect.x + 7.0f, rect.y + 33.0f, scale, color, textStyle, textAlignMode, &textRect, 0, 0, 0);
				rect.x = 307.0f; rect.y = 213.0f;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonStart), rect.x, rect.y + 45.0f, scale, color, textStyle, textAlignMode, &textRect, 0, 0, 0);
				rect.x = 120.0f; rect.y = 213.0f;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonBack), rect.x, rect.y - 3.0f, scale, color, textStyle, 6, &textRect, 0, 0, 0);
				rect.x = -20.0f; rect.y = 298.0f;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonDown), rect.x, rect.y + 25.0f, scale, color, textStyle, 6, &textRect, 0, 0, 0);
				rect.x = 327.0f; rect.y = 404.0f;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonRStick), rect.x, rect.y + 33.0f, scale, color, textStyle, textAlignMode, &textRect, 0, 0, 0);
			}
			else
			{
				//XBOX
				Game::rectDef_s textRect{};
				Game::rectDef_s rect{};
				rect.x = 467.0f;
				rect.y = 348.0f;
				rect.w = 200.0f;
				rect.horzAlign = 0;
				rect.vertAlign = 0;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonA), rect.x, rect.y + 12.0f, scale, color, textStyle, textAlignMode, &textRect, 0, 0, 0);
				rect.x = 466.0f; rect.y = 326.0f;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonX), rect.x, rect.y + 12.0f, scale, color, textStyle, textAlignMode, &textRect, 0, 0, 0);
				rect.x = 462.0f; rect.y = 303.0f;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonB), rect.x, rect.y + 16.0f, scale, color, textStyle, textAlignMode, &textRect, 0, 0, 0);
				rect.x = 457.0f; rect.y = 278.0f;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonY), rect.x, rect.y + 16.0f, scale, color, textStyle, textAlignMode, &textRect, 0, 0, 0);
				rect.x = 0.0f; rect.y = 248.0f;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonBlack), rect.x, rect.y, scale, color, textStyle, 6, &textRect, 0, 0, 0); //R1/L1
				rect.x = 442.0f; rect.y = 248.0f;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonWhite), rect.x, rect.y + 18.0f, scale, color, textStyle, textAlignMode, &textRect, 0, 0, 0); //R1/L1
				rect.x = 105.0f; rect.y = 417.0f;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonLStick), rect.x, rect.y, scale, color, textStyle, 6, &textRect, 0, 0, 0);
				rect.x = 7.0f; rect.y = 228.0f;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonLTrig), rect.x, rect.y, scale, color, textStyle, 6, &textRect, 0, 0, 0);
				rect.x = 435.0f; rect.y = 228.0f;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonRTrig), rect.x, rect.y + 18.0f, scale, color, textStyle, textAlignMode, &textRect, 0, 0, 0);
				rect.x = 307.0f; rect.y = 213.0f;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonStart), rect.x, rect.y + 12.0f, scale, color, textStyle, textAlignMode, &textRect, 0, 0, 0);
				rect.x = 120.0f; rect.y = 180.0f;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonBack), rect.x, rect.y, scale, color, textStyle, 6, &textRect, 0, 0, 0);
				rect.x = -20.0f; rect.y = 298.0f;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonDown), rect.x, rect.y, scale, color, textStyle, 6, &textRect, 0, 0, 0);
				rect.x = 327.0f; rect.y = 404.0f;
				Game::DrawWrappedText(&rect, fontHandle, scrPlace, Game::UI_SafeTranslateString(Gamepad::buttonMappingLayout.buttonRStick), rect.x, rect.y + 16.0f, scale, color, textStyle, textAlignMode, &textRect, 0, 0, 0);
			}
		});

		// UI_RunMenuScript "clearError" hooking
		Utils::Hook(0x566CC4, UI_RunMenuScript_stub, HOOK_JUMP).install()->quick();
		// OwnerDraw hooking
		Utils::Hook(0x565908, OwnerDrawStub, HOOK_JUMP).install()->quick();
	}

	UIScript::~UIScript()
	{
		UIScripts.clear();
		UIOwnerDraws.clear();
	}
}