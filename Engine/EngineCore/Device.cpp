#include "EnginePch.h"
#include "Device.h"

namespace EngineFramework {
	Device::~Device(){

	}

	void Device::LogAdapters(){
        UINT i = 0;
        IDXGIAdapter* Adapter{ nullptr };
        std::vector<IDXGIAdapter*> AdapterList{};

        while (m_dxgiFactory->EnumAdapters(i, &Adapter) != DXGI_ERROR_NOT_FOUND) {
            DXGI_ADAPTER_DESC Desc{};
            Adapter->GetDesc(&Desc);
            std::tstring Text{ L"***Adaptor***" };
            Text += Desc.Description;
            Text += L"\n";
            OutputDebugString(Text.c_str());

            UINT j = 0;
            IDXGIOutput* Output{ nullptr };

            while (Adapter->EnumOutputs(j, &Output) != DXGI_ERROR_NOT_FOUND) {
                DXGI_OUTPUT_DESC Desc_{};
                Output->GetDesc(&Desc_);
                Text = L"***Output***";
                Text += Desc_.DeviceName;
                Text += L"\n";
                OutputDebugString(Text.c_str());

                UINT Count{ 0 };
                UINT Flags{ 0 };

                //카운트만 가져옴
                Output->GetDisplayModeList(m_dxgiFormat, Flags, &Count, nullptr);
                std::vector<DXGI_MODE_DESC> ModeList(Count);

                Output->GetDisplayModeList(m_dxgiFormat, Flags, &Count, &ModeList[0]);

                for (auto& x : ModeList) {
                    UINT n = x.RefreshRate.Numerator;
                    UINT d = x.RefreshRate.Denominator;
                    Text = L"Width = " + std::to_wstring(x.Width) + L" ";
                    Text += L"Height = " + std::to_wstring(x.Height) + L" ";
                    Text += L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) + L"\n";

                    ::OutputDebugString(Text.c_str());
                }
                ReleaseCom(Output);
                ++j;
            }
            AdapterList.push_back(Adapter);
            ++i;
        }
        for (size_t k = 0; k < AdapterList.size(); ++k) {
            ReleaseCom(AdapterList[k]);
        }
	}

	void Device::Initialize(){
#if defined(DEBUG) || defined(_DEBUG)
		CheckFailed(::D3D12GetDebugInterface(IID_PPV_ARGS(m_d3dDebugController.GetAddressOf())));
		m_d3dDebugController->EnableDebugLayer();
#endif // !defined(DEBUG) || defined(_DEBUG)
		CheckFailed(::CreateDXGIFactory1(IID_PPV_ARGS(m_dxgiFactory.GetAddressOf())));
		HRESULT HardwareResult = ::D3D12CreateDevice(nullptr, m_d3dDirectXFeatureLevel, IID_PPV_ARGS(m_d3dDevice.GetAddressOf()));

		if (FAILED(HardwareResult)) {
			ComPtr<IDXGIAdapter> WarpAdapter{};
			CheckFailed(m_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&WarpAdapter)));
			CheckFailed(::D3D12CreateDevice(WarpAdapter.Get(), m_d3dDirectXFeatureLevel, IID_PPV_ARGS(m_d3dDevice.GetAddressOf())));
		}
		
		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS MsaaQualityLevels{};
		MsaaQualityLevels.Format = m_dxgiFormat;
		MsaaQualityLevels.SampleCount = 4;
		MsaaQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
		MsaaQualityLevels.NumQualityLevels = 0;
		CheckFailed(m_d3dDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &MsaaQualityLevels, sizeof(MsaaQualityLevels)));
		m_n4xMsaaQualityLevel = MsaaQualityLevels.NumQualityLevels;
#if defined(DEBUG) || defined(_DEBUG)
        LogAdapters();
#endif // !defined(DEBUG) || defined(_DEBUG)
	}
	



}
