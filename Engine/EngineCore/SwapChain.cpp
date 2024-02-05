#include "EnginePch.h"
#include "SwapChain.h"


namespace EngineFramework {
    SwapChain::SwapChain(){
    }
    SwapChain::~SwapChain(){
    }
    void SwapChain::Initialize(const IDevice* pDevice, const ICommandQueue* pCommandQueue,const App::WindowInfo* cpWindowInfo,bool bMsaa4xState,DXGI_FORMAT dxgiBackBufferFormat, DXGI_FORMAT dxgiDepthStencilFormat){
		m_dxgiBackBufferFormat = dxgiBackBufferFormat;
		m_dxgiDepthStencilFormat = dxgiDepthStencilFormat;
		m_cpWindowInfo = cpWindowInfo;

		m_dxgiSwapChain.Reset();

        D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS MsaaQualityLevels{};
        MsaaQualityLevels.Format = m_dxgiBackBufferFormat;
        MsaaQualityLevels.SampleCount = 4;
        MsaaQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
        MsaaQualityLevels.NumQualityLevels = 0;
        CheckFailed(pDevice->GetDevice()->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &MsaaQualityLevels, sizeof(MsaaQualityLevels)));
        m_msaa4xState.Msaa4xBool = bMsaa4xState;
        m_msaa4xState.Msaa4XQualityLevel = MsaaQualityLevels.NumQualityLevels;

		DXGI_SWAP_CHAIN_DESC SwapChainDesc{};

		SwapChainDesc.BufferDesc.Width = static_cast<UINT>(m_cpWindowInfo->Width);
		SwapChainDesc.BufferDesc.Height = static_cast<UINT>(m_cpWindowInfo->Height);
        SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
        SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        SwapChainDesc.BufferDesc.Format = m_dxgiBackBufferFormat;
        SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        SwapChainDesc.SampleDesc.Count = m_msaa4xState.Msaa4xBool ? 4 : 1;
        SwapChainDesc.SampleDesc.Quality = m_msaa4xState.Msaa4xBool ? (m_msaa4xState.Msaa4XQualityLevel - 1) : 0;
        SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        SwapChainDesc.BufferCount = SWAP_CHAIN_BUFFER_COUNT;
        SwapChainDesc.OutputWindow = m_cpWindowInfo->hWnd;
        SwapChainDesc.Windowed = true;
        SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        CheckFailed(pDevice->GetDxgiFactory()->CreateSwapChain(pCommandQueue->GetCommandQueue().Get(), &SwapChainDesc, m_dxgiSwapChain.GetAddressOf()));

