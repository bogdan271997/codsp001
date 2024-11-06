#pragma once

namespace Components
{
	class FastFiles : public Component
	{
	public:
		FastFiles();
		~FastFiles();
		static void EnumAssets(const Game::XAssetType type, const std::function<void(Game::XAssetHeader)>& callback, const bool includeOverride);
		static void AddZonePath(const std::string& path);
		static void SetupZoneFolder(char* buffer, size_t size, const char* /*format*/, const char* directory, const char* language, const char* zoneName);
		static std::string Current();
		static bool Ready();
		static bool Exists(const std::string& file);
	private:
		static std::vector<std::string> ZonePaths;
		static const char* GetZoneLocation(const char* file);

		static void DB_ModFileExistsStub();
		static void DB_ModXFileHandleStub();
		static void DB_BuildOSPath_FromSourceStub();
		static void DB_LoadModFastFile(char* buffer, size_t size, const char* /*format*/, const char* directory, const char* modDir, const char* zoneName);
	};
}