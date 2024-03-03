#include "EnginePch.h"
#include "ResourceFactory.h"


namespace EngineFramework {
	namespace Resource {
		ResourceFactory::ResourceFactory()
		{
		}
		ResourceFactory::~ResourceFactory()
		{
		}
		MeshFactory::MeshFactory()
		{
		}
		MeshFactory::~MeshFactory()
		{
		}
		ResourceContainTypes MeshFactory::Create(const ICommandList* pCommandList, ResourceParameter Param) {
			const ResourceParameter Parameter = Param;

			if (std::holds_alternative<MeshParam>(Parameter)) {
				std::vector<Vertex> Vertices = std::vector<Vertex>(std::get<MeshParam>(Param).first);
				std::vector<UINT> Indices = std::vector<UINT>(std::get<MeshParam>(Param).second);
			}
			else {
				return ResourceContainTypes(MeshValue(D3D12_VERTEX_BUFFER_VIEW{}, D3D12_INDEX_BUFFER_VIEW{}));
			}




			return ResourceContainTypes();
		}
	}
}

