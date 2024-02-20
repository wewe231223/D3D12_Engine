#pragma once 

namespace EngineFramework {
	class Texture {
	public:
		Texture();
		~Texture();
	private:
		DirectX::ScratchImage m_image{};
		ComPtr<ID3D12Resource> m_d3dTex2D{ nullptr };
		ComPtr<ID3D12DescriptorHeap> m_d3dSRVHeap{ nullptr };
		D3D12_CPU_DESCRIPTOR_HANDLE m_d3dSRVHandle{};
	public:
		void Initialize(const std::tstring& ImagePath);

	};
}