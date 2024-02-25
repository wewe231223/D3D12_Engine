#pragma once

namespace EngineFramework {
	class DescriptorTable {
	public:
		DescriptorTable();
		~DescriptorTable();
	private:
		// 루트 시그니쳐에 제시한 레지스터 순서대로 offset 해줘야함 
		ComPtr<ID3D12DescriptorHeap> m_d3dDescriptorHeap{ nullptr };
		D3D12_DESCRIPTOR_HEAP_TYPE m_d3dHeapType{};
		UINT64 m_nHandleSize{ 0U };
		UINT m_nRootSignatureIndex{ 0U };
	public:
		void Initalize(const IDevice* pDevice,UINT nNumDescriptor,UINT nRootSignatureIndex,D3D12_DESCRIPTOR_HEAP_TYPE d3dHeapType = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		void SetDescriptor(const IDevice* pDevice,D3D12_CPU_DESCRIPTOR_HANDLE d3dDescriptorHandle, UINT nShaderRegister);
		void CommitTable(const ICommandList* pCommandList);
		ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return m_d3dDescriptorHeap; }
	};
}