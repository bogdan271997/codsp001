#pragma once

namespace Game
{
	constexpr auto CMD_MAX_NESTING = 8;

	extern Game::CmdArgs* cmd_args;
	extern Game::cmd_function_s** cmd_functions_ptr;
	extern Game::cmd_function_s* cmd_functions;
	//-------------------------------------------------
	typedef void(*Cmd_ExecuteSingleCommand_t)(int localClientNum, int controllerIndex, const char* cmd);
	extern Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand;
	//-------------------------------------------------
	void Cbuf_AddText(int localClientNum /*eax*/, const char* text /*ecx*/);
	void Cmd_AddCommand(const char* name, void(*callback)(), Game::cmd_function_s* data, char);
	void Cmd_AddCommand(const char* name, const char* args, const char* description, void(*callback)(), Game::cmd_function_s* data, char);
}