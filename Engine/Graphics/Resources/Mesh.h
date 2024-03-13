#pragma once 
namespace EngineFramework {
	namespace Resource {
		class MeshContainer {
			friend class Mesh;
		public:
			MeshContainer();
			MeshContainer(const ICommandList* pCommandList,std::vector<Vertex>& Vertices, std::vector<UINT>& Indices);
			~MeshContainer();
			MeshContainer(const MeshContainer& other);
			MeshContainer& operator=(const MeshContainer& other);
		private:
			ComPtr<ID3D12Resource> m_d3dVertexDefaultBuffer{ nullptr };		// 디폴트 힙  
			ComPtr<ID3D12Resource> m_d3dVertexUploadBuffer{ nullptr };		// 업로드 힙 
			std::vector<Vertex> m_vertices{};								// 실제 데이터 
			D3D12_VERTEX_BUFFER_VIEW m_d3dVertexBufferView{};				// 리턴 값 ( clone 을 위한 )

			ComPtr<ID3D12Resource> m_d3dIndexDefaultBuffer{ nullptr };		// 디폴트 힙
			ComPtr<ID3D12Resource> m_d3dIndexUploadBuffer{ nullptr };		// 업로드 힙 
			std::vector<UINT> m_indices{};									// 실제 데이터
			D3D12_INDEX_BUFFER_VIEW m_d3dIndexBufferView{};					// 리턴 값 ( clone 을 위한 )
		private:
			ComPtr<ID3D12Resource> CreateBuffer(const ICommandList* pCommandList,ComPtr<ID3D12Resource>& d3dUploadBuffer,void* pvData,UINT64 nSize);

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
			void BindResource(const ICommandList* pCommandList,D3D_PRIMITIVE_TOPOLOGY PrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		};

	}
}