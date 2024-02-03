#include "EnginePch.h"
#include "Application.h"
#include "EngineCore/Engine.h"
#include "Input.h"
#include "Timer.h"
App::Application* App::Application::pMainApplication = nullptr;


App::Application::Application(HINSTANCE hInstance,LPCTSTR lpctsWindowName,int nWidth, int nHeight, int nX, int nY) {
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
	m_tsWindowName = lpctsWindowName;
	m_windowPosition.x = nX;
	m_windowPosition.y = nY;
	m_windowInfo.Width = nWidth;
	m_windowInfo.Height = nHeight;
}

App::Application::~Application(){

}

void App::Application::Init(std::shared_ptr<EngineFramework::Engine> pEngine) {
	try{
		RegisterClassExW(&m_wcex);

		m_windowInfo.hWnd = CreateWindowW(_T("APPLICATION"), m_tsWindowName.c_str(), WS_OVERLAPPEDWINDOW,
			m_windowPosition.x, m_windowPosition.y, m_windowInfo.Width, m_windowInfo.Height, nullptr, nullptr, m_hInstance, nullptr);

		if (!m_windowInfo.hWnd) exit(EXIT_FAILURE);

		::ShowWindow(m_windowInfo.hWnd, SW_SHOW);
		::UpdateWindow(m_windowInfo.hWnd);

		INPUT->Init(m_windowInfo.hWnd, m_hInstance);
		m_timer = std::make_unique<System::Timer>(m_windowInfo.hWnd);
		m_timer->Reset();

		m_engine = pEngine;

	} catch (const System::Exeption& e){
		::MessageBox(m_windowInfo.hWnd, e.ToString().c_str(), 0, 0);
	}

}

LRESULT App::Application::Prodedure(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam){
	try {
		switch (nMessage) {
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
	} catch (const System::Exeption& e) {
		::MessageBox(m_windowInfo.hWnd, e.ToString().c_str(), 0, 0);
	}

	return 0;
}

void App::Application::Loop(){
	HACCEL hAccelTable = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDC_CLIENT));
	MSG msg{};
	m_timer->Start();

	try {
		while (true) {
			if (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT) break;
				if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			INPUT->Update();
			m_timer->Update();
		}
	}
	catch (const System::Exeption& e) {
		::MessageBox(m_windowInfo.hWnd, e.ToString().c_str(), 0, 0);
	}

	return INPUT->Terminate();
}

LRESULT App::MainProcedure(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam){
	if (App::Application::pMainApplication) return App::Application::pMainApplication->Prodedure(hWnd, nMessage, wParam, lParam);
	return DefWindowProc(hWnd, nMessage, wParam, lParam);
}

void App::SetMainApplication(Application* pApp){
	if (App::Application::pMainApplication) return;
	App::Application::pMainApplication = pApp;
}
