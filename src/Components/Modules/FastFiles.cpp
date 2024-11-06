#include "STDInc.hpp"

namespace Components
{
	std::vector<std::string> FastFiles::ZonePaths;

	void FastFiles::EnumAssets(const Game::XAssetType type, const std::function<void(Game::XAssetHeader)>& callback, const bool includeOverride)
	{
		Game::DB_EnumXAssets(type, static_cast<void(*)(Game::XAssetHeader, void*)>([](Game::XAssetHeader header, void* data)
		{
			const auto& cb = *static_cast<const std::function<void(Game::XAssetHeader)>*>(data);
			cb(header);
		}), &callback, includeOverride);
	}

	bool FastFiles::Ready()
	{
		return (Game::Sys_IsDatabaseReady() && Game::Sys_IsDatabaseReady2());
	}

	const char* FastFiles::GetZoneLocation(const char* file)
	{
		std::vector<std::string> paths;
		std::string modDir = Dvars::Functions::Dvar_FindVar("fs_game")->current.string;

		if ((file == "mod"s || file == "mod.ff"s) && !modDir.empty())
		{
			paths.push_back(std::format("{}\\", modDir));
		}

		if (Dvars::allowCompactFolderForMod->current.enabled && !modDir.empty())
		{
			Utils::String::Replace(modDir, "/", "\\"s);
			paths.push_back(Utils::String::VA("%s\\zone\\english\\", modDir.data()));
		}

		std::string zone = file;
		if (Utils::String::EndsWith(zone, ".ff"))
		{
			Utils::String::Replace(zone, ".ff", "");
		}

		std::string filename = zone;

		Utils::Merge(&paths, ZonePaths);

		for (auto& path : paths)
		{
			std::string dir = Dvars::Functions::Dvar_FindVar("fs_basepath")->current.string;
			std::string absoluteFile = std::format("{}\\{}{}", dir, path, file);

			// No ".ff" appended, append it manually
			if (!Utils::String::EndsWith(absoluteFile, ".ff"))
			{
				absoluteFile.append(".ff");
			}

			// Check if FastFile exists
			if (Utils::IO::FileExists(absoluteFile))
			{
				return Utils::String::Format("{}", path);
			}
		}

		return Utils::String::Format("zone\\{}\\", Language::GetCurrentLanguage());
	}

	bool FastFiles::Exists(const std::string& file)
	{
		std::string path = FastFiles::GetZoneLocation(file.data());
		path.append(file);

		if (!Utils::String::EndsWith(path, ".ff"))
		{
			path.append(".ff");
		}

		return Utils::IO::FileExists(path);
	}

	void FastFiles::AddZonePath(const std::string& path)
	{
		FastFiles::ZonePaths.push_back(path);
	}

	void FastFiles::SetupZoneFolder(char* buffer, size_t size, const char* /*format*/, const char* directory, const char* language, const char* zoneName)
	{
		std::string modDir = Dvars::Functions::Dvar_FindVar("fs_game")->current.string;

		// Looks similar with FastFiles::GetZoneLocation method, but need sprintf_s return
		for (auto& path : FastFiles::ZonePaths)
		{
			if (Utils::IO::FileExists(Utils::String::VA("%s\\%s\\%s.ff", directory, path.c_str(), zoneName)))
			{
				sprintf_s(buffer, size, "%s\\%s\\%s.ff", directory, path.data(), zoneName);
				return;
			}
		}

		if (!modDir.empty())
		{
			if (Dvars::allowCompactFolderForMod->current.enabled)
			{
				// mainly, mods are compiled in the english version, so i keep continue use the english.
				if (Utils::IO::FileExists(Utils::String::VA("%s\\%s\\zone\\%s\\%s.ff", directory, modDir.data(), "english", zoneName)))
				{
					sprintf_s(buffer, size, "%s\\%s\\zone\\%s\\%s.ff", directory, modDir.data(), "english", zoneName);
				}
				else
				{
					// keep the loading zone files except mod.ff from zone/language
					sprintf_s(buffer, size, "%s\\zone\\%s\\%s.ff", directory, language, zoneName);
				}
			}
			else
			{
				// keep the loading zone files except mod.ff from zone/language
				sprintf_s(buffer, size, "%s\\zone\\%s\\%s.ff", directory, language, zoneName);
			}
		}
		else
		{
			sprintf_s(buffer, size, "%s\\zone\\%s\\%s.ff", directory, language, zoneName);
		}
	}

