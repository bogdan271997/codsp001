#pragma once

namespace Components
{
	class Viewmodel : public Component
	{
	public:
		static int GetCurrentWeaponAnimIndex();
		static int GetCurrentRealWeaponAnimIndex();
		static float GetCurrentWeaponAdsProgress();
		void PM_StartWeaponAnim(Game::playerState_s *ps, int weapAnim);
		bool ViewModelOverride(Game::playerState_s *ps, Game::pml_t* pml);
		static int GetViewmodelWeaponIndex(int ps);
		Viewmodel();
	private:
	};
}