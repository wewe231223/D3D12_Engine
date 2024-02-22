#include "EnginePch.h"
#include "DepthStencil.h"

namespace EngineFramework {
	DepthStencil::DepthStencil()
	{
	}
	DepthStencil::~DepthStencil()
	{
	}
	void DepthStencil::Initialize(const IDevice* pDevice,const App::WindowInfo* cpWindowInfo,DXGI_FORMAT dxgiDepthStencilFormat){
		D3D12_DESCRIPTOR_HEAP_DESC DepthStencilDescriptorDesc{};
		::ZeroMemory(&DepthStencilDescriptorDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
		DepthStencilDescriptorDesc.NumDescriptors = 1;
		DepthStencilDescriptorDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		DepthStencilDescriptorDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		DepthStencilDescriptorDesc.NodeMask = NULL;

		CheckFailed(pDevice->GetDevice()->CreateDescriptorHeap(&DepthStencilDescriptorDesc, IID_PPV_ARGS(m_d3dDepthStencilDescriptorHeap.GetAddressOf())));

		m_nDepthStencilDescriptorHeapSize = pDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

		m_d3dDepthStencilClearValue = D3D12_CLEAR_VALUE{ dxgiDepthStencilFormat,1.f,0 };
		m_cpWindowInfo = cpWindowInfo;
		m_dxgiDepthStencilFormat = dxgiDepthStencilFormat;
	}
	void DepthStencil::Resize(const IDevice* pDevice,const ICommandList* pCommandList,const MsaaState* cpMsaa4xState){
		m_d3dDepthStencilBuffer.Reset();
		D3D12_RESOURCE_DESC DepthStencilDesc;
		DepthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		DepthStencilDesc.Alignment = 0;
		DepthStencilDesc.Width = m_cpWindowInfo->Width;
		DepthStencilDesc.Height = m_cpWindowInfo->Height;
		DepthStencilDesc.DepthOrArraySize = 1;
		DepthStencilDesc.MipLevels = 1;
		DepthStencilDesc.Format = m_dxgiDepthStencilFormat;
		DepthStencilDesc.SampleDesc.Count = cpMsaa4xState->Msaa4xBool ? 4 : 1;
		DepthStencilDesc.SampleDesc.Quality = cpMsaa4xState->Msaa4xBool ? (cpMsaa4xState->Msaa4XQualityLevel - 1) : 0;
		DepthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		DepthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		CheckFailed(pDevice->GetDevice()->CreateCommittedResource(
			&m_d3dDepthStencilDefaultHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&DepthStencilDesc,
			D3D12_RESOURCE_STATE_COMMON,
			&m_d3dDepthStencilClearValue,
			IID_PPV_ARGS(m_d3dDepthStencilBuffer.GetAddressOf())
		));

		pDevice->GetDevice()->CreateDepthStencilView(m_d3dDepthStencilBuffer.Get(), nullptr, m_d3dDepthStencilDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
		CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_d3dDepthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
		pCommandList->GetCommandList()->ResourceBarrier(1, &ResourceBarrier);
	}
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencil::GetCPUDescriptorHandle() const {
		return m_d3dDepthStencilDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	}
}