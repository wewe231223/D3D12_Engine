#pragma once 
namespace EngineFramework {
	namespace Resource {
		class MeshContainer {
			friend class Mesh;
		public:
			MeshContainer();
			MeshContainer(const IDevice* pDevice,const ICommandList* pCommandList,std::vector<Vertex>& Vertices, std::vector<UINT>& Indices);
			~MeshContainer();
			MeshContainer(const MeshContainer& other);
			MeshContainer& operator=(const MeshContainer& other);
		private:
			ComPtr<ID3D12Resource> m_d3dVertexDefaultBuffer{ nullptr };
			ComPtr<ID3D12Resource> m_d3dIndexDefaultBuffer{ nullptr };

			ComPtr<ID3D12Resource> m_d3dVertexUploadBuffer{ nullptr };
			ComPtr<ID3D12Resource> m_d3dIndexUploadBuffer{ nullptr };
			
			D3D12_VERTEX_BUFFER_VIEW m_d3dVertexBufferView{};
			D3D12_INDEX_BUFFER_VIEW m_d3dIndexBufferView{};

			std::vector<Vertex> m_vertices{};
			std::vector<UINT> m_indices{};
		private:
			ComPtr<ID3D12Resource> CreateBuffer(const IDevice* pDevice,const ICommandList* pCommandList,ComPtr<ID3D12Resource>& d3dUploadBuffer,void* pvData,UINT64 nSize);

		};

		class Mesh {
		public:
			Mesh();
			Mesh(const MeshContainer* other);
			~Mesh();
		private:
			D3D12_VERTEX_BUFFER_VIEW m_d3dVertexView{};
			D3D12_INDEX_BUFFER_VIEW m_d3dIndexView{};
		public:
			void BindBuffer(const ICommandList* pCommandList,D3D_PRIMITIVE_TOPOLOGY PrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		};

	}
}