#pragma once 
namespace EngineFramework {
	namespace Resource {
		class MeshClone {
		public:
			MeshClone();
			MeshClone(const D3D12_VERTEX_BUFFER_VIEW& VertexView,const D3D12_INDEX_BUFFER_VIEW& IndexView);
			~MeshClone();
		public:
			D3D12_VERTEX_BUFFER_VIEW m_d3dVertexView{};
			D3D12_INDEX_BUFFER_VIEW m_d3dIndexView{};
		public:
			void BindResource(const ICommandList* pCommandList,D3D_PRIMITIVE_TOPOLOGY PrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		};

		class Mesh : public DelPtr{
		public:
			Mesh();
			Mesh(const ICommandList* pCommandList,std::vector<Vertex>& Vertices, std::vector<UINT>& Indices);
			~Mesh();
			Mesh(const Mesh& other);
			Mesh& operator=(const Mesh& other);
		private:
			ComPtr<ID3D12Resource> m_d3dVertexDefaultBuffer{ nullptr };		// 디폴트 힙  
			ComPtr<ID3D12Resource> m_d3dVertexUploadBuffer{ nullptr };		// 업로드 힙 
			std::vector<Vertex> m_vertices{};								// 실제 데이터 
			D3D12_VERTEX_BUFFER_VIEW m_d3dVertexBufferView{};				// 리턴 값 ( clone 을 위한 )

			ComPtr<ID3D12Resource> m_d3dIndexDefaultBuffer{ nullptr };		// 디폴트 힙
			ComPtr<ID3D12Resource> m_d3dIndexUploadBuffer{ nullptr };		// 업로드 힙 
			std::vector<UINT> m_indices{};									// 실제 데이터
			D3D12_INDEX_BUFFER_VIEW m_d3dIndexBufferView{};					// 리턴 값 ( clone 을 위한 )
		public:
			MeshClone Clone() const;
		private:
			ComPtr<ID3D12Resource> CreateBuffer(const ICommandList* pCommandList,ComPtr<ID3D12Resource>& d3dUploadBuffer,void* pvData,UINT64 nSize);
		
		};

	}
}