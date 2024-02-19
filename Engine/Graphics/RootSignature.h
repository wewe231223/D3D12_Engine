namespace EngineFramework {
	class RootSignature : public IRootSignature {
	public:
		RootSignature();
		~RootSignature();
	private:
		std::vector<CD3DX12_ROOT_PARAMETER> m_d3dRootParameter{};
		ComPtr<ID3D10Blob> m_d3dSerializedRootSignature{ nullptr };
		ComPtr<ID3D12RootSignature> m_d3dRootSignature{ nullptr };
		ComPtr<ID3D12DescriptorHeap> m_d3dResourceDescriptorHeap{ nullptr };
		mutable UINT m_nCBuffer{ 0 };
	public:
		void Initialize(const IDevice* pDevice);
		void Create(const IDevice* pDevice);
		void SetRootSignature(const ICommandQueue* pCommandQueue);
		void SetResourceDescriptorHeap(const ICommandQueue* pCommandQueue);
	public:
		ComPtr<ID3D12RootSignature> GetRootSignature() const override;
		UINT GetBufferIndex() const override;

	};
}