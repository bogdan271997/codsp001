#pragma once

namespace Components
{
	class Discord : public Component
	{
		struct DiscordCustomInformation_t
		{
			std::optional<std::string> state{};
			std::optional<std::string> details{};
			std::optional<std::string> large_image_key{};
			std::pair<std::string, std::string> button{};
		};
	public:
		Discord();
		~Discord();
		static void DiscordUpdate();
		static void	DiscordInit();
		static void	DiscordShutdown();

		static DiscordCustomInformation_t DiscordCustomInformation;
		static inline bool EnableDiscord_RPC = true;

	private:

	};
}