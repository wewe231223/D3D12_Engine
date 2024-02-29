#include "EnginePch.h"
#include "GraphicObject.h"
#include "Resources/Mesh.h"
#include "Resources/Texture.h"
#include "Resources/ResourceManager.h"

namespace EngineFramework {
	void GraphicObject::Initialize(const Resource::IResourceManager* pResourceManager, const std::tstring& ctsMeshName, const std::tstring& ctsTextureName){
		m_mesh = std::make_unique<Resource::Mesh>(pResourceManager->GetMesh(ctsMeshName));
		m_texture = std::make_unique<Resource::Texture>(pResourceManager->GetTexture(ctsTextureName));
	}


}