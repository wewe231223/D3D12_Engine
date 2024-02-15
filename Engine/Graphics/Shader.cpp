#include "EnginePch.h"
#include "Shader.h"

namespace EngineFramework {
	Shader::Shader(){

	}
	Shader::~Shader(){

	}
	void Shader::Initialize(const std::tstring& ctsShaderPath){
		m_tsPath = ctsShaderPath;
	}

	void Shader::CompileShader(ShaderType ShaderType, const D3D_SHADER_MACRO* d3dShaderDefines, const std::string& csEntryPoint, const std::string& csShaderTarget){
		UINT CompileFlags{ 0 };
#if defined(DEBUG) || defined(_DEBUG)
		CompileFlags = D3DCOMPILE_DEBUG bitor D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // !defined(DEBUG) || defined(_DEBUG)
		
		ComPtr<ID3D10Blob> ByteCode{ nullptr };
		ComPtr<ID3D10Blob> ErrorBlob{ nullptr };

		HRESULT hr = S_OK;

		hr = ::D3DCompileFromFile(m_tsPath.c_str(), d3dShaderDefines, D3D_COMPILE_STANDARD_FILE_INCLUDE, csEntryPoint.c_str(), csShaderTarget.c_str(), CompileFlags, 0, ByteCode.GetAddressOf(), ErrorBlob.GetAddressOf());
		if (ErrorBlob != nullptr) ::OutputDebugStringA((char*)ErrorBlob->GetBufferPointer());
		if (FAILED(hr)) throw System::Exeption(hr, _T("D3DCompileFromFile"), _T("Shader.cpp"), 0);

		switch (ShaderType){
		case EngineFramework::VertexShader:
			m_d3dVertexShaderBlob = ByteCode;
			break;
		case EngineFramework::PixelShader:
			m_d3dPixelShaderBlob = ByteCode;
			break;
		default:
			break;
		}
	}

	D3D12_SHADER_BYTECODE Shader::GetShaderByteCode(ShaderType ShaderType) const{
		switch (ShaderType) {
		case VertexShader:
			return GetByteCode(m_d3dVertexShaderBlob);
		case PixelShader:
			return GetByteCode(m_d3dPixelShaderBlob);
		case HullShader:
			return GetByteCode(m_d3dHullShaderBlob);
		case DomainShader:
			return GetByteCode(m_d3dDomainShaderBlob);
		case GeometryShader:
			return GetByteCode(m_d3dGeometryShaderBlob);
		default:
			return D3D12_SHADER_BYTECODE{ nullptr,0 };
		}
	}

	D3D12_SHADER_BYTECODE Shader::GetByteCode(ComPtr<ID3D10Blob> d3dBlob) const{
		if (d3dBlob != nullptr) {
			return D3D12_SHADER_BYTECODE{ reinterpret_cast<BYTE*>(d3dBlob->GetBufferPointer()), d3dBlob->GetBufferSize() };
		}
		else {
			return D3D12_SHADER_BYTECODE{};
		}
		
	}



}