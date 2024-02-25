#include "EnginePch.h"
#include "Scene.h"
#include "Resources/Mesh.h"
#include "Resources/Texture.h"
#include "EngineCore/PipeLineStateObject.h"
#include "EngineCore/RootSignature.h"
#include "Shader.h"
#include "EngineCore/DescriptorTable.h"

namespace EngineFramework {
	Scene::Scene(){

	}

	Scene::Scene(const std::tstring& ctsSceneName) : m_tsSceneName(ctsSceneName){

	}

	Scene::~Scene(){

	}

	void Scene::Initialize(const IDevice* pDevice, const ICommandList* pCommandList){
		//----------------------------------------이니셜라이징 
		m_shader = std::make_unique<Shader>();
		m_rootSignature = std::make_unique<RootSignature>();
		m_pso = std::make_unique<PipelineStateObject>();
		m_meshManager = std::make_unique<Resource::MeshManager>();
		m_descriptortable = std::make_unique<DescriptorTable>();

		m_shader->Initialize(_T("..\\Engine\\Graphics\\DefaultShader.hlsl"));
		m_shader->CompileShader(VertexShader, nullptr, "VS_Main", "vs_5_1");
		m_shader->CompileShader(PixelShader, nullptr, "PS_Main", "ps_5_1");
		m_pso->Initialize();
		m_pso->SetShader(m_shader.get());
		m_descriptortable->Initalize(pDevice, 10, 0);
		//----------------------------------------이니셜라이징

		//----------------------------------------오브젝트 생성 
		testtex = std::make_unique<Resource::Texture>();
		testtex->Initialize(pDevice, pCommandList, _T("..\\Resources\\veigar.jpg"));
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


		m_descriptortable->SetDescriptor(pDevice, testtex->GetDescriptorHandle(), 0);
		//----------------------------------------오브젝트 생성 
		m_meshManager->Upload(pDevice, pCommandList);

		//----------------------------------------루트 시그니쳐 등록 및 PSO 생성 
		CD3DX12_DESCRIPTOR_RANGE DescRange[] = { CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,1,0) };
		CD3DX12_ROOT_PARAMETER RootParam{};
		RootParam.InitAsDescriptorTable(_countof(DescRange), DescRange);

		m_rootSignature->NewParameter(RootParam);
		m_rootSignature->NewSampler(0);

		m_rootSignature->Create(pDevice);
		m_pso->SetRootSignature(m_rootSignature.get());
		m_pso->Create(pDevice);

		//----------------------------------------루트 시그니쳐 등록 및 PSO 생성 

	}

	void Scene::Render(const IDevice* pDevice,const ICommandList* pCommandList){
		
		m_pso->SetPipelineState(pCommandList);
		pCommandList->GetCommandList()->SetGraphicsRootSignature(m_rootSignature->GetRootSignature().Get());

		ID3D12DescriptorHeap* DescriptorHeap = m_descriptortable->GetDescriptorHeap().Get();
		pCommandList->GetCommandList()->SetDescriptorHeaps(1, &DescriptorHeap);

		m_descriptortable->CommitTable(pCommandList);
		m_meshManager->BindBuffer(pCommandList, D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		testmesh->Render(pCommandList);
	}




}