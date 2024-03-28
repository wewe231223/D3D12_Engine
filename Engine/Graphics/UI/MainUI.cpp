#include "EnginePch.h"
#include "MainUI.h"

#pragma region IMGUI
#include "External/Include/ImGUI/imgui.h"
#include "External/Include/ImGUI/imgui_impl_win32.h"
#include "External/Include/ImGUI/imgui_impl_dx12.h"
#pragma endregion IMGUI


EngineFramework::UI::MainUI::MainUI(HWND hWnd,IDevice* pDevice){
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	m_guiIO = std::make_unique<ImGuiIO>(ImGui::GetIO());

	m_guiIO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	m_guiIO->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui_ImplWin32_Init(hWnd);
//	ImGui_ImplDX12_Init(pDevice->GetDevice().Get(),SWAP_CHAIN_BUFFER_COUNT,DXGI_FORMAT_R8G8B8A8_UNORM,)

}
