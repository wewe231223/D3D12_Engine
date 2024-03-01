namespace EngineFramework {
	class Device : public IDevice{
	public:
		Device();
		~Device();
	private:
		// Device and Factory
		ComPtr<ID3D12Device> m_d3dDevice{ nullptr };
		ComPtr<IDXGIFactory4> m_dxgiFactory{ nullptr };
		ComPtr<IDXGIAdapter> m_dxgiOoutputAdapter{ nullptr };
		SIZE_T m_nOutputAdapterDedicatedMemorySize{ 0 };
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
		virtual ComPtr<ID3D12Device> GetDevice() const override { return m_d3dDevice; }
		virtual ComPtr<IDXGIFactory4> GetDxgiFactory() const override{ return m_dxgiFactory; }
	};

}