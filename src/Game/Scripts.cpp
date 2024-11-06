#include "STDInc.hpp"

namespace Game
{
	scr_const_t* scr_const = reinterpret_cast<scr_const_t*>(0xF523E8);
	Game::scrVarPub_t* scrVarPub = reinterpret_cast<Game::scrVarPub_t*>(0x1264980);
	Game::scrVmPub_t* scrVmPub = reinterpret_cast<Game::scrVmPub_t*>(0x1287240);

	//-------------------------------------------------
	Scr_AllocString_t Scr_AllocString = (Scr_AllocString_t)0x54CEC0;
	Scr_AddInt_t Scr_AddInt = Scr_AddInt_t(0x558B80);
	Scr_AddFloat_t Scr_AddFloat = Scr_AddFloat_t(0x558BC0);
	Scr_ParamError_t Scr_ParamError = Scr_ParamError_t(0x5902D0);
	Player_GetMethod_t* Player_GetMethod = reinterpret_cast<Player_GetMethod_t*>(0x49B2E0);
	Scr_GetFunction_t* Scr_GetFunction = reinterpret_cast<Scr_GetFunction_t*>(0x4DAD80);
	Scr_GetFunctionHandle_t Scr_GetFunctionHandle = Scr_GetFunctionHandle_t(0x549050);
	Scr_LoadScriptInternal_t Scr_LoadScriptInternal = Scr_LoadScriptInternal_t(0x549420);
	//-------------------------------------------------
	unsigned int GScr_AllocString(const char* s)
	{
		unsigned int stringVal = Game::Scr_AllocString(s, 1, strlen(s) + 1);
		return stringVal;
	}

	void Scr_AddString(const char* value /*esi*/)
	{
		const static uint32_t Scr_AddString_func = 0x558D10;
		__asm
		{
			mov		esi, value;
			call	Scr_AddString_func;
		}
	}

	const char* Scr_GetString(unsigned int index)
	{
		unsigned int ConstString;
		ConstString = Game::Scr_GetConstString(index);
		return SL_ConvertToString(ConstString);
	}

	unsigned int Scr_GetConstString(unsigned int index /*eax*/)
	{
		const static uint32_t Scr_GetConstString_func = 0x558440;
		__asm
		{
			mov     eax, index;
			call	Scr_GetConstString_func;
		}
	}

	int Scr_GetInt/*eax*/(unsigned int index /*eax*/)
	{
		int result;
		const static uint32_t Scr_GetInt_func = 0x5580F0;
		__asm
		{
			mov		eax, index;
			call	Scr_GetInt_func;
			mov		result, eax;
		}
		return result;
	}

	float Scr_GetFloat/*st0*/(unsigned int index /*eax*/)
	{
		float result;
		const static uint32_t Scr_GetFloat_func = 0x5583A0;
		__asm
		{
			mov		eax, index;
			call	Scr_GetFloat_func;
			fstp    dword ptr[result]; //FIXED!!!
		}
		return result;
	}

	void Scr_Error(const char* error/*eax*/)
	{
		const static uint32_t Scr_Error_func = 0x558FD0;
		__asm
		{
			mov		eax, error;
			call	Scr_Error_func;
		}
	}

	int Scr_GetNumParam() 
	{
		return *reinterpret_cast<int*>(0x128725C);
	}

	void Scr_VerifyWeaponIndex(const char* weaponName /*esi*/)
	{
		const static uint32_t Scr_VerifyWeaponIndex_func = 0x4C7D70;
		__asm
		{
			mov		esi, weaponName;
			call	Scr_VerifyWeaponIndex_func;
		}
	}

	__int16 Scr_ExecThread(int handle/*eax*/)
	{
		__int16 result;
		const static uint32_t Scr_ExecThread_func = 0x557950;
		__asm
		{
			pushad;
			mov		eax, handle;
			call	Scr_ExecThread_func;
			mov		result, ax;
			popad;
		}
		return result;
	}

	int Scr_FreeThread(unsigned __int16 handle)
	{
		return Game::RemoveRefToObject(handle);
	}

	int Scr_LoadScript(const char* filename)
	{
		int result;
		const static uint32_t Scr_LoadScript_func = 0x549630;
		__asm
		{
			pushad;
			mov		ecx, filename;
			call	Scr_LoadScript_func;
			mov		result, eax;
			popad;
		}
		return result;
	}
}