#pragma once 

namespace EngineFramework {
	namespace Resource {
		class ResourceManager {
		public:
			ResourceManager();
			~ResourceManager();
		private:
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
				DXGI_FORMAT m_dxgiIndexFormat{ DXGI_FORMAT_R32_UINT };

				D3D12_VERTEX_BUFFER_VIEW m_d3dVertexBufferView{};
				D3D12_INDEX_BUFFER_VIEW m_d3dIndexBufferView{};

				std::vector<Vertex> m_vertices{};
				std::vector<UINT> m_indices{};
			public:
				void Create(const std::tstring& ctsMeshName, const std::vector<Vertex>& Vertices, const std::vector<UINT>& Indices);
				void Upload(const IDevice* pDevice, const ICommandList* pCommandList);
				void BindBuffer(const ICommandQueue* pCommandQueue, D3D_PRIMITIVE_TOPOLOGY d3dMeshTopology);
				std::unique_ptr<class Mesh> GetMesh(const std::tstring& ctsMeshName);
			private:
				ComPtr<ID3D12Resource> CreateBuffer(const IDevice* pDevice, const ICommandList* pCommandList,ComPtr<ID3D12Resource>& d3dUploadBuffer,void* pvData,UINT64 nByteSize);

			};

			class TextureManager {
			public:
				TextureManager();
				~TextureManager();
			private:

			};
		private:
			std::unordered_map<std::tstring, class Mesh> m_meshMap{};
			std::unordered_map<std::tstring, class Texture> m_textureMap{};
			MeshManager m_cMeshManager{};
			TextureManager m_cTextureManager{};
		};
	}
}