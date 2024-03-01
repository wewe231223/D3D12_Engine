#include "EnginePch.h"
#include "Object.h"
#include "Resources/Mesh.h"
#include "Resources/Texture.h"
#include "Resources/ResourceManager.h"

namespace EngineFramework {
	GraphicObject::~GraphicObject(){

	}

	void GraphicObject::Initialize(Resource::IResourceManager* pResourceManager){
		m_mesh = std::make_unique<Resource::Mesh>(pResourceManager->GetMesh(m_tsMeshName));
		m_texture = std::make_unique<Resource::Texture>(pResourceManager->GetTexture(m_tsTextureName));
	}



}