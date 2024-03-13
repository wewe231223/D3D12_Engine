#include "EnginePch.h"
#include "Engine.h"
#include "Device.h"
#include "CommandQueue.h"
#include "CommandList.h"
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
		m_pMainCommandList = std::make_unique<CommandList>();
		m_pResourceCommandList = std::make_unique<CommandList>();
		m_pSwapChain = std::make_unique<SwapChain>();
		m_scene = std::make_unique<Scene>();
	}

	DirectXEngine::~DirectXEngine(){
		
	}


	void DirectXEngine::Initialize(){		
		m_pDevice->Initialize();
		m_pCommandQueue->Initialize(m_pDevice.get());
		m_pMainCommandList->Initialize(m_pDevice.get());
		m_pResourceCommandList->Initialize(m_pDevice.get());
		m_pSwapChain->Initialize(m_pDevice.get(), m_pCommandQueue.get(), m_cpWindowInfo, m_bMsaa4xState);


		Resize();

		// 1. 리소스를 업로드할 커맨드리스트를 연다 
		m_pResourceCommandList->Open();
		// 2. Scene Initialize 를 통해 리소스들( 정점, 텍스쳐 등 ) 을 업로드한다 
		m_scene->Initialize(m_pDevice.get(),m_pResourceCommandList.get());
		// 3. 리소스를 모두 업로드 했으면 커맨드 리스트를 닫는다.
		m_pResourceCommandList->Close();
		// 4. 업로드한 리소스를 제출한다.
		m_pCommandQueue->Execute(m_pResourceCommandList.get());
		// 5. GPU와 싱크를 맞춘다 ( GPU 에 업로드한다 )
		m_pCommandQueue->Sync();
				
	}

	void DirectXEngine::Render(){
		m_pMainCommandList->Open();
		m_pMainCommandList->TransformState(m_pSwapChain->GetCurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		m_pMainCommandList->SetViewPort(m_pSwapChain->GetViewPort());
		m_pMainCommandList->SetSissorRect(m_pSwapChain->GetSissorRect());
		m_pMainCommandList->ClearBackBuffer(m_pSwapChain.get(), DirectX::Colors::LightSteelBlue);


		m_scene->Render(m_pDevice.get(), m_pMainCommandList.get());

		
		m_pMainCommandList->TransformState(m_pSwapChain->GetCurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		m_pMainCommandList->Close();
		m_pCommandQueue->Execute(m_pMainCommandList.get());
		m_pSwapChain->Present();
		m_pCommandQueue->Sync();
	}

	void DirectXEngine::Update(){

	}

	void DirectXEngine::Resize(){
		m_pMainCommandList->Open();
		m_pSwapChain->Resize(m_pDevice.get(), m_pMainCommandList.get());
		m_pMainCommandList->Close();
		m_pCommandQueue->Execute(m_pMainCommandList.get());
		m_pCommandQueue->Sync();
	}

}