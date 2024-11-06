#include "STDInc.hpp"

#include <rapidjson/document.h>

namespace Components
{
	bool Updater::UpdateRestart;
	bool Updater::AutomaticUpdate;

	Utils::ConcurrentList::Container<Updater::update_data_t> Updater::update_data;

	std::string GetDLLName()
	{
		static const auto name = "game.dll";
		return name;
	}

	std::string GetExeName()
	{
		static const auto name = "ModernOps.exe";
		return name;
	}

	void Updater::SetUpdateDownloadStatus(bool done, bool success)
	{
		update_data.access([done, success](Updater::update_data_t& data_)
		{
			data_.download.done = done;
			data_.download.success = success;
		});
	}

	bool Updater::CheckFile(const std::string& name, const std::string& sha)
	{
		std::string data;

		if (name == GetDLLName() || name == GetExeName())
		{
			if (!Utils::IO::ReadFile(name, &data))
			{
				return false;
			}
		}
		else
		{
			std::filesystem::path game_folder = Dvars::Functions::Dvar_FindVar("fs_game")->current.string;
			const auto path = (game_folder / name).generic_string();

			if (!Utils::IO::ReadFile(path, &data))
			{
				return false;
			}
		}

		// Checking out our SHA file checksum
		if (Utils::Cryptography::SHA1::Compute(data, true) != sha)
		{
			return false;
		}

		return true;
	}

	bool Updater::WriteFile(const std::string& name, const std::string& data)
	{
		if (name == GetDLLName() && Utils::IO::FileExists(name) && !Utils::IO::moveFile(name, name + ".old"))
		{
			return false;
		}

		if (name == GetExeName() && Utils::IO::FileExists(name) && !Utils::IO::moveFile(name, name + ".old"))
		{
			return false;
		}

		std::filesystem::path game_folder = Dvars::Functions::Dvar_FindVar("fs_game")->current.string;
		const auto path = (game_folder / name).generic_string();
		return Utils::IO::WriteFile(path, data);
	}

	std::string Updater::get_time_str()
	{
		return Utils::String::VA("%i", uint32_t(time(nullptr)));
	}

	std::optional<std::string> Updater::DownloadFile(const std::string& name)
	{
		return Utils::HTTP::GetData(URL_MASTER + name + "?" + Updater::get_time_str(), {}, {}, true);
	}

	std::vector<std::string> Updater::FindGarbageFiles(const std::vector<std::string>& update_files)
	{
		std::vector<std::string> garbage_files{};
		std::filesystem::path game_folder = Dvars::Functions::Dvar_FindVar("fs_game")->current.string;
		const auto path = (game_folder).generic_string();
		if (!Utils::IO::DirectoryExists(path))
		{
			return {};
		}

		const auto current_files = Utils::IO::ListFiles(path);
		for (const auto& file : current_files)
		{
			bool found = false;
			for (const auto& update_file : update_files)
			{
				const auto update_file_ = (game_folder / update_file).generic_string();
				const auto path_a = std::filesystem::path(file);
				const auto path_b = std::filesystem::path(update_file_);
				const auto is_directory = Utils::IO::DirectoryExists(file);
				const auto compare = path_a.compare(path_b);

				if ((is_directory && compare == -1) || compare == 0)
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				Game::Com_Printf(0, "[Updater] Found extra file %s\n", file.data());
				if (file.ends_with(".ff"))
				{
					update_data.access([](Updater::update_data_t& data_)
					{
						data_.restart_required = true;
					});
				}

				garbage_files.push_back(file);
			}
		}
		return garbage_files;
	}

	void Updater::ResetData()
	{
		update_data.access([](Updater::update_data_t& data_)
		{
			data_ = {};
		});
	}

	void Updater::CancelUpdate()
	{
		Game::Com_Printf(0, "[Updater]: Cancelling update\n");

		return update_data.access([](Updater::update_data_t& data_)
		{
			data_.cancelled = true;
		});
	}

