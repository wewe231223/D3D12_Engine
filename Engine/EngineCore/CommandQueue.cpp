#include "EnginePch.h"
#include "CommandQueue.h"

namespace EngineFramework {
	CommandQueue::~CommandQueue(){

	}
	void CommandQueue::Initialize(ComPtr<ID3D12Device> pd3dDevice){
		D3D12_COMMAND_QUEUE_DESC CommandDesc{};
		CommandDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		CommandDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		CheckFailed(pd3dDevice->CreateCommandQueue(&CommandDesc, IID_PPV_ARGS(m_d3dCommandQueue.GetAddressOf())));
		CheckFailed(pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_d3dCommandAllocator.GetAddressOf())));
		CheckFailed(pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_d3dCommandAllocator.Get(), NULL, IID_PPV_ARGS(m_d3dGraphicsCommandList.GetAddressOf())));

		m_d3dGraphicsCommandList->Close();

	}
}
