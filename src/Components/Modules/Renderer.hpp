#pragma once

namespace Components
{
	class Renderer : public Component
	{
	public:
		typedef void(BackendCallback)(IDirect3DDevice9*);
		typedef void(Callback)();

		static int Width();
		static int Height();

		static float CG_GetViewFov(int localClientNum);

		Renderer();
		~Renderer();
	private:
		static void PostPreVidRestart();
		static void PreVidRestart();
		static void PostVidRestart();
		static void PostVidRestartStub();

		static Utils::Signal<Renderer::Callback> EndRecoverDeviceSignal;
		static Utils::Signal<Renderer::Callback> BeginRecoverDeviceSignal;

		static Utils::Signal<BackendCallback> BackendFrameSignal;
		static Utils::Signal<BackendCallback> SingleBackendFrameSignal;

		static IDirect3DDevice9* GetDirectX9Device();
		static int AllocDynamicIndexBuffer(IDirect3DIndexBuffer9** ib, int size_in_bytes, const char* buffer_name, bool load_for_renderer);
		static void InitDynamicIndexBufferState(Game::GfxIndexBufferState* ib, int indexCount, const char* buffer_name, bool loadForRenderer);
		static char* AllocDynamicVertexBuffer(IDirect3DVertexBuffer9** vb, int size_in_bytes, const char* buffer_name, bool load_for_renderer);
		static void AllocDynamicVertexBuffer();
		static void InitDynamicVertexBufferState(Game::GfxVertexBufferState* vb, int bytes, const char* buffer_name, bool load_for_renderer);
		static void InitTempSkinBuf(int bytes);
		static void CreateDynamicBuffers();
		static void InitSmodelIndices();
		static void InitSmodelIndices_Stub();
		static void R_WarnTempSkinSizeLimit_Stub();
		static void R_WarnMaxSkinnedCacheVerticesLimit_Stub();
	};
}