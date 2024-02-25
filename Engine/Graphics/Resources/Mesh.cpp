#include "EnginePch.h"
#include "Mesh.h"

namespace EngineFramework {
	namespace Resource {
		Mesh::Mesh(){

		}

		Mesh::~Mesh(){

		}

		Mesh::Mesh(const Mesh& other){
			m_tsMeshName = other.m_tsMeshName;
			m_nVertexStartLocation = other.m_nVertexStartLocation;
			m_nIndexStartLocation = other.m_nIndexStartLocation;
			m_nIndexCount = other.m_nIndexCount;
		}

		Mesh& Mesh::operator=(const Mesh& other){
			m_tsMeshName = other.m_tsMeshName;
			m_nVertexStartLocation = other.m_nVertexStartLocation;
			m_nIndexStartLocation = other.m_nIndexStartLocation;
			m_nIndexCount = other.m_nIndexCount;
			return *this;
		}

		void Mesh::Render(const ICommandList* pCommandList) const {
			// 그리기 연산 인스턴싱에 대한 지원은 오버로딩으로 구현 
			pCommandList->GetCommandList()->DrawIndexedInstanced(m_nIndexCount, 1, m_nIndexStartLocation, m_nVertexStartLocation, 0);
		}

		MeshManager::MeshManager(){

		}

		MeshManager::~MeshManager(){
			if (m_d3dVertexUploadBuffer) m_d3dVertexUploadBuffer = nullptr;
			if (m_d3dIndexUploadBuffer) m_d3dIndexUploadBuffer = nullptr;
		}

		std::shared_ptr<Mesh>MeshManager::CreateMesh(const std::tstring& tcsMeshName, const std::vector<Vertex>& vertices, const std::vector<UINT>& indices) {
			std::shared_ptr<Mesh> Result = std::make_shared<Mesh>();
			Result->m_tsMeshName = tcsMeshName;
			Result->m_nIndexCount = static_cast<UINT>(indices.size());

			if (m_vertices.size() == 0) {
				Result->m_nVertexStartLocation = 0;
			}
			else {
				Result->m_nVertexStartLocation = static_cast<UINT>(m_vertices.size() - 1);
			}

			if (m_indices.size() == 0) {
				Result->m_nIndexStartLocation = 0;
			}
			else {
				Result->m_nIndexStartLocation = static_cast<UINT>(m_indices.size() - 1);
			}

			m_vertices.insert(m_vertices.begin(), vertices.begin(), vertices.end());
			m_indices.insert(m_indices.begin(), indices.begin(), indices.end());

			return Result;
		}

		void MeshManager::Upload(const IDevice* pDevice, const ICommandList* pCommandList){
			if (m_d3dVertexDefaultBuffer) m_d3dVertexDefaultBuffer = nullptr;
			if (m_d3dVertexUploadBuffer) m_d3dVertexUploadBuffer = nullptr;
			if (m_d3dIndexUploadBuffer) m_d3dIndexUploadBuffer = nullptr;
			if (m_d3dIndexDefaultBuffer) m_d3dIndexDefaultBuffer = nullptr;

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

		void MeshManager::BindBuffer(const ICommandList* pCommandList,D3D_PRIMITIVE_TOPOLOGY d3dMeshTopology) const{
			pCommandList->GetCommandList()->IASetVertexBuffers(0, 1, &m_d3dVertexBufferView);
			pCommandList->GetCommandList()->IASetIndexBuffer(&m_d3dIndexBufferView);
			pCommandList->GetCommandList()->IASetPrimitiveTopology(d3dMeshTopology);
		}

		ComPtr<ID3D12Resource> MeshManager::CreateBuffer(const IDevice* pDevice, const ICommandList* pCommandList, ComPtr<ID3D12Resource>& d3dUploadBuffer,void* pvData,UINT64 nByteSize){
			CD3DX12_HEAP_PROPERTIES HeapProperties{ D3D12_HEAP_TYPE_DEFAULT };
			CD3DX12_RESOURCE_DESC BufferDesc{ CD3DX12_RESOURCE_DESC::Buffer(nByteSize) };

			ComPtr<ID3D12Resource> DefaultBuffer{ nullptr };

			CheckFailed(pDevice->GetDevice()->CreateCommittedResource(
				&HeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&BufferDesc,
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				IID_PPV_ARGS(DefaultBuffer.GetAddressOf())
			));

			HeapProperties = CD3DX12_HEAP_PROPERTIES{ D3D12_HEAP_TYPE_UPLOAD };

			CheckFailed(pDevice->GetDevice()->CreateCommittedResource(
				&HeapProperties,
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

			CD3DX12_RESOURCE_BARRIER ResourceBarrier{ CD3DX12_RESOURCE_BARRIER::Transition(DefaultBuffer.Get(),D3D12_RESOURCE_STATE_COMMON,D3D12_RESOURCE_STATE_COPY_DEST) };

			pCommandList->GetCommandList()->ResourceBarrier(1, &ResourceBarrier);
			UpdateSubresources<1>(pCommandList->GetCommandList().Get(), DefaultBuffer.Get(), d3dUploadBuffer.Get(), 0, 0, 1, &ResourceData);

			ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(DefaultBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
			pCommandList->GetCommandList()->ResourceBarrier(1, &ResourceBarrier);

		

			return DefaultBuffer;
		}


	}
}