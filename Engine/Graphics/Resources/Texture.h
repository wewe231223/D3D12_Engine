#pragma once 

namespace EngineFramework {
	namespace Resource {
		class Texture {
		public:
			Texture();
			Texture(const Texture& other);
			~Texture();
		private:
			ComPtr<ID3D12Resource> m_d3dTex2D{ nullptr };
			ComPtr<ID3D12Resource> m_d3dTexUploadHeap{ nullptr };
			ComPtr<ID3D12DescriptorHeap> m_d3dSRVHeap{ nullptr };
			D3D12_CPU_DESCRIPTOR_HANDLE m_d3dSRVHandle{};
			DescriptorID m_descriptorID{};
		public:
			void Initialize(const IDevice* pDevice,const ICommandList* pCommandList,IDescriptorTable* pDescriptorTable,const std::tstring& ImagePath,UINT nSRVRegister = 0U);
			D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorHandle() const { return m_d3dSRVHandle; }
			void BindTexture(const IDevice* pDevice,const IDescriptorTable* pDescriptorTable);
		};
	}
}