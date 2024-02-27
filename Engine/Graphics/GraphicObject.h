#pragma once 

namespace EngineFramework {
	namespace Resource {
		class Mesh;
		class Texture;
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
		virtual void SetMesh(std::unique_ptr<Resource::Mesh> pMesh);
		virtual void SetTexture(std::unique_ptr<Resource::Texture> pTexture);

	};
}