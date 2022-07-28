

//
//  NuoScrollView.cpp
//  ModelViewer
//
//  Created by Dong Feng on 4/26/22.
//  Copyright © 2022 Dong Feng. All rights reserved.
//



#include "NuoScrollView.h"

#include "NuoStrings.h"
#include "NuoAppInstance.h"
#include "NuoDialog.h"





NuoScrollView::NuoScrollView(const PNuoWindow& parent, const std::string& title)
    : NuoControl(title, parent)
{
}


NuoScrollView::NuoScrollView(const PNuoDialog& parent, int controlID)
    : NuoControl("", std::dynamic_pointer_cast<NuoWindow>(parent))
{
    _hWnd = GetDlgItem(parent->Handle(), controlID);
}


void NuoScrollView::Init(int controlID)
{
    std::wstring wtitle = StringToUTF16(_title);
    PNuoWindow parent = _parent.lock();

    _hWnd = CreateWindow(kClassName, L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL,
                         CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                         parent->Handle(),      // Parent window
                         nullptr,
                         NuoAppInstance::GetInstance()->Instance(),
                         nullptr);

    _id = controlID;

    SetWindowLongPtr(_hWnd, kWindowPtr, (LONG_PTR)this);
    parent->Add(shared_from_this());

    Update();
    Show();
}



void NuoScrollView::SetContentHeight(float height)
{
    /***
     * https://www.codeproject.com/Articles/1042516/Custom-Controls-in-Win-API-Scrolling
     */

    NuoRect<long> controlPosition = PositionDevice();
    int contentHeightInt = height * DPI();

    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.nMin = 0;
    si.nMax = contentHeightInt;
    si.nPage = controlPosition.H();
    si.fMask = SIF_RANGE | SIF_PAGE;

    SetScrollInfo(_hWnd, SB_VERT, &si, false);
}


NuoScrollView::~NuoScrollView()
{
    if (_hWnd)
    {
        DestroyWindow(_hWnd);
        _hWnd = 0;
    }
}


int NuoScrollView::ScrollPosDevice() const
{
    SCROLLINFO si;

    // Get current scrollbar state:
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS;
    GetScrollInfo(_hWnd, SB_VERT, &si);

    return si.nPos;
}


void NuoScrollView::SetScrollPosDevice(int pos)
{
    int oldPos = ScrollPosDevice();

    // Update the scrollbar state (nPos) and repaint it. The function ensures
    // the nPos does not fall out of the allowed range between nMin and nMax
    // hence we ask for the corrected nPos again.
    SetScrollPos(_hWnd, SB_VERT, pos, TRUE);
    pos = GetScrollPos(_hWnd, SB_VERT);

    // Refresh the control (repaint it to reflect the new nPos). Note we
    // here multiply with some unspecified scrolling unit which specifies
    // amount of pixels corresponding to the 1 scrolling unit.
    // We will discuss ScrollWindowEx() more later in the article.
    // ScrollWindowEx(_hWnd, 0, (nOldPos - nPos),
    //    NULL, NULL, NULL, NULL, SW_ERASE | SW_INVALIDATE);
    ScrollWindow(_hWnd, 0, oldPos - pos, nullptr, nullptr);
}


void NuoScrollView::OnScroll(UINT message, WPARAM wParam, LPARAM lParam)
{
    int scrollAction = LOWORD(wParam);

    /***
     * https://www.codeproject.com/Articles/1042516/Custom-Controls-in-Win-API-Scrolling
     */

    int nPos;
    int nOldPos;
    SCROLLINFO si;

    // Get current scrollbar state:
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS;
    GetScrollInfo(_hWnd, SB_VERT, &si);

    nOldPos = si.nPos;

    // Compute new nPos.
    // Note we do not care where nPos falls between nMin and nMax. See below.
    switch (scrollAction) {
    case SB_TOP:            nPos = si.nMin; break;
    case SB_BOTTOM:         nPos = si.nMax; break;
    case SB_LINEUP:         nPos = si.nPos - 20; break;
    case SB_LINEDOWN:       nPos = si.nPos + 20; break;
    case SB_PAGEUP:         nPos = si.nPos - si.nPage; break;
    case SB_PAGEDOWN:       nPos = si.nPos + si.nPage; break;
    case SB_THUMBTRACK:     nPos = si.nTrackPos; break;
    default:
    case SB_THUMBPOSITION:  nPos = si.nPos; break;
    }

    SetScrollPosDevice(nPos);
}


bool NuoScrollView::OnScrollWheel(short keyState, short delta)
{
    // Compute how many lines to scroll.
    int nOldPos = ScrollPosDevice();
    int nPos = nOldPos + delta * (-0.2);

    SetScrollPosDevice(nPos);

    return true;
}
