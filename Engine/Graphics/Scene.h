#pragma once

namespace EngineFramework {
	namespace Resource {
		class MeshManager;
		class Mesh;
	};
	
	// Scene 의 기본 토대만 작성해주고 나머지는 Client 에서 작성할수있도록 하자
	class Scene {
	public:
		Scene();
		Scene(const std::tstring& ctsSceneName);
		~Scene();
	private:
		std::tstring m_tsSceneName{};
		std::unique_ptr<class Shader> m_shader{ nullptr };
		std::unique_ptr<class PipelineStateObject> m_pso{ nullptr };
		std::unique_ptr<class RootSignature> m_rootSignature{ nullptr };
	private:
		std::unique_ptr<class Resource::MeshManager> m_meshManager{ nullptr };
		std::unique_ptr<class Resource::Mesh> testmesh{ nullptr };
	public:
		void Initialize(const IDevice* pDevice);
		void Render(const ICommandQueue* pCommandQueue);
	};
}
