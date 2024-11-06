#include "STDInc.hpp"

namespace Game
{
	unsigned int* g_poolSize = reinterpret_cast<unsigned int*>(0x6DEFA0);
	const char** g_assetNames = (const char**)0x6DF440;
	XAssetHeader* DB_XAssetPool = reinterpret_cast<XAssetHeader*>(0x6DF200);
	HANDLE databaseCompletedEvent = reinterpret_cast<HANDLE>(0xFC6308);
	HANDLE databaseCompletedEvent2 = reinterpret_cast<HANDLE>(0xFC6348);
	XZoneName* g_zoneNames = reinterpret_cast<XZoneName*>(0xAC3000);
	//-------------------------------------------------
	DB_LoadXAssets_t DB_LoadXAssets = (DB_LoadXAssets_t)0x45B990;
	DB_EnumXAssets_t DB_EnumXAssets = DB_EnumXAssets_t(0x45A8C0);
	DB_FindXAssetHeader_t DB_FindXAssetHeader = DB_FindXAssetHeader_t(0x45AD10);
	DB_GetXAssetSizeHandler_t* DB_GetXAssetSizeHandlers = reinterpret_cast<DB_GetXAssetSizeHandler_t*>(0x6DF5F0);
	DB_XAssetGetNameHandler_t* DB_XAssetGetNameHandler = reinterpret_cast<DB_XAssetGetNameHandler_t*>(0x6DF4D8);
	DB_BeginRecoverLostDevice_t DB_BeginRecoverLostDevice = DB_BeginRecoverLostDevice_t(0x45B180);
	DB_EndRecoverLostDevice_t DB_EndRecoverLostDevice = DB_EndRecoverLostDevice_t(0x45B210);
	DB_IsXAssetDefault_t DB_IsXAssetDefault = DB_IsXAssetDefault_t(0x45B040);
	//-------------------------------------------------
	XAssetHeader DB_ReallocXAssetPool(XAssetType type, unsigned int new_size)
	{
		const XAssetHeader pool_entry =
		{
			Utils::Memory::GetAllocator()->allocate(new_size * DB_GetXAssetSizeHandlers[type]())
		};

		DB_XAssetPool[type] = pool_entry;
		g_poolSize[type] = new_size;

		return pool_entry;
	}

	const char* DB_GetXAssetName(XAsset* asset)
	{
		if (!asset) return "";

		assert(asset->header.data);

		return DB_XAssetGetNameHandler[asset->type](&asset->header);
	}

	bool DB_IsZoneLoaded(const char* zone)
	{
		for (int i = 1; i < 33; ++i)
		{
			if (Game::g_zoneNames[i].name && !_stricmp(Game::g_zoneNames[i].name, zone))
				return Game::g_zoneNames[i].loaded;
		}

		return false;
	}
}