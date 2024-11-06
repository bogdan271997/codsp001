#pragma once

namespace Components
{
	class Paintball : public Component
	{
	public:
		Paintball();
		~Paintball();
	private:
		static bool PB_Enabled();
		static const Game::FxEffectDef* GetEffect();
		static const Game::FxEffectDef* GetExplosiveEffect();
		static void FireSound(Game::snd_alias_list_t* aliasList/*eax*/, int localClientNum, float* origin);
		static void ImpactSound(Game::snd_alias_list_t* aliasList/*eax*/, int localClientNum, float* origin);
		static void ImpactEffect(const Game::FxEffectDef* fxDef);
		static void ImpactMark(int axis/*edx*/, int markEntnum/*ecx*/, const Game::FxEffectDef* fxDef, int msecBegin, const float* origin);
		static void ImpactExplosion(const float* origin/*edx*/, int axis/*ecx*/, const Game::FxEffectDef* fxDef, int time);

		static void CG_PlayClientSoundAliasStub1();
		static void CG_PlayClientSoundAliasStub2();
		static void CG_PlayBoltedEffectStub();
		static void FX_CreateImpactMarkStub();
		static void DynEntCl_PlayEventFxStub();
	};
}