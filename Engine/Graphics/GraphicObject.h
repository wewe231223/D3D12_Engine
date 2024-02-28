#pragma once 

namespace EngineFramework {
	namespace Resource {
		class Mesh;
		class Texture;
		__interface IResourceManager;
	}
	/// <summary>
	/// Graphic Object is Primitive "Object" it contains Mesh = (Vertex) and Texture
	/// </summary>
	class GraphicObject {
	public:
		GraphicObject();
		~GraphicObject();
	private:
		std::unique_ptr<Resource::Mesh> m_mesh{ nullptr };
		std::unique_ptr<Resource::Texture> m_texture{ nullptr };
	public:
		void Initialize(const Resource::IResourceManager* pResourceManager,const std::tstring& ctsMeshName,const std::tstring& ctsTextureName);
	};
}