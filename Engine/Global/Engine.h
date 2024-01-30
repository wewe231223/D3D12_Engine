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

// DirectX 런타임 헤더 파일입니다.
#include <wrl.h>
#include <d3d12.h>
#include "External/d3dx12.h"
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "comdef.h"

// DirectX 라이브러리 링크 
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

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
		int Width{ 0 };
		int Height{ 0 };
	};
}

#include "System/Exeption.h"