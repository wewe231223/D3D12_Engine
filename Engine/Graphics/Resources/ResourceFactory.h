#pragma once
#include "ResourceDefine.h"

namespace EngineFramework {
	namespace Resource {
		class ResourceFactory abstract {
		public:
			ResourceFactory();
			~ResourceFactory();
		protected:
			ComPtr<ID3D12Resource> m_d3dDefaultHeap{ nullptr };
			ComPtr<ID3D12Resource> m_d3dUploadHeap{ nullptr };
		public:
			virtual ResourceContainTypes Create(const ICommandList*, ResourceParameter) PURE;
		};


		class MeshFactory : public ResourceFactory {
		public:
			MeshFactory();
			~MeshFactory();
		public:
			virtual ResourceContainTypes Create(const ICommandList* pCommandList, ResourceParameter Param) override;
		};
	}
}