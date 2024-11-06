#include "STDInc.hpp"

namespace Components
{
	std::vector<std::string> FileSystem::FS_Folders;

	void FileSystem::FS_AddSearchPath(Game::searchpath_s* search)
	{
		Game::searchpath_s** pSearch = &Game::fs_searchpaths;
		if (search->bLocalized)
		{
			while (*pSearch && !(*pSearch)->bLocalized)
				pSearch = &(*pSearch)->next;
		}
		search->next = *pSearch;
		*pSearch = search;
	}

	void FileSystem::FS_LoadedIwds(const char** names)
	{
		char iwdNames[256];

		std::memset(iwdNames, 0, sizeof(iwdNames));
		for (Game::searchpath_s* search = Game::fs_searchpaths; search; search = search->next)
		{
			if (search->iwd && !search->bLocalized)
			{
				iwdNames[strnlen(iwdNames, sizeof(iwdNames))] = '\n';
				strncat(iwdNames, search->iwd->iwdBasename, _TRUNCATE);
			}
		}
		*names = iwdNames;
	}

	void FileSystem::FS_AddFolder(const std::string& path)
	{
		FileSystem::FS_Folders.push_back(path);
	}

	void FileSystem::FS_DisplayPath_Stub()
	{
		std::string fs_basepath = Dvars::Functions::Dvar_FindVar("fs_basepath")->current.string;

		std::vector<std::string> FileSystemFolders;
		Utils::Merge(&FileSystemFolders, FileSystem::FS_Folders);

		for (auto& FileSystemFolder : FileSystemFolders)
		{
			Game::FS_AddLocalizedGameDirectory(FileSystemFolder.data(), fs_basepath.data());
		}

		Game::FS_DisplayPath(1);
	}

	FileSystem::FileSystem()
	{
		// Currently not used.
		/*
		FS_AddFolder("modern_ops_data");
		Utils::Hook(0x57B008, FileSystem::FS_DisplayPath_Stub, HOOK_CALL).install()->quick();

		Command::Add("fs_openedList", [](Command::Params*)
		{
			const char* names;
			FS_LoadedIwds(&names);
			Game::Com_Printf(0, "Opened IWD Names: %s\n", names);
		});
		*/
	}

	FileSystem::~FileSystem()
	{
	}
}