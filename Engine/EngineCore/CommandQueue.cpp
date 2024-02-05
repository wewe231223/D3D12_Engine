#include "EnginePch.h"
#include "CommandQueue.h"

namespace EngineFramework {
	CommandQueue::CommandQueue(){

	}

	CommandQueue::~CommandQueue(){
		::CloseHandle(m_hEventHandle);
	}
	void CommandQueue::Initialize(const IDevice* pDevice){
		D3D12_COMMAND_QUEUE_DESC CommandDesc{};
		CommandDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		CommandDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		CheckFailed(pDevice->GetDevice()->CreateCommandQueue(&CommandDesc, IID_PPV_ARGS(m_d3dCommandQueue.GetAddressOf())));
		CheckFailed(pDevice->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_d3dCommandAllocator.GetAddressOf())));
		CheckFailed(pDevice->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_d3dCommandAllocator.Get(), nullptr, IID_PPV_ARGS(m_d3dGraphicsCommandList.GetAddressOf())));
		m_d3dGraphicsCommandList->Close();

		m_hEventHandle = ::CreateEventEx(nullptr, NULL, NULL, EVENT_ALL_ACCESS);
		CheckFailed(pDevice->GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_d3dFence.GetAddressOf())));
	}
	void CommandQueue::FlushCommandQueue() const{
		m_nFenceValue++;
		CheckFailed(m_d3dCommandQueue->Signal(m_d3dFence.Get(), m_nFenceValue));

		if (m_d3dFence->GetCompletedValue() < m_nFenceValue) {
			CheckFailed(m_d3dFence->SetEventOnCompletion(m_nFenceValue, m_hEventHandle));
			::WaitForSingleObject(m_hEventHandle, INFINITE);
		}

	}
	void CommandQueue::Resize(){
		FlushCommandQueue();
		CheckFailed(m_d3dGraphicsCommandList->Reset(m_d3dCommandAllocator.Get(), nullptr));
	}
	void CommandQueue::Reset() const {
		CheckFailed(m_d3dCommandAllocator->Reset());
		CheckFailed(m_d3dGraphicsCommandList->Reset(m_d3dCommandAllocator.Get(), nullptr));
	}
	void CommandQueue::RenderReady(const ISwapChain* pSwapChain,const DirectX::XMVECTORF32 dxClearColor){
		Reset();
		m_d3dGraphicsCommandList->RSSetViewports(1, pSwapChain->GetViewPort());
		m_d3dGraphicsCommandList->RSSetScissorRects(1, pSwapChain->GetSissorRect());

		CD3DX12_RESOURCE_BARRIER ResourceBarrier{ CD3DX12_RESOURCE_BARRIER::Transition(pSwapChain->GetCurrentBackBuffer().Get(),D3D12_RESOURCE_STATE_PRESENT ,D3D12_RESOURCE_STATE_RENDER_TARGET) };

		D3D12_CPU_DESCRIPTOR_HANDLE BackBufferView{ pSwapChain->GetCurrentBackBufferView() };
		D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView{ pSwapChain->GetDepthStencilView() };

		m_d3dGraphicsCommandList->ResourceBarrier(1, &ResourceBarrier);
		m_d3dGraphicsCommandList->ClearRenderTargetView(BackBufferView, dxClearColor, 0, nullptr);
		m_d3dGraphicsCommandList->ClearDepthStencilView(DepthStencilView, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.f, NULL, NULL, nullptr);

		m_d3dGraphicsCommandList->OMSetRenderTargets(1, &BackBufferView, true, &DepthStencilView);
	}
	void CommandQueue::RenderFinish(const ISwapChain* pSwapChain){
		CD3DX12_RESOURCE_BARRIER ResourceBarrier{ CD3DX12_RESOURCE_BARRIER::Transition(pSwapChain->GetCurrentBackBuffer().Get(),D3D12_RESOURCE_STATE_RENDER_TARGET ,D3D12_RESOURCE_STATE_PRESENT) };
		m_d3dGraphicsCommandList->ResourceBarrier(1, &ResourceBarrier);
		CheckFailed(m_d3dGraphicsCommandList->Close());
		ID3D12CommandList* CommandLists[] = { m_d3dGraphicsCommandList.Get() };
		m_d3dCommandQueue->ExecuteCommandLists(_countof(CommandLists), CommandLists);

		CheckFailed(pSwapChain->GetDxgiSwapChain()->Present(NULL, NULL));
		pSwapChain->SwapBuffer();
		FlushCommandQueue();
	}
}
