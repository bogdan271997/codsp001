#pragma once

// Keeping the new language array in 'Language' module.
namespace Game
{
	extern Game::languageInfo_t g_languages[Game::LANGUAGE_COUNT];
}

namespace Components
{
	class Language : public Component
	{
	public:
		Language();
		~Language();
		static std::string GetCurrentLanguage();
		static void VideoSubtitlesStub();
	private:
		static bool HasSupportCustomVidSubFolder();
		static const char* GetLanguageForVidSubtitles();
		static void LanguageInstallStub();
		static void LanguageSetupInit();
	};
}