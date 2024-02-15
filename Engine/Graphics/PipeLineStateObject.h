namespace EngineFramework {
	class PipelineStateObject {
	public:
		PipelineStateObject();
		~PipelineStateObject();
	private:
		ComPtr<ID3D12PipelineState> m_d3dPipelineState{ nullptr };
		D3D12_GRAPHICS_PIPELINE_STATE_DESC m_d3dPipelineStateDesc{};
		std::vector <D3D12_INPUT_ELEMENT_DESC> m_d3dInputLayout{};
	public:
		void Initialize();
		void SetShader(const IShader* pShader);
		void SetRootSignature(const IRootSignature* pRootSignature);
		void Create(const IDevice* pDevice);
		void SetPipelineState(const ICommandQueue* pCommandQueue);
	};
}