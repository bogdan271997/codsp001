#include "STDInc.hpp"
#include <random>
namespace Components
{
	bool Paintball::PB_Enabled()
	{
		return !Game::HasLoadedMod() && Dvars::paintball_mode->current.enabled;
	}

	const Game::FxEffectDef* Paintball::GetEffect()
	{
		const std::vector<const Game::FxEffectDef*> effects = {
			Game::cgMediaMod.fxPaintBallGreen,
			Game::cgMediaMod.fxPaintBallOrange,
			Game::cgMediaMod.fxPaintBallBlue,
			Game::cgMediaMod.fxPaintBallRed,
			Game::cgMediaMod.fxPaintBallYellow
		};

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distr(0, effects.size() - 1);

		int randomIndex = distr(gen);

		return effects[randomIndex];
	}

	const Game::FxEffectDef* Paintball::GetExplosiveEffect()
	{
		const std::vector<const Game::FxEffectDef*> effects = {
			Game::cgMediaMod.fxPaintBallExpGreen,
			Game::cgMediaMod.fxPaintBallExpOrange
		};

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distr(0, effects.size() - 1);

		int randomIndex = distr(gen);

		return effects[randomIndex];
	}

	void Paintball::FireSound(Game::snd_alias_list_t* aliasList/*eax*/, int localClientNum, float* origin)
	{
		Game::snd_alias_list_t* soundAlias;
		if (PB_Enabled())
		{
			std::string soundAliasName = aliasList->aliasName;
			if (soundAliasName.ends_with("_plr"))
				soundAlias = Game::cgMediaMod.paintBallFirePlayer;
			else
				soundAlias = Game::cgMediaMod.paintBallFireNPC;
		}
		else
			soundAlias = aliasList;

		Game::CG_PlayClientSoundAlias(soundAlias, localClientNum, origin);
	}

	void Paintball::ImpactSound(Game::snd_alias_list_t* aliasList/*eax*/, int localClientNum, float* origin)
	{
		Game::snd_alias_list_t* soundAlias;
		if (PB_Enabled())
		{
			// Apply effect for bullets only!
			std::string soundAliasName = aliasList->aliasName;
			if (soundAliasName.starts_with("bullet_"))
				soundAlias = Game::cgMediaMod.paintBallImpact;
			else
				soundAlias = aliasList;
		}
		else
			soundAlias = aliasList;

		Game::CG_PlayClientSoundAlias(soundAlias, localClientNum, origin);
	}

	void Paintball::ImpactEffect(const Game::FxEffectDef* fxDef)
	{
		const Game::FxEffectDef* effect = PB_Enabled() ? GetEffect() : fxDef;
		unsigned int weaponIndex = Game::ps->weapon;
		Game::CG_PlayBoltedEffect(Game::cgs->localClientNum,
			Game::scr_const->tag_knife_fx,
			weaponIndex + 2176,
			effect);
	}

	void Paintball::ImpactMark(int axis/*edx*/, int markEntnum/*ecx*/, const Game::FxEffectDef* fxDef, int msecBegin, const float* origin)
	{
		const Game::FxEffectDef* effect = PB_Enabled() ? GetEffect() : fxDef;
		Game::FX_SpawnOrientedEffect(axis, markEntnum, effect, msecBegin, origin);
	}

	void Paintball::ImpactExplosion(const float* origin/*edx*/, int axis/*ecx*/, const Game::FxEffectDef* fxDef, int time)
	{
		const Game::FxEffectDef* effectExplosive = PB_Enabled() ? GetExplosiveEffect() : fxDef;
		Game::DynEntCl_PlayEventFx(origin, axis, effectExplosive, time);
	}

	void __declspec(naked) Paintball::CG_PlayClientSoundAliasStub1()
	{
		const static uint32_t retn_addr = 0x434776;
		__asm
		{
			push	eax;
			call	FireSound;
			add		esp, 4;
			jmp		retn_addr;
		}
	}

