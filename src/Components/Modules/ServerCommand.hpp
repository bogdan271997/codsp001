#pragma once

namespace Components
{
	class ServerCommand : public Component
	{
	public:
		ServerCommand();
		~ServerCommand();

		static void Add(const char* name, const std::function<void()>& callback);
	private:
		static void DispatchServerCommand(const char* cmd, int cmdArgCount);
		static std::unordered_map<std::string, std::function<void()>> ServerCommands;
		static void CG_DispatchServerCommandStub1();
		static void CG_DispatchServerCommandStub2();
		static bool CustomServerCommandExists(const char* cmd);
	};
}