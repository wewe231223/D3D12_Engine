#include "EnginePch.h"
#include "GraphicObject.h"
#include "Resources/Mesh.h"
#include "Resources/Texture.h"

namespace EngineFramework {
	void GraphicObject::SetMesh(std::unique_ptr<Resource::Mesh> pMesh){
		m_mesh = std::move(pMesh);
	}
	void GraphicObject::SetTexture(std::unique_ptr<Resource::Texture> pTexture){
		m_texture = std::move(pTexture);
	}
}