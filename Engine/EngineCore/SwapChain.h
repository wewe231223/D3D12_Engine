#pragma once
namespace EngineFramework {
	class SwapChain : public ISwapChain{
	public:
		SwapChain();
		~SwapChain();
	private:
		ComPtr<IDXGISwapChain> m_dxgiSwapChain{ nullptr };
		ComPtr<ID3D12Resource> m_d3dSwapChainBuffer[SWAP_CHAIN_BUFFER_COUNT]{};
		D3D12_VIEWPORT m_d3dScreenViewPort{};
		D3D12_RECT m_d3dSissorRect{};

		DXGI_FORMAT m_dxgiBackBufferFormat{ DXGI_FORMAT_R8G8B8A8_UNORM };
		DXGI_FORMAT m_dxgiDepthStencilFormat{ DXGI_FORMAT_D24_UNORM_S8_UINT };

		mutable UINT m_nCurrentBackBuffer{ 0 };

		const App::WindowInfo* m_cpWindowInfo{ nullptr };
		MsaaState m_msaa4xState{ false,0 };

		std::unique_ptr<class RenderTarget> m_pRenderTarget{ nullptr };
		std::unique_ptr<class DepthStencil> m_pDepthStencil{ nullptr };
	public:
		void Initialize(const IDevice* pDevice,const ICommandQueue* pCommandQueue,const App::WindowInfo* cpWindowInfo,bool bMsaa4xState,DXGI_FORMAT dxgiBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT dxgiDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT);
		void Resize(const IDevice* pDevice,const ICommandQueue* pCommandQueue);
	public:
		virtual ComPtr<IDXGISwapChain> GetDxgiSwapChain() const override { return m_dxgiSwapChain; }
		virtual const D3D12_VIEWPORT* GetViewPort() const override { return &m_d3dScreenViewPort; }
		virtual const D3D12_RECT* GetSissorRect() const override { return &m_d3dSissorRect; }
		virtual ComPtr<ID3D12Resource> GetCurrentBackBuffer() const override { return m_d3dSwapChainBuffer[m_nCurrentBackBuffer]; }
		virtual D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferView() const override;
		virtual D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() const override;
		virtual void SwapBuffer() const override;

	};
}