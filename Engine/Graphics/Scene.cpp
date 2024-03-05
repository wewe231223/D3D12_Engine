#include "EnginePch.h"
#include "EngineCore/PipeLineStateObject.h"
#include "EngineCore/RootSignature.h"
#include "EngineCore/DescriptorTable.h"

#include "Graphics/Resources/Mesh.h"


#include "Shader.h"
#include "Scene.h"


#include "EngineCore/CommandList.h"

namespace EngineFramework {
	Scene::Scene(){
		m_shader = std::make_unique<Shader>();
		m_rootSignature = std::make_unique<RootSignature>();
		m_pso = std::make_unique<PipelineStateObject>();
		m_descriptortable = std::make_unique<DescriptorTable>();
	}

	Scene::Scene(const std::tstring& ctsSceneName) : m_tsSceneName(ctsSceneName){
		m_shader = std::make_unique<Shader>();
		m_rootSignature = std::make_unique<RootSignature>();
		m_pso = std::make_unique<PipelineStateObject>();
		m_descriptortable = std::make_unique<DescriptorTable>();
	}

	Scene::~Scene(){

	}

	void Scene::Initialize(const IDevice* pDevice, const ICommandList* pCommandList) {




		//----------------------------------------이니셜라이징 
		m_shader->Initialize(_T("..\\Engine\\Graphics\\DefaultShader.hlsl"));
		m_shader->CompileShader(VertexShader, nullptr, "VS_Main", "vs_5_1");
		m_shader->CompileShader(PixelShader, nullptr, "PS_Main", "ps_5_1");
		m_pso->Initialize();
		m_pso->SetShader(m_shader.get());
		//m_descriptortable->Initalize(pDevice, 10, 0);
		//----------------------------------------이니셜라이징

		//----------------------------------------오브젝트 생성 
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


		//----------------------------------------오브젝트 생성 
		testcontainer = std::make_unique<Resource::MeshContainer>(pCommandList, vec, indexVec);
		testmesh = std::make_unique<Resource::Mesh>(testcontainer.get());
		//----------------------------------------루트 시그니쳐 등록 및 PSO 생성 
		//m_rootSignature->NewParameter(m_descriptortable->GetRootParameter());
		//m_rootSignature->NewSampler(0);

		m_rootSignature->Create(pDevice);
		m_pso->SetRootSignature(m_rootSignature.get());
		m_pso->Create(pDevice);






		testResourceContainer = std::make_unique<Resource::ResourceContainer<MeshValue, Resource::MeshFactory>>(pCommandList,MeshParam(vec, indexVec));





		//----------------------------------------루트 시그니쳐 등록 및 PSO 생성 
	}

	void Scene::Render(const IDevice* pDevice,const ICommandList* pCommandList){
		
		m_pso->SetPipelineState(pCommandList);
		pCommandList->GetCommandList()->SetGraphicsRootSignature(m_rootSignature->GetRootSignature().Get());

		//ID3D12DescriptorHeap* DescriptorHeap[]{ m_descriptortable->GetDescriptorHeap().Get() };
		//pCommandList->GetCommandList()->SetDescriptorHeaps(_countof(DescriptorHeap), DescriptorHeap);
		//m_descriptortable->CommitTable(pCommandList);


		
		D3D12_VERTEX_BUFFER_VIEW Vb = testResourceContainer->GetValue().first;
		D3D12_INDEX_BUFFER_VIEW Ib = testResourceContainer->GetValue().second;


		pCommandList->GetCommandList()->IASetVertexBuffers(0, 1, &Vb);
		pCommandList->GetCommandList()->IASetIndexBuffer(&Ib);
		pCommandList->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//testmesh->BindResource(pCommandList);
		pCommandList->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
		
		
	}




}