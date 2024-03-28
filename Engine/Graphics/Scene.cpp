#include "EnginePch.h"
#include "Shader.h"
#include "Scene.h"
#include "EngineCore/CommandList.h"
#include "Graphics/Resources/Mesh.h"
#include "Graphics/IAInput.h"

namespace EngineFramework {

	ID3D12RootSignature* SceneSystem::GetRootSignature(){
		if (m_d3dRootSignature.Get() == nullptr) throw System::Exeption(_T("\n[Scene System] : Any Root Signature Exist!\n"));
		return m_d3dRootSignature.Get();

	}

	ID3D12RootSignature* SceneSystem::GetRootSignature(const IDevice* pDevice) {
		if (m_d3dRootSignature.Get() == nullptr) {
			// 나중에 샘플러 지원도 추가해야된다. 
			CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc{ static_cast<UINT>(m_d3dRootParameter.size()),m_d3dRootParameter.data(),NULL,nullptr,D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT };

			ComPtr<ID3D10Blob> ErrorBlob{ nullptr };
			CheckFailed(D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, m_d3dSerializedRootSignature.GetAddressOf(), ErrorBlob.GetAddressOf()));
			CheckFailed(pDevice->GetDevice()->CreateRootSignature(0, m_d3dSerializedRootSignature->GetBufferPointer(), m_d3dSerializedRootSignature->GetBufferSize(), IID_PPV_ARGS(m_d3dRootSignature.GetAddressOf())));
		}
		return m_d3dRootSignature.Get();
	}



	Scene::Scene(){

	}

	Scene::Scene(const std::string& ctsSceneName) : m_sSceneName(ctsSceneName){
		m_inputLayout = std::make_unique<InputLayout<Vertex>>();
		
		m_shader = std::make_unique<Shader>(_T("..\\Engine\\Graphics\\DefaultShader.hlsl"),m_inputLayout->GetLayout());
		m_shader->CreateShader(VertexShader, nullptr, "VS_Main", "vs_5_1");
		m_shader->CreateShader(PixelShader, nullptr, "PS_Main", "ps_5_1");
	}

	Scene::~Scene(){

	}

	void Scene::Initialize(const IDevice* pDevice, const ICommandList* pCommandList) {

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

		
		NewResource<Resource::Mesh>("Test",pCommandList, vec, indexVec);

		m_shader->BuildShader(pDevice, GetRootSignature(pDevice));


	}


	void Scene::Render(const IDevice* pDevice,const ICommandList* pCommandList){
		m_shader->BindPipeLine(pCommandList);
		pCommandList->GetCommandList()->SetGraphicsRootSignature(GetRootSignature());

		Resource::MeshClone C{ GetResourceClone<Resource::Mesh>("Test") };
		C.BindResource(pCommandList);

		pCommandList->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);


		


	
	}







}

