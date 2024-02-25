#include "EnginePch.h"
#include "DescriptorTable.h"

namespace EngineFramework {
	DescriptorTable::DescriptorTable()
	{
	}
	DescriptorTable::~DescriptorTable()
	{
	}
	void DescriptorTable::Initalize(const IDevice* pDevice, UINT nNumDescriptor,UINT nRootSignatureIndex,D3D12_DESCRIPTOR_HEAP_TYPE d3dHeapType){
		D3D12_DESCRIPTOR_HEAP_DESC DescriptorHeapDesc{};
		DescriptorHeapDesc.NumDescriptors = nNumDescriptor;
		DescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		DescriptorHeapDesc.Type = d3dHeapType;
		pDevice->GetDevice()->CreateDescriptorHeap(&DescriptorHeapDesc, IID_PPV_ARGS(m_d3dDescriptorHeap.GetAddressOf()));

		m_nHandleSize = pDevice->GetDevice()->GetDescriptorHandleIncrementSize(d3dHeapType);
		m_nRootSignatureIndex = nRootSignatureIndex;

		m_d3dHeapType = d3dHeapType;
	}

	void DescriptorTable::SetDescriptor(const IDevice* pDevice,D3D12_CPU_DESCRIPTOR_HANDLE d3dDescriptorHandle, UINT nShaderRegister){
		CD3DX12_CPU_DESCRIPTOR_HANDLE DestinationHandle{ m_d3dDescriptorHeap->GetCPUDescriptorHandleForHeapStart() };
		DestinationHandle.Offset(nShaderRegister*m_nHandleSize);

		UINT32 DestinationRange = 1;
		UINT32 SrcRange = 1;

		pDevice->GetDevice()->CopyDescriptors(1, &DestinationHandle, &DestinationRange, 1, &d3dDescriptorHandle, &SrcRange, m_d3dHeapType);
	}

	void DescriptorTable::CommitTable(const ICommandList* pCommandList){
		pCommandList->GetCommandList()->SetGraphicsRootDescriptorTable(m_nRootSignatureIndex, m_d3dDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	}

	

}