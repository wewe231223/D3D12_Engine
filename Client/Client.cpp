// Client.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "Application.h"

std::unique_ptr<App::Application> m_app{};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    std::tstring WindowName = _T("Client");
    m_app = std::make_unique<App::Application>(hInstance, WindowName);
    App::SetMainApplication(m_app.get());
    m_app->Init();
    m_app->Loop();


    
    return 0xff;
}



