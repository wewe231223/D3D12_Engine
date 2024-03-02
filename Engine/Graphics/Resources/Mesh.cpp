#include "EnginePch.h"
#include "Mesh.h"
#include "ResourceManager.h"

namespace EngineFramework {
	namespace Resource {



		MeshContainer::MeshContainer(){

		}
		MeshContainer::MeshContainer(const ICommandList* pCommandList,std::vector<Vertex>& Vertices, std::vector<UINT>& Indices){
			m_vertices = std::vector<Vertex>(Vertices);
			m_indices = std::vector<UINT>(Indices);

			ID3D12Device* Device{};
			pCommandList->GetCommandList()->GetDevice(IID_PPV_ARGS(&Device));

			m_d3dVertexDefaultBuffer = CreateBuffer(Device,pCommandList,m_d3dVertexUploadBuffer,Vertices.data(), static_cast<UINT64>(m_vertices.size() * sizeof(Vertex)));
			m_d3dIndexDefaultBuffer = CreateBuffer(Device,pCommandList,m_d3dIndexUploadBuffer,Indices.data(), static_cast<UINT64>(m_indices.size() * sizeof(UINT)));
			
			m_d3dVertexBufferView.BufferLocation = m_d3dVertexDefaultBuffer->GetGPUVirtualAddress();
			m_d3dVertexBufferView.SizeInBytes = static_cast<UINT>(sizeof(Vertex));
			m_d3dVertexBufferView.StrideInBytes = static_cast<UINT>(m_vertices.size() * sizeof(Vertex));

			m_d3dIndexBufferView.BufferLocation = m_d3dIndexDefaultBuffer->GetGPUVirtualAddress();
			m_d3dIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
			m_d3dIndexBufferView.SizeInBytes = static_cast<UINT>(m_indices.size() * sizeof(UINT));
		
			Device->Release();
		}
		MeshContainer::~MeshContainer(){

		}
		MeshContainer::MeshContainer(const MeshContainer& other){

		}
		MeshContainer& MeshContainer::operator=(const MeshContainer& other){
			m_d3dVertexBufferView = other.m_d3dVertexBufferView;
			m_d3dIndexBufferView = other.m_d3dIndexBufferView;
			return *this;
		}

		ComPtr<ID3D12Resource> MeshContainer::CreateBuffer(ID3D12Device* pDevice,const ICommandList* pCommandList,ComPtr<ID3D12Resource>& d3dUploadBuffer,void* pvData,UINT64 nSize){
			CD3DX12_HEAP_PROPERTIES DefaultHeapProperties{ D3D12_HEAP_TYPE_DEFAULT };
			CD3DX12_HEAP_PROPERTIES UploadHeapProperties{ D3D12_HEAP_TYPE_UPLOAD };

			CD3DX12_RESOURCE_DESC BufferDesc{ CD3DX12_RESOURCE_DESC::Buffer(nSize) };

			ComPtr<ID3D12Resource> DefaultBuffer{ nullptr };


			CheckFailed(pDevice->CreateCommittedResource(
				&DefaultHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&BufferDesc,
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				IID_PPV_ARGS(DefaultBuffer.GetAddressOf())
			));

			CheckFailed(pDevice->CreateCommittedResource(
				&UploadHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&BufferDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(d3dUploadBuffer.GetAddressOf())
			));
			
			D3D12_SUBRESOURCE_DATA ResourceData{};
			ResourceData.pData = pvData;
			ResourceData.RowPitch = nSize;
			ResourceData.SlicePitch = nSize;

			CD3DX12_RESOURCE_BARRIER ResourceBarrier{ CD3DX12_RESOURCE_BARRIER::Transition(DefaultBuffer.Get(),D3D12_RESOURCE_STATE_COMMON,D3D12_RESOURCE_STATE_COPY_DEST) };
			pCommandList->GetCommandList()->ResourceBarrier(1, &ResourceBarrier);

			UpdateSubresources<1>(pCommandList->GetCommandList().Get(), DefaultBuffer.Get(), d3dUploadBuffer.Get(), 0, 0, 1, &ResourceData);



			return DefaultBuffer;
		}


		Mesh::Mesh(){

		}
		
		Mesh::Mesh(const MeshContainer* other){
			m_d3dVertexView = other->m_d3dVertexBufferView;
			m_d3dIndexView = other->m_d3dIndexBufferView;
		}

		Mesh::~Mesh(){

		}

		void Mesh::BindBuffer(const ICommandList* pCommandList, D3D_PRIMITIVE_TOPOLOGY PrimitiveTopology){
			pCommandList->GetCommandList()->IASetVertexBuffers(0, 1, &m_d3dVertexView);
			pCommandList->GetCommandList()->IASetIndexBuffer(&m_d3dIndexView);
			pCommandList->GetCommandList()->IASetPrimitiveTopology(PrimitiveTopology);
		}


	}
}