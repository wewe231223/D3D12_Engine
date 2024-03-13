namespace EngineFramework {

	struct RegisterHash { 
		std::size_t operator()(const RegisterBlock& other) const {
			std::size_t hash1 = std::hash<UINT>()(other.m_regStart);
			std::size_t hash2 = std::hash<UINT>()(other.m_regCount);
			std::size_t hash3 = std::hash<UINT>()(static_cast<UINT>(other.m_regType));
			return ((hash1 xor hash3) xor hash2);
		}
	};

	class RootSignature : public IRootSignature {
	public:
		RootSignature();
		~RootSignature();
	private:
		std::vector<CD3DX12_ROOT_PARAMETER> m_d3dRootParameter{};
		std::vector<CD3DX12_STATIC_SAMPLER_DESC> m_d3dSampler{};
		ComPtr<ID3D10Blob> m_d3dSerializedRootSignature{ nullptr };
		ComPtr<ID3D12RootSignature> m_d3dRootSignature{ nullptr };
		mutable UINT m_nCBuffer{ 0 };
		std::unordered_map<RegisterBlock,UINT,RegisterHash> m_blocks{};
	private:
	public:
		void Initialize();
		void NewParameter(CD3DX12_ROOT_PARAMETER d3dRootParam);
		void NewSampler(UINT nShaderRegister);
		void NewSampler(CD3DX12_STATIC_SAMPLER_DESC d3dSamplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0));
		void Create(const IDevice* pDevice);
		void SetRootSignature(const ICommandList* pCommandList);
	public:
		ComPtr<ID3D12RootSignature> GetRootSignature() const override;
		UINT GetBufferIndex() const override;
	private:

	};


	template <typename T, typename... Args>
	concept ConstructibleFrom = requires(Args... args) {
		T{ args... };
	};

	// 이제 리소스를 등록하고, 그에 맞는 정보를 받아오는
	class ShaderResourceManager {
	public:
		ShaderResourceManager();
		~ShaderResourceManager();
	private:
		ComPtr<ID3D12RootSignature> m_d3dRootSignature{ nullptr };
		ComPtr<ID3D10Blob> m_d3dSerializedRootSignature{ nullptr };


		std::vector<CD3DX12_ROOT_PARAMETER> m_d3dRootParameters{};
		std::vector<CD3DX12_STATIC_SAMPLER_DESC> m_d3dSamplers{};

		std::unordered_map<RegisterBlock, UINT, RegisterHash> m_blocks{};
	public:
		template<typename T, typename... Args> requires ConstructibleFrom<T, Args...>
		T* GetResource(Args... args);
	public:
		void NewSampler(D3D12_STATIC_SAMPLER_DESC&& SamplerDesc);

	};

	template<typename T, typename ...Args> requires ConstructibleFrom<T, Args...>
	inline T* ShaderResourceManager::GetResource(Args ...args){
		T* Result = new T{ args... };

		// 이 사이에서 T 를 생성하는 코드... 
		

		return Result;
	}

}