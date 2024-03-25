// Client.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "ClientPch.h"
#include "System/Application.h"
#include "EngineCore/Engine.h"

#define PROGRAM_SUCCESS_END 0xff

std::unique_ptr<App::Application> m_app{};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow) {
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    std::atexit([]() {
        OutputDebugString(_T("\nMemory Dumped\n"));
        _CrtDumpMemoryLeaks();
        });
#endif // !defined(DEBUG) | defined(_DEBUG)
   // _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, "Client", NULL);
    
    m_app = std::make_unique<App::Application>(hInstance,_T("Client"));
    App::SetMainApplication(m_app.get());
    std::shared_ptr<EngineFramework::DirectXEngine> DxEngine = std::make_shared<EngineFramework::DirectXEngine>(m_app->GetWindowInfo(),false);
    
    m_app->Init(DxEngine);
    m_app->Loop();

    m_app = nullptr;
    DxEngine = nullptr;

   
    return PROGRAM_SUCCESS_END;
}



