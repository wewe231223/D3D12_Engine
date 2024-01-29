#include "EnginePch.h"
#include "Application.h"
App::Application* App::Application::pMainApplication = nullptr;


App::Application::Application(HINSTANCE hInstance,std::tstring& tsWindowName,int nWidth, int nHeight, int nX, int nY) {
	::ZeroMemory(&m_wcex, sizeof(WNDCLASSEXW));

	m_wcex.cbSize = sizeof(WNDCLASSEXW);
	m_wcex.style = CS_HREDRAW | CS_VREDRAW;
	m_wcex.lpfnWndProc = App::MainProcedure;
	m_wcex.cbClsExtra = 0;
	m_wcex.cbWndExtra = 0;
	m_wcex.hInstance = hInstance;
	m_wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_CLIENT));
	m_wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	m_wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	m_wcex.lpszClassName = _T("APPLICATION");
	m_wcex.lpszMenuName = _T("MENU");
	m_wcex.hIconSm = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_SMALL));


	m_hInstance = hInstance;
	m_tsWindowName = tsWindowName;
	m_windowPosition.x = nX;
	m_windowPosition.y = nY;
	m_windowInfo.Width = nWidth;
	m_windowInfo.Height = nHeight;
}

void App::Application::Init(){
	RegisterClassExW(&m_wcex);


	HWND WindowHandle = CreateWindowW(_T("APPLICATION"), m_tsWindowName.c_str() , WS_OVERLAPPEDWINDOW,
		m_windowPosition.x, m_windowPosition.y,m_windowInfo.Width, m_windowInfo.Height, nullptr, nullptr, m_hInstance, nullptr);

	if (!WindowHandle) exit(EXIT_FAILURE);

	::ShowWindow(WindowHandle, SW_SHOW);
	::UpdateWindow(WindowHandle);

	m_windowInfo.hWnd = &WindowHandle;
}

LRESULT App::Application::Prodedure(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam){
	switch (nMessage){
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId) {
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, nMessage, wParam, lParam);
		}
	}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, nMessage, wParam, lParam);
	}
	return 0;
}

void App::Application::Loop(){
	HACCEL hAccelTable = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDC_CLIENT));
	
	MSG msg;
	OutputDebugString(_T("Application Loop!"));
	while (true) {
		if (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) break;
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
}

LRESULT App::MainProcedure(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam){
	if (App::Application::pMainApplication) return App::Application::pMainApplication->Prodedure(hWnd, nMessage, wParam, lParam);
	return DefWindowProc(hWnd, nMessage, wParam, lParam);
}

void App::SetMainApplication(Application* pApp){
	if (App::Application::pMainApplication) return;
	App::Application::pMainApplication = pApp;
}

