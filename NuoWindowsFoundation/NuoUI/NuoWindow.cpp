
#include "NuoWindow.h"

#include "NuoControl.h"
#include "NuoAppInstance.h"
#include "NuoStrings.h"
#include "NuoMenu.h"
#include "NuoImage.h"
#include "NuoSlider.h"
#include "NuoScrollView.h"

#include <windows.h>
#include <windowsx.h>
#include <shellscalingapi.h>


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
        int notification = HIWORD(wParam);

        bool processed = false;

        NuoWindow* window = (NuoWindow*)GetWindowLongPtr(hWnd, kWindowPtr);
        processed = window->OnCommand(wmId, notification);

        if (!processed)
            return DefWindowProc(hWnd, message, wParam, lParam);

        break;
    }
    case WM_HSCROLL:
    {
        bool processed = false;

        if (lParam)
        {
            NuoWindow* window = (NuoWindow*)GetWindowLongPtr((HWND)lParam, kWindowPtr);
            NuoSlider* slider = dynamic_cast<NuoSlider*>(window);
            if (slider)
            {
                slider->OnScroll(wParam);
                processed = true;
            }
        }
        
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
    case WM_VSCROLL:
    {
        NuoWindow* window = (NuoWindow*)GetWindowLongPtr(hWnd, kWindowPtr);
        bool processed = false;
        
        if (window)
        {
            NuoScrollView* scrollWindow = dynamic_cast<NuoScrollView*>(window);
            if (scrollWindow)
            {
                scrollWindow->OnScroll(message, wParam, lParam);
                processed = true;
            }
        }

        if (!processed)
            return DefWindowProc(hWnd, message, wParam, lParam);

        break;
    }
    case WM_MOUSEWHEEL
        :
    {
        NuoWindow* window = (NuoWindow*)GetWindowLongPtr(hWnd, kWindowPtr);
        bool processed = false;

        if (window)
        {
            short iDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            short keyState = GET_KEYSTATE_WPARAM(wParam);

            processed = window->OnScrollWheel(keyState, iDelta);
        }

        if (!processed)
            return DefWindowProc(hWnd, message, wParam, lParam);

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
    case WM_LBUTTONDOWN:
    case WM_MOUSEMOVE:
    case WM_LBUTTONUP:
    {
        NuoWindow* window = (NuoWindow*)GetWindowLongPtr(hWnd, kWindowPtr);
        if (window)
        {
            short x = GET_X_LPARAM(lParam);
            short y = GET_Y_LPARAM(lParam);

            switch (message)
            {
            case WM_LBUTTONDOWN:
                window->OnMouseDown(x, y);
                break;
            case WM_MOUSEMOVE:
                window->OnMouseMessage(x, y, wParam);
                break;
            case WM_LBUTTONUP:
                window->OnMouseUp(x, y);
                break;
            default:
                break;
            }
        }
        break;
    }
    case WM_CTLCOLORSTATIC:
    {
        NuoWindow* window = (NuoWindow*)GetWindowLongPtr(hWnd, kWindowPtr);

        if (window->_backgroundBrush == NULL)
            window->_backgroundBrush = CreateSolidBrush(RGB(255, 255, 255));

        return (INT_PTR)window->_backgroundBrush;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}


NuoWindow::NuoWindow()
    : _hWnd(0), _inDPIChange(false), _savedDPI(1.0), _inDragging(false), _backgroundBrush(0)
{
}


NuoWindow::NuoWindow(const std::string& title)
    : _title(title), _hWnd(0), _inDPIChange(false), _savedDPI(1.0), _inDragging(false), _backgroundBrush(0)
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
    ::InvalidateRect(_hWnd, NULL, true);
    ::UpdateWindow(_hWnd);
}


void NuoWindow::EnableMouseDrag()
{
    _inDragging = true;
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


long NuoWindow::TextWidth(const std::string& text, const PNuoFont& font)
{
    return TextSize(text, font).cx;
}


long NuoWindow::TextHeight(const std::string& text, const PNuoFont& font)
{
    return TextSize(text, font).cy;
}


SIZE NuoWindow::TextSize(const std::string& text, const PNuoFont& font)
{
    HDC hDC = GetDC(_hWnd);

    std::wstring wtext = StringToUTF16(text);

    SIZE size;
    SelectFont(hDC, font->Handle());
    GetTextExtentPoint32(hDC, wtext.c_str(), (int)wtext.length(), &size);

    ReleaseDC(_hWnd, hDC);

    return size;
}


float NuoWindow::SavedDPI() const
{
    return _savedDPI;
}


void NuoWindow::SetPositionDevice(const NuoRect<long>& pos, bool activate)
{
    UINT flag = 0;
    if (!activate)
    {
        // looks like the only way working for a child, non-free-framed window
        //
        flag = SWP_NOZORDER | SWP_NOACTIVATE;
    }

    SetWindowPos(_hWnd, HWND_NOTOPMOST, pos.X(), pos.Y(), pos.W(), pos.H(), flag);
}


std::shared_ptr<NuoFont> NuoWindow::Font()
{
    HFONT hFont = (HFONT)SendMessage(_hWnd, WM_GETFONT, 0, 0);
    return std::make_shared<NuoFont>(hFont);
}


void NuoWindow::SetFont(const std::shared_ptr<NuoFont>& font)
{
    _font = std::make_shared<NuoFont>(*font);
    _font->CreateFont(DPI());

    SendMessage(_hWnd, WM_SETFONT, (LPARAM)_font->Handle(), TRUE);
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


void NuoWindow::SetText(const std::string& text)
{
    std::wstring wtext = StringToUTF16(text);

    SetWindowText(_hWnd, wtext.c_str());
}


void NuoWindow::OnPaint()
{
}


bool NuoWindow::OnCommand(int id, int notification)
{
    bool processed = false;

    if (_menu)
        processed = _menu->DoAction(id);

    for (PNuoWindow child : _children)
    {
        NuoControl* control = dynamic_cast<NuoControl*>(child.get());
        if (control && control->ID() == id)
            control->OnCommand(notification);
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


void NuoWindow::OnMouseMove(short x, short y)
{
}


void NuoWindow::OnMouseDown(short x, short y)
{
    if (!_inDragging)
        return;

    SetCapture(_hWnd);

    _mouseX = x;
    _mouseY = y;

    BOOL ret;
    MSG msg;
    while ((ret = GetMessage(&msg, 0, 0, 0)) != 0)
    {
        if (ret == -1) /* error found */
            break;

        if (msg.message == WM_MOUSEMOVE ||
            msg.message == WM_LBUTTONUP ||
            msg.message == WM_PAINT || msg.message == WM_TIMER)
        {
            TranslateMessage(&msg); /* translate virtual-key messages */
            DispatchMessage(&msg);  /* send it to dialog procedure */
        }
    }
}


void NuoWindow::OnMouseDrag(short x, short y, short deltaX, short deltaY, const NuoMouseModifer& modifier)
{
}


void NuoWindow::OnMouseUp(short x, short y)
{
    if (_inDragging)
    {
        _inDragging = false;
        ReleaseCapture();

        PostQuitMessage(0);
    }
}


bool NuoWindow::OnScrollWheel(short keyState, short delta)
{
    return false;
}


void NuoWindow::OnDPIChange(const NuoRect<long>& newRect, float newDPI, float oldDPI)
{
    if (_font)
    {
        _font = std::make_shared<NuoFont>(*_font);
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


void NuoWindow::OnMouseMessage(short x, short y, WPARAM wParam)
{
    if (_inDragging)
    {
        short deltaX = x - _mouseX;
        short deltaY = y - _mouseY;

        _mouseX = x;
        _mouseY = y;

        NuoMouseModifer modifier;

        modifier._holdControl = wParam & MK_CONTROL;
        modifier._holdShift = wParam & MK_SHIFT;

        OnMouseDrag(x, y, deltaX, deltaY, modifier);
    }
    else
    {
        OnMouseMove(x, y);
    }
}


void NuoWindow::Detach()
{
    _hWnd = 0;
}


void NuoWindow::Destroy()
{

    if (_backgroundBrush)
    {
        ::DeleteObject(_backgroundBrush);
        _backgroundBrush = 0;
    }

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
    LOGFONT logicFont;

    GetObject(_hFont, sizeof(LOGFONT), &logicFont);

    wchar_t* name = logicFont.lfFaceName;
    _name = StringToUTF8(name);
}


NuoFont::NuoFont(const NuoFont& font)
    : _hFont(0),
      _weight(font._weight),
      _name(font._name),
      _isItalic(font._isItalic),
      _isLight(font._isLight),
      _fontOwner(font._fontOwner)
{
    if (!_fontOwner)
        _hFont = font._hFont;
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


PNuoFont NuoFont::MenuFont(double size)
{
    NONCLIENTMETRICS metric;
    metric.cbSize = sizeof(NONCLIENTMETRICS);

    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, (void*)&metric, 0);

    std::string face = StringToUTF8(metric.lfMenuFont.lfFaceName);
    return std::make_shared<NuoFont>(size, face);
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