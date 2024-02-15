#pragma once
namespace EngineFramework {
	class Shader : public IShader{
	public:
		Shader();
		~Shader();
	private:
		std::tstring m_tsPath{};
		ComPtr<ID3D10Blob> m_d3dVertexShaderBlob{ nullptr };
		ComPtr<ID3D10Blob> m_d3dPixelShaderBlob{ nullptr };
		ComPtr<ID3D10Blob> m_d3dHullShaderBlob{ nullptr };
		ComPtr<ID3D10Blob> m_d3dDomainShaderBlob{ nullptr };
		ComPtr<ID3D10Blob> m_d3dGeometryShaderBlob{ nullptr };
	public:
		void Initialize(const std::tstring& ctsShaderPath);
		void CompileShader(ShaderType ShaderType,const D3D_SHADER_MACRO* d3dShaderDefines, const std::string& csEntryPoint, const std::string& csShaderTarget);
	public:
		D3D12_SHADER_BYTECODE GetShaderByteCode(ShaderType ShaderType) const override;
	private:
		D3D12_SHADER_BYTECODE GetByteCode(ComPtr<ID3D10Blob> d3dBlob) const;
	};
}