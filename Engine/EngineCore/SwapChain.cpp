#include "EnginePch.h"
#include "SwapChain.h"
#include "RenderTarget.h"
#include "DepthStencil.h"

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

        m_pRenderTarget = std::make_unique<RenderTarget>();
        m_pRenderTarget->Initialize(pDevice);

        m_pDepthStencil = std::make_unique<DepthStencil>();
        m_pDepthStencil->Initialize(pDevice, m_cpWindowInfo, m_dxgiDepthStencilFormat);

    }
    void SwapChain::Resize(const IDevice* pDevice,const ICommandQueue* pCommandQueue) {
        for (UINT i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i) {
            m_d3dSwapChainBuffer[i].Reset();
        }
        CheckFailed(m_dxgiSwapChain->ResizeBuffers(SWAP_CHAIN_BUFFER_COUNT, m_cpWindowInfo->Width, m_cpWindowInfo->Height, m_dxgiBackBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));
        m_nCurrentBackBuffer = 0;

        CD3DX12_CPU_DESCRIPTOR_HANDLE RenderTargetViewHeapHandle{ m_pRenderTarget->GetCPUDescriptorHandle() };

        for (UINT i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i) {
            CheckFailed(m_dxgiSwapChain->GetBuffer(i, IID_PPV_ARGS(m_d3dSwapChainBuffer[i].GetAddressOf())));
            pDevice->GetDevice()->CreateRenderTargetView(m_d3dSwapChainBuffer[i].Get(), nullptr, RenderTargetViewHeapHandle);
            RenderTargetViewHeapHandle.Offset(1, m_pRenderTarget->GetRenderTargetDescriptorHeapIncreasement());
        }

        m_pDepthStencil->Resize(pDevice, pCommandQueue, &m_msaa4xState);

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
        return m_pRenderTarget->GetCPUDescriptorHandle(m_nCurrentBackBuffer);
    }

    D3D12_CPU_DESCRIPTOR_HANDLE SwapChain::GetDepthStencilView() const {
        return m_pDepthStencil->GetCPUDescriptorHandle();
    }

    void SwapChain::SwapBuffer() const{
        m_nCurrentBackBuffer = (m_nCurrentBackBuffer + 1) % SWAP_CHAIN_BUFFER_COUNT;
    }

}
