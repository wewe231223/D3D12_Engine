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
		m_pCommandList = std::make_unique<CommandList>();
		m_pSwapChain = std::make_unique<SwapChain>();
		m_scene = std::make_unique<Scene>();
	}

	DirectXEngine::~DirectXEngine(){

	}

	void DirectXEngine::Initialize(){		
		m_pDevice->Initialize();
		m_pCommandQueue->Initialize(m_pDevice.get());
		m_pCommandList->Initialize(m_pDevice.get());
		m_pSwapChain->Initialize(m_pDevice.get(), m_pCommandQueue.get(), m_cpWindowInfo, m_bMsaa4xState);

		Resize();


		// 이부분은 좀 손봐야할듯
		// Default - Upload 버퍼를 사용하면 이를 CommandList에 업로드해야하는데 이때 커맨드리스트를 열고, 
		// 정점을 제출한뒤 닫는 과정이 필요함
		// ==> 그렇게 할정도로 효율적인가? 
		// 하지만 여기서 커맨드 리스트를 열었기 때문에 텍스쳐를 별도 커맨드 리스트 없이 삽입 가능
		// 1. 닫혀있는 ( 커맨드리스트를 생성할때 바로 닫는다 - 원치않는 명령입력을 피하기 위해) 
		m_pCommandQueue->OpenCommandList();
		// 2. Scene 을 Init 한다. ( 이때 정점, 인덱스를 업로드 버퍼에 제출하고 업로드한다 )
		m_scene->Initialize(m_pDevice.get(),m_pCommandQueue.get());
		// 3. 앞 과정에서 업로드 했으므로, 다시 닫아줘야 한다.( 닫고 업로드한 정점 제출 )
		m_pCommandQueue->SubmitCommandList();
		m_pCommandQueue->Sync();

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