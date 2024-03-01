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
		GraphicObject(const std::tstring& tsMeshName, const std::tstring& tsTextureName) : m_tsMeshName(tsMeshName), m_tsTextureName(tsTextureName) {};
		~GraphicObject();
	private:
		std::tstring m_tsObjectName{ _T("UNKNOWN") };
		std::tstring m_tsMeshName{};
		std::tstring m_tsTextureName{};
		std::unique_ptr<Resource::Mesh> m_mesh{ nullptr };
		std::unique_ptr<Resource::Texture> m_texture{ nullptr };
	public:
		const std::tstring GetMeshNane() const noexcept { return m_tsMeshName; }
		const std::tstring GetTextureName() const noexcept { return m_tsTextureName; }
		void Initialize(Resource::IResourceManager* pResourceManager);
		void Render(const ICommandList* pCommandList);
	};
}