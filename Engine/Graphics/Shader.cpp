#include "EnginePch.h"
#include "Shader.h"

namespace EngineFramework {
	Shader::Shader(const std::tstring& TargetShaderPath, D3D12_INPUT_LAYOUT_DESC ShaderInputLayout) : m_path(TargetShaderPath){
		m_d3dPipelineDesc.InputLayout = ShaderInputLayout;

		CD3DX12_RASTERIZER_DESC RasterizerDesc{ D3D12_DEFAULT };
		CD3DX12_BLEND_DESC BlendDesc{ D3D12_DEFAULT };
		CD3DX12_DEPTH_STENCIL_DESC DepthStencilDesc{ D3D12_DEFAULT };
		
			
		m_d3dPipelineDesc.RasterizerState = RasterizerDesc;
		m_d3dPipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		m_d3dPipelineDesc.BlendState = BlendDesc;
		m_d3dPipelineDesc.DepthStencilState = DepthStencilDesc;
		m_d3dPipelineDesc.SampleMask = UINT_MAX;
		m_d3dPipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		m_d3dPipelineDesc.NumRenderTargets = 1;
		m_d3dPipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		// 일단 임시로 꺼두기로 한다 
		m_d3dPipelineDesc.SampleDesc.Count = 1;
		m_d3dPipelineDesc.SampleDesc.Quality = 0;
		m_d3dPipelineDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	}

	Shader::~Shader() {

	}


	void Shader::BuildShader(const IDevice* pDevice,ID3D12RootSignature* pRootSignature){
		m_d3dPipelineDesc.pRootSignature = pRootSignature;
		pDevice->GetDevice()->CreateGraphicsPipelineState(&m_d3dPipelineDesc, IID_PPV_ARGS(m_d3dPipeLine.GetAddressOf()));
	}

	void Shader::CreateShader(ShaderType ShaderType_, const D3D_SHADER_MACRO* d3dShaderDefines, const std::string& ShaderEntry, const std::string& ShaderTarget){
		UINT CompileFlags{ 0 };
#if defined(DEBUG) || defined(_DEBUG)
		CompileFlags = D3DCOMPILE_DEBUG bitor D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // !defined(DEBUG) || defined(_DEBUG)


		ComPtr<ID3D10Blob> ErrorBlob{ nullptr };

		HRESULT hr = S_OK;
			
		hr = ::D3DCompileFromFile(m_path.c_str(), d3dShaderDefines, D3D_COMPILE_STANDARD_FILE_INCLUDE, ShaderEntry.c_str(), ShaderTarget.c_str(), CompileFlags, 0, m_shaderBytecodes[static_cast<UINT>(ShaderType_)].GetAddressOf(), ErrorBlob.GetAddressOf());
		if (ErrorBlob != nullptr) ::OutputDebugStringA((char*)ErrorBlob->GetBufferPointer());
		if (FAILED(hr)) throw System::Exeption(hr, _T("D3DCompileFromFile"), _T("Shader.cpp"), 0);
			
		switch (ShaderType_){
		case VertexShader:
			m_d3dPipelineDesc.VS = D3D12_SHADER_BYTECODE{ m_shaderBytecodes[static_cast<UINT>(ShaderType_)]->GetBufferPointer(),m_shaderBytecodes[static_cast<UINT>(ShaderType_)]->GetBufferSize() };
			break;
		case HullShader:
			m_d3dPipelineDesc.HS = D3D12_SHADER_BYTECODE{ m_shaderBytecodes[static_cast<UINT>(ShaderType_)]->GetBufferPointer(),m_shaderBytecodes[static_cast<UINT>(ShaderType_)]->GetBufferSize() };
			break;
		case DomainShader:
			m_d3dPipelineDesc.DS = D3D12_SHADER_BYTECODE{ m_shaderBytecodes[static_cast<UINT>(ShaderType_)]->GetBufferPointer(),m_shaderBytecodes[static_cast<UINT>(ShaderType_)]->GetBufferSize() };
			break;
		case GeometryShader:
			m_d3dPipelineDesc.GS = D3D12_SHADER_BYTECODE{ m_shaderBytecodes[static_cast<UINT>(ShaderType_)]->GetBufferPointer(),m_shaderBytecodes[static_cast<UINT>(ShaderType_)]->GetBufferSize() };
			break;
		case PixelShader:
			m_d3dPipelineDesc.PS = D3D12_SHADER_BYTECODE{ m_shaderBytecodes[static_cast<UINT>(ShaderType_)]->GetBufferPointer(),m_shaderBytecodes[static_cast<UINT>(ShaderType_)]->GetBufferSize() };
			break;
		default:
			break;
		}
	}

	void Shader::BindPipeLine(const ICommandList* pCommandList){
		pCommandList->GetCommandList()->SetPipelineState(m_d3dPipeLine.Get());
	}
}