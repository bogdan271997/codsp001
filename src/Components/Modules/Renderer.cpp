#include "STDInc.hpp"

namespace Components
{
	Utils::Signal<Renderer::BackendCallback> Renderer::BackendFrameSignal;
	Utils::Signal<Renderer::BackendCallback> Renderer::SingleBackendFrameSignal;

	Utils::Signal<Renderer::Callback> Renderer::EndRecoverDeviceSignal;
	Utils::Signal<Renderer::Callback> Renderer::BeginRecoverDeviceSignal;


	void Renderer::PreVidRestart()
	{
		Renderer::BeginRecoverDeviceSignal();
	}

	void Renderer::PostVidRestart()
	{
		Renderer::EndRecoverDeviceSignal();
	}

	int Renderer::Width()
	{
		return reinterpret_cast<LPPOINT>(0x1621DB4)->x;
	}

	int Renderer::Height()
	{
		return reinterpret_cast<LPPOINT>(0x1621DB4)->y;
	}

	__declspec(naked) void Renderer::PostPreVidRestart()
	{
		__asm
		{
			pushad
			call Renderer::PreVidRestart
			popad

			push 535E80h
			retn
		}
	}

	__declspec(naked) void Renderer::PostVidRestartStub()
	{
		__asm
		{
			pushad
			call Renderer::PostVidRestart
			popad

			push 445390h
			retn
		}
	}

	float Renderer::CG_GetViewFov(int localClientNum)
	{
		float calc_fov = Dvars::cg_fov->current.value;
		unsigned int offhand_index = Game::ps->offHandIndex;
		if ((Game::ps->weapFlags & 2) == 0)
		{
			offhand_index = Game::ps->weapon;
		}

		Game::WeaponDef_s* weapon = Game::bg_weaponDefs[offhand_index];

		auto check_flags_and_fovmin = [&]() -> float
		{
			if ((Game::ps->eFlags & 0x300) != 0)
			{
				if (Game::ps->viewlocked)
				{
					auto func = Utils::Hook::Call<int()>(0x4356B0)();
					if (Game::bg_weaponDefs[func]->overlayInterface == Game::WEAPOVERLAYINTERFACE_TURRETSCOPE)
						return calc_fov = Dvars::Functions::Dvar_FindVar("turretScopeZoom")->current.value;
				}
				return calc_fov = 55.0f;
			}

			calc_fov = Dvars::cg_fovScale->current.value * calc_fov;
			calc_fov = Dvars::cg_fovMenu->current.value * calc_fov;// Menu option.
			float calc_fov_min = Dvars::cg_fovMin->current.value - calc_fov;

			if (calc_fov_min >= 0.0)
				return Dvars::cg_fovMin->current.value;

			//	From MW2
			if (calc_fov > 170.0f)
				return calc_fov = 170.0f;

			return calc_fov;
		};

		if (Game::ps->pm_type == 5)
		{
			calc_fov = 90.0f;
			return check_flags_and_fovmin();
		}

		if (weapon->aimDownSight)
		{
			if (Game::ps->fWeaponPosFrac == 1.0f)
			{
				calc_fov = weapon->fAdsZoomFov;
				return check_flags_and_fovmin();
			}

			if (0.0f != Game::ps->fWeaponPosFrac)
			{
				float ads_factor;

				if (Game::cgs->playerEntity.bPositionToADS)
				{
					const float w_pos_frac = Game::ps->fWeaponPosFrac - (1.0f - weapon->fAdsZoomInFrac);
					if (w_pos_frac <= 0.0f)
					{
						return check_flags_and_fovmin();
					}
					ads_factor = w_pos_frac / weapon->fAdsZoomInFrac;
				}
				else
				{
					const float w_pos_frac = Game::ps->fWeaponPosFrac - (1.0f - weapon->fAdsZoomOutFrac);
					if (w_pos_frac <= 0.0f)
					{
						return check_flags_and_fovmin();
					}
					ads_factor = w_pos_frac / weapon->fAdsZoomOutFrac;
				}

				if (ads_factor > 0.0f)
				{
					calc_fov = calc_fov - ads_factor * (calc_fov - weapon->fAdsZoomFov);
				}
			}
		}

		return check_flags_and_fovmin();
	}

	IDirect3DDevice9* Renderer::GetDirectX9Device()
	{
		if (Game::d3d9_device)
		{
			return Game::d3d9_device;
		}

		return *Game::dx9_device_ptr;
	}

