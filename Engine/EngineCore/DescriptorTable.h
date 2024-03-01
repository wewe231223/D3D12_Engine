#pragma once

namespace EngineFramework {
	class DescriptorTable : public IDescriptorTable{
	public:
		DescriptorTable();
		~DescriptorTable();
	private:
		// 루트 시그니쳐에 제시한 레지스터 순서대로 offset 해줘야함 
		ComPtr<ID3D12DescriptorHeap> m_d3dDescriptorHeap{ nullptr };
		std::vector<CD3DX12_DESCRIPTOR_RANGE> m_d3dDescriptorRange{};
		D3D12_DESCRIPTOR_HEAP_TYPE m_d3dHeapType{};
		UINT64 m_nHandleSize{ 0U };
		UINT m_nRootSignatureIndex{ 0U };
	public:
		void Initalize(const IDevice* pDevice,UINT nNumDescriptor,UINT nRootSignatureIndex,D3D12_DESCRIPTOR_HEAP_TYPE d3dHeapType = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		void CommitTable(const ICommandList* pCommandList);
		ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return m_d3dDescriptorHeap; }
		CD3DX12_ROOT_PARAMETER GetRootParameter();
	public:
		virtual DescriptorID CreateDescriptor(D3D12_DESCRIPTOR_RANGE_TYPE d3dDescriptorType, UINT nShaderRegister) override;
		virtual void SetDescriptor(const IDevice* pDevice,DescriptorID nDescId,D3D12_CPU_DESCRIPTOR_HANDLE d3dDescriptorHandle) const override;
	};
}