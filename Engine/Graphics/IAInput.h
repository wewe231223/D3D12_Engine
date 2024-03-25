#pragma once 

class IAInputLayout {
public:
	template<typename T>
	static D3D12_INPUT_LAYOUT_DESC GetInputLayout();
	
	~IAInputLayout() { OutputDebugString(_T("\nIAInput Deleted\n")); };
};

template<>
inline D3D12_INPUT_LAYOUT_DESC IAInputLayout::GetInputLayout<Vertex>() {
	static std::vector<D3D12_INPUT_ELEMENT_DESC> VertexInputLayout{};

	VertexInputLayout.emplace_back("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0);
	VertexInputLayout.emplace_back("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0);
	VertexInputLayout.emplace_back("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0);
	VertexInputLayout.emplace_back("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0);

	std::atexit([]() {
		VertexInputLayout.clear();
		VertexInputLayout.shrink_to_fit();
		});

	return D3D12_INPUT_LAYOUT_DESC{ VertexInputLayout.data(),static_cast<UINT>(VertexInputLayout.size())};
}

template<typename T>
inline D3D12_INPUT_LAYOUT_DESC IAInputLayout::GetInputLayout() {
	_CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, "InputLayout", NULL);
	throw System::Exeption{ _T("[InputLayout] : Any Layout Supported!") };
	return D3D12_INPUT_LAYOUT_DESC{};
}
