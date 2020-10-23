
#include "NuoWindow.h"

#include "NuoControl.h"
#include "NuoAppInstance.h"
#include "NuoStrings.h"
#include "NuoMenu.h"
#include "NuoImage.h"

#include <windows.h>
#include <shellscalingapi.h>

#include "Resource.h"


wchar_t kClassName[100];



extern const int kWindowPtr = GWLP_USERDATA;



extern LRESULT CALLBACK NuoWindowProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK NuoWindow::NuoWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
    case WM_SIZE:
    {
        UINT width = LOWORD(lParam);
        UINT height = HIWORD(lParam);
        NuoWindow* window = (NuoWindow*)GetWindowLongPtr(hWnd, kWindowPtr);

        if (window)
            window->OnSize(width, height);

        break;
    }
    case WM_DESTROY:
    {
        NuoWindow* window = (NuoWindow*)GetWindowLongPtr(hWnd, kWindowPtr);

        if (window)
            window->OnDestroy();

        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT lc;
        BeginPaint(hWnd, &lc);

        NuoWindow* window = (NuoWindow*)GetWindowLongPtr(hWnd, kWindowPtr);

        if (window)
            window->OnPaint();

        EndPaint(hWnd, &lc);

        break;
    }
    case WM_DPICHANGED:
    {
        // Resize the window
        auto lprcNewScale = reinterpret_cast<RECT*>(lParam);
        NuoRect<long> newPos(lprcNewScale->left, lprcNewScale->top,
                             lprcNewScale->right - lprcNewScale->left,
                             lprcNewScale->bottom - lprcNewScale->top);

        NuoWindow* window = (NuoWindow*)GetWindowLongPtr(hWnd, kWindowPtr);
        float oldDPI = window->SavedDPI();
        float newDPI = window->DPI();
        if (window)
            window->OnDPIChange(newPos, newDPI, oldDPI);

        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}


NuoWindow::NuoWindow()
    : _hWnd(0), _inDPIChange(false), _savedDPI(1.0)
{
}


