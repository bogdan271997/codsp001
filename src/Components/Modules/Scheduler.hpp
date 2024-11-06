#pragma once
#include "Utils/ConcurrentList.hpp"

namespace Components
{
	class Scheduler : public Component
	{
	public:
		enum Pipeline : int
		{
			MAIN,
			RENDERER,
			ASYNC,
			QUIT,
			COUNT,
		};

		enum evaluation : bool
		{
			reschedule = false,
			remove = true,
		};

		Scheduler();

		void preDestroy() override;

		static void Schedule(const std::function<bool()>& callback, Pipeline type,
			std::chrono::milliseconds delay = 0ms);
		static void Loop(const std::function<void()>& callback, Pipeline type,
			std::chrono::milliseconds delay = 0ms);
		static void Once(const std::function<void()>& callback, Pipeline type,
			std::chrono::milliseconds delay = 0ms);
		static void OnGameInitialized(const std::function<void()>& callback, Pipeline type,
			std::chrono::milliseconds delay = 0ms);
		static void OnGameShutdown(const std::function<void()>& callback);

		static void error(const std::string& message, int level);

	private:

		struct Task
		{
			std::function<bool()> handler{};
			std::chrono::milliseconds interval{};
			std::chrono::high_resolution_clock::time_point lastCall{};
		};

		using taskList = std::vector<Task>;

		class TaskPipeline
		{
		public:
			void add(Task&& task);
			void execute();

		private:
			Utils::ConcurrentList::Container<taskList> newCallbacks_;
			Utils::ConcurrentList::Container<taskList, std::recursive_mutex> callbacks_;

			void mergeCallbacks();
		};

		static volatile bool Kill;
		static std::thread Thread;
		static TaskPipeline Pipelines[];

		static void Execute(Pipeline type);

		static std::mutex mutex_;
		static std::queue<std::pair<std::string, int>> errors_;
		//static Utils::ConcurrentList<std::pair<std::function<void()>, thread>> callbacks_;
		//static Utils::ConcurrentList<std::pair<std::function<void()>, thread>> single_callbacks_;
		//static Utils::ConcurrentList<std::pair<std::function<Scheduler::evaluation()>, thread>> condition_callbacks_;

		static void MainFrameStub();
		static void RenderFrameStub(int index);
		static void SysQuitStub(int index);
	};
}