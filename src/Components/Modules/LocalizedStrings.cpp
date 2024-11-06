#include "STDInc.hpp"

namespace Components
{
	std::recursive_mutex LocalizedStrings::LocalizeMutex;
	std::unordered_map<std::string, Game::LocalizeEntry*> LocalizedStrings::LocalizeMap;

	std::optional<std::string> LocalizedStrings::PrefixOverride;
	std::function<void(Game::LocalizeEntry*)> LocalizedStrings::ParseCallback;

	void LocalizedStrings::OverrideLocalizeStrings()
	{
		// Clearing previous localizeMap objects when game has been initialized or restarted.
		// Otherwise, game will be replacing the localize entries even if you have you have a another localization.
		LocalizeMap.clear();

		const char* languageName = Game::SEH_GetLanguageName(Dvars::Functions::Dvar_FindVar("loc_language")->current.unsignedInt);

		// Not used.
		/*
		LocalizedStrings::Set("MODERN_OPS_LOC_PLATFORM_UI_SELECTBUTTON_XBOX", "^\x01\x32\x32\x0E""button_xbox_01");
		LocalizedStrings::Set("MODERN_OPS_LOC_PLATFORM_UI_SELECTBUTTON_SONY", "^\x01\x32\x32\x0E""button_sony_01");
		LocalizedStrings::Set("MODERN_OPS_LOC_PLATFORM_BACK_GAMEPAD_XBOX", "^\x01\x32\x32\x0E""button_xbox_02");
		LocalizedStrings::Set("MODERN_OPS_LOC_PLATFORM_BACK_GAMEPAD_SONY", "^\x01\x32\x32\x0E""button_sony_02");

		if (!strcmp(languageName, "english"))
		{
			return;
		}
		else if (!strcmp(languageName, "german"))
		{
			return;
		}
		else if (!strcmp(languageName, "russian"))
		{
			return;
		}
		else
			return;
		*/
	}

	void LocalizedStrings::Set(const std::string& psLocalReference, const std::string& psNewString)
	{
		std::lock_guard _(LocalizeMutex);
		Utils::Memory::Allocator* allocator = Utils::Memory::GetAllocator();

		auto key = psLocalReference;
		if (PrefixOverride.has_value())
		{
			key.insert(0, PrefixOverride.value());
		}

		if (LocalizeMap.contains(key))
		{
			auto* entry = LocalizeMap[key];

			const auto* newStaticValue = allocator->duplicateString(psNewString);
			if (!newStaticValue) return;

			if (entry->value) allocator->free(entry->value);
			entry->value = newStaticValue;

			LocalizedStrings::SaveParseOutput(entry);

			return;
		}

		auto* entry = allocator->allocate<Game::LocalizeEntry>();
		if (!entry) return;

		entry->name = allocator->duplicateString(key);
		if (!entry->name)
		{
			allocator->free(entry);
			return;
		}

		entry->value = allocator->duplicateString(psNewString);
		if (!entry->value)
		{
			allocator->free(entry->name);
			allocator->free(entry);
			return;
		}

		LocalizedStrings::SaveParseOutput(entry);

		LocalizeMap[key] = entry;
	}

	void LocalizedStrings::SaveParseOutput(Game::LocalizeEntry* asset)
	{
		if (ParseCallback)
		{
			ParseCallback(asset);
		}
	}

	const char* LocalizedStrings::Get(const char* key)
	{
		Game::LocalizeEntry* entry = nullptr;
		{
			std::lock_guard _(LocalizeMutex);

			if (LocalizeMap.contains(key))
			{
				entry = LocalizeMap[key];
			}
		}

		if (!entry || !entry->value)
		{
			entry = Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_LOCALIZE_ENTRY, key).localize;
		}

		if (entry && entry->value)
		{
			return entry->value;
		}

		return key;
	}

	void __stdcall LocalizedStrings::SetStringStub(const char* psLocalReference, const char* psNewString, [[maybe_unused]] int bSentenceIsEnglish)
	{
		LocalizedStrings::Set(psLocalReference, psNewString);
	}

	LocalizedStrings::LocalizedStrings()
	{
		AssetHandler::OnFind(Game::XAssetType::ASSET_TYPE_LOCALIZE_ENTRY, [](Game::XAssetType, const std::string& name)
		{
			Game::XAssetHeader header = { nullptr };
			std::lock_guard _(LocalizeMutex);

			if (const auto itr = LocalizeMap.find(name); itr != LocalizeMap.end())
			{
				header.localize = itr->second;
			}

			return header;
		});

		// Resolving hook
		Utils::Hook(0x55BEA0, LocalizedStrings::Get, HOOK_JUMP).install()->quick();
		Utils::Hook(0x55CFE9, LocalizedStrings::SetStringStub, HOOK_CALL).install()->quick();
	}

	LocalizedStrings::~LocalizedStrings()
	{
		LocalizeMap.clear();
	}
}