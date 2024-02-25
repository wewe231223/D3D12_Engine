#pragma once 
namespace EngineFramework {
	namespace Resource {
		class Mesh {
			friend class MeshManager;
		public:
			Mesh();
			~Mesh();
			Mesh(const Mesh& other);
			Mesh& operator=(const Mesh& other);
		private:
			std::tstring m_tsMeshName{};
			UINT m_nVertexStartLocation{};
			UINT m_nIndexStartLocation{};
			UINT m_nIndexCount{};
		public:
			void Render(const ICommandList* pCommandList) const;
		};

		class MeshManager {
		public:
			MeshManager();
			~MeshManager();
		private:
			ComPtr<ID3D12Resource> m_d3dVertexDefaultBuffer{ nullptr };
			ComPtr<ID3D12Resource> m_d3dIndexDefaultBuffer{ nullptr };

			ComPtr<ID3D12Resource> m_d3dVertexUploadBuffer{ nullptr };
			ComPtr<ID3D12Resource> m_d3dIndexUploadBuffer{ nullptr };

			UINT m_nVertexByteStride{ 0 };
			UINT m_nVertexByteSize{ 0 };
			UINT m_nIndexBufferByteSize{ 0 };
			DXGI_FORMAT m_dxgiIndexFormat{DXGI_FORMAT_R32_UINT};

			D3D12_VERTEX_BUFFER_VIEW m_d3dVertexBufferView{};
			D3D12_INDEX_BUFFER_VIEW m_d3dIndexBufferView{};

			std::vector<Vertex> m_vertices{};
			std::vector<UINT> m_indices{};
		public:
			std::shared_ptr<Mesh> CreateMesh(const std::tstring& tcsMeshName,const std::vector<Vertex>& vertices, const std::vector<UINT>& indices);
			void Upload(const IDevice* pDevice,const ICommandList* pCommandList);
			void BindBuffer(const ICommandList* pCommandList, D3D_PRIMITIVE_TOPOLOGY d3dMeshTopology) const;
		private:
			ComPtr<ID3D12Resource> CreateBuffer(const IDevice* pDevice, const ICommandList* pCommandList,ComPtr<ID3D12Resource>& d3dUploadBuffer,void* pvData,UINT64 nByteSize);

		};


	}
}