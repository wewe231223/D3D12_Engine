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
		HWND* hWnd{ nullptr };
		int Width{ 0 };
		int Height{ 0 };
	};
}