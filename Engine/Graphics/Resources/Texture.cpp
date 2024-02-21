#include "EnginePch.h"
#include "Texture.h"

namespace EngineFramework {
	Texture::Texture() {

	}

	Texture::~Texture() {

	}

	void Texture::Initialize(const IDevice* pDevice, const ICommandQueue* pCommandQueue,const std::tstring& ImagePath){
		std::tstring ext = fs::path(ImagePath).extension();

		if (ext == _T(".dds") or ext == _T(".DDS")) {
			DirectX::LoadFromDDSFile(ImagePath.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, m_image);
		}
		else if (ext == _T(".tga") or ext == _T(".TGA")) {
			DirectX::LoadFromTGAFile(ImagePath.c_str(), nullptr, m_image);
		}
		else {
			DirectX::LoadFromWICFile(ImagePath.c_str(), DirectX::WIC_FLAGS_NONE,nullptr, m_image);
		}


		CheckFailed(DirectX::CreateTexture(pDevice->GetDevice().Get(), m_image.GetMetadata(), m_d3dTex2D.GetAddressOf()));


		std::vector<D3D12_SUBRESOURCE_DATA> SubResources{};

		CheckFailed(DirectX::PrepareUpload(
			pDevice->GetDevice().Get(),
			m_image.GetImages(),
			m_image.GetImageCount(),
			m_image.GetMetadata(),
			SubResources));

		const UINT64 BufferSize = ::GetRequiredIntermediateSize(m_d3dTex2D.Get(), 0, static_cast<UINT32>(SubResources.size()));
		CD3DX12_HEAP_PROPERTIES HeapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(BufferSize);

		 


	}


}