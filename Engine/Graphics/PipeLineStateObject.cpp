#include "EnginePch.h"
#include "PipeLineStateObject.h"

namespace EngineFramework{
	PipelineStateObject::PipelineStateObject()
	{
	}
	PipelineStateObject::~PipelineStateObject()
	{
	}
	void PipelineStateObject::Initialize(const IShader* pShader){
		::ZeroMemory(&m_d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
		m_d3dInputLayout = {
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,24,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,32,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0}
		};

		CD3DX12_RASTERIZER_DESC RasterizerDesc{ D3D12_DEFAULT };
		CD3DX12_BLEND_DESC BlendDesc{ D3D12_DEFAULT };
		CD3DX12_DEPTH_STENCIL_DESC DepthStencilDesc{ D3D12_DEFAULT };

		m_d3dPipelineStateDesc.InputLayout = { m_d3dInputLayout.data(),static_cast<UINT>(m_d3dInputLayout.size()) };
		m_d3dPipelineStateDesc.VS = pShader->GetShaderByteCode(VertexShader);
		m_d3dPipelineStateDesc.PS = pShader->GetShaderByteCode(PixelShader);
		m_d3dPipelineStateDesc.RasterizerState = RasterizerDesc;
		m_d3dPipelineStateDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		m_d3dPipelineStateDesc.BlendState = BlendDesc;
		m_d3dPipelineStateDesc.DepthStencilState = DepthStencilDesc;
		m_d3dPipelineStateDesc.SampleMask = UINT_MAX;
		m_d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		m_d3dPipelineStateDesc.NumRenderTargets = 1;
		m_d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		// 일단 임시로 꺼두기로 한다 
		m_d3dPipelineStateDesc.SampleDesc.Count = 1;
		m_d3dPipelineStateDesc.SampleDesc.Quality = 0;
		m_d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	}
	void PipelineStateObject::Create(const IDevice* pDevice){
		pDevice->GetDevice()->CreateGraphicsPipelineState(&m_d3dPipelineStateDesc, IID_PPV_ARGS(m_d3dPipelineState.GetAddressOf()));
	}
	void PipelineStateObject::SetPipelineState(const ICommandQueue* pCommandQueue){
		pCommandQueue->GetCommandList()->SetPipelineState(m_d3dPipelineState.Get());
	}
}