NuoWindow::NuoWindow(const std::string& title)
    : _title(title), _hWnd(0), _inDPIChange(false), _savedDPI(1.0)
{
    std::wstring wtitle = StringToUTF16(title);
    HINSTANCE hInstance = NuoAppInstance::GetInstance()->Instance();

    _hWnd = CreateWindowW(kClassName, wtitle.c_str(), WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    _savedDPI = DPI();

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


void NuoWindow::Hide()
{
    ::ShowWindow(_hWnd, SW_HIDE);
}


void NuoWindow::Update()
{
    ::UpdateWindow(_hWnd);
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


NuoRect<long> NuoWindow::PositionDevice()
{
    RECT rect;

    GetWindowRect(_hWnd, &rect);
    return NuoRect<long>(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
}


NuoRect<long> NuoWindow::ClientRectDevice()
{
    RECT rect;

    GetClientRect(_hWnd, &rect);
    return NuoRect<long>(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
}


NuoRect<float> NuoWindow::ClientRect()
{
    auto result = ClientRectDevice();
    float dpi = DPI();

    return result / dpi;
}


float NuoWindow::DPI() const
{
    UINT scale = GetDpiForWindow(_hWnd);

    return (float)scale / 100.0f;
}


float NuoWindow::SavedDPI() const
{
    return _savedDPI;
}


void NuoWindow::SetPositionDevice(const NuoRect<long>& pos, bool activate)
{
    UINT flag = 0;
    if (!activate)
        flag = SWP_NOZORDER | SWP_NOACTIVATE;

    SetWindowPos(_hWnd, HWND_NOTOPMOST, pos.X(), pos.Y(), pos.W(), pos.H(), flag);
}


std::shared_ptr<NuoFont> NuoWindow::Font()
{
    HFONT hFont = (HFONT)SendMessage(_hWnd, WM_GETFONT, 0, 0);
    return std::make_shared<NuoFont>(hFont);
}


void NuoWindow::SetFont(const std::shared_ptr<NuoFont>& font)
{
    _font = font;

    SendMessage(_hWnd, WM_SETFONT, (LPARAM)font->Handle(), TRUE);
}


void NuoWindow::SetIcon(const PNuoIcon& icon)
{
    LPARAM lParam = (LPARAM)icon->Handle();

    SendMessage(_hWnd, WM_SETICON, ICON_SMALL, lParam);
    SendMessage(_hWnd, WM_SETICON, ICON_BIG, lParam);

    HWND owner = GetWindow(_hWnd, GW_OWNER);
    SendMessage(owner, WM_SETICON, ICON_SMALL, lParam);
    SendMessage(owner, WM_SETICON, ICON_BIG, lParam);

    _icon = icon;
}


void NuoWindow::OnPaint()
{
}


bool NuoWindow::OnCommand(int id)
{
    bool processed = false;

    if (_menu)
        processed = _menu->DoAction(id);

    for (PNuoWindow child : _children)
    {
        NuoControl* control = dynamic_cast<NuoControl*>(child.get());
        if (control && control->ID() == id)
            control->OnCommand();
    }

    return processed;
}


void NuoWindow::OnSize(unsigned int x, unsigned int y)
{
    for (PNuoWindow child : _children)
    {
        NuoControl* control = dynamic_cast<NuoControl*>(child.get());
        if (!control)
            continue;

        NuoRect<float> rect = control->AutoPosition();
        control->SetPosition(rect, false);
    }
}


void NuoWindow::OnDPIChange(const NuoRect<long>& newRect, float newDPI, float oldDPI)
{
    if (_font)
    {
        _font->CreateFont(newDPI);
        SendMessage(_hWnd, WM_SETFONT, (LPARAM)_font->Handle(), TRUE);
    }

    for (PNuoWindow child : _children)
    {
        float ratio = newDPI / oldDPI;

        NuoRect<long> rect = child->PositionDevice();
        rect.SetX((long)(rect.X() * ratio));
        rect.SetY((long)(rect.Y() * ratio));
        rect.SetW((long)(rect.W() * ratio));
        rect.SetH((long)(rect.H() * ratio));

        NuoControl* control = dynamic_cast<NuoControl*>(child.get());
        if (control)
            control->OnDPIChange(newRect, newDPI, oldDPI);

        child->SetPositionDevice(rect, false);
    }

    SetPositionDevice(newRect, false);

    _savedDPI = newDPI;
}


void NuoWindow::Detach()
{
    _hWnd = 0;
}


void NuoWindow::Destroy()
{
    for (auto child : _children)
        child->Detach();

    _children.clear();

    ::DestroyWindow(_hWnd);
    _hWnd = 0;
}


void NuoWindow::OnDestroy()
{
    if (_onDestroy)
        _onDestroy();

    _hWnd = 0;
}


void NuoWindow::SetOnDestroy(SimpleFunc func)
{
    _onDestroy = func;
}


void NuoWindow::Add(const PNuoWindow& child)
{
    _children.insert(child);
}


NuoFont::NuoFont(HFONT font)
    : _hFont(font),
      _fontOwner(false),
      _weight(0.0),
      _isItalic(false),
      _isLight(false)
{
}


NuoFont::NuoFont(double weight, const std::string& name)
    : _hFont(0),
      _weight(weight),
      _name(name),
      _isItalic(false),
      _isLight(false),
      _fontOwner(true)
{
}

NuoFont::~NuoFont()
{
    if (_hFont && _fontOwner)
        DeleteObject(_hFont);
}

void NuoFont::SetLight(bool b)
{
    _isLight = b;
}


void NuoFont::SetItalic(bool b)
{
    _isItalic = b;
}


HFONT NuoFont::Handle() const
{
    return _hFont;
}


void NuoFont::CreateFont(float scale)
{
    if (_hFont && _fontOwner)
        DeleteObject(_hFont);

    int flag = 0;
    if (_isLight)
        flag = FW_THIN | FW_LIGHT;

    std::wstring wname = StringToUTF16(_name);
    _hFont = ::CreateFont((int)(_weight * scale), 0, 0, 0, flag /* weight */,
                          _isItalic /* non-italic */, 0, 0, 0, 0, 0, 0, 0, wname.c_str());

    _fontOwner = true;
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