#include "EnginePch.h"
#include "RenderTarget.h"

namespace EngineFramework {
	RenderTarget::RenderTarget(){

	}
	RenderTarget::~RenderTarget(){

	}
	void RenderTarget::Initialize(const IDevice* pDevice){
		D3D12_DESCRIPTOR_HEAP_DESC RenderTargetViewDesc{};
		::ZeroMemory(&RenderTargetViewDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
		RenderTargetViewDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;
		RenderTargetViewDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		RenderTargetViewDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		RenderTargetViewDesc.NodeMask = 0;
		CheckFailed(pDevice->GetDevice()->CreateDescriptorHeap(&RenderTargetViewDesc, IID_PPV_ARGS(m_d3dRenderTargetDescriptorHeap.GetAddressOf())));

		m_nRenderTargetDescriptorHeapSize = pDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	}
	CD3DX12_CPU_DESCRIPTOR_HANDLE RenderTarget::GetCPUDescriptorHandle() const {
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_d3dRenderTargetDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	}
	CD3DX12_CPU_DESCRIPTOR_HANDLE RenderTarget::GetCPUDescriptorHandle(UINT nOffset) const {
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_d3dRenderTargetDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),nOffset,m_nRenderTargetDescriptorHeapSize);
	}
}