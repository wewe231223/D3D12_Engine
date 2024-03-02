#pragma once

namespace EngineFramework {
	namespace Resource {
		class Mesh;
		class MeshContainer;
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
		std::unique_ptr<class DescriptorTable> m_descriptortable{ nullptr };
	private:
		std::unique_ptr<Resource::MeshContainer> testcontainer{ nullptr };
		std::unique_ptr<Resource::Mesh> testmesh{ nullptr };
	public:
		void Initialize(const IDevice* pDevice,const ICommandList* pCommandList);
		void Render(const IDevice* pDevice,const ICommandList* pCommandList);
	};
}