	bool Updater::UpdateAvailable()
	{
		return update_data.access<bool>([](Updater::update_data_t& data_)
		{
			return data_.required_files.size() > 0 || data_.garbage_files.size() > 0;
		});
	}

	bool Updater::UpdateCancelled()
	{
		return update_data.access<bool>([](Updater::update_data_t& data_)
		{
			return data_.cancelled;
		});
	}

	void Updater::CL_StartUpdate()
	{
		if (Updater::UpdateCancelled())
		{
			return;
		}

		const auto garbage_files = update_data.access<std::vector<std::string>>([](Updater::update_data_t& data_)
		{
			return data_.garbage_files;
		});

		for (const auto& file : garbage_files)
		{
			try
			{
				std::filesystem::remove_all(file);
			}
			catch (...)
			{
				Game::Com_Printf(0, "[Updater]: ^1Failed to delete %s\n", file.c_str());
			}
		}

		Scheduler::Once([]
		{
			Game::menuDef_t* menu = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MENU, "updater_download_menu").menu;

			const auto required_files = update_data.access<std::vector<Updater::file_info>>([](Updater::update_data_t& data_)
			{
				return data_.required_files;
			});

			std::vector<Updater::file_data> downloads;

			for (const auto& file : required_files)
			{
				update_data.access([&](Updater::update_data_t& data_)
				{
					data_.current_file = file.name;
				});

				menu->items[4]->text = file.name.c_str();

				Game::Com_Printf(0, "[Updater]: Downloading file %s\n", file.name.data());

				const auto data = Updater::DownloadFile(file.name);

				if (Updater::UpdateCancelled())
				{
					Updater::ResetData();
					return;
				}

				if (!data.has_value())
				{
					Updater::SetUpdateDownloadStatus(true, false);
					Game::Com_Printf(0, "[Updater]: ^1Failed to download file %s\n", file.name.c_str());
					return;
				}

				const auto& value = data.value();

				if (file.hash != Utils::Cryptography::SHA1::Compute(value, true))
				{
					Updater::SetUpdateDownloadStatus(true, false);
					Game::Com_Printf(0, "[Updater]: ^1Failed to download file %s\n", file.name.c_str());
					return;
				}

				downloads.emplace_back(file.name, data.value());
			}

			for (const auto& download : downloads)
			{
				if (!Updater::WriteFile(download.name, download.data))
				{
					Updater::SetUpdateDownloadStatus(true, false);
					Game::Com_Printf(0, "[Updater]: ^1Failed to write file %s\n", download.name.c_str());
					return;
				}
			}

			Updater::SetUpdateDownloadStatus(true, true);

			menu->items[4]->text = "";
			
			Updater::ResetData();
			Updater::UpdateRestart = true;

			Command::Execute("closemenu updater_download_menu", false);
			Command::Execute("openmenu updater_restart", false);
		}, Scheduler::Pipeline::ASYNC);
	}

	void Updater::CL_GetAutoUpdate(bool needCheck)
	{
		if (needCheck)
		{
			Updater::AutomaticUpdate = true;
			// If used disable the auto-update then just exit from function.
			if (!Dvars::Functions::Dvar_FindVar("modern_ops_auto_update") || !Dvars::Functions::Dvar_FindVar("modern_ops_auto_update")->current.enabled)
				return;
		}
		else Updater::AutomaticUpdate = false;

		CancelUpdate();
		ResetData();

		Scheduler::Once([]
		{
			const auto files_data = Utils::HTTP::GetData(URL_MASTER + "files.json"s, {}, {}, false);
			
			if (UpdateCancelled())
			{
				ResetData();
				return;
			}

			// �heck the result of our existence request json list or when our request cannot reach the server, then drop error.
			if (!files_data.has_value())
			{
				Game::Com_Printf(0, "[Updater]: ^1json file is empty or HTTP request can't be reach to the server. Abort The Update.\n");

				if (!Updater::AutomaticUpdate)
				{
					Command::Execute("closemenu updater_checking_for_updates", false);
					Command::Execute("closemenu updater_checking_for_updates_internal", false);
					Command::Execute("openmenu updater_server_offline", false);
				}

				return;
			}

			rapidjson::Document j;
			j.Parse(files_data.value().data());

			if (!j.IsArray())
			{
				SetUpdateDownloadStatus(true, false);
				Game::Com_Printf(0, "[Updater]: ^1json file doesn't have array list. Abort The Update.\n");
				return;
			}

			std::vector<file_info> required_files;
			std::vector<std::string> update_files;

			const auto files = j.GetArray();
			for (const auto& file : files)
			{
				if (!file.IsArray() || file.Size() != 3 || !file[0].IsString() || !file[2].IsString())
				{
					continue;
				}

				const auto name = file[0].GetString();
				const auto sha = file[2].GetString();

				update_files.push_back(name);

				if (!CheckFile(name, sha))
				{
					std::string name_ = name;

					if (name_.contains(GetDLLName()) || name_.contains(GetExeName()))
					{
						update_data.access([](update_data_t& data_)
						{
							data_.restart_required = true;
						});
					}

					if (name_.ends_with(".ff"))
					{
						update_data.access([](update_data_t& data_)
						{
							data_.restart_required = true;
						});
					}

					Game::Com_Printf(0, "[Updater]: ^3File which need download - %s\n", name);

					required_files.emplace_back(name, sha);
				}
			}

			const auto garbage_files = FindGarbageFiles(update_files);

			update_data.access([&](update_data_t& data_)
			{
				data_.check.done = true;
				data_.check.success = true;
				data_.required_files = required_files;
				data_.garbage_files = garbage_files;
			});

			if (UpdateAvailable())
			{
				Command::Execute("closemenu updater_checking_for_updates", false);
				Command::Execute("closemenu updater_checking_for_updates_internal", false);
				Command::Execute("openmenu updater_popmenu", false);
			}
			else
			{
				if (!Updater::AutomaticUpdate)
				{
					Command::Execute("closemenu updater_checking_for_updates", false);
					Command::Execute("closemenu updater_checking_for_updates_internal", false);
					Command::Execute("openmenu updater_new_updates_no_found", false);
				}
			}
		}, Scheduler::Pipeline::ASYNC);
	}

	void Updater::DeleteOldFiles()
	{
		Utils::IO::RemoveFile("__ModernOps");
		Utils::IO::RemoveFile(GetDLLName() + ".old");
		Utils::IO::RemoveFile(GetExeName() + ".old");
	}

	Updater::Updater()
	{
		// Not used.
		/*
		Scheduler::Once([]
		{
			// Deleting our garbage files after auto-updating :>
			Updater::DeleteOldFiles();
		}, Scheduler::Pipeline::MAIN);

		Events::OnDvarInit([]
		{
			Game::dvar_s* cl_auto_update = Dvars::Register::Dvar_RegisterBool("modern_ops_auto_update", "Enable automatic update checks on launch.", true, Game::saved);
		});

		UIScript::Add("update_download_cancel", []([[maybe_unused]] const UIScript::Token& token, [[maybe_unused]] const Game::uiInfo_s* info)
		{
			Updater::CancelUpdate();
		});

		UIScript::Add("check_avaliable_updates_internal", []([[maybe_unused]] const UIScript::Token& token, [[maybe_unused]] const Game::uiInfo_s* info)
		{
			Updater::CL_GetAutoUpdate(false);
		});

		UIScript::Add("check_avaliable_updates", []([[maybe_unused]] const UIScript::Token& token, [[maybe_unused]] const Game::uiInfo_s* info)
		{
			Updater::CL_GetAutoUpdate(true);
		});

		UIScript::Add("update_start_download", []([[maybe_unused]] const UIScript::Token& token, [[maybe_unused]] const Game::uiInfo_s* info)
		{
			Updater::CL_StartUpdate();
		});
		*/
	}

	Updater::~Updater()
	{

	}
}