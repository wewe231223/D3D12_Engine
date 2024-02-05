#pragma once 
namespace EngineFramework {
	class RenderTarget {
	public:
		RenderTarget();
		~RenderTarget();
	private:
		ComPtr<ID3D12DescriptorHeap> m_d3dRenderTargetDescriptorHeap{ nullptr };
		UINT m_nRenderTargetDescriptorHeapSize{ 0 };
	public:
		void Initialize(const IDevice* pDevice);
	public:
		UINT GetRenderTargetDescriptorHeapIncreasement() const { return m_nRenderTargetDescriptorHeapSize; }
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle() const;
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(UINT nOffset) const;
	};
}