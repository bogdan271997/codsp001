#include "STDInc.hpp"

namespace Components
{
	std::unordered_map<std::string, std::function<void()>> ServerCommand::ServerCommands;

	void ServerCommand::Add(const char* name, const std::function<void()>& callback)
	{
		const auto command = Utils::String::ToLower(name);
		ServerCommands[command] = callback;
	}

	void ServerCommand::DispatchServerCommand(const char* cmd, int cmdArgCount)
	{
		//Game::Com_Printf(0, "Command Name %s\n", cmd);
		//Game::Com_Printf(0, "Command Argument Count %d\n", cmdArgCount);

		if (const auto itr = ServerCommands.begin(); itr != ServerCommands.end())
		{
			if (!std::strcmp(cmd, itr->first.c_str()))
			{
				itr->second();
				//Game::Com_Printf(0, "^2game find custom command :D\n");
				return;
			}
		}
	}

	bool ServerCommand::CustomServerCommandExists(const char* cmd)
	{
		for (const auto& [key, value] : ServerCommands)
		{
			if (!std::strcmp(cmd, key.c_str()))
			{
				return true;
			}
		}

		return false;
	}

	void __declspec(naked) ServerCommand::CG_DispatchServerCommandStub1()
	{
		const static uint32_t end_jump = 0x42A1C7;
		const static uint32_t original_checks = 0x42992D;
		__asm
		{
			jz		cmd_not_found;

			pushad;
			push	ebp;
			push	eax;
			call	DispatchServerCommand;
			add		esp, 8;
			popad;

			jmp		original_checks;

		cmd_not_found:
			jmp		end_jump;
		}
	}

	void __declspec(naked) ServerCommand::CG_DispatchServerCommandStub2()
	{
		const static uint32_t retn_addr = 0x42A1AB;
		const static uint32_t jmp_unknown_cmd = 0x42A1B7;
		const static uint32_t return_op = 0x42A1C7;
		__asm
		{
			//stock op's
			xor		edx, edx;
			repe	cmpsb;
			jnz     UNKNOWN_COMMAND_HOOK;
			jmp		retn_addr;

		UNKNOWN_COMMAND_HOOK:
			
			// just making another check variant here with custom container.
			pushad;
			push	eax;
			call	CustomServerCommandExists;
			add		esp, 4;
			test	al, al;
			jz		UNKNOWN_COMMAND_REAL;
			popad;
			
			jmp		return_op;
		UNKNOWN_COMMAND_REAL:
			popad;
			jmp		jmp_unknown_cmd;
		}
	}

	ServerCommand::ServerCommand()
	{
		Utils::Hook(0x429927, CG_DispatchServerCommandStub1, HOOK_JUMP).install()->quick();
		Utils::Hook::Nop(0x42A1A5, 6);
		Utils::Hook(0x42A1A5, CG_DispatchServerCommandStub2, HOOK_JUMP).install()->quick();
	}

	ServerCommand::~ServerCommand()
	{
		ServerCommands.clear();
	}
}