	// R_AllocDynamicIndexBuffer
	int Renderer::AllocDynamicIndexBuffer(IDirect3DIndexBuffer9** ib, int size_in_bytes, const char* buffer_name, bool load_for_renderer)
	{
		if (!load_for_renderer)
		{
			return 0;
		}

		HRESULT hr = GetDirectX9Device()->CreateIndexBuffer(size_in_bytes, (D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY), D3DFMT_INDEX16, D3DPOOL_DEFAULT, ib, nullptr);
		if (hr < 0)
		{
			const char* msg = Utils::function<const char* __stdcall(HRESULT)>(0x685F98)(hr); // R_ErrorDescription v1.0 0x652ECE
			msg = Utils::String::VA("DirectX didn't create a 0x%.8x dynamic index buffer: %s\n", size_in_bytes, msg);

			Utils::function<void(const char*)>(0x5951C0)(msg); // Sys_Error v1.0 0x594AC0
		}
		return 0;
	}

	void Renderer::AllocDynamicVertexBuffer()
	{
		auto& gfx_buf = *reinterpret_cast<Game::GfxBuffers*>(0x1AF5EB8);// v1.0 0x1AF5E98
		const auto& r_loadForRenderer = Dvars::Functions::Dvar_FindVar("r_loadForRenderer")->current.enabled;

		// default size in bytes
		std::uint32_t smodel_cache_vb_size = 8 * 9000000; //1048576;;

		if (r_loadForRenderer)
		{
			HRESULT hr = GetDirectX9Device()->CreateVertexBuffer(smodel_cache_vb_size, (D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY), 0, D3DPOOL_DEFAULT, &gfx_buf.smodelCacheVb, 0);
			if (hr < 0)
			{
				const char* msg = Utils::function<const char* __stdcall(HRESULT)>(0x65350E)(hr); // R_ErrorDescription
				msg = Utils::String::VA("DirectX didn't create a 0x%.8x dynamic vertex buffer: %s\n", smodel_cache_vb_size, msg);

				Utils::function<void(const char*)>(0x5951C0)(msg); // Sys_Error v1.0 0x594AC0
			}

#ifdef DEVELOPER_MODE
			Game::Com_Printf(0, Utils::String::VA("D3D9: Created Vertexbuffer (smodelCacheVb) of size: 0x%.8x\n", smodel_cache_vb_size));
#endif
		}
	}

	// R_AllocDynamicVertexBuffer
	char* Renderer::AllocDynamicVertexBuffer(IDirect3DVertexBuffer9** vb, int size_in_bytes, const char* buffer_name, bool load_for_renderer)
	{
		if (!load_for_renderer)
		{
			return nullptr;
		}

		HRESULT hr = GetDirectX9Device()->CreateVertexBuffer(size_in_bytes, (D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY), 0, D3DPOOL_DEFAULT, vb, 0);
		if (hr < 0)
		{
			const char* msg = Utils::function<const char* __stdcall(HRESULT)>(0x65350E)(hr); // R_ErrorDescription v1.0 0x652ECE
			msg = Utils::String::VA("DirectX didn't create a 0x%.8x dynamic vertex buffer: %s\n", size_in_bytes, msg);

			Utils::function<void(const char*)>(0x5951C0)(msg); // Sys_Error v1.0 0x594AC0
		}

#ifdef DEVELOPER_MODE
		Game::Com_Printf(0, Utils::String::VA("D3D9: Created Vertexbuffer (%s) of size: 0x%.8x\n", buffer_name, size_in_bytes));
#endif
		return nullptr;
	}

	// R_InitDynamicVertexBufferState
	void Renderer::InitDynamicVertexBufferState(Game::GfxVertexBufferState* vb, int bytes, const char* buffer_name, bool load_for_renderer)
	{
		vb->used = 0;
		vb->total = bytes;
		vb->verts = 0;

		AllocDynamicVertexBuffer(&vb->buffer, bytes, buffer_name, load_for_renderer);
	}

