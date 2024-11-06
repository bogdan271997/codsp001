#include "STDInc.hpp"

namespace Game
{
	Game::cgMedia_t_mod cgMediaMod = {};
	Game::gui_t gui = {};

	IDirect3DDevice9* d3d9_device = nullptr;

	bool HasLoadedMod()
	{
		std::string modDir = Dvars::Functions::Dvar_FindVar("fs_game")->current.string;
		return !modDir.empty();
	}

	bool DebugModeEnabled()
	{
		return Components::Flags::HasFlag("debug");
	}
}