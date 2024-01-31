// Client.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "ClientPch.h"
#include "System/Application.h"

std::unique_ptr<App::DirectXApplication> m_app{};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow) {
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // !defined(DEBUG) | defined(_DEBUG)
    
 
    m_app = std::make_unique<App::DirectXApplication>(hInstance,_T("Client"));
    App::SetMainApplication(m_app.get());

    m_app->Init();
    m_app->Loop();

    m_app.reset();

    _CrtDumpMemoryLeaks();
    return 0xff;
}



