#define _CRT_SECURE_NO_WARNINGS
#include <intrin.h>
#include <Windows.h>
#include  <Psapi.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

#include "Config.h"
#include "GUI.h"
#include "Interfaces.h"
#include "Memory.h"
#include "Hooks.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static HRESULT __stdcall hookedPresent(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND windowOverride, const RGNDATA* dirtyRegion) noexcept 
{
	static bool isInitialised{ false };

	static bool isMenuToggled{ false };

	if (!isInitialised) {
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(FindWindowA("Valve001", NULL));
		ImGui_ImplDX9_Init(device);

		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 5.0f;
		style.WindowBorderSize = 0.0f;
		style.ChildBorderSize = 0.0f;
		style.GrabMinSize = 7.0f;
		style.GrabRounding = 5.0f;
		style.FrameRounding = 5.0f;
		style.PopupRounding = 5.0f;
		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = nullptr;
		io.LogFilename = nullptr;
		io.MouseDrawCursor = true;
		char buffer[MAX_PATH];
		GetWindowsDirectoryA(buffer, MAX_PATH);
		io.Fonts->AddFontFromFileTTF(strcat(buffer, "\\Fonts\\Tahoma.ttf"), 16.0f);

		hooks.originalWndProc = reinterpret_cast<WNDPROC>(
			SetWindowLongPtr(FindWindowA("Valve001", NULL), GWLP_WNDPROC, LONG_PTR(hookedPresent))
			);
		isInitialised = true;
	}
	else if (gui.isOpen) {
		device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
		IDirect3DVertexDeclaration9* vertexDeclaration;
		device->GetVertexDeclaration(&vertexDeclaration);

		if (!isMenuToggled) {
			//
			//
			//
			isMenuToggled = true;
		}
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		gui.render();

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		device->SetVertexDeclaration(vertexDeclaration);
	}
	else {
		if (isMenuToggled) {
			//
			//
			//
			isMenuToggled = false;
		}
	}
	return hooks.originalPresent(device, src, dest, windowOverride, dirtyRegion);
}

static HRESULT __stdcall hookedReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	auto result = hooks.originalReset(device, params);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;
}

Hooks::Hooks()
{
	originalPresent = **reinterpret_cast<decltype(&originalPresent)*>(memory.present);
	**reinterpret_cast<void***>(memory.present) = reinterpret_cast<void*>(&hookedPresent);
	originalReset = **reinterpret_cast<decltype(&originalReset)*>(memory.reset);
	**reinterpret_cast<void***>(memory.reset) = reinterpret_cast<void*>(&hookedReset);
}
