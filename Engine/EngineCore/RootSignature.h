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



}