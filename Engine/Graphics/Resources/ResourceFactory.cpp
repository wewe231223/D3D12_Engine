#include "EnginePch.h"
#include "ResourceFactory.h"


namespace EngineFramework {
	namespace Resource {
		SingleHeapFactory::SingleHeapFactory(){

		}
		SingleHeapFactory::~SingleHeapFactory(){

		}

		DoubleHeapFactory::DoubleHeapFactory(){

		}
		DoubleHeapFactory::~DoubleHeapFactory(){
		}

		MeshFactory::MeshFactory()
		{
		}
		MeshFactory::~MeshFactory(){
			OutputDebugString(_T("\n\nDescructer Called\n\n"));
		}
		ResourceContainTypes MeshFactory::Create(const ICommandList* pCommandList, ResourceParameter Param) {
			const ResourceParameter Parameter = Param;
			std::vector<Vertex> Vertices{};
			std::vector<UINT> Indices{};
			if (std::holds_alternative<MeshParam>(Parameter)) {
				Vertices = std::vector<Vertex>(std::get<MeshParam>(Param).first);
				Indices = std::vector<UINT>(std::get<MeshParam>(Param).second);
			}
			else {
				throw System::Exeption(_T("[MeshFactory] : Param Doesn't Match for ContainType"));
			}

			m_d3dDefaultHeap1 = CreateBuffer(pCommandList, m_d3dUploadHeap1, Vertices.data(), static_cast<UINT64>(Vertices.size() * sizeof(Vertex)));
			m_d3dDefaultHeap2 = CreateBuffer(pCommandList, m_d3dUploadHeap2, Indices.data(), static_cast<UINT64>(Indices.size() * sizeof(UINT)));

			D3D12_VERTEX_BUFFER_VIEW VertexView{};
			VertexView.BufferLocation = m_d3dDefaultHeap1->GetGPUVirtualAddress();
			VertexView.StrideInBytes = static_cast<UINT>(sizeof(Vertex));
			VertexView.SizeInBytes = static_cast<UINT>(sizeof(Vertex) * Vertices.size());

			D3D12_INDEX_BUFFER_VIEW IndexView{};
			IndexView.BufferLocation = m_d3dDefaultHeap2->GetGPUVirtualAddress();
			IndexView.Format = DXGI_FORMAT_R32_UINT;
			IndexView.SizeInBytes = static_cast<UINT>(sizeof(UINT) * Indices.size());

			return ResourceContainTypes(MeshValue(VertexView,IndexView));
		}

		ComPtr<ID3D12Resource> MeshFactory::CreateBuffer(const ICommandList* pCommandList, ComPtr<ID3D12Resource>& d3dUploadBuffer, void* pvData, UINT64 nSize){
			
			ComPtr<ID3D12Device> Device{};
			pCommandList->GetCommandList()->GetDevice(IID_PPV_ARGS(Device.GetAddressOf()));

			CD3DX12_HEAP_PROPERTIES DefaultHeapProperties{ D3D12_HEAP_TYPE_DEFAULT };
			CD3DX12_HEAP_PROPERTIES UploadHeapProperties{ D3D12_HEAP_TYPE_UPLOAD };

			CD3DX12_RESOURCE_DESC BufferDesc{ CD3DX12_RESOURCE_DESC::Buffer(nSize) };

			ComPtr<ID3D12Resource> DefaultBuffer{ nullptr };

			CheckFailed(Device->CreateCommittedResource(
				&DefaultHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&BufferDesc,
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				IID_PPV_ARGS(DefaultBuffer.GetAddressOf())
			));

			CheckFailed(Device->CreateCommittedResource(
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

			ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(DefaultBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
			pCommandList->GetCommandList()->ResourceBarrier(1, &ResourceBarrier);
			
			
			Device = nullptr;

			return DefaultBuffer;
		}
		ResourceFactory::~ResourceFactory()
		{
		}
	}
}

