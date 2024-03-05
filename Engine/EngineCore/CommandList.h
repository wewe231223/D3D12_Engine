#pragma once

namespace EngineFramework {
	class CommandList : public ICommandList{
	public:
		CommandList();
		~CommandList();
	private:
		ComPtr<ID3D12GraphicsCommandList> m_d3dGraphicsCommandList{ nullptr };
		ComPtr<ID3D12CommandAllocator> m_d3dCommandAllocator{ nullptr };
	public:
		void Initialize(const IDevice* pDevice);
		void SetViewPort(const D3D12_VIEWPORT* cpViewport);
		void SetSissorRect(const D3D12_RECT* cpRect);
		void ClearBackBuffer(const ISwapChain* pSwapChain,const DirectX::XMVECTORF32 dxClearColor);
		void TransformState(ComPtr<ID3D12Resource> d3dResource,D3D12_RESOURCE_STATES d3dPrevState, D3D12_RESOURCE_STATES d3dTarState);
	public: // Interface 
		virtual void Open() const override;
		virtual void Close() const override;
		virtual ComPtr<ID3D12GraphicsCommandList> GetCommandList() const override;
	};
}