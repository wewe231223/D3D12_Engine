#pragma once
namespace EngineFramework {
	class CommandQueue : public ICommandQueue {
	public:
		CommandQueue();
		~CommandQueue();
	private:
		ComPtr<ID3D12CommandQueue> m_d3dCommandQueue{ nullptr };
		ComPtr<ID3D12CommandAllocator> m_d3dCommandAllocator{ nullptr };
		ComPtr<ID3D12GraphicsCommandList> m_d3dGraphicsCommandList{ nullptr };

		ComPtr<ID3D12Fence> m_d3dFence{ nullptr };
		HANDLE m_hEventHandle{ nullptr };
		mutable UINT64 m_nFenceValue{ 0 };
		
	public:
		void Initialize(const IDevice* pDevice);
		void Resize();

	public:
		void OpenCommandList() const;
		void SubmitCommandList() const;
		void PrepareRender(const ISwapChain* pSwapChain,const DirectX::XMVECTORF32 dxClearColor);
		void FinishRender(const ISwapChain* pSwapChain);
	public: // Interface 
		virtual ComPtr<ID3D12CommandQueue> GetCommandQueue() const override { return m_d3dCommandQueue; }
		virtual ComPtr<ID3D12CommandAllocator> GetCommandAllocator() const override { return m_d3dCommandAllocator; }
		virtual ComPtr<ID3D12GraphicsCommandList> GetCommandList() const override { return m_d3dGraphicsCommandList; }
		virtual void FlushCommandQueue() const override;
	};
}