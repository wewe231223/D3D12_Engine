#pragma once 

namespace EngineFramework {
	namespace Resource {
		class Texture {
		public:
			Texture();
			~Texture();
		private:
			DirectX::ScratchImage m_image{};
			ComPtr<ID3D12Resource> m_d3dTex2D{ nullptr };
			ComPtr<ID3D12Resource> m_d3dTexUploadHeap{ nullptr };
			ComPtr<ID3D12DescriptorHeap> m_d3dSRVHeap{ nullptr };
			D3D12_CPU_DESCRIPTOR_HANDLE m_d3dSRVHandle{};
			std::vector<D3D12_SUBRESOURCE_DATA> m_subresources{};
		public:
			void Initialize(const IDevice* pDevice, const std::tstring& ImagePath);
			void Upload(const ICommandList* pCommandList);
			D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorHandle() const { return m_d3dSRVHandle; }
		};
	}
}