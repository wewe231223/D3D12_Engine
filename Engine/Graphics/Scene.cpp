#include "EnginePch.h"
#include "Scene.h"
#include "Resources/Mesh.h"
#include "PipeLineStateObject.h"
#include "RootSignature.h"
#include "Shader.h"

namespace EngineFramework {
	Scene::Scene(){

	}

	Scene::Scene(const std::tstring& ctsSceneName) : m_tsSceneName(ctsSceneName){

	}

	Scene::~Scene(){

	}

	void Scene::Initialize(const IDevice* pDevice,const ICommandQueue* pCommandQueue){

		m_shader = std::make_unique<Shader>();
		m_rootSignature = std::make_unique<RootSignature>();
		m_pso = std::make_unique<PipelineStateObject>();
		m_meshManager = std::make_unique<Resource::MeshManager>();

		m_shader->Initialize(_T("..\\Engine\\Graphics\\DefaultShader.hlsl"));
		m_shader->CompileShader(VertexShader, nullptr, "VS_Main", "vs_5_1");
		m_shader->CompileShader(PixelShader, nullptr, "PS_Main", "ps_5_1");
		m_rootSignature->Initialize(pDevice);
		m_pso->Initialize();
		m_pso->SetShader(m_shader.get());

		// 이 사이에서 RootSignature 을 통한 상수 버퍼 생성을 진행 


		std::vector<Vertex> vec(4);
		vec[0].Position = DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f);
		vec[0].Color = DirectX::XMFLOAT4(1.f, 0.f, 0.f, 1.f);
		vec[0].TexCoord = DirectX::XMFLOAT2(0.f, 0.f);

		vec[1].Position = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
		vec[1].Color = DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f);
		vec[1].TexCoord = DirectX::XMFLOAT2(1.f, 0.f);
		
		vec[2].Position = DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f);
		vec[2].Color = DirectX::XMFLOAT4(0.f, 0.f, 1.f, 1.f);
		vec[2].TexCoord = DirectX::XMFLOAT2(1.f, 1.f);
		
		vec[3].Position = DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f);
		vec[3].Color = DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f);
		vec[3].TexCoord = DirectX::XMFLOAT2(0.f, 1.f);

		std::vector<UINT> indexVec;
		{
			indexVec.push_back(0);
			indexVec.push_back(1);
			indexVec.push_back(2);
		}
		{
			indexVec.push_back(0);
			indexVec.push_back(2);
			indexVec.push_back(3);
		}

		testmesh = m_meshManager->CreateMesh(_T("Testmesh"), vec, indexVec);


		m_rootSignature->Create(pDevice);
		m_pso->SetRootSignature(m_rootSignature.get());
		m_pso->Create(pDevice);

		m_meshManager->Upload(pDevice, pCommandQueue);
	}

	void Scene::Render(const ICommandQueue* pCommandQueue){
		m_pso->SetPipelineState(pCommandQueue);
		pCommandQueue->GetCommandList()->SetGraphicsRootSignature(m_rootSignature->GetRootSignature().Get());
		m_meshManager->BindBuffer(pCommandQueue, D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		testmesh->Render(pCommandQueue);
	}




}