#pragma once
#include "ResourceDefine.h"

namespace EngineFramework {
	namespace Resource {
#pragma region FactoryLevel1
		class ResourceFactory abstract{
		public:
			virtual ~ResourceFactory();
			virtual ResourceContainTypes Create(const ICommandList*, ResourceParameter) PURE;
		};	
#pragma endregion FactoryLevel1
#pragma region FactoryLevel2
		class SingleHeapFactory abstract: public ResourceFactory{
		public:
			SingleHeapFactory();
			~SingleHeapFactory();
		protected:
			ComPtr<ID3D12Resource> m_d3dDefaultHeap{ nullptr };
			ComPtr<ID3D12Resource> m_d3dUploadHeap{ nullptr };
		public:
			virtual ResourceContainTypes Create(const ICommandList*, ResourceParameter) PURE;
		};

		class DoubleHeapFactory abstract : public ResourceFactory {
		public:
			DoubleHeapFactory();
			virtual ~DoubleHeapFactory();
		protected:
			ComPtr<ID3D12Resource> m_d3dDefaultHeap1{ nullptr };
			ComPtr<ID3D12Resource> m_d3dUploadHeap1{ nullptr };
			ComPtr<ID3D12Resource> m_d3dDefaultHeap2{ nullptr };
			ComPtr<ID3D12Resource> m_d3dUploadHeap2{ nullptr };
		public:
			virtual ResourceContainTypes Create(const ICommandList*, ResourceParameter) PURE;
		};

#pragma endregion FactoryLevel2
		

		class MeshFactory : public ResourceFactory {
		public:
			MeshFactory();
			virtual ~MeshFactory();
		private:
			ComPtr<ID3D12Resource> m_d3dDefaultHeap1{ nullptr };
			ComPtr<ID3D12Resource> m_d3dUploadHeap1{ nullptr };
			ComPtr<ID3D12Resource> m_d3dDefaultHeap2{ nullptr };
			ComPtr<ID3D12Resource> m_d3dUploadHeap2{ nullptr };
		public:
			virtual ResourceContainTypes Create(const ICommandList* pCommandList, ResourceParameter Param) override;
		private:	
			ComPtr<ID3D12Resource> CreateBuffer(const ICommandList* pCommandList, ComPtr<ID3D12Resource>& d3dUploadBuffer, void* pvData, UINT64 nSize);
		};
	}
}