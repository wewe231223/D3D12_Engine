#include "EnginePch.h"
#include "CommandQueue.h"

namespace EngineFramework {
	CommandQueue::~CommandQueue(){
		::CloseHandle(m_hEventHandle);
	}
	void CommandQueue::Initialize(ComPtr<ID3D12Device> pd3dDevice){
		D3D12_COMMAND_QUEUE_DESC CommandDesc{};
		CommandDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		CommandDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		CheckFailed(pd3dDevice->CreateCommandQueue(&CommandDesc, IID_PPV_ARGS(m_d3dCommandQueue.GetAddressOf())));
		CheckFailed(pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_d3dCommandAllocator.GetAddressOf())));
		CheckFailed(pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_d3dCommandAllocator.Get(), NULL, IID_PPV_ARGS(m_d3dGraphicsCommandList.GetAddressOf())));
		m_d3dGraphicsCommandList->Close();

		m_hEventHandle = ::CreateEventEx(nullptr, NULL, NULL, EVENT_ALL_ACCESS);
		CheckFailed(pd3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_d3dFence.GetAddressOf())));
	}
	void CommandQueue::FlushCommandQueue(){
		m_nFenceValue++;
		CheckFailed(m_d3dCommandQueue->Signal(m_d3dFence.Get(), m_nFenceValue));

		if (m_d3dFence->GetCompletedValue() < m_nFenceValue) {
			CheckFailed(m_d3dFence->SetEventOnCompletion(m_nFenceValue, m_hEventHandle));
			::WaitForSingleObject(m_hEventHandle, INFINITE);
		}

	}
}
