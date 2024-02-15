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

	void Scene::Initialize(const IDevice* pDevice){

		m_shader = std::make_unique<Shader>();
		m_rootSignature = std::make_unique<RootSignature>();
		m_pso = std::make_unique<PipelineStateObject>();


		m_shader->Initialize(_T("DefaultShader.hlsl"));
		m_shader->CompileShader(VertexShader, nullptr, "VS_Main", "vs_5_1");
		m_shader->CompileShader(PixelShader, nullptr, "PS_Main", "ps_5_1");
		m_rootSignature->Initialize(pDevice);
		m_pso->Initialize();
		m_pso->SetShader(m_shader.get());

		// 이 사이에서 RootSignature 을 통한 상수 버퍼 생성을 진행 

		
		m_rootSignature->Create(pDevice);
		m_pso->SetRootSignature(m_rootSignature.get());

	}

	void Scene::Render(const ICommandQueue* pCommandQueue){

	}




}