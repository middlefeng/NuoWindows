// NuoDirect.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "NuoDirect.h"

#include "NuoAppInstance.h"
#include "NuoDirectWindow.h"

#include "D3DHello/D3D12HelloFrameBuffering.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    D3D12HelloFrameBuffering sample(1280, 720, L"D3D12 Hello Frame Buffering");

    NuoAppInstance::Init(hInstance, nCmdShow);
    NuoWindow::RegisterClass();

    PNuoDirectWindow window = std::make_shared<NuoDirectWindow>("  Nuo Direct");

    sample._hWnd = window->Handle();
    sample.OnInit();

    window->SetOnPaint([&sample]()
        {
            sample.OnUpdate();
            sample.OnRender();
        });

    auto exitFunc = []()
    {
        NuoAppInstance::Exit();
    };
    window->SetOnDestroy(exitFunc);
    window->Show();
    window->Update();

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NUODIRECT));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }


    NuoAppInstance::UnInit();

    return (int) msg.wParam;
}



