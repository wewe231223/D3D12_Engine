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
	// t의 순서가 보장된다는 조건 
	DescriptorID DescriptorTable::CreateDescriptor(D3D12_DESCRIPTOR_RANGE_TYPE d3dDescriptorType,UINT nShaderRegister){

		static bool CBVRegister[256]{ false, };
		static bool SRVRegister[256]{ false, };
		static bool UAVRegister[256]{ false, };


		switch (d3dDescriptorType){
		case D3D12_DESCRIPTOR_RANGE_TYPE_SRV:
		{
			if (!SRVRegister[nShaderRegister]) {
				m_d3dDescriptorRange.emplace_back(CD3DX12_DESCRIPTOR_RANGE{ d3dDescriptorType,1,nShaderRegister});
				SRVRegister[nShaderRegister] = true;
			}
		}
			break;
		case D3D12_DESCRIPTOR_RANGE_TYPE_UAV:
		{
			if (!CBVRegister[nShaderRegister]) {
				m_d3dDescriptorRange.emplace_back(CD3DX12_DESCRIPTOR_RANGE{ d3dDescriptorType,1,nShaderRegister });
				CBVRegister[nShaderRegister] = true;
			}
		}
			break;
		case D3D12_DESCRIPTOR_RANGE_TYPE_CBV:
		{
			if (!UAVRegister[nShaderRegister]) {
				m_d3dDescriptorRange.emplace_back(CD3DX12_DESCRIPTOR_RANGE{ d3dDescriptorType,1,nShaderRegister });
				UAVRegister[nShaderRegister] = true;
			}
		}
			break;
		case D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER:
			break;
		default:
			break;
		}

		return static_cast<int>(m_d3dDescriptorRange.size() - 1);
	}

	void DescriptorTable::SetDescriptor(const IDevice* pDevice,DescriptorID nDescId,D3D12_CPU_DESCRIPTOR_HANDLE d3dDescriptorHandle) const {
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle{ m_d3dDescriptorHeap->GetCPUDescriptorHandleForHeapStart() };
		Handle.Offset(nDescId, m_nHandleSize);
		
		pDevice->GetDevice()->CopyDescriptorsSimple(1, Handle, d3dDescriptorHandle, m_d3dHeapType);

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