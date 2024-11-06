#pragma once

namespace Components
{
	class Events : public Component
	{
	public:
		using Callback = std::vector<std::function<void()>>;
		using ScriptShutdownCallback = std::vector<std::function<void(int clearScripts)>>;

		Events();
		~Events();

		static void OnDvarInit(const std::function<void()>& callback);
		static void OnScriptsShutdown(const std::function<void(int clearScripts)>& callback);
		static void OnVMShutdown(const std::function<void()>& callback);
		static void OnMapLoad(const std::function<void()>& callback);
		static void RegisterGraphic(const std::function<void()>& callback);
		static void RegisterSound(const std::function<void()>& callback);
	private:
		static Utils::ConcurrentList::Container<Callback> DvarInitTasks_;
		static Utils::ConcurrentList::Container<ScriptShutdownCallback> ScriptsShutdownTasks_;
		static Utils::ConcurrentList::Container<Callback> ShutdownSystemTasks_;
		static Utils::ConcurrentList::Container<Callback> OnMapLoadTasks_;
		static Utils::ConcurrentList::Container<Callback> RegisterGraphic_;
		static Utils::ConcurrentList::Container<Callback> RegisterSound_;
		
		static void Com_InitDvars_stub();
		static void CG_InitStub(const char* mapname);
		static void SV_SpawnServerStub();
		static void Scr_OnMapLoadStub(int channel, const char* message, const char* format);
		static void Scr_ShutdownSystem_Hk(unsigned char sys);
		static void SV_ShutdownGameVM_Hk();
		static void SV_ShutdownGameVMStub(int clearScripts);
	};
}