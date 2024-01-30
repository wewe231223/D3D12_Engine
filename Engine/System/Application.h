#pragma once
namespace App {
	class Application {
		friend LRESULT CALLBACK MainProcedure(HWND,UINT,WPARAM,LPARAM);
		friend void SetMainApplication(Application*);
	public:
		Application() = default;
		Application(
			HINSTANCE hInstance,
			LPCTSTR lpctsWindowName,
			int nWidth = DEFAULT_WIDTH,
			int nHeight = DEFAULT_HEIGHT,
			int nX =  0,int nY = 0);
		~Application();
	private:
		HINSTANCE m_hInstance{};
		WNDCLASSEXW m_wcex{};
		WindowInfo m_windowInfo{};
		POINT m_windowPosition{};
		std::tstring m_tsWindowName{};
	private:
		static Application* pMainApplication;
	public:
		 LRESULT Prodedure(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam);
	public:
		void Init();
		void Loop();

	};
	LRESULT CALLBACK MainProcedure(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam);
	void SetMainApplication(Application* pApp);
}

