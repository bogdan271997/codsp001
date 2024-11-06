#pragma once

#include "Commands.hpp"
#include "Common.hpp"
#include "Database.hpp"
#include "Functions.hpp"
#include "Scripts.hpp"
#include "System.hpp"

namespace Game
{
	struct cgMedia_t_mod
	{
		// Paintball assets
		const FxEffectDef* fxPaintBallGreen;
		const FxEffectDef* fxPaintBallOrange;
		const FxEffectDef* fxPaintBallBlue;
		const FxEffectDef* fxPaintBallRed;
		const FxEffectDef* fxPaintBallYellow;
		const FxEffectDef* fxPaintBallExpGreen;
		const FxEffectDef* fxPaintBallExpOrange;
		snd_alias_list_t* paintBallFirePlayer;
		snd_alias_list_t* paintBallFireNPC;
		snd_alias_list_t* paintBallImpact;
	};

	extern Game::cgMedia_t_mod cgMediaMod;
	extern Game::gui_t gui;

	extern IDirect3DDevice9* d3d9_device;

	bool HasLoadedMod();
	bool DebugModeEnabled();
}