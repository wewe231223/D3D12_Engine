namespace EngineFramework {
	class Device {
	public:
		Device() = default;
		~Device();
	private:
		// Device and Factory
		ComPtr<ID3D12Device> m_d3dDevice{ nullptr };
		ComPtr<IDXGIFactory4> m_dxgiFactory{ nullptr };
#if defined(DEBUG) || defined(_DEBUG)
		ComPtr<ID3D12Debug> m_d3dDebugController{ nullptr };
#endif // !defined(DEBUG) || defined(_DEBUG)
	private:
		D3D_FEATURE_LEVEL m_d3dDirectXFeatureLevel{ D3D_FEATURE_LEVEL_11_0 };
		DXGI_FORMAT m_dxgiFormat{ DXGI_FORMAT_R8G8B8A8_UNORM };
		UINT m_n4xMsaaQualityLevel{ 0 };
	private:
		void LogAdapters();
	public:
		void Initialize();
		ComPtr<ID3D12Device> GetDevice() const { return m_d3dDevice; }
		ComPtr<IDXGIFactory> GetDxgiFactory() const { return m_dxgiFactory; }
		UINT Get4xMsaaQuality() const { return m_n4xMsaaQualityLevel; }
	};

}