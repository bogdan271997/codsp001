#include "STDInc.hpp"

// Keeping the new language array in 'Language' module.
namespace Game
{
	Game::languageInfo_t g_languages[Game::LANGUAGE_COUNT] =
	{
		{"english", 0},
		{"french", 0},
		{"german", 0},
		{"italian", 0},
		{"spanish", 0},
		{"british", 0},
		{"russian", 0},
		{"polish", 0},
		{"korean", 0},
		{"taiwanese", 0},
		{"japanese", 0},
		{"chinese", 0},
		{"thai", 0},
		{"leet", 0},
		{"czech", 0}, //end original
		{"portuguese", 0},
		{"slovak", 0}
	};

	std::vector<Game::language_t> CustomLanguagesIndex =
	{
		{Game::LANGUAGE_PORTUGUESE},
		{Game::LANGUAGE_SLOVAK},
	};
}

namespace Components
{
	std::string Language::GetCurrentLanguage()
	{
		return Config::Get<std::string>("language").value();
	}

	void Language::LanguageSetupInit()
	{
		if (Config::Get<bool>("language_first_setting").value() == false)
		{
			int langIndexDef = 0;
			Game::SEH_GetLanguageIndexFromName(Config::Get<std::string>("language").value().c_str(), &langIndexDef);

			const char* language = Game::SEH_GetLanguageName(langIndexDef);
			Game::localization->language = language;

			Dvars::Functions::Dvar_FindVar("loc_language")->current.integer = langIndexDef;
			Dvars::Functions::Dvar_FindVar("loc_language")->latched.integer = langIndexDef;
			Dvars::Functions::Dvar_FindVar("ui_language")->current.integer = langIndexDef;
			Dvars::Functions::Dvar_FindVar("ui_language")->latched.integer = langIndexDef;
		}
		else if (Config::Get<bool>("language_first_setting").value() == true)
		{
			const char* language_buffer = reinterpret_cast<const char*>(0x13E0708);
			int langIndexDef = 0;
			Game::SEH_GetLanguageIndexFromName(Config::Get<std::string>("language").value().c_str(), &langIndexDef);

			Config::Set<std::string>("language", language_buffer);

			Dvars::Functions::Dvar_FindVar("loc_language")->current.integer = langIndexDef;
			Dvars::Functions::Dvar_FindVar("loc_language")->latched.integer = langIndexDef;
			Dvars::Functions::Dvar_FindVar("ui_language")->current.integer = Dvars::Functions::Dvar_FindVar("loc_language")->current.integer;
			Dvars::Functions::Dvar_FindVar("ui_language")->latched.integer = Dvars::Functions::Dvar_FindVar("loc_language")->latched.integer;

			Config::Set<bool>("language_first_setting", false);
		}
		else
		{
			Config::Set<bool>("language_first_setting", true);

			const char* language_buffer = reinterpret_cast<const char*>(0x13E0708);
			int langIndexDef = 0;
			Game::SEH_GetLanguageIndexFromName(Config::Get<std::string>("language").value().c_str(), &langIndexDef);

			Config::Set<std::string>("language", language_buffer);

			Dvars::Functions::Dvar_FindVar("loc_language")->current.integer = langIndexDef;
			Dvars::Functions::Dvar_FindVar("loc_language")->latched.integer = langIndexDef;
			Dvars::Functions::Dvar_FindVar("ui_language")->current.integer = Dvars::Functions::Dvar_FindVar("loc_language")->current.integer;
			Dvars::Functions::Dvar_FindVar("ui_language")->latched.integer = Dvars::Functions::Dvar_FindVar("loc_language")->latched.integer;

			Config::Set<bool>("language_first_setting", false);
		}
	}

	bool Language::HasSupportCustomVidSubFolder()
	{
		// Portuguese and Slovak do not have subtitle folders, so i just use the original file.
		if (Language::GetCurrentLanguage() == "english" ||
			Language::GetCurrentLanguage() == "french"	||
			Language::GetCurrentLanguage() == "german"	||
			Language::GetCurrentLanguage() == "italian" ||
			Language::GetCurrentLanguage() == "russian" ||
			Language::GetCurrentLanguage() == "spanish")
		{
			return true;
		}

		return false;
	}

	const char* Language::GetLanguageForVidSubtitles()
	{
		int language = Dvars::Functions::Dvar_FindVar("loc_language")->current.integer;

		switch (language)
		{
			case 0: return "video/en/vidsubtitles.csv"; break;
			case 1: return "video/fr/vidsubtitles.csv"; break;
			case 2: return "video/ge/vidsubtitles.csv"; break;
			case 3: return "video/it/vidsubtitles.csv"; break;
			case 4: return "video/sp/vidsubtitles.csv"; break;
			case 5: return "video/br/vidsubtitles.csv"; break;
			case 6: return "video/ru/vidsubtitles.csv"; break;
			case 7: return "video/po/vidsubtitles.csv"; break;
			case 8: return "video/ko/vidsubtitles.csv"; break;
			case 9: return "video/tw/vidsubtitles.csv"; break;
			case 10: return "video/jp/vidsubtitles.csv"; break;
			case 11: return "video/cn/vidsubtitles.csv"; break;
			case 12: return "video/th/vidsubtitles.csv"; break;
			case 13: return "video/1337/vidsubtitles.csv"; break;
			case 14: return "video/cz/vidsubtitles.csv"; break;
			default: return "video/en/vidsubtitles.csv"; break;
		}
	}
	
