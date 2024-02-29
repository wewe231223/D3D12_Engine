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

	void DescriptorTable::SetDescriptor(const IDevice* pDevice,D3D12_CPU_DESCRIPTOR_HANDLE d3dDescriptorHandle,D3D12_DESCRIPTOR_RANGE_TYPE d3dDescriptorType){
		UINT32 DestinationRange = 1;
		UINT32 SrcRange = 1;

		static UINT DescriptorCount = 0;
		static UINT SRVRegCount = 0;
		static UINT CBVRegCount = 0;
		static UINT UAVRegCount = 0;

		CD3DX12_CPU_DESCRIPTOR_HANDLE DestinationHandle{ m_d3dDescriptorHeap->GetCPUDescriptorHandleForHeapStart() };
		DestinationHandle.Offset(static_cast<UINT>(DescriptorCount * m_nHandleSize)); // nShaderRegister == 어떤 리소스건 상관없이 몇번째 descriptor heap 에 들어가느냐 -> Root signature 에 정의된대로 
		
		DescriptorCount++;

		pDevice->GetDevice()->CopyDescriptors(1, &DestinationHandle, &DestinationRange, 1, &d3dDescriptorHandle, &SrcRange, m_d3dHeapType);

		switch (d3dDescriptorType){
		case D3D12_DESCRIPTOR_RANGE_TYPE_SRV:
			m_d3dDescriptorRange.emplace_back(CD3DX12_DESCRIPTOR_RANGE{ d3dDescriptorType,1,SRVRegCount++ });
			break;
		case D3D12_DESCRIPTOR_RANGE_TYPE_UAV:
			m_d3dDescriptorRange.emplace_back(CD3DX12_DESCRIPTOR_RANGE{ d3dDescriptorType,1,UAVRegCount++ });
			break;
		case D3D12_DESCRIPTOR_RANGE_TYPE_CBV:
			m_d3dDescriptorRange.emplace_back(CD3DX12_DESCRIPTOR_RANGE{ d3dDescriptorType,1,CBVRegCount++ });
			break;
		case D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER:
			break;
		default:
			break;
		}
	}

	void DescriptorTable::CommitTable(const ICommandList* pCommandList){
		pCommandList->GetCommandList()->SetGraphicsRootDescriptorTable(m_nRootSignatureIndex, m_d3dDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	}

	CD3DX12_ROOT_PARAMETER DescriptorTable::GetRootParameter(){
		CD3DX12_ROOT_PARAMETER RootParam{};
		RootParam.InitAsDescriptorTable(static_cast<UINT>(m_d3dDescriptorRange.size()), m_d3dDescriptorRange.data());
		return RootParam;
	}

	

}