	// R_InitTempSkinBuf :: Temp skin buffer within backenddata
	void Renderer::InitTempSkinBuf(int bytes)
	{
		auto* back_end_data = reinterpret_cast<Game::GfxBackEndData*>(0x1629100);

		for (auto i = 0; i < 2; ++i)
		{
			back_end_data[i].tempSkinBuf = (char*)VirtualAlloc(nullptr, bytes, MEM_RESERVE, PAGE_READWRITE);
#ifdef DEVELOPER_MODE
			Game::Com_Printf(0, Utils::String::VA("Allocated tempSkinBuffer of size: 0x%.8x\n", bytes));
#endif
		}
	}

	// R_InitDynamicIndexBufferState
	void Renderer::InitDynamicIndexBufferState(Game::GfxIndexBufferState* ib, int indexCount, const char* buffer_name, bool loadForRenderer)
	{
		ib->used = 0;
		ib->total = indexCount;

		AllocDynamicIndexBuffer(&ib->buffer, 2 * indexCount, buffer_name, loadForRenderer);
	}

	/* ---------------------------------------------------------- */
	/* ---------- Alloc dynamic indices (smodelCache) ----------- */

	void Renderer::InitSmodelIndices()
	{
		auto& gfx_buf = *reinterpret_cast<Game::GfxBuffers*>(0x1AF5EB8);// v1.0 0x1AF5E98

		// default size in bytes
		std::uint32_t smodel_cache_ib_size = 2 * 9000000; //1048576;;

		gfx_buf.smodelCache.used = 0;
		gfx_buf.smodelCache.total = static_cast<int>(smodel_cache_ib_size / 2); // why half?

		const auto mem_reserve = VirtualAlloc(nullptr, 0x200000u, MEM_RESERVE, PAGE_READWRITE);
		const auto mem_commit = VirtualAlloc(mem_reserve, 0x200000u, MEM_COMMIT, PAGE_READWRITE);

		if (!mem_commit || !mem_reserve)
		{
			if (!mem_commit && mem_reserve)
			{
				VirtualFree(mem_reserve, 0, MEM_RESET);
			}

			const char* msg = Utils::String::VA("r_init_smodel_indices :: Unable to allocate 0x%.8x bytes. Out of memory?\n", smodel_cache_ib_size);
			Utils::function<void(const char*)>(0x5951C0)(msg); // Sys_Error v1.0 0x594AC0
		}

		gfx_buf.smodelCache.indices = static_cast<unsigned __int16*>(mem_reserve);
	}

	// R_CreateDynamicBuffers
	void Renderer::CreateDynamicBuffers()
	{
		auto& gfx_buf = *reinterpret_cast<Game::GfxBuffers*>(0x1AF5EB8);// v1.0 0x1AF5E98
		const auto& r_loadForRenderer = Dvars::Functions::Dvar_FindVar("r_loadForRenderer")->current.enabled;

		// default size in bytes
		std::uint32_t dynamic_vb_size = 1 * 9000000; //1048576;;
		std::uint32_t skinned_cache_size = 4 * 9000000; //1048576;;
		std::uint32_t temp_skin_size = 4 * 9000000; //1048576;;
		std::uint32_t dynamic_ib_size = 2 * 9000000; //1048576;;
		std::uint32_t pretess_ib_size = 2 * 9000000; //1048576;;


		for (auto i = 0; i != 1; ++i)
		{
			InitDynamicVertexBufferState(&gfx_buf.dynamicVertexBufferPool[i], dynamic_vb_size, "dynamicVertexBufferPool", r_loadForRenderer);
		}
		gfx_buf.dynamicVertexBuffer = gfx_buf.dynamicVertexBufferPool;


		for (auto i = 0; i != 2; ++i)
		{
			InitDynamicVertexBufferState(&gfx_buf.skinnedCacheVbPool[i], skinned_cache_size, "skinnedCacheVbPool", r_loadForRenderer);
		}

		InitTempSkinBuf(temp_skin_size);


		for (auto i = 0; i != 1; ++i)
		{
			InitDynamicIndexBufferState(&gfx_buf.dynamicIndexBufferPool[i], dynamic_ib_size, "dynamicIndexBufferPool", r_loadForRenderer);
		}
		gfx_buf.dynamicIndexBuffer = gfx_buf.dynamicIndexBufferPool;


		for (auto i = 0; i != 2; ++i)
		{
			InitDynamicIndexBufferState(&gfx_buf.preTessIndexBufferPool[i], pretess_ib_size, "preTessIndexBufferPool", r_loadForRenderer);
		}
		gfx_buf.preTessBufferFrame = 0;
		gfx_buf.preTessIndexBuffer = gfx_buf.preTessIndexBufferPool;
	}