	void Language::VideoSubtitlesStub()
	{
		// Not used.
		/*
		if (HasSupportCustomVidSubFolder() && strlen(Dvars::Functions::Dvar_FindVar("fs_game")->current.string) == 0)
		{
			Utils::Hook::Set<const char*>(0x420F45, GetLanguageForVidSubtitles());
			Utils::Hook::Set<const char*>(0x567AA4, GetLanguageForVidSubtitles());
		}
		else
		{
			Utils::Hook::Set<const char*>(0x420F45, "video/subtitles.csv");
			Utils::Hook::Set<const char*>(0x567AA4, "video/subtitles.csv");
		}
		*/
		Utils::Hook::Set<const char*>(0x420F45, "video/subtitles.csv");
		Utils::Hook::Set<const char*>(0x567AA4, "video/subtitles.csv");
	}

	void Language::LanguageInstallStub()
	{
		Language::LanguageSetupInit();
		Language::VideoSubtitlesStub();

		if (Language::GetCurrentLanguage() == "russian")
		{
			Utils::Hook::Set<const char*>(0x40D653, "%.1fì");
			Utils::Hook::Set<const char*>(0x40D668, "%iì");
		}
		else
		{
			Utils::Hook::Set<const char*>(0x40D653, "%.1fm");
			Utils::Hook::Set<const char*>(0x40D668, "%im");
		}

		Utils::Hook::Call<std::uint8_t()>(0x614640)();
	}

	Language::Language()
	{
		Events::OnDvarInit([]
		{
			Dvars::ui_language = Dvars::Register::Dvar_RegisterInt("ui_language", "Game Language", 0, 0, Game::LANGUAGE_COUNT - 1, Game::none);
		});

		Utils::Hook(0x5D9BC4, Language::LanguageInstallStub, HOOK_CALL).install()->quick();

		// Increased the 'loc_language' max count
		Utils::Hook::Set<std::uint8_t>(0x444FFA, Game::LANGUAGE_COUNT - 1);
		Utils::Hook::Set<std::uint8_t>(0x55BAB2, Game::LANGUAGE_COUNT - 1);

		// Replacing the original language array on own language array.
		Utils::Hook::Set(0x55BCEF, &Game::g_languages[Game::LANGUAGE_ENGLISH].bPresent);
		Utils::Hook::Set(0x55BC83, &Game::g_languages[Game::LANGUAGE_ENGLISH].bPresent);
		Utils::Hook::Set(0x55BDE3, &Game::g_languages[Game::LANGUAGE_ENGLISH].bPresent);
		Utils::Hook::Set(0x55BDFE, &Game::g_languages[Game::LANGUAGE_ENGLISH].bPresent);
		Utils::Hook::Set(0x55BE24, &Game::g_languages[Game::LANGUAGE_ENGLISH].bPresent);

		Utils::Hook::Set(0x55C4D8, Game::g_languages);
		Utils::Hook::Set(0x55C4DE, Game::g_languages);
		Utils::Hook::Set(0x55C503, Game::g_languages);
		Utils::Hook::Set(0x579C94, Game::g_languages);
		Utils::Hook::Set(0x579C9B, Game::g_languages);
		Utils::Hook::Set(0x579D49, Game::g_languages);
		Utils::Hook::Set(0x579D56, Game::g_languages);
		Utils::Hook::Set(0x579D8D, Game::g_languages);
		Utils::Hook::Set(0x579D94, Game::g_languages);
		Utils::Hook::Set(0x57A278, Game::g_languages);
		Utils::Hook::Set(0x57A27F, Game::g_languages);
		Utils::Hook::Set(0x57A446, Game::g_languages);
		Utils::Hook::Set(0x57A44D, Game::g_languages);

		Utils::Hook::Set<std::uint8_t>(0x55C4D2, Game::LANGUAGE_COUNT - 1);
		Utils::Hook::Set<std::uint8_t>(0x579C8E, Game::LANGUAGE_COUNT - 1);
		Utils::Hook::Set<std::uint8_t>(0x579D43, Game::LANGUAGE_COUNT - 1);
		Utils::Hook::Set<std::uint8_t>(0x579D87, Game::LANGUAGE_COUNT - 1);
		Utils::Hook::Set<std::uint8_t>(0x57A272, Game::LANGUAGE_COUNT - 1);
		Utils::Hook::Set<std::uint8_t>(0x57A440, Game::LANGUAGE_COUNT - 1);

		Utils::Hook::Set<std::uint8_t>(0x55C520, Game::LANGUAGE_COUNT);
		Utils::Hook::Set<std::uint8_t>(0x57A297, Game::LANGUAGE_COUNT);
		Utils::Hook::Set<std::uint8_t>(0x55BDF5, Game::LANGUAGE_COUNT);

		for (const auto& language : Game::CustomLanguagesIndex)
		{
			Game::g_languages[language].bPresent = 1;
		}
	}

	Language::~Language()
	{
	}
}