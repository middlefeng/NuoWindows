// NuoWindow.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "NuoApp.h"
#include "NuoAppInstance.h";
#include "NuoWindow.h"
#include "NuoMenu.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR lpCmdLine,
                     _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    NuoAppInstance::Init(hInstance, nCmdShow);
    NuoWindow::RegisterClass();

    PNuoWindow window = std::make_shared<NuoWindow>("  Nuo Window");

    PNuoMenuBar menu = std::make_shared<NuoMenuBar>();
    PNuoMenu fileMenu = std::make_shared<NuoMenu>("  &File ");
    PNuoMenuItem exitItem = std::make_shared<NuoMenuItem>(IDM_EXIT, "E&xit");
    fileMenu->AppenMenuItem(exitItem);
    fileMenu->Update();
    menu->AppendMenu(fileMenu);
    menu->Update();
    window->SetMenu(menu);

    auto exitFunc = []()
    {
        NuoAppInstance::Exit();
    };

    std::weak_ptr<NuoWindow> weakWindow = window;

    window->SetOnDestroy(exitFunc);
    exitItem->SetAction([weakWindow](PNuoMenuItem)
        {
            PNuoWindow aWindow = weakWindow.lock();

            if (aWindow)
                aWindow->Destroy();
        });

    window->SetIcon(IDI_NUOWINDOW);
    window->Show();
    window->Update();

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        // if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