        D3D12_DESCRIPTOR_HEAP_DESC RenderTargetViewDesc{};
        ::ZeroMemory(&RenderTargetViewDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
        RenderTargetViewDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;
        RenderTargetViewDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        RenderTargetViewDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        RenderTargetViewDesc.NodeMask = 0;
        CheckFailed(pDevice->GetDevice()->CreateDescriptorHeap(&RenderTargetViewDesc, IID_PPV_ARGS(m_d3dRenderTargetDescriptorHeap.GetAddressOf())));

        D3D12_DESCRIPTOR_HEAP_DESC DepthStencilViewDescriptorHeap{};
        ::ZeroMemory(&DepthStencilViewDescriptorHeap, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
        DepthStencilViewDescriptorHeap.NumDescriptors = 1;
        DepthStencilViewDescriptorHeap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        DepthStencilViewDescriptorHeap.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        DepthStencilViewDescriptorHeap.NodeMask = 0;
        CheckFailed(pDevice->GetDevice()->CreateDescriptorHeap(&DepthStencilViewDescriptorHeap, IID_PPV_ARGS(m_d3dDepthStencilDescriptorHeap.GetAddressOf())));
	
        m_nRenderTargetViewDescriptorHeapSize = pDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        m_nDepthStencilViewDescriptorHeapSize = pDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    
    
        m_d3dDepthStencilClearValue.Format = m_dxgiDepthStencilFormat;
        m_d3dDepthStencilClearValue.DepthStencil.Depth = 1.f;
        m_d3dDepthStencilClearValue.DepthStencil.Stencil = 0;

    }
    void SwapChain::Resize(const IDevice* pDevice,const ICommandQueue* pCommandQueue) {
        for (UINT i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i) {
            m_d3dSwapChainBuffer[i].Reset();
        }
        m_d3dDepthStencilBuffer.Reset();
        CheckFailed(m_dxgiSwapChain->ResizeBuffers(SWAP_CHAIN_BUFFER_COUNT, m_cpWindowInfo->Width, m_cpWindowInfo->Height, m_dxgiBackBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));
        m_nCurrentBackBuffer = 0;

        CD3DX12_CPU_DESCRIPTOR_HANDLE RenderTargetViewHeapHandle(m_d3dRenderTargetDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

        for (UINT i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i) {
            CheckFailed(m_dxgiSwapChain->GetBuffer(i, IID_PPV_ARGS(m_d3dSwapChainBuffer[i].GetAddressOf())));
            pDevice->GetDevice()->CreateRenderTargetView(m_d3dSwapChainBuffer[i].Get(), nullptr, RenderTargetViewHeapHandle);
            RenderTargetViewHeapHandle.Offset(1, m_nRenderTargetViewDescriptorHeapSize);
        }

        D3D12_RESOURCE_DESC DepthStencilDesc;
        DepthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        DepthStencilDesc.Alignment = 0;
        DepthStencilDesc.Width = m_cpWindowInfo->Width;
        DepthStencilDesc.Height = m_cpWindowInfo->Height;
        DepthStencilDesc.DepthOrArraySize = 1;
        DepthStencilDesc.MipLevels = 1;
        DepthStencilDesc.Format = m_dxgiDepthStencilFormat;
        DepthStencilDesc.SampleDesc.Count = m_msaa4xState.Msaa4xBool ? 4 : 1;
        DepthStencilDesc.SampleDesc.Quality = m_msaa4xState.Msaa4xBool ? (m_msaa4xState.Msaa4XQualityLevel - 1) : 0;
        DepthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        DepthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;


        CheckFailed(pDevice->GetDevice()->CreateCommittedResource(
            &m_d3dDefaultHeapProperties,
            D3D12_HEAP_FLAG_NONE,
            &DepthStencilDesc,
            D3D12_RESOURCE_STATE_COMMON,
            &m_d3dDepthStencilClearValue,
            IID_PPV_ARGS(m_d3dDepthStencilBuffer.GetAddressOf())));


        pDevice->GetDevice()->CreateDepthStencilView(m_d3dDepthStencilBuffer.Get(), nullptr, m_d3dDepthStencilDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
        CD3DX12_RESOURCE_BARRIER DepthStencilResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_d3dDepthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
        pCommandQueue->GetCommandList()->ResourceBarrier(1, &DepthStencilResourceBarrier);

        CheckFailed(pCommandQueue->GetCommandList()->Close());
        ID3D12CommandList* CommandLists[] = { pCommandQueue->GetCommandList().Get()};
        pCommandQueue->GetCommandQueue()->ExecuteCommandLists(_countof(CommandLists), CommandLists);

        pCommandQueue->FlushCommandQueue();
        

        m_d3dScreenViewPort.TopLeftX = 0;
        m_d3dScreenViewPort.TopLeftY = 0;
        m_d3dScreenViewPort.Width = static_cast<float>(m_cpWindowInfo->Width);
        m_d3dScreenViewPort.Height = static_cast<float>(m_cpWindowInfo->Height);
        m_d3dScreenViewPort.MaxDepth = 0.f;
        m_d3dScreenViewPort.MinDepth = 1.f;

        m_d3dSissorRect = D3D12_RECT{ 0,0,m_cpWindowInfo->Width,m_cpWindowInfo->Height };
    }

    D3D12_CPU_DESCRIPTOR_HANDLE SwapChain::GetCurrentBackBufferView() const {
        return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_d3dRenderTargetDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),m_nCurrentBackBuffer,m_nRenderTargetViewDescriptorHeapSize);
    }

    D3D12_CPU_DESCRIPTOR_HANDLE SwapChain::GetDepthStencilView() const {
        return m_d3dDepthStencilDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
    }

    void SwapChain::SwapBuffer() const{
        m_nCurrentBackBuffer = (m_nCurrentBackBuffer + 1) % SWAP_CHAIN_BUFFER_COUNT;
    }

}
