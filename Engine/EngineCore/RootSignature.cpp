#include "EnginePch.h"
#include "RootSignature.h"

namespace EngineFramework {
	RootSignature::RootSignature(){

	}
	RootSignature::~RootSignature(){

	}
	void RootSignature::Initialize(){

	}

	void RootSignature::NewParameter(CD3DX12_ROOT_PARAMETER d3dRootParam) {
		m_d3dRootParameter.emplace_back(d3dRootParam);
	}

	void RootSignature::NewSampler(UINT nShaderRegister){
		m_d3dSampler.emplace_back(CD3DX12_STATIC_SAMPLER_DESC(nShaderRegister));
	}

	void RootSignature::NewSampler(CD3DX12_STATIC_SAMPLER_DESC d3dSamplerDesc){
		m_d3dSampler.emplace_back(d3dSamplerDesc);
	}

	void RootSignature::Create(const IDevice* pDevice){
		CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc{ static_cast<UINT>(m_d3dRootParameter.size()),m_d3dRootParameter.data(),static_cast<UINT>(m_d3dSampler.size()),m_d3dSampler.data(),D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT};

		ComPtr<ID3D10Blob> ErrorBlob{ nullptr };
		CheckFailed(D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, m_d3dSerializedRootSignature.GetAddressOf(), ErrorBlob.GetAddressOf()));
		CheckFailed(pDevice->GetDevice()->CreateRootSignature(0, m_d3dSerializedRootSignature->GetBufferPointer(), m_d3dSerializedRootSignature->GetBufferSize(), IID_PPV_ARGS(m_d3dRootSignature.GetAddressOf())));
	}
	void RootSignature::SetRootSignature(const ICommandList* pCommandList){
		pCommandList->GetCommandList()->SetGraphicsRootSignature(m_d3dRootSignature.Get());
		
	}

	ComPtr<ID3D12RootSignature> RootSignature::GetRootSignature() const {
		return m_d3dRootSignature;
	}
	UINT RootSignature::GetBufferIndex() const {
		return ++m_nCBuffer;
	}
}