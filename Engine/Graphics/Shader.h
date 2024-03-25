#pragma once
namespace EngineFramework {



	class Shader {
	public:
		Shader(const std::tstring& TargetShaderPath, D3D12_INPUT_LAYOUT_DESC ShaderInputLayout);
		~Shader();
	private:
		std::tstring m_path{};
		std::array<ComPtr<ID3D10Blob>, 5> m_shaderBytecodes{ nullptr };
	private:
		ComPtr<ID3D12PipelineState> m_d3dPipeLine{ nullptr };
		D3D12_GRAPHICS_PIPELINE_STATE_DESC m_d3dPipelineDesc{};
	public:
		void BuildShader(const IDevice* pDevice,ID3D12RootSignature* pRootSignature);
		void CreateShader(ShaderType ShaderType_, const D3D_SHADER_MACRO* d3dShaderDefines, const std::string& ShaderEntry, const std::string& ShaderTarget);
		void BindPipeLine(const ICommandList* pCommandList);
	};






	


}