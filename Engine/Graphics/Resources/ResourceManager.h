#pragma once 

namespace EngineFramework {
	class CommandList;
	namespace Resource {
		class Mesh;
		class Texture;

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
				std::unordered_map<std::tstring,Mesh> m_meshMap{};
			public:
				void CreateMesh(const std::tstring& ctsMeshName, const std::vector<Vertex>& Vertices, const std::vector<UINT>& Indices);
				void Upload(const IDevice* pDevice, const ICommandList* pCommandList);
				void BindBuffer(const ICommandList* pCommandList, D3D_PRIMITIVE_TOPOLOGY d3dMeshTopology);
				Mesh GetMesh(const std::tstring& ctsMeshName);
			private:
				ComPtr<ID3D12Resource> CreateBuffer(const IDevice* pDevice, const ICommandList* pCommandList,ComPtr<ID3D12Resource>& d3dUploadBuffer,void* pvData,UINT64 nByteSize);

			};

			class TextureManager {
			public:
				TextureManager();
				~TextureManager();
			private:
				std::unordered_map<std::tstring,Texture> m_textureMap{};
			public:
				void CreateTexture(const IDevice* pDevice,const ICommandList* pCommandList,IDescriptorTable* pDescriptorTable,const std::tstring& ctsTextureName,const std::tstring& ctsImagePath);
				Texture GetTexture(const std::tstring& ctsTextureName);
			};

		private:
			std::unique_ptr<CommandList> m_pCommandList{ nullptr };
			std::unique_ptr<MeshManager> m_pMeshManager{ nullptr };
			std::unique_ptr<TextureManager> m_pTextureManager{ nullptr };
		public:
			void Initialize(const IDevice* pDevice);
			void OpenCommandList();
			void CloseCommandList();
			void UploadResource(const IDevice* pDevice);
			void ExecuteList(const ICommandQueue* pCommandQueue);
		public:
			void NewResource(const std::tstring& MeshName,const std::vector<Vertex>& Vertices, const std::vector<UINT>& Indices);
			void NewResource(const IDevice* pDevice,IDescriptorTable* pDescriptorTable,const std::tstring& TextureName,const std::tstring& TexturePath);
		public:
			 Mesh GetMesh(const std::tstring& ctsMeshName) ;
			 Texture GetTexture(const std::tstring& ctsTextureName) ;
		};
	}
}