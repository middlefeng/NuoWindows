// NuoDirect.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "ModelViewerMain.h"

#include "NuoImage.h"
#include "NuoAppInstance.h"

#include "ModelViewerWindow.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    NuoAppInstance::Init(hInstance, nCmdShow);
    NuoWindow::RegisterClass();

    PNuoDirectWindow window = std::make_shared<ModelViewerWindow>("  Model Viewer");
    window->Init();

    PNuoIcon icon = std::make_shared<NuoIcon>(IDI_NUODIRECT);
    window->SetIcon(icon);

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



