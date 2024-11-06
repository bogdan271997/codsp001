#pragma once

namespace Components
{
	class FileSystem : public Component
	{
	public:
		static void FS_AddSearchPath(Game::searchpath_s* search);
		static void FS_AddFolder(const std::string& path);
		static void FS_LoadedIwds(const char** names);

		FileSystem();
		~FileSystem();
	private:
		static std::vector<std::string> FS_Folders;
		static void FS_DisplayPath_Stub();
	};
}