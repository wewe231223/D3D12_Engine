#pragma once

namespace EngineFramework {
    template <typename T, typename... Args>	
    concept ConstructibleFrom = requires(Args... args) {
        T{ args... };
    };

	template<typename T>
	concept SupportClone = requires(T ty) {
		{ ty.Clone() };
	};

	template<typename T>
	concept SupportGetResourceType = requires(T ty) {
		{ ty.GetResourceType() } -> std::same_as<D3D12_ROOT_PARAMETER>;
	};

	template<typename T>
	concept ValidResourceContainer = std::derived_from<T, DelPtr> and SupportClone<T> and SupportGetResourceType<T>;

	template<typename T>
	concept MeshContainer = std::derived_from<T, DelPtr> and SupportClone<T>;



	class SceneSystem {
	public:
		SceneSystem() = default;
		virtual ~SceneSystem() {};
	private:
		std::vector<D3D12_ROOT_PARAMETER> m_d3dRootParameter{};
		ComPtr<ID3D10Blob> m_d3dSerializedRootSignature{ nullptr };
		ComPtr<ID3D12RootSignature> m_d3dRootSignature{ nullptr };
		std::unordered_map<std::string, std::unique_ptr<DelPtr>> m_resources{};
	protected:
		template<ValidResourceContainer T, typename... Args>
		requires ConstructibleFrom<T,Args...>
		void NewResource(std::string name, Args... args);

		template<MeshContainer T,typename... Args>
		requires ConstructibleFrom<T,Args...>
		void NewResource(std::string name,Args... args);
	private:
		DelPtr* GetResourcePtr(const std::string& name) { return m_resources[name].get(); }
		template<typename T>
		T* PointerCasting(DelPtr* ptr) { return reinterpret_cast<T*>(ptr); }
	public:
		template<typename T>
		auto GetResourceClone(const std::string& name) -> decltype(PointerCasting<T>(GetResourcePtr(name))->Clone());
	protected:
		ID3D12RootSignature* GetRootSignature();
		ID3D12RootSignature* GetRootSignature(const IDevice* pDevice);
	};
	
	template<ValidResourceContainer T, typename...Args>
		requires ConstructibleFrom<T, Args...>
	inline void SceneSystem::NewResource(std::string name,Args ...args){
		std::unique_ptr<T> Temp = std::make_unique<T>(args...);
		m_d3dRootParameter.push_back(Temp->GetResourceType());
		m_resources.insert(std::pair<std::string, std::unique_ptr<DelPtr>>(name,std::move(Temp))); 
		// ## ALERT ##
		// DO NOT USE TEMP AFTER HERE!! 
		// TEMP is Dangling Pointer After Here 
	}

	template<MeshContainer T, typename...Args>
		requires ConstructibleFrom<T, Args...>
	inline void SceneSystem::NewResource(std::string name, Args ...args) {
		std::unique_ptr<T> Temp = std::make_unique<T>(args...);
		m_resources.insert(std::pair<std::string, std::unique_ptr<DelPtr>>(name, std::move(Temp)));
		// ## ALERT ##
		// DO NOT USE TEMP AFTER HERE!! 
		// TEMP is Dangling Pointer After Here 
	}
	
	template<typename T>
	inline auto SceneSystem::GetResourceClone(const std::string& name) -> decltype(PointerCasting<T>(GetResourcePtr(name))-> Clone()) {
		return PointerCasting<T>(GetResourcePtr(name))->Clone();
	}

	/*
	* Material 안에 DescriptorTable
	* Object 가 Material 을 소유.
	* Material 을 
	*/

	// Scene 의 기본 토대만 작성해주고 나머지는 Client 에서 작성할수있도록 하자
	class Scene : public SceneSystem{
	public:
		Scene();
		Scene(const std::string& ctsSceneName);
		~Scene();
	private:
		std::string m_sSceneName{};
		std::unique_ptr<class Shader> m_shader{ nullptr };
	public:
		void Initialize(const IDevice* pDevice,const ICommandList* pCommandList);
		void Render(const IDevice* pDevice,const ICommandList* pCommandList);
	private:
		void BuildShader();
	};
}
