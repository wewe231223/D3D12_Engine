#pragma once
#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����
#include <windows.h>
// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <memory.h>
#include <malloc.h>
#include <tchar.h>

// C++ ��Ÿ�� ��� �����Դϴ�.
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