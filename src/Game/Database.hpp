#pragma once

namespace Game
{
	extern unsigned int* g_poolSize;
	extern const char** g_assetNames;
	extern XAssetHeader* DB_XAssetPool;
	extern HANDLE databaseCompletedEvent;
	extern HANDLE databaseCompletedEvent2;
	extern XZoneName* g_zoneNames;
	//-------------------------------------------------
	typedef void (*DB_LoadXAssets_t)(XZoneInfo* data, unsigned int count, int sync);
	extern DB_LoadXAssets_t DB_LoadXAssets;

	typedef void(*DB_EnumXAssets_t)(XAssetType type, void(*)(XAssetHeader, void*), const void* userdata, bool overrides);
	extern DB_EnumXAssets_t DB_EnumXAssets;

	typedef XAssetHeader(*DB_FindXAssetHeader_t)(XAssetType type, const char* name);
	extern DB_FindXAssetHeader_t DB_FindXAssetHeader;

	typedef int(__cdecl* DB_GetXAssetSizeHandler_t)();
	extern DB_GetXAssetSizeHandler_t* DB_GetXAssetSizeHandlers;

	typedef const char* (*DB_XAssetGetNameHandler_t)(XAssetHeader* asset);
	extern DB_XAssetGetNameHandler_t* DB_XAssetGetNameHandler;

	typedef void(*DB_BeginRecoverLostDevice_t)();
	extern DB_BeginRecoverLostDevice_t DB_BeginRecoverLostDevice;

	typedef void(*DB_EndRecoverLostDevice_t)();
	extern DB_EndRecoverLostDevice_t DB_EndRecoverLostDevice;

	typedef bool(__cdecl* DB_IsXAssetDefault_t)(XAssetType assettype, const char* name);
	extern DB_IsXAssetDefault_t DB_IsXAssetDefault;
	//-------------------------------------------------
	XAssetHeader DB_ReallocXAssetPool(XAssetType type, unsigned int new_size);
	extern const char* DB_GetXAssetName(XAsset* asset);
	extern bool DB_IsZoneLoaded(const char* zone);
}