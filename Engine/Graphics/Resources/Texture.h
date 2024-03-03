#pragma once 

namespace EngineFramework {
	namespace Resource{
		class TextureContainer {
			friend class Texture;
		public:
			TextureContainer();
			TextureContainer(const ICommandList* pCommandList, const std::tstring& ctsPath);
			~TextureContainer();
		private:
			ComPtr<ID3D12Resource> m_d3dTextureDefaultHeap{ nullptr };
			ComPtr<ID3D12Resource> m_d3dTextureUploadHeap{ nullptr };
			ComPtr<ID3D12DescriptorHeap> m_d3dSRVHeap{ nullptr };
			D3D12_CPU_DESCRIPTOR_HANDLE m_d3dSRVHandle{};
		};

		class Texture {
		public:
			Texture();
			Texture(const TextureContainer* other);
			~Texture();
		private:
			D3D12_CPU_DESCRIPTOR_HANDLE m_d3dSRVHandle{};
		public:
			void BindResource(IDescriptorTable* pDescriptorTable);
		};


	}
}