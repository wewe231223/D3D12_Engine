#pragma once
namespace EngineFramework {
	class Shader : public IShader{
	public:
		Shader();
		~Shader();
	private:
		ComPtr<ID3D10Blob> m_d3dVertexShaderBlob{ nullptr };
		ComPtr<ID3D10Blob> m_d3dPixelShaderBlob{ nullptr };

	public:
		void Initialize();
		void CompileShader(ShaderType ShaderType,const std::tstring& ctsPath, const D3D_SHADER_MACRO* d3dShaderDefines, const std::string& csEntryPoint, const std::string& csShaderTarget);
		D3D12_SHADER_BYTECODE GetShaderByteCode(ShaderType ShaderType) const override;
	};
}