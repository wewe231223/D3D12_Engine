#pragma once 

namespace EngineFramework {
	namespace Resource{
		class TextureClone {
		public:
			TextureClone(ID3D12Resource* pResource, D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc) : m_pResource(pResource), m_SRVDesc(SRVDesc) {}
		private:
			ID3D12Resource* m_pResource{ nullptr };
			D3D12_SHADER_RESOURCE_VIEW_DESC m_SRVDesc{};
		public:
			void MakeSRV(IDevice* pDevice,ID3D12DescriptorHeap* pDescriptorHeap,UINT nDescIndex);
		};


		class TextureContainer {
		public:
			TextureContainer();
			TextureContainer(const ICommandList* pCommandList, const std::tstring& ctsPath);
			~TextureContainer();
		private:
			ComPtr<ID3D12Resource> m_d3dTextureDefaultHeap{ nullptr };
			ComPtr<ID3D12Resource> m_d3dTextureUploadHeap{ nullptr };
			D3D12_SHADER_RESOURCE_VIEW_DESC m_SRVDesc{};
		public:
			TextureClone Clone() const noexcept;
		};
	}
}