	void FastFiles::DB_LoadModFastFile(char* buffer, size_t size, const char* /*format*/, const char* directory, const char* modDir, const char* zoneName)
	{
		if (Dvars::allowCompactFolderForMod->current.enabled)
		{
			if (Language::GetCurrentLanguage() != "english")
			{
				std::string language = Language::GetCurrentLanguage();
				if (Utils::IO::FileExists(Utils::String::VA("%s\\%s\\%s.ff", modDir, language.c_str(), zoneName)))
				{
					sprintf_s(buffer, size, "%s\\%s\\%s\\%s.ff", directory, modDir, language.c_str(), zoneName);
				}
				else if (Utils::IO::FileExists(Utils::String::VA("%s\\non_english\\%s.ff", modDir, zoneName)))
				{
					sprintf_s(buffer, size, "%s\\%s\\non_english\\%s.ff", directory, modDir, zoneName);
				}
				else
				{
					sprintf_s(buffer, size, "%s\\%s\\%s.ff", directory, modDir, zoneName);
				}
			}
			else
			{
				sprintf_s(buffer, size, "%s\\%s\\%s.ff", directory, modDir, zoneName);
			}
		}
		else
		{
			sprintf_s(buffer, size, "%s\\%s\\%s.ff", directory, modDir, zoneName);
		}
	}

	void __declspec(naked) DB_TryLoadXFileInternal_stub()
	{
		const static uint32_t retn_addr = 0x45BFEC;
		__asm
		{
			push	esi; // size
			push	edi; // fileName
			call	FastFiles::SetupZoneFolder;
			add		esp, 8;
			jmp		retn_addr;
		}
	}

	void __declspec(naked) FastFiles::DB_ModFileExistsStub()
	{
		const static uint32_t retn_addr = 0x45CC34;
		__asm
		{
			push	esi; // size
			push	edi; // fileName
			call	DB_LoadModFastFile;
			add		esp, 8;
			jmp		retn_addr;
		}
	}

	void __declspec(naked) FastFiles::DB_ModXFileHandleStub()
	{
		const static uint32_t retn_addr = 0x45BE15;
		__asm
		{
			push	esi; // size
			push	edi; // fileName
			call	DB_LoadModFastFile;
			add		esp, 8;
			jmp		retn_addr;
		}
	}

	void __declspec(naked) FastFiles::DB_BuildOSPath_FromSourceStub()
	{
		const static uint32_t retn_addr = 0x45B73C;
		__asm
		{
			push	esi; // size
			push	edi; // fileName
			call	DB_LoadModFastFile;
			add		esp, 8;
			jmp		retn_addr;
		}
	}

	FastFiles::FastFiles()
	{
		// Custom zone folder
		Utils::Hook(0x45BFE7, DB_TryLoadXFileInternal_stub, HOOK_JUMP).install()->quick();

		// Hooks for mod.ff
		Utils::Hook(0x45CC2F, DB_ModFileExistsStub, HOOK_JUMP).install()->quick();
		Utils::Hook(0x45BE10, DB_ModXFileHandleStub, HOOK_JUMP).install()->quick();
		Utils::Hook(0x45B737, DB_BuildOSPath_FromSourceStub, HOOK_JUMP).install()->quick();

		FastFiles::AddZonePath("modern_ops_data\\zone\\");
		FastFiles::AddZonePath("modern_ops_data\\zone\\english\\");
		FastFiles::AddZonePath("modern_ops_data\\zone\\french\\");
		FastFiles::AddZonePath("modern_ops_data\\zone\\german\\");
		FastFiles::AddZonePath("modern_ops_data\\zone\\italian\\");
		FastFiles::AddZonePath("modern_ops_data\\zone\\spanish\\");
		FastFiles::AddZonePath("modern_ops_data\\zone\\russian\\");
		// custom languages
		FastFiles::AddZonePath("modern_ops_data\\zone\\portuguese\\");
		FastFiles::AddZonePath("modern_ops_data\\zone\\slovak\\");

		#ifdef DEVELOPER_MODE
			Command::Add("loadzone", [](Command::Params* params)
			{
				if (params->size() < 2) return;

				Game::XZoneInfo info[2];

				// unload
				info[0].name = 0;
				info[0].allocFlags = Game::XZONE_FLAGS::XZONE_ZERO;
				info[0].freeFlags = Game::XZONE_FLAGS::XZONE_MOD;

				info[1].name = params->get(1);
				info[1].allocFlags = Game::XZONE_FLAGS::XZONE_MOD;
				info[1].freeFlags = Game::XZONE_FLAGS::XZONE_ZERO;

				Game::DB_LoadXAssets(info, 2, true);
			});
		#endif
	}

	FastFiles::~FastFiles()
	{
	}
}