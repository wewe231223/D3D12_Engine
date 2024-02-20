#pragma once
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <memory.h>
#include <malloc.h>
#include <tchar.h>

// C++ 런타임 헤더 파일입니다.
#include <string>
#include <memory>
#include <vector>
#include <array>

// DirectX 런타임 헤더 파일입니다.
#include <wrl.h>
#include <d3d12.h>
#include "External/Include/d3dx12.h"
#include "External/Include/DirectXTex.h"
#include "External/Include/DirectXTex.inl"
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "comdef.h"

using Microsoft::WRL::ComPtr;

// DirectX 라이브러리 링크 
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "External/Lib/DirectXTex_debug.lib")
#else 
#pragma comment(lib, "External/Lib/DirectXTex.lib")
#endif // !defined(DEBUG) || defined(_DEBUG)

namespace std {
#if defined(UNICODE)
	using tstring = wstring;
#else 
	using tstring = string;
#endif // !defined(UNICODE)
}

namespace App {
	constexpr int DEFAULT_WIDTH = 860;
	constexpr int DEFAULT_HEIGHT = 600;
	struct WindowInfo {
		HWND hWnd{ nullptr };
		int Width{ DEFAULT_WIDTH };
		int Height{ DEFAULT_HEIGHT };
	};
}

namespace EngineFramework {
	enum {
		SWAP_CHAIN_BUFFER_COUNT = 2
	};

	struct MsaaState {
		bool Msaa4xBool{ false };
		UINT Msaa4XQualityLevel{ 0 };
	};

	enum ShaderType {
		VertexShader,
		HullShader,
		DomainShader,
		GeometryShader,
		PixelShader,
	};
}

struct Vertex {
	DirectX::XMFLOAT3 Position{};
	DirectX::XMFLOAT3 Normal{};
	DirectX::XMFLOAT2 TexCoord{};
	DirectX::XMFLOAT4 Color{};
};

#define ReleaseCom(x)	\
	if(x){				\
		x->Release();	\
		x = 0;			\
	}


#if defined(_DEBUG) || defined(DEBUG)
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)
#define malloc(s) _malloc_dbg(s,_NORMAL_BLOCK,__FILE__,LINE__)
#endif // !defined(_DEBUG) || defined(DEBUG)

#include "System/Exeption.h"
#include "EngineCore/EngineInterface.h"

