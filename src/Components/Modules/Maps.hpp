#pragma once

namespace Components
{
	class Maps : public Component
	{
	public:
		inline static bool mpsp_is_sp_map = false;
		inline static const char* mpsp_mapents_original = nullptr;
		static void DB_LoadLevelXAssets(char* zoneName, unsigned int zoneFlags);
		//static void mpsp_map_replacement_mapents(Game::clipMap_t* cm);
		//static void sp_map_unload();
		Maps();
		~Maps();
	private:
	};
}