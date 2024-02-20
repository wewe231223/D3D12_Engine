#include "EnginePch.h"
#include "Texture.h"

namespace EngineFramework {
	Texture::Texture() {

	}

	Texture::~Texture() {

	}

	void Texture::Initialize(const std::tstring& ImagePath){
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



	}


}