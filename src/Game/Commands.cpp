#include "STDInc.hpp"

namespace Game
{
	Game::CmdArgs* cmd_args = reinterpret_cast<Game::CmdArgs*>(0xF789E8);
	Game::cmd_function_s** cmd_functions_ptr = reinterpret_cast<Game::cmd_function_s**>(0xF78A6C);
	Game::cmd_function_s* cmd_functions = reinterpret_cast<Game::cmd_function_s*>(0xF78A6C);
	//-------------------------------------------------
	Cmd_ExecuteSingleCommand_t	Cmd_ExecuteSingleCommand = Cmd_ExecuteSingleCommand_t(0x531380);
	//-------------------------------------------------
	void Cbuf_AddText(int localClientNum/*ecx*/, const char* text /*eax*/)
	{
		const static uint32_t Cbuf_AddText_func = 0x530320;
		__asm
		{
			mov		eax, text;
			mov		ecx, localClientNum;
			call	Cbuf_AddText_func;
		}
	}

	void Cmd_AddCommand(const char* name, void(*callback)(), Game::cmd_function_s* data, char)
	{
		data->name = name;
		data->autoCompleteDir = nullptr;
		data->autoCompleteExt = nullptr;
		data->function = callback;
		data->next = *cmd_functions_ptr;
		*cmd_functions_ptr = data;
	}

	void Cmd_AddCommand(const char* name, const char* args, const char* description, void(*callback)(), Game::cmd_function_s* data, char)
	{
		data->name = name;
		data->autoCompleteDir = args;
		data->autoCompleteExt = description;
		data->function = callback;
		data->next = *cmd_functions_ptr;
		*cmd_functions_ptr = data;
	}

	void Cmd_ForEach(void (__cdecl *callback)(const char *))
	{
		for (cmd_function_s *i = cmd_functions; i; i = i->next)
		{
			callback(i->name);
		}
	}
}