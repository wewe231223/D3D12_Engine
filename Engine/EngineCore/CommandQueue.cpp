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

		m_hEventHandle = ::CreateEventEx(nullptr, NULL, NULL, EVENT_ALL_ACCESS);
		CheckFailed(pDevice->GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_d3dFence.GetAddressOf())));
	}
	void CommandQueue::Sync() const{
		m_nFenceValue++;
		CheckFailed(m_d3dCommandQueue->Signal(m_d3dFence.Get(), m_nFenceValue));

		if (m_d3dFence->GetCompletedValue() < m_nFenceValue) {
			CheckFailed(m_d3dFence->SetEventOnCompletion(m_nFenceValue, m_hEventHandle));
			::WaitForSingleObject(m_hEventHandle, INFINITE);
		}

	}
}