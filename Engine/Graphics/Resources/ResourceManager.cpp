#include "EnginePch.h"
#include "ResourceManager.h"
#include "EngineCore/CommandList.h"
#include "Mesh.h"
#include "Texture.h"

namespace EngineFramework {
	namespace Resource {
		void ResourceManager::MeshManager::CreateMesh(const std::tstring& ctsMeshName, const std::vector<Vertex>& Vertices, const std::vector<UINT>& Indices) {
			if (m_meshMap.find(ctsMeshName) != m_meshMap.end()) return;

			UINT VertexStart{ 0 };
			UINT IndexStart{ 0 };

			if (m_vertices.size() != 0) {
				VertexStart = static_cast<UINT>(m_vertices.size() - 1);
			}
			
			if (m_indices.size() != 0) {
				IndexStart = static_cast<UINT>(m_indices.size() - 1);
			}


			m_vertices.insert(m_vertices.begin(), Vertices.begin(), Vertices.end());
			m_indices.insert(m_indices.begin(), Indices.begin(), Indices.end());

			m_meshMap.insert(std::pair<std::tstring, Mesh>(ctsMeshName, Mesh{ctsMeshName,VertexStart,IndexStart,Indices.size()}));
		}

		void ResourceManager::MeshManager::Upload(const IDevice* pDevice, const ICommandList* pCommandList) {
			m_d3dVertexDefaultBuffer = CreateBuffer(pDevice, pCommandList, m_d3dVertexUploadBuffer, m_vertices.data(), static_cast<UINT64>(m_vertices.size() * sizeof(Vertex)));
			m_d3dIndexDefaultBuffer = CreateBuffer(pDevice, pCommandList, m_d3dIndexUploadBuffer, m_indices.data(), static_cast<UINT64>(m_indices.size() * sizeof(UINT)));

			UINT VertexByteSize{ static_cast<UINT>(m_vertices.size() * sizeof(Vertex)) };
			UINT IndexByteSize{ static_cast<UINT>(m_indices.size() * sizeof(UINT)) };

			m_d3dVertexBufferView.BufferLocation = m_d3dVertexDefaultBuffer->GetGPUVirtualAddress();
			m_d3dVertexBufferView.StrideInBytes = sizeof(Vertex);
			m_d3dVertexBufferView.SizeInBytes = VertexByteSize;

			m_d3dIndexBufferView.BufferLocation = m_d3dIndexDefaultBuffer->GetGPUVirtualAddress();
			m_d3dIndexBufferView.Format = m_dxgiIndexFormat;
			m_d3dIndexBufferView.SizeInBytes = IndexByteSize;
		}

		void ResourceManager::MeshManager::BindBuffer(const ICommandList* pCommandList, D3D_PRIMITIVE_TOPOLOGY d3dMeshTopology){
			pCommandList->GetCommandList()->IASetVertexBuffers(0, 1, &m_d3dVertexBufferView);
			pCommandList->GetCommandList()->IASetIndexBuffer(&m_d3dIndexBufferView);
			pCommandList->GetCommandList()->IASetPrimitiveTopology(d3dMeshTopology);
		}


		ComPtr<ID3D12Resource> ResourceManager::MeshManager::CreateBuffer(const IDevice* pDevice, const ICommandList* pCommandList, ComPtr<ID3D12Resource>& d3dUploadBuffer, void* pvData, UINT64 nByteSize) {
			CD3DX12_HEAP_PROPERTIES DefaultHeapProperties{ D3D12_HEAP_TYPE_DEFAULT };
			CD3DX12_HEAP_PROPERTIES UploadHeapProperties{ D3D12_HEAP_TYPE_UPLOAD };

			CD3DX12_RESOURCE_DESC BufferDesc{ CD3DX12_RESOURCE_DESC::Buffer(nByteSize) };

			ComPtr<ID3D12Resource> DefaultBuffer{ nullptr };

			CheckFailed(pDevice->GetDevice()->CreateCommittedResource(
				&DefaultHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&BufferDesc,
				D3D12_RESOURCE_STATE_COPY_DEST,
				nullptr,
				IID_PPV_ARGS(DefaultBuffer.GetAddressOf())
			));

			CheckFailed(pDevice->GetDevice()->CreateCommittedResource(
				&UploadHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&BufferDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(d3dUploadBuffer.GetAddressOf())
			));

			D3D12_SUBRESOURCE_DATA ResourceData{};
			ResourceData.pData = pvData;
			ResourceData.RowPitch = nByteSize;
			ResourceData.SlicePitch = nByteSize;

			UpdateSubresources<1>(pCommandList->GetCommandList().Get(), DefaultBuffer.Get(), d3dUploadBuffer.Get(), 0, 0, 1, &ResourceData);

			return DefaultBuffer;
		}

		Mesh ResourceManager::MeshManager::GetMesh(const std::tstring& ctsMeshName) {
			return m_meshMap[ctsMeshName];
		}

		ResourceManager::TextureManager::TextureManager(){
		}
		ResourceManager::TextureManager::~TextureManager(){
		}

		void ResourceManager::TextureManager::CreateTexture(const IDevice* pDevice, const ICommandList* pCommandList, const std::tstring& ctsTextureName, const std::tstring& ctsImagePath){
			Texture Tex{};
			Tex.Initialize(pDevice, pCommandList, ctsImagePath);
			m_textureMap.insert(std::pair<std::tstring, Texture>(ctsTextureName, Tex));
		}

		Texture ResourceManager::TextureManager::GetTexture(const std::tstring& ctsTextureName)
		{
			return m_textureMap[ctsTextureName];
		}

		ResourceManager::ResourceManager(){
			m_pCommandList = std::make_unique<CommandList>();
			m_pMeshManager = std::make_unique<MeshManager>();
			m_pTextureManager = std::make_unique<TextureManager>();
		}

		ResourceManager::~ResourceManager()
		{
		}

		void ResourceManager::Initialize(const IDevice* pDevice){
			m_pCommandList->Initialize(pDevice);
		}

		void ResourceManager::OpenCommandList(){
			m_pCommandList->Open();
		}

		void ResourceManager::CloseCommandList(){
			m_pCommandList->Close();
		}

		void ResourceManager::UploadResource(const IDevice* pDevice){
			m_pMeshManager->Upload(pDevice, m_pCommandList.get());
		}

		void ResourceManager::ExecuteList(const ICommandQueue* pCommandQueue){
			m_pCommandList->Execute(pCommandQueue);
			pCommandQueue->Sync();
		}

		void ResourceManager::NewResource(const std::tstring& MeshName, const std::vector<Vertex>& Vertices, const std::vector<UINT>& Indices){
			m_pMeshManager->CreateMesh(MeshName, Vertices, Indices);
		}

		void ResourceManager::NewResource(const IDevice* pDevice,const std::tstring& TextureName, const std::tstring& TexturePath){
			m_pTextureManager->CreateTexture(pDevice, m_pCommandList.get(), TextureName, TexturePath);
		}
		Mesh ResourceManager::GetMesh(const std::tstring& ctsMeshName){
			return m_pMeshManager->GetMesh(ctsMeshName);
		}
		Texture ResourceManager::GetTexture(const std::tstring& ctsTextureName){
			return m_pTextureManager->GetTexture(ctsTextureName);
		}
	}
}