	__declspec(naked) void Renderer::InitSmodelIndices_Stub()
	{
		const static uint32_t retn_addr = 0x5DAB3D;// v1.0 0x5DA43D
		__asm
		{
			call	InitSmodelIndices;
			pop     edi;
			jmp		retn_addr;
		}
	}

	__declspec(naked) void Renderer::R_WarnTempSkinSizeLimit_Stub()
	{
		const static uint32_t mb_size = 9000000; //1048576;;
		const static uint32_t retn_addr = 0x61E262;// v1.0 0x61E388
		__asm
		{
			push	eax;
			mov		eax, 480000h
				mov		eax, dword ptr[eax + 12];	// current->integer
			imul	eax, mb_size;				// get size in bytes

			cmp		edx, eax;					// warning limit compare
			pop		eax;

			jmp		retn_addr;
		}
	}

	__declspec(naked) void Renderer::R_WarnMaxSkinnedCacheVerticesLimit_Stub()
	{
		const static uint32_t mb_size = 9000000; //1048576;;
		const static uint32_t retn_addr = 0x61E388;// v1.0 0x61E4A2
		__asm
		{
			push	eax;
			mov		eax, 480000h
				mov		eax, dword ptr[eax + 12];	// current->integer
			imul	eax, mb_size;				// get size in bytes

			cmp		edx, eax;					// warning limit compare
			pop		eax;

			jmp		retn_addr;
		}
	}

	Renderer::Renderer()
	{
		//	Begin device recovery (not D3D9Ex)
		Utils::Hook(0x5DA0A0, []
		{
			Game::DB_BeginRecoverLostDevice();
			BeginRecoverDeviceSignal();
		}, HOOK_CALL).install()->quick();

		//	End device recovery (not D3D9Ex)
		Utils::Hook(0x5DA1A3, []
		{
			EndRecoverDeviceSignal();
			Game::DB_EndRecoverLostDevice();
		}, HOOK_CALL).install()->quick();

		//	Begin vid_restart
		Utils::Hook(0x444FCF, PostPreVidRestart, HOOK_CALL).install()->quick();
		//	End vid_restart
		Utils::Hook(0x4450CD, PostVidRestartStub, HOOK_CALL).install()->quick();
		
		Utils::Hook(0x42E1D1, CG_GetViewFov, HOOK_CALL).install()->quick();

		/*
		* Increase the amount of skinned vertices (bone controlled meshes) per frame.
		*      (R_MAX_SKINNED_CACHE_VERTICES | TEMP_SKIN_BUF_SIZE) Warnings
		*           'r_fastSkin' or 'r_skinCache' needs to be disabled or
		*			  the client will crash if you hit an unkown limit
		*/

		Utils::Hook(0x5D8BD2, CreateDynamicBuffers, HOOK_CALL).install()->quick();
		Utils::Hook(0x5D8BB9, AllocDynamicVertexBuffer, HOOK_CALL).install()->quick();

		// Alloc dynamic indices (smodelCache)
		Utils::Hook::Nop(0x5DAA57, 7); // clear
		Utils::Hook(0x5DAA57, InitSmodelIndices_Stub, HOOK_JUMP).install()->quick();

		// Change 'R_WARN_TEMP_SKIN_BUF_SIZE' warning limit to new buffer size
		Utils::Hook::Nop(0x61E25C, 6); // clear
		Utils::Hook(0x61E25C, R_WarnTempSkinSizeLimit_Stub, HOOK_JUMP).install()->quick();

		// Change 'R_WARN_MAX_SKINNED_CACHE_VERTICES' warning limit to new buffer size
		Utils::Hook::Nop(0x61E382, 6); // clear
		Utils::Hook(0x61E382, R_WarnMaxSkinnedCacheVerticesLimit_Stub, HOOK_JUMP).install()->quick();

		Utils::Hook::Set<DWORD>(0x5DACC8, 0x9000000);
		Utils::Hook::Set<DWORD>(0x6005A8, 0x9000000);
	}

	Renderer::~Renderer()
	{
		BackendFrameSignal.clear();
		SingleBackendFrameSignal.clear();

		EndRecoverDeviceSignal.clear();
		BeginRecoverDeviceSignal.clear();
	}
}