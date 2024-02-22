#include "EnginePch.h"
#include "CommandList.h"

namespace EngineFramework {
	CommandList::CommandList()
	{
	}

	CommandList::~CommandList()
	{
	}

	void CommandList::Initialize(const IDevice* pDevice){
		CheckFailed(pDevice->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_d3dCommandAllocator.GetAddressOf())));
		CheckFailed(pDevice->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_d3dCommandAllocator.Get(),nullptr,IID_PPV_ARGS(m_d3dGraphicsCommandList.GetAddressOf())));
		CheckFailed(m_d3dGraphicsCommandList->Close());
	}

	void CommandList::SetViewPort(const D3D12_VIEWPORT* cpViewport){
		m_d3dGraphicsCommandList->RSSetViewports(1, cpViewport);
	}

	void CommandList::SetSissorRect(const D3D12_RECT* cpRect){
		m_d3dGraphicsCommandList->RSSetScissorRects(1, cpRect);
	}

	void CommandList::ClearBackBuffer(const ISwapChain* pSwapChain,const DirectX::XMVECTORF32 dxClearColor){
		D3D12_CPU_DESCRIPTOR_HANDLE BackBufferView{ pSwapChain->GetCurrentBackBufferView() };
		D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView{ pSwapChain->GetDepthStencilView() };

		m_d3dGraphicsCommandList->ClearRenderTargetView(BackBufferView, dxClearColor, 0, nullptr);
		m_d3dGraphicsCommandList->ClearDepthStencilView(DepthStencilView, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.f, NULL, NULL, nullptr);

		m_d3dGraphicsCommandList->OMSetRenderTargets(1, &BackBufferView, true, &DepthStencilView);
	}

	void CommandList::TransformState(ComPtr<ID3D12Resource> d3dResource,D3D12_RESOURCE_STATES d3dPrevState, D3D12_RESOURCE_STATES d3dTarState){
		CD3DX12_RESOURCE_BARRIER ResourceBarrier{ CD3DX12_RESOURCE_BARRIER::Transition(d3dResource.Get(),d3dPrevState,d3dTarState) };
		m_d3dGraphicsCommandList->ResourceBarrier(1, &ResourceBarrier);
	}

	void CommandList::Open() const {
		CheckFailed(m_d3dCommandAllocator->Reset());
		CheckFailed(m_d3dGraphicsCommandList->Reset(m_d3dCommandAllocator.Get(), nullptr));
	}

	void CommandList::Execute(const ICommandQueue* pCommandQueue) const {
		ID3D12CommandList* CommandLists[] = { m_d3dGraphicsCommandList.Get() };
		pCommandQueue->GetCommandQueue()->ExecuteCommandLists(_countof(CommandLists), CommandLists);
	}

	void CommandList::Close() const {
		CheckFailed(m_d3dGraphicsCommandList->Close());
		
	}

	ComPtr<ID3D12GraphicsCommandList> CommandList::GetCommandList() const {
		return m_d3dGraphicsCommandList;
	}



}