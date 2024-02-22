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

		// 이부분은 좀 손봐야할듯
		// Default - Upload 버퍼를 사용하면 이를 CommandList에 업로드해야하는데 이때 커맨드리스트를 열고, 
		// 정점을 제출한뒤 닫는 과정이 필요함
		// ==> 그렇게 할정도로 효율적인가? 
		// 하지만 여기서 커맨드 리스트를 열었기 때문에 텍스쳐를 별도 커맨드 리스트 없이 삽입 가능
		// 1. 닫혀있는 ( 커맨드리스트를 생성할때 바로 닫는다 - 원치않는 명령입력을 피하기 위해) 
		m_pResourceCommandList->Open();
		// 2. Scene 을 Init 한다. ( 이때 정점, 인덱스를 업로드 버퍼에 제출하고 업로드한다 )
		m_scene->Initialize(m_pDevice.get(),m_pResourceCommandList.get());
		// 3. 앞 과정에서 업로드 했으므로, 다시 닫아줘야 한다.( 닫고 업로드한 정점 제출 )
		m_pResourceCommandList->Close();
		m_pResourceCommandList->Execute(m_pCommandQueue.get());
		m_pCommandQueue->Sync();
		m_pResourceCommandList->Open();
	}

	void DirectXEngine::Render(){
		m_pMainCommandList->Open();
		m_pMainCommandList->TransformState(m_pSwapChain->GetCurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		m_pMainCommandList->SetViewPort(m_pSwapChain->GetViewPort());
		m_pMainCommandList->SetSissorRect(m_pSwapChain->GetSissorRect());
		m_pMainCommandList->ClearBackBuffer(m_pSwapChain.get(), DirectX::Colors::LightSteelBlue);


		m_scene->Render(m_pMainCommandList.get());

		
		m_pMainCommandList->TransformState(m_pSwapChain->GetCurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		m_pMainCommandList->Close();
		m_pMainCommandList->Execute(m_pCommandQueue.get());
		m_pSwapChain->Present();
		m_pCommandQueue->Sync();
	}

	void DirectXEngine::Update(){

	}

	void DirectXEngine::Resize(){
		m_pMainCommandList->Open();
		m_pSwapChain->Resize(m_pDevice.get(), m_pMainCommandList.get());
		m_pMainCommandList->Close();
		m_pMainCommandList->Execute(m_pCommandQueue.get());
		m_pCommandQueue->Sync();
	}

}