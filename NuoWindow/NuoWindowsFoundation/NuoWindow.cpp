
#include "NuoWindow.h"
#include "NuoAppInstance.h"
#include "NuoStrings.h"
#include "NuoMenu.h"

#include <Windows.h>

#include "resource.h"


static wchar_t kClassName[100];// = L"NuoWindowClass";



static const int kWindowPtr = GWLP_USERDATA;



extern LRESULT CALLBACK NuoWindowProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK NuoWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);

        bool processed = false;

        NuoWindow* window = (NuoWindow*)GetWindowLongPtr(hWnd, kWindowPtr);
        processed = window->OnCommand(wmId);

        if (!processed)
            return DefWindowProc(hWnd, message, wParam, lParam);

        break;
    }
    case WM_DESTROY:
    {
        NuoWindow* window = (NuoWindow*)GetWindowLongPtr(hWnd, kWindowPtr);

        if (window)
        {
            window->OnDestroy();
        }
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}


NuoWindow::NuoWindow(const std::string& title)
    : _title(title)
{
    std::wstring wtitle = StringToUTF16(title);
    HINSTANCE hInstance = NuoAppInstance::GetInstance()->Instance();

    _hWnd = CreateWindowW(kClassName, wtitle.c_str(), WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    SetWindowLongPtr(_hWnd, kWindowPtr, (LONG_PTR)this);
}

NuoWindow::~NuoWindow()
{
    // window destroy is handled by the OnDestroy()
    //
    _ASSERT(_hWnd == 0);
}


void NuoWindow::Show()
{
    ::ShowWindow(_hWnd, NuoAppInstance::GetInstance()->CommandShow());
}


void NuoWindow::Update()
{
    ::UpdateWindow(_hWnd);
}


void NuoWindow::SetIcon(const int icon)
{
    HICON hIcon = LoadIcon(NuoAppInstance::GetInstance()->Instance(),
                           MAKEINTRESOURCE(icon));
    SendMessage(_hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
}


void NuoWindow::SetMenu(const PNuoMenuBar& menu)
{
    _menu = menu;

    ::SetMenu(_hWnd, _menu->Handle());
}


HWND NuoWindow::Handle() const
{
    return _hWnd;
}


NuoRect<long> NuoWindow::Position()
{
    RECT rect;

    GetWindowRect(_hWnd, &rect);
    return NuoRect<long>(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
}


bool NuoWindow::OnCommand(int id)
{
    bool processed = false;

    if (_menu)
        processed = _menu->DoAction(id);

    return processed;
}


void NuoWindow::Destroy()
{
    ::DestroyWindow(_hWnd);
    _hWnd = 0;
}


void NuoWindow::OnDestroy()
{
    _onDestroy();
    _hWnd = 0;
}


void NuoWindow::SetOnDestroy(SimpleFunc func)
{
    _onDestroy = func;
}


void NuoWindow::RegisterClass()
{
    wsprintf(kClassName, L"%s", L"NuoWindowClass");

    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = NuoWindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = NuoAppInstance::GetInstance()->Instance();
    wcex.hIcon = 0;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = L"NuoWindowClass";
    wcex.hIconSm = 0;

    RegisterClassEx(&wcex);
}