#pragma once 

namespace EngineFramework {
	namespace Resource{
		class TextureContainer : public DescriptorResourceContainer{
		public:
			TextureContainer();
			TextureContainer(const ICommandList* pCommandList, const std::tstring& ctsPath);
			~TextureContainer();
		private:
			ComPtr<ID3D12Resource> m_d3dTextureDefaultHeap{ nullptr };
			ComPtr<ID3D12Resource> m_d3dTextureUploadHeap{ nullptr };
			ComPtr<ID3D12DescriptorHeap> m_d3dSRVHeap{ nullptr };
			D3D12_CPU_DESCRIPTOR_HANDLE m_d3dSRVHandle{};
			const D3D12_DESCRIPTOR_RANGE_TYPE m_d3dResourceType{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV };
			DescriptorID m_nDescriptorID{ -1 };
		public:
			virtual DescriptorObject Clone() const override;
			virtual void GetDescriptorID(IDescriptorTable* pDescriptorTable,UINT nShaderRegister) override;
		};



	}
}