#include "EnginePch.h"
#include "Device.h"

namespace EngineFramework {
    Device::Device(){

    }
    Device::~Device(){

	}

	void Device::LogAdapters(){
        UINT i = 0;
        IDXGIAdapter* Adapter{ nullptr };
        std::vector<IDXGIAdapter*> AdapterList{};

        while (m_dxgiFactory->EnumAdapters(i, &Adapter) != DXGI_ERROR_NOT_FOUND) {
            DXGI_ADAPTER_DESC Desc{};
            Adapter->GetDesc(&Desc);

            if (m_nOutputAdapterDedicatedMemorySize < Desc.DedicatedVideoMemory) {
                m_dxgiOoutputAdapter = Adapter;
                m_nOutputAdapterDedicatedMemorySize = Desc.DedicatedVideoMemory;
            }


            std::tstring Text{ L"***Adaptor***" };
            Text += Desc.Description;
            Text += L"\n";
            OutputDebugString(Text.c_str());

            OutputDebugString(_T("Adapter LUID : "));
            OutputDebugString(std::to_wstring(Desc.AdapterLuid.HighPart).c_str());
            OutputDebugString(std::to_wstring(Desc.AdapterLuid.LowPart).c_str());
            OutputDebugString(_T("\n\n"));
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
        {
            CheckFailed(::D3D12GetDebugInterface(IID_PPV_ARGS(m_d3dDebugController.GetAddressOf())));
            m_d3dDebugController->EnableDebugLayer();
            OutputDebugString(_T("DebugLayer Enabled\n"));
        }
#endif // !defined(DEBUG) || defined(_DEBUG)
        CheckFailed(::CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG,IID_PPV_ARGS(m_dxgiFactory.GetAddressOf())));
#if defined(DEBUG) || defined(_DEBUG)
        LogAdapters();
#endif // !defined(DEBUG) || defined(_DEBUG)
        // 여기서 첫번째 인자로 nullptr 을 전달하면 enumadapter 하여 가장 먼저 나오는 어댑터를 출력 어댑터로 설정한다.

        

		HRESULT HardwareResult = ::D3D12CreateDevice(m_dxgiOoutputAdapter.Get(), m_d3dDirectXFeatureLevel, IID_PPV_ARGS(m_d3dDevice.GetAddressOf()));

		if (FAILED(HardwareResult)) {
			ComPtr<IDXGIAdapter> WarpAdapter{};
			CheckFailed(m_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&WarpAdapter)));
			CheckFailed(::D3D12CreateDevice(WarpAdapter.Get(), m_d3dDirectXFeatureLevel, IID_PPV_ARGS(m_d3dDevice.GetAddressOf())));
		}


        LUID CurrentAdapterLuid{ m_d3dDevice->GetAdapterLuid() };
        OutputDebugString(_T("\n==============================================================\nCurrent Adapter LUID : "));
        OutputDebugString(std::to_wstring(CurrentAdapterLuid.HighPart).c_str());
        OutputDebugString(std::to_wstring(CurrentAdapterLuid.LowPart).c_str());
        OutputDebugString(_T("\n==============================================================\n"));

	}
	


}
