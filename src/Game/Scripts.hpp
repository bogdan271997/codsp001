#pragma once

namespace Game
{
	extern scr_const_t* scr_const;
	extern Game::scrVarPub_t* scrVarPub;
	extern Game::scrVmPub_t* scrVmPub;

	//-------------------------------------------------
	typedef unsigned int(__cdecl* Scr_AllocString_t)(const char* s, int sys, size_t size);
	extern Scr_AllocString_t Scr_AllocString;

	typedef void(*Scr_AddInt_t)(int value);
	extern Scr_AddInt_t Scr_AddInt;

	typedef void(__cdecl* Scr_AddFloat_t)(float value);
	extern Scr_AddFloat_t Scr_AddFloat;

	typedef void(__cdecl* Scr_ParamError_t)(const char* error, ...);
	extern Scr_ParamError_t Scr_ParamError;

	typedef Game::xmethod_t(Player_GetMethod_t)(const char**);
	extern Player_GetMethod_t* Player_GetMethod;

	typedef Game::xfunction_t(Scr_GetFunction_t)(const char**, int*);
	extern Scr_GetFunction_t* Scr_GetFunction;

	typedef int(*Scr_GetFunctionHandle_t)(const char* filename, const char* name);
	extern Scr_GetFunctionHandle_t Scr_GetFunctionHandle;

	typedef int(*Scr_LoadScriptInternal_t)(const char* filename, Game::PrecacheEntry* entries, int entriesCount);
	extern Scr_LoadScriptInternal_t Scr_LoadScriptInternal;

	//-------------------------------------------------
	unsigned int GScr_AllocString(const char* s);
	void Scr_AddString(const char* value /*esi*/);
	const char* Scr_GetString(unsigned int index);
	unsigned int Scr_GetConstString(unsigned int index /*eax*/);
	int Scr_GetInt/*eax*/(unsigned int index /*eax*/);
	float Scr_GetFloat/*st0*/(unsigned int index /*eax*/);
	int Scr_GetNumParam();
	void Scr_Error(const char* error/*eax*/);
	void Scr_VerifyWeaponIndex(const char* weaponName /*esi*/);
	__int16 Scr_ExecThread(int handle/*eax*/);
	int Scr_FreeThread(unsigned __int16 handle);
	int Scr_LoadScript(const char* filename);
}