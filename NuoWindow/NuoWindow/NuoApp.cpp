// NuoWindow.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "NuoApp.h"
#include "NuoAppInstance.h"
#include "NuoWindow.h"
#include "NuoButton.h"
#include "NuoMenu.h"
#include "NuoMonitorScale.h"
#include "NuoImage.h"
#include "NuoStrings.h"

#include "IconWindow.h"
#include "ListViewWindow.h"
#include "AppAboutDialog.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR lpCmdLine,
                     _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    NuoAppInstance::Init(hInstance, nCmdShow);
    NuoWindow::RegisterClass();

    std::string appPath = NuoAppInstance::GetInstance()->ModulePath();
    appPath = RemoveLastPathComponent(appPath);
    std::string iconPath = appPath + "\\Nuclear.png";

    NuoImage image;
    image.Load(iconPath, -1);
    PNuoIcon icon = image.Icon();

    std::string icoFilePath = appPath + "\\Nuclear.ico";
    icon->Save(icoFilePath);

    PNuoWindow window = std::make_shared<NuoWindow>("  Nuo Window");

    PNuoMenuBar menu = std::make_shared<NuoMenuBar>();
    
    PNuoMenu fileMenu = std::make_shared<NuoMenu>("  &File ");
    PNuoMenuItem exitItem = std::make_shared<NuoMenuItem>(IDM_EXIT, "E&xit");
    PNuoMenuItem iconTools = std::make_shared<NuoMenuItem>(IDM_ICONTOOLS, "Icon Tools ...");
    PNuoMenuItem listView = std::make_shared<NuoMenuItem>(IDM_LISTVIEW, "List View ...");
    fileMenu->AppenMenuItem(iconTools);
    fileMenu->AppenMenuItem(listView);
    fileMenu->AppenMenuItem(exitItem);
    fileMenu->Update();

    PNuoMenu aboutMenu = std::make_shared<NuoMenu>(" &About ");
    PNuoMenuItem aboutItem = std::make_shared<NuoMenuItem>(IDM_ABOUT, "About");
    aboutMenu->AppenMenuItem(aboutItem);
    aboutMenu->Update();

    menu->AppendMenu(fileMenu);
    menu->AppendMenu(aboutMenu);
    menu->Update();
    window->SetMenu(menu);

    PIconWindow iconWindow;
    PListViewWindow listViewWindow;

    auto exitFunc = [&iconWindow]()
    {
        if (iconWindow)
            iconWindow->Destroy();

        NuoAppInstance::Exit();
    };

    std::weak_ptr<NuoWindow> weakWindow = window;

    iconTools->SetAction([&iconWindow, weakWindow](PNuoMenuItem)
        {
            if (!iconWindow)
            {
                iconWindow = std::make_shared<IconWindow>(weakWindow.lock());
                iconWindow->SetPositionDevice(iconWindow->PreferredRect(), true);
                iconWindow->Init();
            }

            iconWindow->Show();
            iconWindow->Update();
            iconWindow->SetOnDestroy([&iconWindow]()
                {
                    iconWindow->Detach();
                    iconWindow.reset();
                });
        });

    listView->SetAction([&listViewWindow, weakWindow](PNuoMenuItem)
        {
            if (!listViewWindow)
            {
                listViewWindow = std::make_shared<ListViewWindow>(weakWindow.lock());
                listViewWindow->SetPositionDevice(listViewWindow->PreferredRect(), true);
                listViewWindow->Init();
            }

            listViewWindow->Show();
            listViewWindow->Update();
            listViewWindow->SetOnDestroy([&listViewWindow]()
                {
                    listViewWindow->Detach();
                    listViewWindow.reset();
                });
        });

    window->SetOnDestroy(exitFunc);
    exitItem->SetAction([weakWindow](PNuoMenuItem)
        {
            PNuoWindow aWindow = weakWindow.lock();

            if (aWindow)
                aWindow->Destroy();
        });
    aboutItem->SetAction([window](PNuoMenuItem)
        {
            auto windowPos = window->PositionDevice();

            NuoPoint<double> windowPosF(windowPos.X(), windowPos.Y());
            auto scale = MonitorDPI<double>(windowPosF);

            auto dialogPos = windowPos;
            dialogPos.SetX(dialogPos.X() + (long)(60 * scale.X()));
            dialogPos.SetY(dialogPos.Y() + (long)(60 * scale.Y()));
            dialogPos.SetW((long)(300 * scale.X()));
            dialogPos.SetH((long)(150 * scale.Y()));

            PAppAboutDialog dlg = std::make_shared<AppAboutDialog>("About");
            dlg->SetPosition(dialogPos);
            dlg->DoModal(window);
        });

    window->Show();
    window->Update();

    window->SetIcon(icon);
    icon.reset();

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

    NuoAppInstance::UnInit();

    return (int) msg.wParam;
}


