// Client.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "ClientPch.h"
#include "System/Application.h"
#include "EngineCore/Engine.h"

std::unique_ptr<App::Application> m_app{};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow) {
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // !defined(DEBUG) | defined(_DEBUG)
    

    m_app = std::make_unique<App::Application>(hInstance,_T("Client"));
    App::SetMainApplication(m_app.get());
    std::shared_ptr<EngineFramework::DirectXEngine> DxEngine = std::make_shared<EngineFramework::DirectXEngine>(m_app->GetWindowInfo(),false);
    m_app->Init(DxEngine);
    m_app->Loop();

    m_app = nullptr;
    DxEngine = nullptr;

    _CrtDumpMemoryLeaks();
    return 0xff;
}



