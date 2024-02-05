#pragma once

namespace EngineFramework {
	class DepthStencil {
	public:
		DepthStencil();
		~DepthStencil();
	private:
		ComPtr<ID3D12DescriptorHeap> m_d3dDepthStencilDescriptorHeap{ nullptr };
		ComPtr<ID3D12Resource> m_d3dDepthStencilBuffer{ nullptr };
		UINT m_nDepthStencilDescriptorHeapSize{ 0 };
		D3D12_CLEAR_VALUE m_d3dDepthStencilClearValue{};
		CD3DX12_HEAP_PROPERTIES m_d3dDepthStencilDefaultHeapProperties{ D3D12_HEAP_TYPE_DEFAULT };
		const App::WindowInfo* m_cpWindowInfo{ nullptr };
		DXGI_FORMAT m_dxgiDepthStencilFormat{};
	public:
		void Initialize(const IDevice* pDevice,const App::WindowInfo* cpWindowInfo,DXGI_FORMAT dxgiDepthStencilFormat);
		void Resize(const IDevice* pDevice, const ICommandQueue* pCommandQueue, const MsaaState* cpMsaa4xState);
	public:
		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle() const;
	};
}