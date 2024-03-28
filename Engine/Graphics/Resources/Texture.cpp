#include "EnginePch.h"
#include "Texture.h"

namespace EngineFramework {
	namespace Resource {
		TextureContainer::TextureContainer()
		{
		}

		TextureContainer::TextureContainer(const ICommandList* pCommandList, const std::tstring& ctsPath){

			ID3D12Device* Device{};
			pCommandList->GetCommandList()->GetDevice(IID_PPV_ARGS(&Device));
				
			DirectX::ScratchImage Image{};
			std::vector<D3D12_SUBRESOURCE_DATA> SubresourceData{};

			std::tstring ext = fs::path(ctsPath).extension();

			if (ext == _T(".dds") or ext == _T(".DDS")) {
				DirectX::LoadFromDDSFile(ctsPath.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, Image);
			}
			else if (ext == _T(".tga") or ext == _T(".TGA")) {
				DirectX::LoadFromTGAFile(ctsPath.c_str(), nullptr, Image);
			}
			else {
				DirectX::LoadFromWICFile(ctsPath.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, Image);
			}


			CheckFailed(DirectX::CreateTexture(Device, Image.GetMetadata(), m_d3dTextureDefaultHeap.GetAddressOf())); // <-- 만들어질때 적절한 상태로 생성되나?

			CheckFailed(DirectX::PrepareUpload(
				Device,
				Image.GetImages(),
				Image.GetImageCount(),
				Image.GetMetadata(),
				SubresourceData));

			const UINT64 BufferSize = ::GetRequiredIntermediateSize(m_d3dTextureDefaultHeap.Get(), 0, static_cast<UINT32>(SubresourceData.size()));
			CD3DX12_HEAP_PROPERTIES HeapProperty = CD3DX12_HEAP_PROPERTIES{ D3D12_HEAP_TYPE_UPLOAD };
			CD3DX12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(BufferSize);
			CheckFailed(
				Device->CreateCommittedResource(
					&HeapProperty,
					D3D12_HEAP_FLAG_NONE,
					&ResourceDesc,
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(m_d3dTextureUploadHeap.GetAddressOf())
				)
			);
			        
		
			m_SRVDesc.Format = Image.GetMetadata().format;
			m_SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			m_SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			m_SRVDesc.Texture2D.MipLevels = 1;
			
			::UpdateSubresources(
				pCommandList->GetCommandList().Get(),
				m_d3dTextureDefaultHeap.Get(),
				m_d3dTextureUploadHeap.Get(),
				0, 0, static_cast<UINT>(SubresourceData.size()),
				SubresourceData.data());

			Device->Release();
		}

		TextureContainer::~TextureContainer(){

		}

		TextureClone TextureContainer::Clone() const noexcept {
			return TextureClone(m_d3dTextureDefaultHeap.Get(),m_SRVDesc);
		}


		void TextureClone::MakeSRV(IDevice* pDevice, ID3D12DescriptorHeap* pDescriptorHeap,UINT nDescIndex){
			D3D12_CPU_DESCRIPTOR_HANDLE Handle{ pDescriptorHeap->GetCPUDescriptorHandleForHeapStart() };
			Handle.ptr += pDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * nDescIndex;
			pDevice->GetDevice()->CreateShaderResourceView(m_pResource, &m_SRVDesc, Handle);
		}

	}
}