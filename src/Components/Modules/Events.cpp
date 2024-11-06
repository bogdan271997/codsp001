#include "STDInc.hpp"

namespace Components
{
	Utils::ConcurrentList::Container<Events::Callback> Events::DvarInitTasks_;
	Utils::ConcurrentList::Container<Events::Callback> Events::ShutdownSystemTasks_;
	Utils::ConcurrentList::Container<Events::ScriptShutdownCallback> Events::ScriptsShutdownTasks_;
	Utils::ConcurrentList::Container<Events::Callback> Events::OnMapLoadTasks_;
	Utils::ConcurrentList::Container<Events::Callback> Events::RegisterGraphic_;
	Utils::ConcurrentList::Container<Events::Callback> Events::RegisterSound_;

	void Events::OnDvarInit(const std::function<void()>& callback)
	{
		DvarInitTasks_.access([&callback](Callback& tasks)
		{
			tasks.emplace_back(callback);
		});
	}

	void Events::OnVMShutdown(const std::function<void()>& callback)
	{
		ShutdownSystemTasks_.access([&callback](Callback& tasks)
		{
			tasks.emplace_back(callback);
		});
	}

	void Events::OnScriptsShutdown(const std::function<void(int clearScripts)>& callback)
	{
		ScriptsShutdownTasks_.access([&callback](ScriptShutdownCallback& tasks)
		{
			tasks.emplace_back(callback);
		});
	}

	void Events::OnMapLoad(const std::function<void()>& callback)
	{
		OnMapLoadTasks_.access([&callback](Callback& tasks)
		{
			tasks.emplace_back(callback);
		});
	}

	void Events::RegisterGraphic(const std::function<void()>& callback)
	{
		RegisterGraphic_.access([&callback](Callback& tasks)
		{
			tasks.emplace_back(callback);
		});
	}

	void Events::RegisterSound(const std::function<void()>& callback)
	{
		RegisterSound_.access([&callback](Callback& tasks)
		{
			tasks.emplace_back(callback);
		});
	}

	void Events::Com_InitDvars_stub()
	{
		DvarInitTasks_.access([](Callback& tasks)
		{
			for (const auto& func : tasks)
			{
				func();
			}

			tasks = {}; // Only called once. Clear
		});

		Utils::Hook::Call<void()>(0x534630)(); // Com_InitDvars
	}

	void Events::Scr_ShutdownSystem_Hk(unsigned char bComplete)
	{
		ShutdownSystemTasks_.access([](Callback& tasks)
		{
			for (const auto& func : tasks)
			{
				func();
			}
		});

		Utils::Hook::Call<void(unsigned char)>(0x557D00)(bComplete); // Scr_ShutdownSystem
	}

	void Events::Scr_OnMapLoadStub(int channel, const char* message, const char* format)
	{
		OnMapLoadTasks_.access([](Callback& tasks)
		{
			for (const auto& func : tasks)
			{
				func();
			}
		});

		Game::Com_Printf(channel, message, format);
	}

	void Events::SV_ShutdownGameVMStub(int clearScripts)
	{
		ScriptsShutdownTasks_.access([&clearScripts](ScriptShutdownCallback& tasks)
		{
			for (const auto& func : tasks)
			{
				func(clearScripts);
			}
		});

		Game::SV_ShutdownGameVM(clearScripts);
	}

	void __declspec(naked) Events::SV_ShutdownGameVM_Hk()
	{
		__asm
		{
			pushad;
			push	edi;
			call	SV_ShutdownGameVMStub;
			add		esp, 4;
			popad;

			ret;
		}
	}

	void Events::CG_InitStub(const char* mapname)
	{
		RegisterGraphic_.access([](Callback& tasks)
		{
			for (const auto& func : tasks)
			{
				func();
			}		
		});

		Utils::Hook::Call<void(const char*)>(0x41FE10)(mapname); // CG_RegisterGraphics
	}

	void Events::SV_SpawnServerStub()
	{
		RegisterSound_.access([](Callback& tasks)
		{
			for (const auto& func : tasks)
			{
				func();
			}
		});

		Utils::Hook::Call<void()>(0x41F8F0)(); //CG_RegisterSounds
	}

	Events::Events()
	{
		Utils::Hook(0x534B15, Com_InitDvars_stub, HOOK_CALL).install()->quick();

		Utils::Hook(0x4C5478, Scr_ShutdownSystem_Hk, HOOK_CALL).install()->quick(); // G_LoadGame
		Utils::Hook(0x4B9C13, Scr_ShutdownSystem_Hk, HOOK_CALL).install()->quick(); // G_ShutdownGame

		Utils::Hook(0x5C4BB4, SV_ShutdownGameVM_Hk, HOOK_CALL).install()->quick();
		Utils::Hook(0x5C657C, SV_ShutdownGameVM_Hk, HOOK_CALL).install()->quick();

		Utils::Hook(0x5C7748, Scr_OnMapLoadStub, HOOK_CALL).install()->quick();

		// For custom object 'cgMediaMod'
		Utils::Hook(0x421011, CG_InitStub, HOOK_CALL).install()->quick();
		Utils::Hook(0x5C7638, SV_SpawnServerStub, HOOK_CALL).install()->quick();
	}

	Events::~Events()
	{
	}
}