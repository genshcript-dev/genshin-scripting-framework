#include "hooks.h"
#include "../global.h"
#include <d3d11.h>
#include <MinHook.h>
#include <vector>
#include <macro.h>
#include "../definitions.h"
#include "../game.h"

bool gsf::hooks::install()
{
    DEBUG_CON_PRINT("\nInstalling hooks...");

	if (!DEBUG_CON_C_LOG(L"Initializing MinHook...", MH_Initialize() == MH_OK))
		return 0;

	DEBUG_CON_C_LOG(L"Init WndProc hook...", gsf::hooks::WindowProc.init(global::game_window));
    DEBUG_CON_C_LOG(L"Init ShowCursor hook...", gsf::hooks::ShowCursor.init(::ShowCursor));

    DEBUG_CON_C_LOG(L"Init UnityEngine.Cursor::set_lockState(UnityEngine.CursorLockMode)...", gsf::hooks::UnityEngine_Cursor_set_lockState.init(game::engine_cursor_set_lockstate));
    DEBUG_CON_C_LOG(L"Init UnityEngine_Cursor_set_visible...", gsf::hooks::UnityEngine_Cursor_set_visible.init(game::engine_cursor_set_visible));

	ID3D11Device   *dummy_device_ptr;
	IDXGISwapChain *dummy_swapchain_ptr;

	D3D_FEATURE_LEVEL fl;
    DXGI_SWAP_CHAIN_DESC scd =
    {
        .BufferDesc
        {
            .Width            = 800,
            .Height           = 600,
            .RefreshRate      = { 60, 1 },
            .Format           = DXGI_FORMAT_R8G8B8A8_UNORM,
            .ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
            .Scaling          = DXGI_MODE_SCALING_UNSPECIFIED
        },

        .SampleDesc
        {
            .Count   = 1,
            .Quality = 0
        },

        .BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .BufferCount  = 1,
        .OutputWindow = reinterpret_cast<HWND>(global::game_window),
        .Windowed     = TRUE,
        .SwapEffect   = DXGI_SWAP_EFFECT_DISCARD,
        .Flags        = 0
    };



    if (!DEBUG_CON_C_LOG(L"D3D11CreateDeviceAndSwapChain...", SUCCEEDED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_REFERENCE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &scd, &dummy_swapchain_ptr, &dummy_device_ptr, &fl, nullptr))) || !dummy_swapchain_ptr)
    {
        return false;
    }

    ID3D11DeviceContext *dummy_device_context_ptr = nullptr;
    dummy_device_ptr->GetImmediateContext(&dummy_device_context_ptr);

	DEBUG_CON_C_LOG(L"Init IDXGISwapChain::Present hook...",          gsf::hooks::Present.init(GET_VFUNC_FROM_VCLASS_BY_IDX(dummy_swapchain_ptr, 0, gsf::def::vtidx::IDXGISwapChain::Present)));                   // IDXGISwapChain::Present
    DEBUG_CON_C_LOG(L"Init ID3D11DeviceContext::Draw hook...",        gsf::hooks::Draw.init(GET_VFUNC_FROM_VCLASS_BY_IDX(dummy_device_context_ptr, 0, gsf::def::vtidx::ID3D11DeviceContext::Draw)));               // ID3D11DeviceContext::Draw
    DEBUG_CON_C_LOG(L"Init ID3D11DeviceContext::DrawIndexed hook...", gsf::hooks::DrawIndexed.init(GET_VFUNC_FROM_VCLASS_BY_IDX(dummy_device_context_ptr, 0, gsf::def::vtidx::ID3D11DeviceContext::DrawIndexed))); // ID3D11DeviceContext::DrawIndexed

    dummy_device_context_ptr->Release();
	dummy_swapchain_ptr->Release();
	dummy_device_ptr->Release();

	// Hook all initialized hook instances
    for (auto &hook_instance : utils::hook_base::instances)
    {
        if (!hook_instance->hook())
        {
            DEBUG_COUT("\nEnabling all hooks... failed!");
            return false;
        }
    }
    DEBUG_COUT("\nEnabling all hooks... ok!");

    return true;
}

bool gsf::hooks::uninstall()
{
	// Copy the entire list to a vector before iterating through and calling unhook.
	// This is done since we can't iterate the list while modifying it in the middle of an iteration.
	// Modifying it in terms of calling unhook which calls internal_on_unhook which then removes itself from the static instances std::list
	std::vector<utils::hook_base *> hook_instances { std::begin(utils::hook_base::instances), std::end(utils::hook_base::instances) };

	for (auto &hook_instance : hook_instances)
		if (!hook_instance->unhook())
			return false;

	return true;
}
