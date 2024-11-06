#pragma once

namespace Components
{
	class PlayerStats : public Component
	{
	public:
		static std::string JsonToString(const nlohmann::json& j)
		{
			return j.dump();
		}

		static nlohmann::json Get(const std::string& key)
		{
			const auto cfg = ReadPlayerStats();
			if (!cfg.is_object() || !cfg.contains(key) || !cfg[key].is_null())
			{
				Set(key, defaultValue);
				return cfg[key];
			}

			return cfg[key];
		}

		static nlohmann::json GetStruct(const std::string& name, const std::string& field)
		{
			const auto cfg = ReadPlayerStats();
			if (!cfg.is_object() || !cfg.contains(name) || !cfg[name].is_object() || !cfg[name].contains(field) || cfg[name][field].is_null())
			{
				SetStruct(name, field, defaultValue); // You may want to handle default value differently
				return defaultValue;
			}

			return cfg[name][field];
		}

		static void Set(const std::string& key, const nlohmann::json& value)
		{
			auto cfg = ReadPlayerStats();
			cfg[key] = value;
			WriteData(cfg);
		}

		static void SetStruct(const std::string& name, const std::string& field, const nlohmann::json& value)
		{
			auto cfg = ReadPlayerStats();
			cfg[name][field] = value;
			WriteData(cfg);
		}

		PlayerStats();
		~PlayerStats();
	private:
		inline static int defaultValue = 0;

		static uint32_t CalculateSeconds(uint32_t totalMilliseconds);
		static uint32_t CalculateMinutes(uint32_t totalMilliseconds);
		static uint32_t CalculateHours(uint32_t totalMilliseconds);
		static uint32_t CalculateDays(uint32_t totalMilliseconds);

		static std::string GetPlayerStatsPath();
		static nlohmann::json ReadPlayerStats();
		static void WriteData(const nlohmann::json& json);
		static void ResetData();
		static void DefaultStats();
		static void PlayTimeStart();
		static void ChangeItemDefText(const char* menuName, const char* itemDefName, const char* value);
		static const char* getHighestCompletedDifficulty(const float stat_easy, const float stat_regular, const float stat_hardened, const float stat_veteran);
		static double CalculateAccuracy(uint32_t bullets_fired, uint32_t bullets_hit);
		static float getStatProgression(const std::string& difficulty_string, int difficulty);
		static float getStatEasy(const std::string& difficulty_string);
		static float getStatRegular(const std::string& difficulty_string);
		static float getStatHardened(const std::string& difficulty_string);
		static float getStatVeteran(const std::string& difficulty_string);
		static float getStatIntel(int intelCount, int totalIntelItems);
		static int getTotalPercentCompleteSP();

		static void Com_Init_Try_Block_Function_Stub();
	};
}