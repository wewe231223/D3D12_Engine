#include "EnginePch.h"
#include "RootSignature.h"

namespace EngineFramework {
	RootSignature::RootSignature(){

	}
	RootSignature::~RootSignature(){

	}
	void RootSignature::Initialize(const IDevice* pDevice){
		D3D12_DESCRIPTOR_HEAP_DESC ResourceDescriptorHeapDesc{};
		ResourceDescriptorHeapDesc.NumDescriptors = 100;
		ResourceDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		ResourceDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		ResourceDescriptorHeapDesc.NodeMask = 0;
		CheckFailed(pDevice->GetDevice()->CreateDescriptorHeap(&ResourceDescriptorHeapDesc, IID_PPV_ARGS(m_d3dResourceDescriptorHeap.GetAddressOf())));
	}
	void RootSignature::Create(const IDevice* pDevice){
		CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc{ m_nCBuffer,m_d3dRootParameter.data(),0,nullptr,D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT };

		ComPtr<ID3D10Blob> ErrorBlob{ nullptr };
		CheckFailed(D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, m_d3dSerializedRootSignature.GetAddressOf(), ErrorBlob.GetAddressOf()));
		CheckFailed(pDevice->GetDevice()->CreateRootSignature(0, m_d3dSerializedRootSignature->GetBufferPointer(), m_d3dSerializedRootSignature->GetBufferSize(), IID_PPV_ARGS(m_d3dRootSignature.GetAddressOf())));
	}
	void RootSignature::SetRootSignature(const ICommandList* pCommandList){
		pCommandList->GetCommandList()->SetGraphicsRootSignature(m_d3dRootSignature.Get());
		
	}
	void RootSignature::SetResourceDescriptorHeap(const ICommandList* pCommandList){
		ID3D12DescriptorHeap* DescriptorHeap[] = { m_d3dResourceDescriptorHeap.Get() };
		pCommandList->GetCommandList()->SetDescriptorHeaps(_countof(DescriptorHeap), DescriptorHeap);
	}
	ComPtr<ID3D12RootSignature> RootSignature::GetRootSignature() const {
		return m_d3dRootSignature;
	}
	UINT RootSignature::GetBufferIndex() const {
		return ++m_nCBuffer;
	}
}