	void __declspec(naked) Paintball::CG_PlayClientSoundAliasStub2()
	{
		const static uint32_t retn_addr = 0x43536B;
		__asm
		{
			push	eax;
			call	ImpactSound;
			add		esp, 4;
			jmp		retn_addr;
		}
	}

	void __declspec(naked) Paintball::CG_PlayBoltedEffectStub()
	{
		const static uint32_t retn_addr = 0x435510;
		__asm
		{
			call	ImpactEffect
			jmp		retn_addr;
		}
	}

	void __declspec(naked) Paintball::FX_CreateImpactMarkStub()
	{
		const static uint32_t retn_addr = 0x435404;
		__asm
		{
			//mov     ecx, [esp + 4Ch];
			push	ecx;

			//lea     edx, [esp + 20h];
			push	edx;

 			call	ImpactMark
			add		esp, 0x8;
			jmp		retn_addr;
		}
	}

	//void DynEntCl_PlayEventFx(const float* origin/*edx*/, int axis/*ecx*/, const Game::FxEffectDef* def, int time)
	void __declspec(naked) Paintball::DynEntCl_PlayEventFxStub()
	{
		const static uint32_t retn_addr = 0x419A0F;
		__asm
		{
			push	ecx;
			push	edx;
			call	ImpactExplosion;
			add		esp, 0x8;
			jmp		retn_addr;
		}
	}

	Paintball::Paintball()
	{
		Events::OnDvarInit([]
		{
			Dvars::paintball_mode = Dvars::Register::Dvar_RegisterBool("paintball_mode", "Enables/Disables the paintball mode", false, Game::saved_flag);
		});

		Events::RegisterGraphic([]
		{
			if(Game::HasLoadedMod())
				return;
			// Bullets
			Game::cgMediaMod.fxPaintBallGreen = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_FX, "impacts/flesh_hit_pball_green").fx;
			Game::cgMediaMod.fxPaintBallOrange = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_FX, "impacts/flesh_hit_pball_orange").fx;
			Game::cgMediaMod.fxPaintBallBlue = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_FX, "impacts/flesh_hit_pball_blue").fx;
			Game::cgMediaMod.fxPaintBallRed = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_FX, "impacts/flesh_hit_pball_red").fx;
			Game::cgMediaMod.fxPaintBallYellow = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_FX, "impacts/flesh_hit_pball_yellow").fx;
			// Explosion
			Game::cgMediaMod.fxPaintBallExpGreen = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_FX, "explosions/exp_paintball_green").fx;
			Game::cgMediaMod.fxPaintBallExpOrange = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_FX, "explosions/exp_paintball_orange").fx;
		});

		Events::RegisterSound([]
		{
			if(Game::HasLoadedMod())
				return;
			Game::cgMediaMod.paintBallFirePlayer = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_SOUND, "weap_paintball_fire_plr").sound;
			Game::cgMediaMod.paintBallFireNPC = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_SOUND, "weap_paintball_fire_npc").sound;
			Game::cgMediaMod.paintBallImpact = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_SOUND, "bulletimpact_paintball").sound;
		});

		// Hook the soundalias in 'CG_FireWeapon'
		Utils::Hook(0x434771, CG_PlayClientSoundAliasStub1, HOOK_JUMP).install()->quick();

		// Hook the impact sound effect in 'CG_BulletHitEvent_Internal'
		Utils::Hook(0x435366, CG_PlayClientSoundAliasStub2, HOOK_JUMP).install()->quick();

		// Hook the impact in 'CG_EntityEvent'
		Utils::Hook(0x43550B, CG_PlayBoltedEffectStub, HOOK_JUMP).install()->quick();

		// Hook the impact in 'FX_SpawnOrientedEffect'
		Utils::Hook(0x4353FF, FX_CreateImpactMarkStub, HOOK_JUMP).install()->quick();

		// Hook the explosion impact for grenade launchers, grenades, c4 and claymore in 'CG_EntityEvent'
		Utils::Hook(0x419A0A, DynEntCl_PlayEventFxStub, HOOK_JUMP).install()->quick();
	}

	Paintball::~Paintball()
	{
	}
}