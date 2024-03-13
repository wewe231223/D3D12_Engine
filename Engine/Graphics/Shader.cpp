#include "EnginePch.h"
#include "Shader.h"

namespace EngineFramework {

	Shader::Shader(){

	}
	Shader::~Shader(){

	}

	void Shader::Initialize(const std::tstring& ctsShaderPath){
		m_tsPath = ctsShaderPath;
		//GetRegister(m_tsPath);
		std::ifstream in{ m_tsPath,std::ios::app };

		











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

	void Shader::GetRegister(std::tstring path){

		std::ifstream file{ path }; // "example.txt" 파일을 열습니다.
		if (!file) {
			OutputDebugString(_T("\nCan't Open the file\n"));
			return;
		}

		std::string wordToFind = "register( s"; // 찾을 문구
		std::string line;
		int count = 0;

		while (std::getline(file, line)) { // 파일의 각 줄을 읽습니다.
			std::string::size_type pos = 0;
			while ((pos = line.find(wordToFind, pos)) != std::string::npos) {
				++count;
				pos += wordToFind.length();
			}
		}

		OutputDebugString(_T("\nFind : "));
		OutputDebugString(std::to_wstring(count).c_str());
		OutputDebugString(_T("\n"));

	}



	namespace TestBed {
	
		RegisterLibrary::RegisterLibrary(const std::string& ShaderFolderPath){
			for (const auto& entry : fs::directory_iterator(ShaderFolderPath)) {
				std::string RegisterWord{};
				if (entry.is_regular_file() and entry.path().extension() == ".hlsl") {
					FindRegisterInFile(entry.path().string());
				}
			}

			for (auto iter = m_Lib.begin(); iter != m_Lib.end(); iter++) {
				std::cout << iter->first << " " << iter->second.RegType << " " << iter->second.RegNum << std::endl;
			}


			m_Lib["test"];
		}

		RegisterLibrary::~RegisterLibrary(){}

		void RegisterLibrary::FindRegisterInFile(const std::string& File){
			std::ifstream in{ File, std::ios::app };
			std::regex RegNoOneDigit("\\s*(\\w+)\\s*:\\s*register\\((.)(.)\\);");
			std::regex RegNoTwoDigit("\\s*(\\w+)\\s*:\\s*register\\((.)(..)\\);");
			std::string Line{};


			while (std::getline(in, Line)) {

				std::smatch match;

				if (std::regex_search(Line, match, RegNoOneDigit) and match.size() > 3) {
					std::string RegisterName = match.str(1);
					CHAR RegisterType = match.str(2).at(0);
					UINT RegisterNum = std::stoi(match.str(3));

					m_Lib.insert(std::pair<std::string_view, RegisterProfile>(RegisterName, RegisterProfile{ RegisterType,RegisterNum }));
				}
				else if (std::regex_search(Line, match, RegNoTwoDigit) and match.size() > 3) {
					std::string RegisterName = match.str(1);
					CHAR RegisterType = match.str(2).at(0);
					UINT RegisterNum = std::stoi(match.str(3));

					m_Lib.insert(std::pair<std::string_view, RegisterProfile>(RegisterName, RegisterProfile{ RegisterType,RegisterNum }));
				}
				else {

				}

			}
		}

		RegisterProfile RegisterLibrary::GetRegister(const std::string& RegName){
			if (!m_Lib.contains(RegName)) {
				throw System::Exeption{ _T("RegisterProfile  :  Invalid RegName!") };
			}
			return m_Lib[RegName];
		}






	}


	


}