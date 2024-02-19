#include "EnginePch.h"
#include "Engine.h"
#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "Graphics/Scene.h"

namespace EngineFramework {

	Engine::~Engine(){

	}

	DirectXEngine::DirectXEngine()
	{
	}

	DirectXEngine::DirectXEngine(const App::WindowInfo* cpWindowInfo, bool bMsaa4xState) : m_cpWindowInfo(cpWindowInfo), m_bMsaa4xState(bMsaa4xState){
		m_pDevice = std::make_unique<Device>();
		m_pCommandQueue = std::make_unique<CommandQueue>();
		m_pSwapChain = std::make_unique<SwapChain>();
		m_scene = std::make_unique<Scene>();
	}

	DirectXEngine::~DirectXEngine(){

	}

	void DirectXEngine::Initialize(){		
		m_pDevice->Initialize();
		m_pCommandQueue->Initialize(m_pDevice.get());
		m_pSwapChain->Initialize(m_pDevice.get(), m_pCommandQueue.get(), m_cpWindowInfo, m_bMsaa4xState);

		Resize();

		m_pCommandQueue->GetCommandList()->Reset(m_pCommandQueue->GetCommandAllocator().Get(), nullptr);
		m_scene->Initialize(m_pDevice.get(),m_pCommandQueue.get());
		m_pCommandQueue->GetCommandList()->Close();

		ID3D12CommandList* CommandList[] = { m_pCommandQueue->GetCommandList().Get() };
		m_pCommandQueue->GetCommandQueue()->ExecuteCommandLists(_countof(CommandList), CommandList);
		m_pCommandQueue->FlushCommandQueue();

	}

	void DirectXEngine::Render(){
		m_pCommandQueue->PrepareRender(m_pSwapChain.get(), DirectX::Colors::SteelBlue);

		m_scene->Render(m_pCommandQueue.get());

		m_pCommandQueue->FinishRender(m_pSwapChain.get());
	}

	void DirectXEngine::Update(){

	}

	void DirectXEngine::Resize(){
		m_pCommandQueue->Resize();
		m_pSwapChain->Resize(m_pDevice.get(), m_pCommandQueue.get());
	}

}