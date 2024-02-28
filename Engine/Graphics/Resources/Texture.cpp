#include "EnginePch.h"
#include "Texture.h"

namespace EngineFramework {
	namespace Resource {
		Texture::Texture() {

		}


		Texture::Texture(const Texture& other){
			m_d3dTex2D = other.m_d3dTex2D;
			m_d3dTexUploadHeap = other.m_d3dTexUploadHeap;
			m_d3dSRVHeap = other.m_d3dSRVHeap;
			m_d3dSRVHandle = other.m_d3dSRVHandle;
		}

		Texture::~Texture() {
			
		}

		void Texture::Initialize(const IDevice* pDevice,const ICommandList* pCommandList,const std::tstring& ImagePath) {

			DirectX::ScratchImage Image{};
			std::vector<D3D12_SUBRESOURCE_DATA> SubresourceData{};

			std::tstring ext = fs::path(ImagePath).extension();

			if (ext == _T(".dds") or ext == _T(".DDS")) {
				DirectX::LoadFromDDSFile(ImagePath.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, Image);
			}
			else if (ext == _T(".tga") or ext == _T(".TGA")) {
				DirectX::LoadFromTGAFile(ImagePath.c_str(), nullptr, Image);
			}
			else {
				DirectX::LoadFromWICFile(ImagePath.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, Image);
			}


			CheckFailed(DirectX::CreateTexture(pDevice->GetDevice().Get(), Image.GetMetadata(), m_d3dTex2D.GetAddressOf())); // <-- 만들어질때 적절한 상태로 생성되나?



			CheckFailed(DirectX::PrepareUpload(
				pDevice->GetDevice().Get(),
				Image.GetImages(),
				Image.GetImageCount(),
				Image.GetMetadata(),
				SubresourceData));

			const UINT64 BufferSize = ::GetRequiredIntermediateSize(m_d3dTex2D.Get(), 0, static_cast<UINT32>(SubresourceData.size()));
			CD3DX12_HEAP_PROPERTIES HeapProperty = CD3DX12_HEAP_PROPERTIES{ D3D12_HEAP_TYPE_UPLOAD };
			CD3DX12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(BufferSize);
			CheckFailed(
				pDevice->GetDevice()->CreateCommittedResource(
					&HeapProperty,
					D3D12_HEAP_FLAG_NONE,
					&ResourceDesc,
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(m_d3dTexUploadHeap.GetAddressOf())
				)
			);

			D3D12_DESCRIPTOR_HEAP_DESC SRVHeapDesc{};
			SRVHeapDesc.NumDescriptors = 1;
			SRVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			SRVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			SRVHeapDesc.NodeMask = 0;
			pDevice->GetDevice()->CreateDescriptorHeap(&SRVHeapDesc, IID_PPV_ARGS(m_d3dSRVHeap.GetAddressOf()));

			m_d3dSRVHandle = m_d3dSRVHeap->GetCPUDescriptorHandleForHeapStart();

			D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc{};
			SRVDesc.Format = Image.GetMetadata().format;
			SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			SRVDesc.Texture2D.MipLevels = 1;
			pDevice->GetDevice()->CreateShaderResourceView(m_d3dTex2D.Get(), &SRVDesc, m_d3dSRVHandle);


			::UpdateSubresources(
				pCommandList->GetCommandList().Get(),
				m_d3dTex2D.Get(),
				m_d3dTexUploadHeap.Get(),
				0, 0, static_cast<UINT>(SubresourceData.size()),
				SubresourceData.data());
		}
	}
}