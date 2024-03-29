/*
 * PROJECT:     ReactOS header files
 * LICENSE:     CC-BY-4.0 (https://spdx.org/licenses/CC-BY-4.0.html)
 * PURPOSE:     Win32API message dumping
 * COPYRIGHT:   Copyright 2018-2019 Katayama Hirofumi MZ (katayama.hirofumi.mz@gmail.com)
 */
#ifndef _INC_MSGDUMP
#define _INC_MSGDUMP    6   /* Version 6 */

/*
 * NOTE: MD_msgdump function in this file provides Win32API message dump feature.
 * NOTE: This header file takes time to compile.
 *       You might indirectly use MD_msgdump function.
 */
#pragma once

#ifndef _INC_WINXX
    #include "winxx.h"      /* An unofficial extension of <windowsx.h>. */
#endif
#include <strsafe.h>

#ifndef MSGDUMP_TPRINTF
    #error Please define MSGDUMP_TPRINTF macro before #include "msgdump.h".
#endif

#ifndef MSGDUMP_API
    #define MSGDUMP_API  WINAPI
#endif

#ifndef MSGDUMP_PREFIX
    #define MSGDUMP_PREFIX TEXT("")
#endif

/* MD_msgdump function */
static __inline LRESULT MSGDUMP_API
MD_msgdump(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/* MD_msgresult function */
static __inline LRESULT MSGDUMP_API
MD_msgresult(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT lResult);

/*---- The below codes are boring details of MD_msgdump and MD_msgresult implementation. ----*/

#define MSGDUMP_MAX_RECT_TEXT   64

static __inline const TCHAR * MSGDUMP_API
MD_rect_text(TCHAR *buf, size_t bufsize, const RECT *prc)
{
    if (prc == NULL)
    {
        StringCbCopy(buf, bufsize, TEXT("(null)"));
    }
    else
    {
        StringCbPrintf(buf, bufsize, TEXT("(%ld, %ld, %ld, %ld)"),
                       prc->left, prc->top, prc->right, prc->bottom);
    }
    return buf;
}

static __inline LRESULT MSGDUMP_API
MD_OnUnknown(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    TCHAR szName[64];
    if (0xC000 <= uMsg && uMsg <= 0xFFFF &&
        GlobalGetAtomName(uMsg, szName, ARRAYSIZE(szName)))
    {
        /* RegisterWindowMessage'd message */
        MSGDUMP_TPRINTF(TEXT("%s'%s'(%u)(hwnd:%p, wParam:%p, lParam:%p)\n"),
                        MSGDUMP_PREFIX, szName, uMsg, (void *)hwnd, (void *)wParam,
                        (void *)lParam);
        return 0;
    }

    MSGDUMP_TPRINTF(TEXT("%sWM_%u(hwnd:%p, wParam:%p, lParam:%p)\n"),
                    MSGDUMP_PREFIX, uMsg, (void *)hwnd, (void *)wParam, (void *)lParam);
    return 0;
}

static __inline LRESULT MSGDUMP_API
MD_OnUser(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_USER+%u(hwnd:%p, wParam:%p, lParam:%p)\n"),
                    MSGDUMP_PREFIX, uMsg - WM_USER, (void *)hwnd, (void *)wParam, (void *)lParam);
    return 0;
}

static __inline LRESULT MSGDUMP_API
MD_OnApp(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_APP+%u(hwnd:%p, wParam:%p, lParam:%p)\n"),
                    MSGDUMP_PREFIX, uMsg - WM_APP, (void *)hwnd, (void *)wParam, (void *)lParam);
    return 0;
}

static __inline LRESULT MSGDUMP_API
MD_OnNull(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_NULL(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return 0;
}

static __inline BOOL MSGDUMP_API
MD_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_CREATE(hwnd:%p, lpCreateStruct:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)lpCreateStruct);
    return TRUE;
}

static __inline void MSGDUMP_API
MD_OnDestroy(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_DESTROY(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline void MSGDUMP_API
MD_OnMove(HWND hwnd, int x, int y)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MOVE(hwnd:%p, x:%d, y:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, x, y);
}

static __inline void MSGDUMP_API
MD_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_SIZE(hwnd:%p, state:%u, cx:%d, cy:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, state, cx, cy);
}

static __inline void MSGDUMP_API
MD_OnActivate(HWND hwnd, UINT state, HWND hwndActDeact, BOOL fMinimized)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_ACTIVATE(hwnd:%p, state:%u, hwndActDeact:%p, fMinimized:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, state, (void *)hwndActDeact, fMinimized);
}

static __inline void MSGDUMP_API
MD_OnSetFocus(HWND hwnd, HWND hwndOldFocus)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_SETFOCUS(hwnd:%p, hwndOldFocus:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hwndOldFocus);
}

static __inline void MSGDUMP_API
MD_OnKillFocus(HWND hwnd, HWND hwndNewFocus)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_KILLFOCUS(hwnd:%p, hwndNewFocus:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hwndNewFocus);
}

static __inline void MSGDUMP_API
MD_OnEnable(HWND hwnd, BOOL fEnable)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_ENABLE(hwnd:%p, fEnable:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, fEnable);
}

static __inline void MSGDUMP_API
MD_OnSetRedraw(HWND hwnd, BOOL fRedraw)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_SETREDRAW(hwnd:%p, fRedraw:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, fRedraw);
}

static __inline void MSGDUMP_API
MD_OnSetText(HWND hwnd, LPCTSTR lpszText)
{
    if (IsWindowUnicode(hwnd))
        MSGDUMP_TPRINTF(TEXT("%sWM_SETTEXT(hwnd:%p, lpszText:%ls)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, (LPCWSTR)lpszText);
    else
        MSGDUMP_TPRINTF(TEXT("%sWM_SETTEXT(hwnd:%p, lpszText:%hs)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, (LPCSTR)lpszText);
}

static __inline INT MSGDUMP_API
MD_OnGetText(HWND hwnd, int cchTextMax, LPTSTR lpszText)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_GETTEXT(hwnd:%p, cchTextMax:%d, lpszText:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, cchTextMax, (void *)lpszText);
    return 0;
}

static __inline INT MSGDUMP_API
MD_OnGetTextLength(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_GETTEXTLENGTH(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return 0;
}

static __inline void MSGDUMP_API
MD_OnPaint(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_PAINT(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline void MSGDUMP_API
MD_OnClose(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_CLOSE(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline BOOL MSGDUMP_API
MD_OnQueryEndSession(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_QUERYENDSESSION(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return FALSE;
}

static __inline BOOL MSGDUMP_API
MD_OnQueryOpen(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_QUERYOPEN(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return FALSE;
}

static __inline void MSGDUMP_API
MD_OnEndSession(HWND hwnd, BOOL fEnding)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_ENDSESSION(hwnd:%p, fEnding:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, fEnding);
}

static __inline void MSGDUMP_API
MD_OnQuit(HWND hwnd, int exitCode)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_QUIT(hwnd:%p, exitCode:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, exitCode);
}

static __inline BOOL MSGDUMP_API
MD_OnEraseBkgnd(HWND hwnd, HDC hdc)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_ERASEBKGND(hwnd:%p, hdc:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hdc);
    return FALSE;
}

static __inline void MSGDUMP_API
MD_OnSysColorChange(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_SYSCOLORCHANGE(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline void MSGDUMP_API
MD_OnShowWindow(HWND hwnd, BOOL fShow, UINT status)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_SHOWWINDOW(hwnd:%p, fShow:%d, status:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, fShow, status);
}

static __inline void MSGDUMP_API
MD_OnWinIniChange(HWND hwnd, LPCTSTR lpszSectionName)
{
    if (IsWindowUnicode(hwnd))
        MSGDUMP_TPRINTF(TEXT("%sWM_WININICHANGE(hwnd:%p, lpszSectionName:%ls)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, (LPCWSTR)lpszSectionName);
    else
        MSGDUMP_TPRINTF(TEXT("%sWM_WININICHANGE(hwnd:%p, lpszSectionName:%hs)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, (LPCSTR)lpszSectionName);
}

static __inline void MSGDUMP_API
MD_OnSettingChange(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_SETTINGCHANGE(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline void MSGDUMP_API
MD_OnDevModeChange(HWND hwnd, LPCTSTR lpszDeviceName)
{
    if (IsWindowUnicode(hwnd))
        MSGDUMP_TPRINTF(TEXT("%sWM_DEVMODECHANGE(hwnd:%p, lpszDeviceName:%ls)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, (LPCWSTR)lpszDeviceName);
    else
        MSGDUMP_TPRINTF(TEXT("%sWM_DEVMODECHANGE(hwnd:%p, lpszDeviceName:%hs)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, (LPCSTR)lpszDeviceName);
}

static __inline void MSGDUMP_API
MD_OnActivateApp(HWND hwnd, BOOL fActivate, DWORD dwThreadId)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_ACTIVATEAPP(hwnd:%p, fActivate:%d, dwThreadId:0x%08lX)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, fActivate, dwThreadId);
}

static __inline void MSGDUMP_API
MD_OnFontChange(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_FONTCHANGE(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline void MSGDUMP_API
MD_OnTimeChange(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_TIMECHANGE(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline void MSGDUMP_API
MD_OnCancelMode(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_CANCELMODE(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline BOOL MSGDUMP_API
MD_OnSetCursor(HWND hwnd, HWND hwndCursor, UINT codeHitTest, UINT msg)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_SETCURSOR(hwnd:%p, hwndCursor:%p, codeHitTest:%u, msg:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hwndCursor, codeHitTest, msg);
    return FALSE;
}

static __inline int MSGDUMP_API
MD_OnMouseActivate(HWND hwnd, HWND hwndTopLevel, UINT codeHitTest, UINT msg)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MOUSEACTIVATE(hwnd:%p, hwndTopLevel:%p, codeHitTest:%u, msg:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hwndTopLevel, codeHitTest, msg);
    return 0;
}

static __inline void MSGDUMP_API
MD_OnChildActivate(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_CHILDACTIVATE(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline void MSGDUMP_API
MD_OnQueueSync(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_QUEUESYNC(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline void MSGDUMP_API
MD_OnGetMinMaxInfo(HWND hwnd, LPMINMAXINFO lpMinMaxInfo)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_GETMINMAXINFO(hwnd:%p, lpMinMaxInfo:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)lpMinMaxInfo);
}

static __inline BOOL MSGDUMP_API
MD_OnIconEraseBkgnd(HWND hwnd, HDC hdc)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_ICONERASEBKGND(hwnd:%p, hdc:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hdc);
    return FALSE;
}

static __inline HWND MSGDUMP_API
MD_OnNextDlgCtl(HWND hwnd, HWND hwndSetFocus, BOOL fNext)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_NEXTDLGCTL(hwnd:%p, hwndSetFocus:%p, fNext:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hwndSetFocus, fNext);
    return NULL;
}

static __inline void MSGDUMP_API
MD_OnSpoolerStatus(HWND hwnd, UINT status, int cJobInQueue)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_SPOOLERSTATUS(hwnd:%p, status:%u, cJobInQueue:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, status, cJobInQueue);
}

static __inline void MSGDUMP_API
MD_OnDrawItem(HWND hwnd, const DRAWITEMSTRUCT * lpDrawItem)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_DRAWITEM(hwnd:%p, lpDrawItem:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)lpDrawItem);
}

static __inline void MSGDUMP_API
MD_OnMeasureItem(HWND hwnd, MEASUREITEMSTRUCT * lpMeasureItem)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MEASUREITEM(hwnd:%p, lpMeasureItem:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)lpMeasureItem);
}

static __inline void MSGDUMP_API
MD_OnDeleteItem(HWND hwnd, const DELETEITEMSTRUCT * lpDeleteItem)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_DELETEITEM(hwnd:%p, lpDeleteItem:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)lpDeleteItem);
}

static __inline int MSGDUMP_API
MD_OnVkeyToItem(HWND hwnd, UINT vk, HWND hwndListbox, int iCaret)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_VKEYTOITEM(hwnd:%p, vk:%u, hwndListbox:%p, iCaret:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, vk, (void *)hwndListbox, iCaret);
    return 0;
}

static __inline int MSGDUMP_API
MD_OnCharToItem(HWND hwnd, UINT ch, HWND hwndListbox, int iCaret)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_CHARTOITEM(hwnd:%p, ch:%u, hwndListbox:%p, iCaret:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, ch, (void *)hwndListbox, iCaret);
    return 0;
}

static __inline void MSGDUMP_API
MD_OnSetFont(HWND hwnd, HFONT hfont, BOOL fRedraw)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_SETFONT(hwnd:%p, hfont:%p, fRedraw:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hfont, fRedraw);
}

static __inline HFONT MSGDUMP_API
MD_OnGetFont(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_GETFONT(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return NULL;
}

static __inline HICON MSGDUMP_API
MD_OnQueryDragIcon(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_QUERYDRAGICON(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return NULL;
}

static __inline int MSGDUMP_API
MD_OnCompareItem(HWND hwnd, const COMPAREITEMSTRUCT * lpCompareItem)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_COMPAREITEM(hwnd:%p, lpCompareItem:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)lpCompareItem);
    return 0;
}

static __inline void MSGDUMP_API
MD_OnCompacting(HWND hwnd, UINT compactRatio)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_COMPACTING(hwnd:%p, compactRatio:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, compactRatio);
}

static __inline void MSGDUMP_API
MD_OnCommNotify(HWND hwnd, int cid, UINT flags)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_COMMNOTIFY(hwnd:%p, cid:%d, flags:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, cid, flags);
}

static __inline BOOL MSGDUMP_API
MD_OnWindowPosChanging(HWND hwnd, LPWINDOWPOS lpwpos)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_WINDOWPOSCHANGING(hwnd:%p, lpwpos:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)lpwpos);
    return FALSE;
}

static __inline void MSGDUMP_API
MD_OnWindowPosChanged(HWND hwnd, const LPWINDOWPOS lpwpos)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_WINDOWPOSCHANGED(hwnd:%p, lpwpos:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)lpwpos);
}

static __inline void MSGDUMP_API
MD_OnPower(HWND hwnd, int code)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_POWER(hwnd:%p, code:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, code);
}

static __inline BOOL MSGDUMP_API
MD_OnCopyData(HWND hwnd, HWND hwndFrom, PCOPYDATASTRUCT pcds)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_COPYDATA(hwnd:%p, hwndFrom:%p, pcds:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hwndFrom, (void *)pcds);
    return FALSE;
}

static __inline LRESULT MSGDUMP_API
MD_OnNotify(HWND hwnd, int idFrom, LPNMHDR pnmhdr)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_NOTIFY(hwnd:%p, idFrom:%d, pnmhdr:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, idFrom, (void *)pnmhdr);
    return 0;
}

static __inline void MSGDUMP_API
MD_OnContextMenu(HWND hwnd, HWND hwndContext, UINT xPos, UINT yPos)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_CONTEXTMENU(hwnd:%p, hwndContext:%p, xPos:%u, yPos:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hwndContext, xPos, yPos);
}

static __inline void MSGDUMP_API
MD_OnDisplayChange(HWND hwnd, UINT bitsPerPixel, UINT cxScreen, UINT cyScreen)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_DISPLAYCHANGE(hwnd:%p, bitsPerPixel:%u, cxScreen:%u, cyScreen:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, bitsPerPixel, cxScreen, cyScreen);
}

static __inline BOOL MSGDUMP_API
MD_OnNCCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_NCCREATE(hwnd:%p, lpCreateStruct:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)lpCreateStruct);
    return FALSE;
}

static __inline void MSGDUMP_API
MD_OnNCDestroy(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_NCDESTROY(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline UINT MSGDUMP_API
MD_OnNCCalcSize(HWND hwnd, BOOL fCalcValidRects, NCCALCSIZE_PARAMS * lpcsp)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_NCCALCSIZE(hwnd:%p, fCalcValidRects:%d, lpcsp:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, fCalcValidRects, (void *)lpcsp);
    return 0;
}

static __inline UINT MSGDUMP_API
MD_OnNCHitTest(HWND hwnd, int x, int y)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_NCHITTEST(hwnd:%p, x:%d, y:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, x, y);
    return 0;
}

static __inline void MSGDUMP_API
MD_OnNCPaint(HWND hwnd, HRGN hrgn)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_NCPAINT(hwnd:%p, hrgn:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hrgn);
}

static __inline BOOL MSGDUMP_API
MD_OnNCActivate(HWND hwnd, BOOL fActive, HWND hwndActDeact, BOOL fMinimized)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_NCACTIVATE(hwnd:%p, fActive:%d, hwndActDeact:%p, fMinimized:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, fActive, (void *)hwndActDeact, fMinimized);
    return FALSE;
}

static __inline UINT MSGDUMP_API
MD_OnGetDlgCode(HWND hwnd, LPMSG lpmsg)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_GETDLGCODE(hwnd:%p, lpmsg:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)lpmsg);
    return 0;
}

static __inline void MSGDUMP_API
MD_OnNCMouseMove(HWND hwnd, int x, int y, UINT codeHitTest)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_NCMOUSEMOVE(hwnd:%p, x:%d, y:%d, codeHitTest:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, x, y, codeHitTest);
}

static __inline void MSGDUMP_API
MD_OnNCLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT codeHitTest)
{
    if (fDoubleClick)
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_NCLBUTTONDBLCLK(hwnd:%p, x:%d, y:%d, codeHitTest:%u)\n"),
                       MSGDUMP_PREFIX, (void *)hwnd, x, y, codeHitTest);
    }
    else
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_NCLBUTTONDOWN(hwnd:%p, x:%d, y:%d, codeHitTest:%u)\n"),
                       MSGDUMP_PREFIX, (void *)hwnd, x, y, codeHitTest);
    }
}

static __inline void MSGDUMP_API
MD_OnNCLButtonUp(HWND hwnd, int x, int y, UINT codeHitTest)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_NCLBUTTONUP(hwnd:%p, x:%d, y:%d, codeHitTest:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, x, y, codeHitTest);
}

static __inline void MSGDUMP_API
MD_OnNCRButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT codeHitTest)
{
    if (fDoubleClick)
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_NCRBUTTONDBLCLK(hwnd:%p, x:%d, y:%d, codeHitTest:%u)\n"),
                       MSGDUMP_PREFIX, (void *)hwnd, x, y, codeHitTest);
    }
    else
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_NCRBUTTONDOWN(hwnd:%p, x:%d, y:%d, codeHitTest:%u)\n"),
                       MSGDUMP_PREFIX, (void *)hwnd, x, y, codeHitTest);
    }
}

static __inline void MSGDUMP_API
MD_OnNCRButtonUp(HWND hwnd, int x, int y, UINT codeHitTest)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_NCRBUTTONUP(hwnd:%p, x:%d, y:%d, codeHitTest:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, x, y, codeHitTest);
}

static __inline void MSGDUMP_API
MD_OnNCMButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT codeHitTest)
{
    if (fDoubleClick)
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_NCMBUTTONDBLCLK(hwnd:%p, x:%d, y:%d, codeHitTest:%u)\n"),
                       MSGDUMP_PREFIX, (void *)hwnd, x, y, codeHitTest);
    }
    else
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_NCMBUTTONDOWN(hwnd:%p, x:%d, y:%d, codeHitTest:%u)\n"),
                       MSGDUMP_PREFIX, (void *)hwnd, x, y, codeHitTest);
    }
}

static __inline void MSGDUMP_API
MD_OnNCMButtonUp(HWND hwnd, int x, int y, UINT codeHitTest)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_NCMBUTTONUP(hwnd:%p, x:%d, y:%d, codeHitTest:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, x, y, codeHitTest);
}

static __inline void MSGDUMP_API
MD_OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
    if (fDown)
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_KEYDOWN(hwnd:%p, vk:%u, cRepeat:%d, flags:%u)\n"),
                       MSGDUMP_PREFIX, (void *)hwnd, vk, cRepeat, flags);
    }
    else
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_KEYUP(hwnd:%p, vk:%u, cRepeat:%d, flags:%u)\n"),
                       MSGDUMP_PREFIX, (void *)hwnd, vk, cRepeat, flags);
    }
}

static __inline void MSGDUMP_API
MD_OnChar(HWND hwnd, TCHAR ch, int cRepeat)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_CHAR(hwnd:%p, ch:%u, cRepeat:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, ch, cRepeat);
}

static __inline void MSGDUMP_API
MD_OnDeadChar(HWND hwnd, TCHAR ch, int cRepeat)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_DEADCHAR(hwnd:%p, ch:%u, cRepeat:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, ch, cRepeat);
}

static __inline void MSGDUMP_API
MD_OnSysKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
    if (fDown)
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_SYSKEYDOWN(hwnd:%p, vk:%u, cRepeat:%d, flags:%u)\n"),
                       MSGDUMP_PREFIX, (void *)hwnd, vk, cRepeat, flags);
    }
    else
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_SYSKEYUP(hwnd:%p, vk:%u, cRepeat:%d, flags:%u)\n"),
                       MSGDUMP_PREFIX, (void *)hwnd, vk, cRepeat, flags);
    }
}

static __inline void MSGDUMP_API
MD_OnSysChar(HWND hwnd, TCHAR ch, int cRepeat)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_SYSCHAR(hwnd:%p, ch:%u, cRepeat:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, ch, cRepeat);
}

static __inline void MSGDUMP_API
MD_OnSysDeadChar(HWND hwnd, TCHAR ch, int cRepeat)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_SYSDEADCHAR(hwnd:%p, ch:%u, cRepeat:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, ch, cRepeat);
}

static __inline BOOL MSGDUMP_API
MD_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_INITDIALOG(hwnd:%p, hwndFocus:%p, lParam:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hwndFocus, (void *)lParam);
    return FALSE;
}

static __inline void MSGDUMP_API
MD_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_COMMAND(hwnd:%p, id:%d, hwndCtl:%p, codeNotify:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, id, (void *)hwndCtl, codeNotify);
}

static __inline void MSGDUMP_API
MD_OnSysCommand(HWND hwnd, UINT cmd, int x, int y)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_SYSCOMMAND(hwnd:%p, cmd:%u, x:%d, y:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, cmd, x, y);
}

static __inline void MSGDUMP_API
MD_OnTimer(HWND hwnd, UINT id)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_TIMER(hwnd:%p, id:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, id);
}

static __inline void MSGDUMP_API
MD_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_HSCROLL(hwnd:%p, hwndCtl:%p, code:%u, pos:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hwndCtl, code, pos);
}

static __inline void MSGDUMP_API
MD_OnVScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_VSCROLL(hwnd:%p, hwndCtl:%p, code:%u, pos:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hwndCtl, code, pos);
}

static __inline void MSGDUMP_API
MD_OnInitMenu(HWND hwnd, HMENU hMenu)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_INITMENU(hwnd:%p, hMenu:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hMenu);
}

static __inline void MSGDUMP_API
MD_OnInitMenuPopup(HWND hwnd, HMENU hMenu, UINT item, BOOL fSystemMenu)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_INITMENUPOPUP(hwnd:%p, hMenu:%p, item:%u, fSystemMenu:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hMenu, item, fSystemMenu);
}

static __inline void MSGDUMP_API
MD_OnMenuSelect(HWND hwnd, HMENU hmenu, int item, HMENU hmenuPopup, UINT flags)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MENUSELECT(hwnd:%p, hmenu:%p, item:%d, hmenuPopup:%p, flags:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hmenu, item, (void *)hmenuPopup, flags);
}

static __inline DWORD MSGDUMP_API
MD_OnMenuChar(HWND hwnd, UINT ch, UINT flags, HMENU hmenu)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MENUCHAR(hwnd:%p, ch:%u, flags:%u, hmenu:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, ch, flags, (void *)hmenu);
    return 0;
}

static __inline void MSGDUMP_API
MD_OnEnterIdle(HWND hwnd, UINT source, HWND hwndSource)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_ENTERIDLE(hwnd:%p, source:%u, hwndSource:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, source, (void *)hwndSource);
}

static __inline HBRUSH MSGDUMP_API
MD_OnCtlColor(HWND hwnd, HDC hdc, HWND hwndChild, int type)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_CTLCOLOR(hwnd:%p, hdc:%p, hwndChild:%p, type:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hdc, (void *)hwndChild, type);
    return NULL;
}

static __inline void MSGDUMP_API
MD_OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MOUSEMOVE(hwnd:%p, x:%d, y:%d, keyFlags:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, x, y, keyFlags);
}

static __inline void MSGDUMP_API
MD_OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
    if (fDoubleClick)
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_LBUTTONDBLCLK(hwnd:%p, x:%d, y:%d, keyFlags:%u)\n"),
                       MSGDUMP_PREFIX, (void *)hwnd, x, y, keyFlags);
    }
    else
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_LBUTTONDOWN(hwnd:%p, x:%d, y:%d, keyFlags:%u)\n"),
                       MSGDUMP_PREFIX, (void *)hwnd, x, y, keyFlags);
    }
}

static __inline void MSGDUMP_API
MD_OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_LBUTTONUP(hwnd:%p, x:%d, y:%d, keyFlags:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, x, y, keyFlags);
}

static __inline void MSGDUMP_API
MD_OnRButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
    if (fDoubleClick)
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_RBUTTONDBLCLK(hwnd:%p, x:%d, y:%d, keyFlags:%u)\n"),
                       MSGDUMP_PREFIX, (void *)hwnd, x, y, keyFlags);
    }
    else
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_RBUTTONDOWN(hwnd:%p, x:%d, y:%d, keyFlags:%u)\n"),
                       MSGDUMP_PREFIX, (void *)hwnd, x, y, keyFlags);
    }
}

static __inline void MSGDUMP_API
MD_OnRButtonUp(HWND hwnd, int x, int y, UINT flags)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_RBUTTONUP(hwnd:%p, x:%d, y:%d, flags:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, x, y, flags);
}

static __inline void MSGDUMP_API
MD_OnMButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
    if (fDoubleClick)
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_MBUTTONDBLCLK(hwnd:%p, x:%d, y:%d, keyFlags:%u)\n"),
                       MSGDUMP_PREFIX, (void *)hwnd, x, y, keyFlags);
    }
    else
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_MBUTTONDOWN(hwnd:%p, x:%d, y:%d, keyFlags:%u)\n"),
                       MSGDUMP_PREFIX, (void *)hwnd, x, y, keyFlags);
    }
}

static __inline void MSGDUMP_API
MD_OnMButtonUp(HWND hwnd, int x, int y, UINT flags)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MBUTTONUP(hwnd:%p, x:%d, y:%d, flags:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, x, y, flags);
}

static __inline void MSGDUMP_API
MD_OnMouseWheel(HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MOUSEWHEEL(hwnd:%p, xPos:%d, yPos:%d, zDelta:%d, fwKeys:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, xPos, yPos, zDelta, fwKeys);
}

static __inline void MSGDUMP_API
MD_OnParentNotify(HWND hwnd, UINT msg, HWND hwndChild, int idChild)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_PARENTNOTIFY(hwnd:%p, msg:%u, hwndChild:%p, idChild:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, msg, (void *)hwndChild, idChild);
}

static __inline BOOL MSGDUMP_API
MD_OnDeviceChange(HWND hwnd, UINT uEvent, DWORD dwEventData)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_DEVICECHANGE(hwnd:%p, uEvent:%u, dwEventData:0x%08lX)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, uEvent, dwEventData);
    return FALSE;
}

static __inline HWND MSGDUMP_API
MD_MDICreate(HWND hwnd, const LPMDICREATESTRUCT lpmcs)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MDICREATE(hwnd:%p, lpmcs:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)lpmcs);
    return NULL;
}

static __inline void MSGDUMP_API
MD_MDIDestroy(HWND hwnd, HWND hwndDestroy)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MDIDESTROY(hwnd:%p, hwndDestroy:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hwndDestroy);
}

static __inline void MSGDUMP_API
MD_MDIActivate(HWND hwnd, BOOL fActive, HWND hwndActivate, HWND hwndDeactivate)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MDIACTIVATE(hwnd:%p, fActive:%d, hwndActivate:%p, hwndDeactivate:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, fActive, (void *)hwndActivate, (void *)hwndDeactivate);
}

static __inline void MSGDUMP_API
MD_MDIRestore(HWND hwnd, HWND hwndRestore)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MDIRESTORE(hwnd:%p, hwndRestore:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hwndRestore);
}

static __inline HWND MSGDUMP_API
MD_MDINext(HWND hwnd, HWND hwndCur, BOOL fPrev)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MDINEXT(hwnd:%p, hwndCur:%p, fPrev:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hwndCur, fPrev);
    return NULL;
}

static __inline void MSGDUMP_API
MD_MDIMaximize(HWND hwnd, HWND hwndMaximize)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MDIMAXIMIZE(hwnd:%p, hwndMaximize:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hwndMaximize);
}

static __inline BOOL MSGDUMP_API
MD_MDITile(HWND hwnd, UINT cmd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MDITILE(hwnd:%p, cmd:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, cmd);
    return FALSE;
}

static __inline BOOL MSGDUMP_API
MD_MDICascade(HWND hwnd, UINT cmd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MDICASCADE(hwnd:%p, cmd:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, cmd);
    return FALSE;
}

static __inline void MSGDUMP_API
MD_MDIIconArrange(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MDIICONARRANGE(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline HWND MSGDUMP_API
MD_MDIGetActive(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MDIGETACTIVE(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return NULL;
}

#ifdef _UNDOCUSER_H
    static __inline LRESULT MSGDUMP_API
    MD_OnDropObject(HWND hwnd, WPARAM wParam, LPARAM lParam)
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_DROPOBJECT(hwnd:%p, wParam:%p, lParam:%p)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, (void *)wParam, (void *)lParam);
        return 0;
    }

    static __inline LRESULT MSGDUMP_API
    MD_OnQueryDropObject(HWND hwnd, WPARAM wParam, LPARAM lParam)
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_QUERYDROPOBJECT(hwnd:%p, wParam:%p, lParam:%p)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, (void *)wParam, (void *)lParam);
        return 0;
    }

    static __inline LRESULT MSGDUMP_API
    MD_OnBeginDrag(HWND hwnd, WPARAM wParam, LPARAM lParam)
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_BEGINDRAG(hwnd:%p, wParam:%p, lParam:%p)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, (void *)wParam, (void *)lParam);
        return 0;
    }

    static __inline LRESULT MSGDUMP_API
    MD_OnDragLoop(HWND hwnd, WPARAM wParam, LPARAM lParam)
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_DRAGLOOP(hwnd:%p, wParam:%p, lParam:%p)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, (void *)wParam, (void *)lParam);
        return 0;
    }

    static __inline LRESULT MSGDUMP_API
    MD_OnDragSelect(HWND hwnd, WPARAM wParam, LPARAM lParam)
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_DRAGSELECT(hwnd:%p, wParam:%p, lParam:%p)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, (void *)wParam, (void *)lParam);
        return 0;
    }

    static __inline LRESULT MSGDUMP_API
    MD_OnDragMove(HWND hwnd, WPARAM wParam, LPARAM lParam)
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_DRAGMOVE(hwnd:%p, wParam:%p, lParam:%p)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, (void *)wParam, (void *)lParam);
        return 0;
    }
#endif

static __inline HMENU MSGDUMP_API
MD_MDISetMenu(HWND hwnd, BOOL fRefresh, HMENU hmenuFrame, HMENU hmenuWindow)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MDISETMENU(hwnd:%p, fRefresh:%d, hmenuFrame:%p, hmenuWindow:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, fRefresh, (void *)hmenuFrame, (void *)hmenuWindow);
    return NULL;
}

static __inline void MSGDUMP_API
MD_OnDropFiles(HWND hwnd, HDROP hdrop)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_DROPFILES(hwnd:%p, hdrop:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hdrop);
}

static __inline void MSGDUMP_API
MD_OnCut(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_CUT(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline void MSGDUMP_API
MD_OnCopy(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_COPY(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline void MSGDUMP_API
MD_OnPaste(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_PASTE(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline void MSGDUMP_API
MD_OnClear(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_CLEAR(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline void MSGDUMP_API
MD_OnUndo(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_UNDO(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline HANDLE MSGDUMP_API
MD_OnRenderFormat(HWND hwnd, UINT fmt)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_RENDERFORMAT(hwnd:%p, fmt:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, fmt);
    return NULL;
}

static __inline void MSGDUMP_API
MD_OnRenderAllFormats(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_RENDERALLFORMATS(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline void MSGDUMP_API
MD_OnDestroyClipboard(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_DESTROYCLIPBOARD(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline void MSGDUMP_API
MD_OnDrawClipboard(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_DRAWCLIPBOARD(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline void MSGDUMP_API
MD_OnPaintClipboard(HWND hwnd, HWND hwndCBViewer, const LPPAINTSTRUCT lpPaintStruct)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_PAINTCLIPBOARD(hwnd:%p, hwndCBViewer:%p, lpPaintStruct:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hwndCBViewer, (void *)lpPaintStruct);
}

static __inline void MSGDUMP_API
MD_OnVScrollClipboard(HWND hwnd, HWND hwndCBViewer, UINT code, int pos)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_VSCROLLCLIPBOARD(hwnd:%p, hwndCBViewer:%p, code:%u, pos:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hwndCBViewer, code, pos);
}

static __inline void MSGDUMP_API
MD_OnSizeClipboard(HWND hwnd, HWND hwndCBViewer, const LPRECT lprc)
{
    TCHAR buf[MSGDUMP_MAX_RECT_TEXT];
    MSGDUMP_TPRINTF(TEXT("%sWM_SIZECLIPBOARD(hwnd:%p, hwndCBViewer:%p, lprc:%s)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hwndCBViewer,
                    MD_rect_text(buf, sizeof(buf), lprc));
}

static __inline void MSGDUMP_API
MD_OnAskCBFormatName(HWND hwnd, int cchMax, LPTSTR rgchName)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_ASKCBFORMATNAME(hwnd:%p, cchMax:%d, rgchName:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, cchMax, (void *)rgchName);
}

static __inline void MSGDUMP_API
MD_OnChangeCBChain(HWND hwnd, HWND hwndRemove, HWND hwndNext)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_CHANGECBCHAIN(hwnd:%p, hwndRemove:%p, hwndNext:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hwndRemove, (void *)hwndNext);
}

static __inline void MSGDUMP_API
MD_OnHScrollClipboard(HWND hwnd, HWND hwndCBViewer, UINT code, int pos)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_HSCROLLCLIPBOARD(hwnd:%p, hwndCBViewer:%p, code:%u, pos:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hwndCBViewer, code, pos);
}

static __inline BOOL MSGDUMP_API
MD_OnQueryNewPalette(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_QUERYNEWPALETTE(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return FALSE;
}

static __inline void MSGDUMP_API
MD_OnPaletteIsChanging(HWND hwnd, HWND hwndPaletteChange)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_PALETTEISCHANGING(hwnd:%p, hwndPaletteChange:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hwndPaletteChange);
}

static __inline void MSGDUMP_API
MD_OnPaletteChanged(HWND hwnd, HWND hwndPaletteChange)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_PALETTECHANGED(hwnd:%p, hwndPaletteChange:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hwndPaletteChange);
}

static __inline void MSGDUMP_API
MD_OnHotKey(HWND hwnd, int idHotKey, UINT fuModifiers, UINT vk)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_HOTKEY(hwnd:%p, idHotKey:%d, fuModifiers:%u, vk:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, idHotKey, fuModifiers, vk);
}

static __inline INT MSGDUMP_API
MD_OnSetHotKey(HWND hwnd, INT nCode, UINT nOptions)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_SETHOTKEY(hwnd:%p, nCode:%d, nOptions:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, nCode, nOptions);
    return 0;
}

static __inline INT MSGDUMP_API
MD_OnGetHotKey(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_GETHOTKEY(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return 0;
}

static __inline void MSGDUMP_API
MD_OnPaintIcon(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_PAINTICON(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline LRESULT MSGDUMP_API
MD_OnGetObject(HWND hwnd, WPARAM wParam, DWORD dwObjId)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_GETOBJECT(hwnd:%p, wParam:%p, dwObjId:0x%08lX)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)wParam, dwObjId);
    return 0;
}

static __inline void MSGDUMP_API
MD_OnCancelJournal(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_CANCELJOURNAL(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline void MSGDUMP_API
MD_OnInputLangChangeRequest(HWND hwnd, BOOL bFlag, HKL hKL)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_INPUTLANGCHANGEREQUEST(hwnd:%p, bFlag:%d, hKL:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, bFlag, (void *)hKL);
}

static __inline void MSGDUMP_API
MD_OnInputLangChange(HWND hwnd, DWORD dwCharSet, HKL hKL)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_INPUTLANGCHANGE(hwnd:%p, dwCharSet:0x%08lX, hKL:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, dwCharSet, (void *)hKL);
}

static __inline void MSGDUMP_API
MD_OnTCard(HWND hwnd, UINT idAction, DWORD dwActionData)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_TCARD(hwnd:%p, idAction:%u, dwActionData:0x%08lX)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, idAction, dwActionData);
}

static __inline void MSGDUMP_API
MD_OnHelp(HWND hwnd, LPHELPINFO lpHelpInfo)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_HELP(hwnd:%p, lpHelpInfo:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)lpHelpInfo);
}

static __inline void MSGDUMP_API
MD_OnUserChanged(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_USERCHANGED(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline INT MSGDUMP_API
MD_OnNotifyFormat(HWND hwnd, HWND hwndTarget, INT nCommand)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_NOTIFYFORMAT(hwnd:%p, hwndTarget:%p, nCommand:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hwndTarget, nCommand);
    return 0;
}

static __inline void MSGDUMP_API
MD_OnStyleChanging(HWND hwnd, UINT nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_STYLECHANGING(hwnd:%p, nStyleType:%u, lpStyleStruct:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, nStyleType, (void *)lpStyleStruct);
}

static __inline void MSGDUMP_API
MD_OnStyleChanged(HWND hwnd, UINT nStyleType, const STYLESTRUCT *lpStyleStruct)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_STYLECHANGED(hwnd:%p, nStyleType:%u, lpStyleStruct:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, nStyleType, (void *)lpStyleStruct);
}

static __inline HICON MSGDUMP_API
MD_OnGetIcon(HWND hwnd, UINT nType, LPARAM dpi)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_GETICON(hwnd:%p, nType:%u, dpi:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, nType, (void *)dpi);
    return NULL;
}

static __inline HICON MSGDUMP_API
MD_OnSetIcon(HWND hwnd, UINT nType, HICON hIcon)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_SETICON(hwnd:%p, nType:%u, hIcon:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, nType, (void *)hIcon);
    return NULL;
}

static __inline void MSGDUMP_API
MD_OnSyncPaint(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_SYNCPAINT(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline void MSGDUMP_API
MD_OnNCXButtonDown(HWND hwnd, BOOL fDoubleClick, UINT nHitTest, WORD fwButton,
                   INT xPos, INT yPos)
{
    if (fDoubleClick)
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_NCXBUTTONDBLCLK(hwnd:%p, nHitTest:%u, fwButton:%u, xPos:%d, yPos:%d)\n"),
                       MSGDUMP_PREFIX, (void *)hwnd, nHitTest, fwButton, xPos, yPos);
    }
    else
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_NCXBUTTONDOWN(hwnd:%p, nHitTest:%u, fwButton:%u, xPos:%d, yPos:%d)\n"),
                       MSGDUMP_PREFIX, (void *)hwnd, nHitTest, fwButton, xPos, yPos);
    }
}

static __inline void MSGDUMP_API
MD_OnNCXButtonUp(HWND hwnd, UINT nHitTest, WORD fwButton, INT xPos, INT yPos)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_NCXBUTTONUP(hwnd:%p, nHitTest:%u, fwButton:%u, xPos:%d, yPos:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, nHitTest, fwButton, xPos, yPos);
}

static __inline void MSGDUMP_API
MD_OnImeStartComposition(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_IME_STARTCOMPOSITION(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline void MSGDUMP_API
MD_OnImeEndComposition(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_IME_ENDCOMPOSITION(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline void MSGDUMP_API
MD_OnImeComposition(HWND hwnd, WORD wChar, DWORD lAttribute)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_IME_COMPOSITION(hwnd:%p, wChar:%u, lAttribute:0x%08lX)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, wChar, lAttribute);
}

static __inline void MSGDUMP_API
MD_OnMenuRButtonUp(HWND hwnd, UINT nPos, HMENU hMenu)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MENURBUTTONUP(hwnd:%p, nPos:%u, hMenu:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, nPos, (void *)hMenu);
}

static __inline UINT MSGDUMP_API
MD_OnMenuDrag(HWND hwnd, UINT nPos, HMENU hMenu)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MENUDRAG(hwnd:%p, nPos:%u, hMenu:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, nPos, (void *)hMenu);
    return 0;
}

static __inline UINT MSGDUMP_API
MD_OnMenuGetObject(HWND hwnd, MENUGETOBJECTINFO *pmgoi)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MENUGETOBJECT(hwnd:%p, pmgoi:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)pmgoi);
    return 0;
}

static __inline void MSGDUMP_API
MD_OnUninitMenuPopup(HWND hwnd, HMENU hMenu, UINT nFlags)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_UNINITMENUPOPUP(hwnd:%p, hMenu:%p, nFlags:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hMenu, nFlags);
}

static __inline void MSGDUMP_API
MD_OnMenuCommand(HWND hwnd, UINT nPos, HMENU hMenu)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MENUCOMMAND(hwnd:%p, nPos:%u, hMenu:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, nPos, (void *)hMenu);
}

static __inline void MSGDUMP_API
MD_OnChangeUIState(HWND hwnd, UINT nAction, UINT nUIElement)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_CHANGEUISTATE(hwnd:%p, nAction:%u, nUIElement:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, nAction, nUIElement);
}

static __inline void MSGDUMP_API
MD_OnUpdateUIState(HWND hwnd, UINT nAction, UINT nUIElement)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_UPDATEUISTATE(hwnd:%p, nAction:%u, nUIElement:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, nAction, nUIElement);
}

static __inline UINT MSGDUMP_API
MD_OnQueryUIState(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_QUERYUISTATE(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return 0;
}

static __inline void MSGDUMP_API
MD_OnXButtonDown(HWND hwnd, BOOL fDoubleClick, WORD fwKeys, WORD fwButton, INT xPos, INT yPos)
{
    if (fDoubleClick)
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_XBUTTONDBLCLK(hwnd:%p, fwKeys:%u, fwButton:%u, xPos:%d, yPos:%d)\n"),
                       MSGDUMP_PREFIX, (void *)hwnd, fwKeys, fwButton, xPos, yPos);
    }
    else
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_XBUTTONDOWN(hwnd:%p, fwKeys:%u, fwButton:%u, xPos:%d, yPos:%d)\n"),
                       MSGDUMP_PREFIX, (void *)hwnd, fwKeys, fwButton, xPos, yPos);
    }
}

static __inline void MSGDUMP_API
MD_OnXButtonUp(HWND hwnd, WORD fwKeys, WORD fwButton, INT xPos, INT yPos)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_XBUTTONUP(hwnd:%p, fwKeys:%u, fwButton:%u, xPos:%d, yPos:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, fwKeys, fwButton, xPos, yPos);
}

static __inline void MSGDUMP_API
MD_OnEnterMenuLoop(HWND hwnd, BOOL bIsTrackPopupMenu)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_ENTERMENULOOP(hwnd:%p, bIsTrackPopupMenu:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, bIsTrackPopupMenu);
}

static __inline void MSGDUMP_API
MD_OnExitMenuLoop(HWND hwnd, BOOL bIsTrackPopupMenu)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_EXITMENULOOP(hwnd:%p, bIsTrackPopupMenu:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, bIsTrackPopupMenu);
}

static __inline void MSGDUMP_API
MD_OnNextMenu(HWND hwnd, INT nCode, LPMDINEXTMENU lpMDINextMenu)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_NEXTMENU(hwnd:%p, nCode:%d, lpMDINextMenu:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, nCode, (void *)lpMDINextMenu);
}

static __inline void MSGDUMP_API
MD_OnSizing(HWND hwnd, UINT nSide, LPRECT lpRect)
{
    TCHAR buf[MSGDUMP_MAX_RECT_TEXT];
    MSGDUMP_TPRINTF(TEXT("%sWM_SIZING(hwnd:%p, nSide:%u, lpRect:%s)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, nSide, MD_rect_text(buf, sizeof(buf), lpRect));
}

static __inline void MSGDUMP_API
MD_OnCaptureChanged(HWND hwnd, HWND hwndNewCapture)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_CAPTURECHANGED(hwnd:%p, hwndNewCapture:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hwndNewCapture);
}

static __inline void MSGDUMP_API
MD_OnMoving(HWND hwnd, UINT nSide, LPRECT lpRect)
{
    TCHAR buf[MSGDUMP_MAX_RECT_TEXT];
    MSGDUMP_TPRINTF(TEXT("%sWM_MOVING(hwnd:%p, nSide:%u, lpRect:%s)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, nSide, MD_rect_text(buf, sizeof(buf), lpRect));
}

static __inline LRESULT MSGDUMP_API
MD_OnPowerBroadcast(HWND hwnd, UINT nPowerEvent, UINT nEventData)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_POWERBROADCAST(hwnd:%p, nPowerEvent:%u, nEventData:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, nPowerEvent, nEventData);
    return 0;
}

static __inline void MSGDUMP_API
MD_OnEnterSizeMove(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_ENTERSIZEMOVE(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline void MSGDUMP_API
MD_OnExitSizeMove(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_EXITSIZEMOVE(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline HMENU MSGDUMP_API
MD_MDIRefreshMenu(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MDIREFRESHMENU(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return NULL;
}

static __inline BOOL MSGDUMP_API
MD_OnImeSetContext(HWND hwnd, BOOL fActive, DWORD dwShow)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_IME_SETCONTEXT(hwnd:%p, fActive:%d, dwShow:0x%08lX)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, fActive, dwShow);
    return FALSE;
}

static __inline LRESULT MSGDUMP_API
MD_OnImeNotify(HWND hwnd, WPARAM wSubMessage, LPARAM lParam)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_IME_NOTIFY(hwnd:%p, wSubMessage:%p, lParam:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)wSubMessage, (void *)lParam);
    return 0;
}

static __inline LRESULT MSGDUMP_API
MD_OnImeControl(HWND hwnd, WPARAM wSubMessage, LPVOID lpData)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_IME_CONTROL(hwnd:%p, wSubMessage:%p, lpData:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)wSubMessage, (void *)lpData);
    return 0;
}

static __inline void MSGDUMP_API
MD_OnImeCompositionFull(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_IME_COMPOSITIONFULL(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline void MSGDUMP_API
MD_OnImeSelect(HWND hwnd, BOOL fSelect, HKL hKL)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_IME_SELECT(hwnd:%p, fSelect:%d, hKL:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, fSelect, (void *)hKL);
}

static __inline void MSGDUMP_API
MD_OnImeChar(HWND hwnd, WORD wCharCode, LONG lKeyData)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_IME_CHAR(hwnd:%p, wCharCode:%u, lKeyData:%ld)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, wCharCode, lKeyData);
}

static __inline LRESULT MSGDUMP_API
MD_OnImeRequest(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_IME_REQUEST(hwnd:%p, wParam:%p, lParam:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)wParam, (void *)lParam);
    return 0;
}

static __inline void MSGDUMP_API
MD_OnImeKey(HWND hwnd, BOOL fDown, UINT nVirtKey, LONG lKeyData)
{
    if (fDown)
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_IME_KEYDOWN(hwnd:%p, nVirtKey:%u, lKeyData:%ld)\n"),
                       MSGDUMP_PREFIX, (void *)hwnd, nVirtKey, lKeyData);
    }
    else
    {
        MSGDUMP_TPRINTF(TEXT("%sWM_IME_KEYUP(hwnd:%p, nVirtKey:%u, lKeyData:%ld)\n"),
                       MSGDUMP_PREFIX, (void *)hwnd, nVirtKey, lKeyData);
    }
}

static __inline void MSGDUMP_API
MD_OnMouseHover(HWND hwnd, UINT nFlags, INT xPos, INT yPos)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MOUSEHOVER(hwnd:%p, nFlags:%u, xPos:%d, yPos:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, nFlags, xPos, yPos);
}

static __inline void MSGDUMP_API
MD_OnMouseLeave(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_MOUSELEAVE(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline void MSGDUMP_API
MD_OnNCMouseHover(HWND hwnd, UINT nHitTest, INT xPos, INT yPos)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_NCMOUSEHOVER(hwnd:%p, nHitTest:%u, xPos:%d, yPos:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, nHitTest, xPos, yPos);
}

static __inline void MSGDUMP_API
MD_OnNCMouseLeave(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_NCMOUSELEAVE(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline void MSGDUMP_API
MD_OnPrint(HWND hwnd, HDC hDC, UINT uFlags)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_PRINT(hwnd:%p, hDC:%p, uFlags:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hDC, uFlags);
}

static __inline void MSGDUMP_API
MD_OnPrintClient(HWND hwnd, HDC hDC, UINT uFlags)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_PRINTCLIENT(hwnd:%p, hDC:%p, uFlags:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hDC, uFlags);
}

static __inline BOOL MSGDUMP_API
MD_OnAppCommand(HWND hwnd, HWND hwndTarget, UINT cmd, UINT nDevice, UINT nKey)
{
    MSGDUMP_TPRINTF(TEXT("%sWM_APPCOMMAND(hwnd:%p, hwndTarget:%p, cmd:%u, nDevice:%u, nKey:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hwndTarget, cmd, nDevice, nKey);
    return FALSE;
}

static __inline LONG MSGDUMP_API
MD_Edit_OnGetSel(HWND hwnd, LPDWORD lpdwStart, LPDWORD lpdwEnd)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_GETSEL(hwnd:%p, lpdwStart:%p, lpdwEnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)lpdwStart, (void *)lpdwEnd);
    return 0;
}

static __inline void MSGDUMP_API
MD_Edit_OnSetSel(HWND hwnd, INT nStart, INT nEnd)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_SETSEL(hwnd:%p, nStart:%d, nEnd:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, nStart, nEnd);
}

static __inline void MSGDUMP_API
MD_Edit_OnGetRect(HWND hwnd, LPRECT prc)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_GETRECT(hwnd:%p, prc:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)prc);
}

static __inline void MSGDUMP_API
MD_Edit_OnSetRect(HWND hwnd, LPCRECT prc)
{
    TCHAR buf[MSGDUMP_MAX_RECT_TEXT];
    MSGDUMP_TPRINTF(TEXT("%sEM_SETRECT(hwnd:%p, prc:%s)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, MD_rect_text(buf, sizeof(buf), prc));
}

static __inline void MSGDUMP_API
MD_Edit_OnSetRectNP(HWND hwnd, LPCRECT prc)
{
    TCHAR buf[MSGDUMP_MAX_RECT_TEXT];
    MSGDUMP_TPRINTF(TEXT("%sEM_SETRECTNP(hwnd:%p, prc:%s)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, MD_rect_text(buf, sizeof(buf), prc));
}

static __inline DWORD MSGDUMP_API
MD_Edit_OnScroll(HWND hwnd, INT nScroll)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_SCROLL(hwnd:%p, nScroll:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, nScroll);
    return 0;
}

static __inline BOOL MSGDUMP_API
MD_Edit_OnLineScroll(HWND hwnd, INT cxScroll, INT cyScroll)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_LINESCROLL(hwnd:%p, cxScroll:%d, cyScroll:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, cxScroll, cyScroll);
    return FALSE;
}

static __inline BOOL MSGDUMP_API
MD_Edit_OnScrollCaret(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_SCROLLCARET(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return FALSE;
}

static __inline BOOL MSGDUMP_API
MD_Edit_OnGetModify(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_GETMODIFY(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return FALSE;
}

static __inline void MSGDUMP_API
MD_Edit_OnSetModify(HWND hwnd, BOOL fModified)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_SETMODIFY(hwnd:%p, fModified:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, fModified);
}

static __inline INT MSGDUMP_API
MD_Edit_OnGetLineCount(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_GETLINECOUNT(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return 0;
}

static __inline INT MSGDUMP_API
MD_Edit_OnLineIndex(HWND hwnd, INT line)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_LINEINDEX(hwnd:%p, line:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, line);
    return 0;
}

static __inline void MSGDUMP_API
MD_Edit_OnSetHandle(HWND hwnd, HLOCAL hloc)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_SETHANDLE(hwnd:%p, hloc:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hloc);
}

static __inline HLOCAL MSGDUMP_API
MD_Edit_OnGetHandle(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_GETHANDLE(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return NULL;
}

static __inline INT MSGDUMP_API
MD_Edit_OnGetThumb(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_GETTHUMB(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return 0;
}

static __inline INT MSGDUMP_API
MD_Edit_OnLineLength(HWND hwnd, INT ich)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_LINELENGTH(hwnd:%p, ich:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, ich);
    return 0;
}

static __inline void MSGDUMP_API
MD_Edit_OnReplaceSel(HWND hwnd, BOOL fCanUndo, LPCTSTR lpszReplace)
{
    if (IsWindowUnicode(hwnd))
        MSGDUMP_TPRINTF(TEXT("%sEM_REPLACESEL(hwnd:%p, fCanUndo:%d, %ls)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, fCanUndo, (LPCWSTR)lpszReplace);
    else
        MSGDUMP_TPRINTF(TEXT("%sEM_REPLACESEL(hwnd:%p, fCanUndo:%d, %hs)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, fCanUndo, (LPCSTR)lpszReplace);
}

static __inline INT MSGDUMP_API
MD_Edit_OnGetLine(HWND hwnd, INT line, LPCTSTR lpch)
{
    if (IsWindowUnicode(hwnd))
        MSGDUMP_TPRINTF(TEXT("%sEM_GETLINE(hwnd:%p, line:%d, lpch:%ls)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, line, (LPCWSTR)lpch);
    else
        MSGDUMP_TPRINTF(TEXT("%sEM_GETLINE(hwnd:%p, line:%d, lpch:%hs)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, line, (LPCSTR)lpch);
    return 0;
}

static __inline BOOL MSGDUMP_API
MD_Edit_OnCanUndo(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_CANUNDO(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return FALSE;
}

static __inline BOOL MSGDUMP_API
MD_Edit_OnUndo(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_UNDO(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return FALSE;
}

static __inline BOOL MSGDUMP_API
MD_Edit_OnFmtLines(HWND hwnd, BOOL fAddEOL)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_UNDO(hwnd:%p, fAddEOL:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, fAddEOL);
    return FALSE;
}

static __inline INT MSGDUMP_API
MD_Edit_OnLineFromChar(HWND hwnd, INT ich)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_LINEFROMCHAR(hwnd:%p, ich:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, ich);
    return 0;
}

static __inline BOOL MSGDUMP_API
MD_Edit_OnSetTabStops(HWND hwnd, INT cTabs, LPDWORD lpdwTabs)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_SETTABSTOPS(hwnd:%p, cTabs:%d, lpdwTabs:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, cTabs, (void *)lpdwTabs);
    return FALSE;
}

static __inline void MSGDUMP_API
MD_Edit_OnSetPasswordChar(HWND hwnd, UINT ch)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_SETPASSWORDCHAR(hwnd:%p, ch:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, ch);
}

static __inline void MSGDUMP_API
MD_Edit_OnEmptyUndoBuffer(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_EMPTYUNDOBUFFER(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline INT MSGDUMP_API
MD_Edit_OnGetFirstVisibleLine(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_GETFIRSTVISIBLELINE(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return 0;
}

static __inline BOOL MSGDUMP_API
MD_Edit_OnSetReadOnly(HWND hwnd, BOOL fReadOnly)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_SETREADONLY(hwnd:%p, fReadOnly:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, fReadOnly);
    return FALSE;
}

static __inline void MSGDUMP_API
MD_Edit_OnSetWordBreakProc(HWND hwnd, EDITWORDBREAKPROC ewbprc)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_SETWORDBREAKPROC(hwnd:%p, ewbprc:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)ewbprc);
}

static __inline EDITWORDBREAKPROC MSGDUMP_API
MD_Edit_OnGetWordBreakProc(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_GETWORDBREAKPROC(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return NULL;
}

static __inline UINT MSGDUMP_API
MD_Edit_OnGetPasswordChar(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_GETPASSWORDCHAR(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return 0;
}

static __inline void MSGDUMP_API
MD_Edit_OnSetMargins(HWND hwnd, UINT fwMargin, WORD wLeft, WORD wRight)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_SETMARGINS(hwnd:%p, fwMargin:%u, wLeft:%d, wRight:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, fwMargin, wLeft, wRight);
}

static __inline DWORD MSGDUMP_API
MD_Edit_OnGetMargins(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_GETMARGINS(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return 0;
}

static __inline void MSGDUMP_API
MD_Edit_OnSetLimitText(HWND hwnd, DWORD cbMax)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_SETLIMITTEXT(hwnd:%p, cbMax:%ld)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, cbMax);
}

static __inline DWORD MSGDUMP_API
MD_Edit_OnGetLimitText(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_GETLIMITTEXT(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return 0;
}

static __inline void MSGDUMP_API
MD_Edit_OnPosFromChar(HWND hwnd, LPPOINT lpPoint, UINT wCharIndex)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_POSFROMCHAR(hwnd:%p, lpPoint:%p, wCharIndex:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)lpPoint, wCharIndex);
}

static __inline LONG MSGDUMP_API
MD_Edit_OnCharFromPos(HWND hwnd, INT x, INT y)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_CHARFROMPOS(hwnd:%p, x:%d, y:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, x, y);
    return 0;
}

static __inline DWORD MSGDUMP_API
MD_Edit_OnSetIMEStatus(HWND hwnd, UINT uType, DWORD dwFlags)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_SETIMESTATUS(hwnd:%p, uType:%u, dwFlags:0x%08lX)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, uType, dwFlags);
    return 0;
}

static __inline DWORD MSGDUMP_API
MD_Edit_OnGetIMEStatus(HWND hwnd, UINT uType)
{
    MSGDUMP_TPRINTF(TEXT("%sEM_SETIMESTATUS(hwnd:%p, uType:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, uType);
    return 0;
}

static __inline HICON MSGDUMP_API
MD_Static_OnSetIcon(HWND hwnd, HICON hIcon)
{
    MSGDUMP_TPRINTF(TEXT("%sSTM_SETICON(hwnd:%p, hIcon:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)hIcon);
    return NULL;
}

static __inline HICON MSGDUMP_API
MD_Static_OnGetIcon(HWND hwnd)
{

    MSGDUMP_TPRINTF(TEXT("%sSTM_SETICON(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return NULL;
}

static __inline HANDLE MSGDUMP_API
MD_Static_OnSetImage(HWND hwnd, UINT fImageType, HANDLE hImage)
{
    MSGDUMP_TPRINTF(TEXT("%sSTM_SETIMAGE(hwnd:%p, fImageType:%u, hImage:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, fImageType, (void *)hImage);
    return NULL;
}

static __inline HANDLE MSGDUMP_API
MD_Static_OnGetImage(HWND hwnd, UINT fImageType)
{
    MSGDUMP_TPRINTF(TEXT("%sSTM_GETIMAGE(hwnd:%p, fImageType:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, fImageType);
    return NULL;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnAddString(HWND hwnd, LPCTSTR lpsz)
{
    if (IsWindowUnicode(hwnd))
        MSGDUMP_TPRINTF(TEXT("%sLB_ADDSTRING(hwnd:%p, lpsz:%ls)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, (LPCWSTR)lpsz);
    else
        MSGDUMP_TPRINTF(TEXT("%sLB_ADDSTRING(hwnd:%p, lpsz:%hs)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, (LPCSTR)lpsz);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnInsertString(HWND hwnd, INT index, LPCTSTR lpsz)
{
    if (IsWindowUnicode(hwnd))
        MSGDUMP_TPRINTF(TEXT("%sLB_INSERTSTRING(hwnd:%p, index:%d, lpsz:%ls)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, index, (LPCWSTR)lpsz);
    else
        MSGDUMP_TPRINTF(TEXT("%sLB_INSERTSTRING(hwnd:%p, index:%d, lpsz:%hs)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, index, (LPCSTR)lpsz);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnDeleteString(HWND hwnd, INT index)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_DELETESTRING(hwnd:%p, index:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, index);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnSelItemRangeEx(HWND hwnd, UINT wFirst, UINT wLast)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_SELITEMRANGEEX(hwnd:%p, wFirst:%u, wLast:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, wFirst, wLast);
    return 0;
}

static __inline void MSGDUMP_API
MD_ListBox_OnResetContent(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_RESETCONTENT(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline INT MSGDUMP_API
MD_ListBox_OnSetSel(HWND hwnd, BOOL fSelect, UINT index)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_SETSEL(hwnd:%p, fSelect:%d, index:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, fSelect, index);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnSetCurSel(HWND hwnd, INT index)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_SETCURSEL(hwnd:%p, index:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, index);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnGetSel(HWND hwnd, INT index)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_GETSEL(hwnd:%p, index:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, index);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnGetCurSel(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_GETCURSEL(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnGetText(HWND hwnd, INT index, LPTSTR lpszBuffer)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_GETTEXT(hwnd:%p, lpszBuffer:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)lpszBuffer);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnGetTextLen(HWND hwnd, INT index)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_GETTEXTLEN(hwnd:%p, index:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, index);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnGetCount(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_GETCOUNT(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnSelectString(HWND hwnd, INT indexStart, LPCTSTR lpszFind)
{
    if (IsWindowUnicode(hwnd))
        MSGDUMP_TPRINTF(TEXT("%sLB_SELECTSTRING(hwnd:%p, indexStart:%d, lpszFind:%ls)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, indexStart, (LPCWSTR)lpszFind);
    else
        MSGDUMP_TPRINTF(TEXT("%sLB_SELECTSTRING(hwnd:%p, indexStart:%d, lpszFind:%hs)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, indexStart, (LPCSTR)lpszFind);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnDir(HWND hwnd, UINT uAttrs, LPCTSTR lpszFileSpec)
{
    if (IsWindowUnicode(hwnd))
        MSGDUMP_TPRINTF(TEXT("%sLB_DIR(hwnd:%p, uAttrs:%u, lpszFileSpec:%ls)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, uAttrs, (LPCWSTR)lpszFileSpec);
    else
        MSGDUMP_TPRINTF(TEXT("%sLB_DIR(hwnd:%p, uAttrs:%u, lpszFileSpec:%hs)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, uAttrs, (LPCSTR)lpszFileSpec);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnGetTopIndex(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_DIR(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnFindString(HWND hwnd, INT indexStart, LPCTSTR lpszFind)
{
    if (IsWindowUnicode(hwnd))
        MSGDUMP_TPRINTF(TEXT("%sLB_FINDSTRING(hwnd:%p, indexStart:%d, lpszFind:%ls)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, indexStart, (LPCWSTR)lpszFind);
    else
        MSGDUMP_TPRINTF(TEXT("%sLB_FINDSTRING(hwnd:%p, indexStart:%d, lpszFind:%ls)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, indexStart, (LPCSTR)lpszFind);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnGetSelCount(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_GETSELCOUNT(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnGetSelItems(HWND hwnd, UINT cItems, LPINT lpnItems)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_GETSELITEMS(hwnd:%p, cItems:%u, lpnItems:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, cItems, (void *)lpnItems);
    return 0;
}

static __inline BOOL MSGDUMP_API
MD_ListBox_OnSetTabStops(HWND hwnd, UINT cTabs, LPINT lpnTabs)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_SETTABSTOPS(hwnd:%p, cTabs:%u, lpnTabs:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, cTabs, (void *)lpnTabs);
    return FALSE;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnGetHorizontalExtent(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_GETHORIZONTALEXTENT(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return 0;
}

static __inline void MSGDUMP_API
MD_ListBox_OnSetHorizontalExtent(HWND hwnd, INT cxExtent)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_SETHORIZONTALEXTENT(hwnd:%p, cxExtent:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, cxExtent);
}

static __inline void MSGDUMP_API
MD_ListBox_OnSetColumnWidth(HWND hwnd, INT cxColumn)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_SETCOLUMNWIDTH(hwnd:%p, cxColumn:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, cxColumn);
}

static __inline INT MSGDUMP_API
MD_ListBox_OnAddFile(HWND hwnd, LPCTSTR lpszFilename)
{
    if (IsWindowUnicode(hwnd))
        MSGDUMP_TPRINTF(TEXT("%sLB_ADDFILE(hwnd:%p, lpszFilename:%ls)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, (LPCWSTR)lpszFilename);
    else
        MSGDUMP_TPRINTF(TEXT("%sLB_ADDFILE(hwnd:%p, lpszFilename:%ls)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, (LPCSTR)lpszFilename);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnSetTopIndex(HWND hwnd, INT index)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_SETTOPINDEX(hwnd:%p, index:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, index);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnGetItemRect(HWND hwnd, INT index, RECT FAR *lprc)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_GETITEMRECT(hwnd:%p, index:%d, lprc:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, index, (void *)lprc);
    return 0;
}

static __inline INT_PTR MSGDUMP_API
MD_ListBox_OnGetItemData(HWND hwnd, INT index)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_GETITEMDATA(hwnd:%p, index:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, index);
    return 0;
}

static __inline INT_PTR MSGDUMP_API
MD_ListBox_OnSetItemData(HWND hwnd, INT index, LPARAM dwData)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_SETITEMDATA(hwnd:%p, index:%d, dwData:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, index, (void *)dwData);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnSelItemRange(HWND hwnd, BOOL fSelect, UINT wFirst, UINT wLast)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_SELITEMRANGE(hwnd:%p, fSelect:%d, wFirst:%u, wLast:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, fSelect, wFirst, wLast);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnSetAnchorIndex(HWND hwnd, INT index)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_SETANCHORINDEX(hwnd:%p, index:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, index);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnGetAnchorIndex(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_GETANCHORINDEX(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnSetCaretIndex(HWND hwnd, INT index, BOOL fScroll)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_SETCARETINDEX(hwnd:%p, index:%d, fScroll:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, index, fScroll);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnGetCaretIndex(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_GETCARETINDEX(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnSetItemHeight(HWND hwnd, INT index, INT cyItem)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_SETITEMHEIGHT(hwnd:%p, index:%d, cyItem:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, index, cyItem);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnGetItemHeight(HWND hwnd, INT index)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_GETITEMHEIGHT(hwnd:%p, index:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, index);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnFindStringExact(HWND hwnd, INT indexStart, LPCTSTR lpszFind)
{
    if (IsWindowUnicode(hwnd))
        MSGDUMP_TPRINTF(TEXT("%sLB_FINDSTRINGEXACT(hwnd:%p, indexStart:%d, lpszFind:%ls)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, indexStart, (LPCWSTR)lpszFind);
    else
        MSGDUMP_TPRINTF(TEXT("%sLB_FINDSTRINGEXACT(hwnd:%p, indexStart:%d, lpszFind:%hs)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, indexStart, (LPCSTR)lpszFind);
    return 0;
}

static __inline LCID MSGDUMP_API
MD_ListBox_OnSetLocale(HWND hwnd, LCID wLocaleID)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_SETLOCALE(hwnd:%p, wLocaleID:0x%08X)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, wLocaleID);
    return 0;
}

static __inline LCID MSGDUMP_API
MD_ListBox_OnGetLocale(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_GETLOCALE(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ListBox_OnSetCount(HWND hwnd, INT cItems)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_SETCOUNT(hwnd:%p, cItems:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, cItems);
    return 0;
}

static __inline DWORD MSGDUMP_API
MD_ListBox_OnInitStorage(HWND hwnd, UINT cItems, DWORD cb)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_INITSTORAGE(hwnd:%p, cItems:%d, cb:%lu)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, cItems, cb);
    return 0;
}

static __inline DWORD MSGDUMP_API
MD_ListBox_OnItemFromPoint(HWND hwnd, INT xPos, INT yPos)
{
    MSGDUMP_TPRINTF(TEXT("%sLB_ITEMFROMPOINT(hwnd:%p, xPos:%d, yPos:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, xPos, yPos);
    return 0;
}

static __inline DWORD MSGDUMP_API
MD_ComboBox_OnGetEditSel(HWND hwnd, LPDWORD lpdwStart, LPDWORD lpdwEnd)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_GETEDITSEL(hwnd:%p, lpdwStart:%p, lpdwEnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)lpdwStart, (void *)lpdwEnd);
    return 0;
}

static __inline void MSGDUMP_API
MD_ComboBox_OnLimitText(HWND hwnd, UINT cchLimit)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_LIMITTEXT(hwnd:%p, cchLimit:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, cchLimit);
}

static __inline INT MSGDUMP_API
MD_ComboBox_OnSetEditSel(HWND hwnd, INT ichStart, INT ichEnd)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_SETEDITSEL(hwnd:%p, ichStart:%d, ichEnd:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, ichStart, ichEnd);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ComboBox_OnAddString(HWND hwnd, LPCTSTR lpsz)
{
    if (IsWindowUnicode(hwnd))
        MSGDUMP_TPRINTF(TEXT("%sCB_ADDSTRING(hwnd:%p, lpsz:%ls)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, (LPCWSTR)lpsz);
    else
        MSGDUMP_TPRINTF(TEXT("%sCB_ADDSTRING(hwnd:%p, lpsz:%hs)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, (LPCSTR)lpsz);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ComboBox_OnDeleteString(HWND hwnd, INT index)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_DELETESTRING(hwnd:%p, index:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, index);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ComboBox_OnDir(HWND hwnd, UINT uAttrs, LPCTSTR lpszFileSpec)
{
    if (IsWindowUnicode(hwnd))
        MSGDUMP_TPRINTF(TEXT("%sCB_DIR(hwnd:%p, uAttrs:%u, lpszFileSpec:%ls)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, uAttrs, (LPCWSTR)lpszFileSpec);
    else
        MSGDUMP_TPRINTF(TEXT("%sCB_DIR(hwnd:%p, uAttrs:%u, lpszFileSpec:%ls)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, uAttrs, (LPCSTR)lpszFileSpec);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ComboBox_OnGetCount(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_GETCOUNT(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ComboBox_GetCurSel(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_GETCURSEL(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ComboBox_GetLBText(HWND hwnd, INT index, LPTSTR lpszBuffer)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_GETLBTEXT(hwnd:%p, index:%d, lpszBuffer:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, index, (void *)lpszBuffer);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ComboBox_OnGetLBTextLen(HWND hwnd, INT index)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_GETLBTEXTLEN(hwnd:%p, index:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, index);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ComboBox_OnInsertString(HWND hwnd, INT index, LPCTSTR lpsz)
{
    if (IsWindowUnicode(hwnd))
        MSGDUMP_TPRINTF(TEXT("%sCB_INSERTSTRING(hwnd:%p, index:%d, lpsz:%ls)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, index, (LPCWSTR)lpsz);
    else
        MSGDUMP_TPRINTF(TEXT("%sCB_INSERTSTRING(hwnd:%p, index:%d, lpsz:%hs)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, index, (LPCSTR)lpsz);
    return 0;
}

static __inline void MSGDUMP_API
MD_ComboBox_OnResetContent(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_RESETCONTENT(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
}

static __inline INT MSGDUMP_API
MD_ComboBox_OnFindString(HWND hwnd, INT indexStart, LPCTSTR lpszFind)
{
    if (IsWindowUnicode(hwnd))
        MSGDUMP_TPRINTF(TEXT("%sCB_FINDSTRING(hwnd:%p, indexStart:%d, lpszFind:%ls)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, indexStart, (LPCWSTR)lpszFind);
    else
        MSGDUMP_TPRINTF(TEXT("%sCB_FINDSTRING(hwnd:%p, indexStart:%d, lpszFind:%hs)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, indexStart, (LPCSTR)lpszFind);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ComboBox_OnSelectString(HWND hwnd, INT indexStart, LPCTSTR lpszSelect)
{
    if (IsWindowUnicode(hwnd))
        MSGDUMP_TPRINTF(TEXT("%sCB_SELECTSTRING(hwnd:%p, indexStart:%d, lpszSelect:%ls)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, indexStart, (LPCWSTR)lpszSelect);
    else
        MSGDUMP_TPRINTF(TEXT("%sCB_SELECTSTRING(hwnd:%p, indexStart:%d, lpszSelect:%hs)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, indexStart, (LPCSTR)lpszSelect);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ComboBox_OnSetCurSel(HWND hwnd, INT index)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_SETCURSEL(hwnd:%p, index:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, index);
    return 0;
}

static __inline BOOL MSGDUMP_API
MD_ComboBox_OnShowDropDown(HWND hwnd, BOOL fShow)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_SHOWDROPDOWN(hwnd:%p, fShow:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, fShow);
    return FALSE;
}

static __inline INT_PTR MSGDUMP_API
MD_ComboBox_OnGetItemData(HWND hwnd, INT index)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_GETITEMDATA(hwnd:%p, index:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, index);
    return 0;
}

static __inline INT_PTR MSGDUMP_API
MD_ComboBox_OnSetItemData(HWND hwnd, INT index, DWORD_PTR dwData)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_SETITEMDATA(hwnd:%p, index:%d, dwData:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, index, (void *)dwData);
    return 0;
}

static __inline void MSGDUMP_API
MD_ComboBox_OnGetDroppedControlRect(HWND hwnd, RECT FAR *lprc)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_GETDROPPEDCONTROLRECT(hwnd:%p, lprc:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)lprc);
}

static __inline INT MSGDUMP_API
MD_ComboBox_OnSetItemHeight(HWND hwnd, INT index, INT height)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_SETITEMHEIGHT(hwnd:%p, index:%d, height:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, index, height);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ComboBox_OnGetItemHeight(HWND hwnd, INT index)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_SETITEMHEIGHT(hwnd:%p, index:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, index);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ComboBox_OnSetExtendedUI(HWND hwnd, BOOL fExtended)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_SETEXTENDEDUI(hwnd:%p, fExtended:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, fExtended);
    return 0;
}

static __inline BOOL MSGDUMP_API
MD_ComboBox_OnGetExtendedUI(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_SETEXTENDEDUI(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return FALSE;
}

static __inline BOOL MSGDUMP_API
MD_ComboBox_OnGetDroppedState(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_GETDROPPEDSTATE(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return FALSE;
}

static __inline INT MSGDUMP_API
MD_ComboBox_OnFindStringExact(HWND hwnd, INT indexStart, LPCTSTR lpszFind)
{
    if (IsWindowUnicode(hwnd))
        MSGDUMP_TPRINTF(TEXT("%sCB_FINDSTRINGEXACT(hwnd:%p, indexStart:%d, lpszFind:%ls)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, indexStart, (LPCWSTR)lpszFind);
    else
        MSGDUMP_TPRINTF(TEXT("%sCB_FINDSTRINGEXACT(hwnd:%p, indexStart:%d, lpszFind:%hs)\n"),
                        MSGDUMP_PREFIX, (void *)hwnd, indexStart, (LPCSTR)lpszFind);
    return 0;
}

static __inline LCID MSGDUMP_API
MD_ComboBox_OnSetLocale(HWND hwnd, LCID wLocaleID)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_SETLOCALE(hwnd:%p, wLocaleID:0x%08lX)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, wLocaleID);
    return 0;
}

static __inline LCID MSGDUMP_API
MD_ComboBox_OnGetLocale(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_GETLOCALE(hwnd:%p, wLocaleID:0x%08lX)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ComboBox_OnGetTopIndex(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_GETTOPINDEX(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ComboBox_OnSetTopIndex(HWND hwnd, INT index)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_SETTOPINDEX(hwnd:%p, index:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, index);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ComboBox_OnGetHorizontalExtent(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_GETHORIZONTALEXTENT(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return 0;
}

static __inline void MSGDUMP_API
MD_ComboBox_OnSetHorizontalExtent(HWND hwnd, INT cxExtent)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_SETHORIZONTALEXTENT(hwnd:%p, cxExtent:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, cxExtent);
}

static __inline INT MSGDUMP_API
MD_ComboBox_OnGetDroppedWidth(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_GETDROPPEDWIDTH(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ComboBox_OnSetDroppedWidth(HWND hwnd, INT wWidth)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_SETDROPPEDWIDTH(hwnd:%p, wWidth:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, wWidth);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ComboBox_OnInitStorage(HWND hwnd, INT cItems, DWORD cb)
{
    MSGDUMP_TPRINTF(TEXT("%sCB_INITSTORAGE(hwnd:%p, cItems:%d, cb:%lu)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, cItems, cb);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ScrollBar_OnSetPos(HWND hwnd, INT nPos, BOOL fRedraw)
{
    MSGDUMP_TPRINTF(TEXT("%sSBM_SETPOS(hwnd:%p, nPos:%d, fRedraw:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, nPos, fRedraw);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ScrollBar_OnGetPos(HWND hwnd)
{
    MSGDUMP_TPRINTF(TEXT("%sSBM_GETPOS(hwnd:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ScrollBar_OnSetRange(HWND hwnd, INT nMinPos, INT nMaxPos)
{
    MSGDUMP_TPRINTF(TEXT("%sSBM_SETRANGE(hwnd:%p, nMinPos:%d, nMaxPos:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, nMinPos, nMaxPos);
    return 0;
}

static __inline INT MSGDUMP_API
MD_ScrollBar_OnSetRangeRedraw(HWND hwnd, INT nMinPos, INT nMaxPos)
{
    MSGDUMP_TPRINTF(TEXT("%sSBM_SETRANGEREDRAW(hwnd:%p, nMinPos:%d, nMaxPos:%d)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, nMinPos, nMaxPos);
    return 0;
}

static __inline void MSGDUMP_API
MD_ScrollBar_OnGetRange(HWND hwnd, LPINT lpnMinPos, LPINT lpnMaxPos)
{
    MSGDUMP_TPRINTF(TEXT("%sSBM_SETRANGEREDRAW(hwnd:%p, lpnMinPos:%p, lpnMaxPos:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)lpnMinPos, (void *)lpnMaxPos);
}

static __inline BOOL MSGDUMP_API
MD_ScrollBar_OnEnableArrows(HWND hwnd, UINT fuArrowFlags)
{
    MSGDUMP_TPRINTF(TEXT("%sSBM_ENABLE_ARROWS(hwnd:%p, fuArrowFlags:%u)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, fuArrowFlags);
    return FALSE;
}

static __inline INT MSGDUMP_API
MD_ScrollBar_OnSetScrollInfo(HWND hwnd, BOOL fRedraw, LPSCROLLINFO lpsi)
{
    MSGDUMP_TPRINTF(TEXT("%sSBM_SETSCROLLINFO(hwnd:%p, fRedraw:%d, lpsi:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, fRedraw, (void *)lpsi);
    return 0;
}

static __inline BOOL MSGDUMP_API
MD_ScrollBar_OnGetScrollInfo(HWND hwnd, LPSCROLLINFO lpsi)
{
    MSGDUMP_TPRINTF(TEXT("%sSBM_GETSCROLLINFO(hwnd:%p, lpsi:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)lpsi);
    return FALSE;
}

static __inline BOOL MSGDUMP_API
MD_ScrollBar_OnGetScrollBarInfo(HWND hwnd, LPSCROLLBARINFO lpsbi)
{
    MSGDUMP_TPRINTF(TEXT("%sSBM_GETSCROLLBARINFO(hwnd:%p, lpsbi:%p)\n"),
                    MSGDUMP_PREFIX, (void *)hwnd, (void *)lpsbi);
    return FALSE;
}

static __inline LRESULT MSGDUMP_API
MD_msgdump(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    TCHAR szClass[32];
    szClass[0] = 0;
    GetClassName(hwnd, szClass, ARRAYSIZE(szClass));
    if (lstrcmpi(szClass, TEXT("EDIT")) == 0)
    {
        switch (uMsg)
        {
            HANDLE_MSG(hwnd, EM_GETSEL, MD_Edit_OnGetSel);
            HANDLE_MSG(hwnd, EM_SETSEL, MD_Edit_OnSetSel);
            HANDLE_MSG(hwnd, EM_GETRECT, MD_Edit_OnGetRect);
            HANDLE_MSG(hwnd, EM_SETRECT, MD_Edit_OnSetRect);
            HANDLE_MSG(hwnd, EM_SETRECTNP, MD_Edit_OnSetRectNP);
            HANDLE_MSG(hwnd, EM_SCROLL, MD_Edit_OnScroll);
            HANDLE_MSG(hwnd, EM_LINESCROLL, MD_Edit_OnLineScroll);
            HANDLE_MSG(hwnd, EM_SCROLLCARET, MD_Edit_OnScrollCaret);
            HANDLE_MSG(hwnd, EM_GETMODIFY, MD_Edit_OnGetModify);
            HANDLE_MSG(hwnd, EM_SETMODIFY, MD_Edit_OnSetModify);
            HANDLE_MSG(hwnd, EM_GETLINECOUNT, MD_Edit_OnGetLineCount);
            HANDLE_MSG(hwnd, EM_LINEINDEX, MD_Edit_OnLineIndex);
            HANDLE_MSG(hwnd, EM_SETHANDLE, MD_Edit_OnSetHandle);
            HANDLE_MSG(hwnd, EM_GETHANDLE, MD_Edit_OnGetHandle);
            HANDLE_MSG(hwnd, EM_GETTHUMB, MD_Edit_OnGetThumb);
            HANDLE_MSG(hwnd, EM_LINELENGTH, MD_Edit_OnLineLength);
            HANDLE_MSG(hwnd, EM_REPLACESEL, MD_Edit_OnReplaceSel);
            HANDLE_MSG(hwnd, EM_GETLINE, MD_Edit_OnGetLine);
            HANDLE_MSG(hwnd, EM_CANUNDO, MD_Edit_OnCanUndo);
            HANDLE_MSG(hwnd, EM_UNDO, MD_Edit_OnUndo);
            HANDLE_MSG(hwnd, EM_FMTLINES, MD_Edit_OnFmtLines);
            HANDLE_MSG(hwnd, EM_LINEFROMCHAR, MD_Edit_OnLineFromChar);
            HANDLE_MSG(hwnd, EM_SETTABSTOPS, MD_Edit_OnSetTabStops);
            HANDLE_MSG(hwnd, EM_SETPASSWORDCHAR, MD_Edit_OnSetPasswordChar);
            HANDLE_MSG(hwnd, EM_EMPTYUNDOBUFFER, MD_Edit_OnEmptyUndoBuffer);
            HANDLE_MSG(hwnd, EM_GETFIRSTVISIBLELINE, MD_Edit_OnGetFirstVisibleLine);
            HANDLE_MSG(hwnd, EM_SETREADONLY, MD_Edit_OnSetReadOnly);
            HANDLE_MSG(hwnd, EM_SETWORDBREAKPROC, MD_Edit_OnSetWordBreakProc);
            HANDLE_MSG(hwnd, EM_GETWORDBREAKPROC, MD_Edit_OnGetWordBreakProc);
            HANDLE_MSG(hwnd, EM_GETPASSWORDCHAR, MD_Edit_OnGetPasswordChar);
            HANDLE_MSG(hwnd, EM_SETMARGINS, MD_Edit_OnSetMargins);
            HANDLE_MSG(hwnd, EM_GETMARGINS, MD_Edit_OnGetMargins);
            HANDLE_MSG(hwnd, EM_SETLIMITTEXT, MD_Edit_OnSetLimitText);
            HANDLE_MSG(hwnd, EM_GETLIMITTEXT, MD_Edit_OnGetLimitText);
            HANDLE_MSG(hwnd, EM_POSFROMCHAR, MD_Edit_OnPosFromChar);
            HANDLE_MSG(hwnd, EM_CHARFROMPOS, MD_Edit_OnCharFromPos);
            HANDLE_MSG(hwnd, EM_SETIMESTATUS, MD_Edit_OnSetIMEStatus);
            HANDLE_MSG(hwnd, EM_GETIMESTATUS, MD_Edit_OnGetIMEStatus);
        }
    }
    else if (lstrcmpi(szClass, TEXT("STATIC")) == 0)
    {
        switch (uMsg)
        {
            HANDLE_MSG(hwnd, STM_SETICON, MD_Static_OnSetIcon);
            HANDLE_MSG(hwnd, STM_GETICON, MD_Static_OnGetIcon);
            HANDLE_MSG(hwnd, STM_SETIMAGE, MD_Static_OnSetImage);
            HANDLE_MSG(hwnd, STM_GETIMAGE, MD_Static_OnGetImage);
        }
    }
    else if (lstrcmpi(szClass, TEXT("LISTBOX")) == 0)
    {
        switch (uMsg)
        {
            HANDLE_MSG(hwnd, LB_ADDSTRING, MD_ListBox_OnAddString);
            HANDLE_MSG(hwnd, LB_INSERTSTRING, MD_ListBox_OnInsertString);
            HANDLE_MSG(hwnd, LB_DELETESTRING, MD_ListBox_OnDeleteString);
            HANDLE_MSG(hwnd, LB_SELITEMRANGEEX, MD_ListBox_OnSelItemRangeEx);
            HANDLE_MSG(hwnd, LB_RESETCONTENT, MD_ListBox_OnResetContent);
            HANDLE_MSG(hwnd, LB_SETSEL, MD_ListBox_OnSetSel);
            HANDLE_MSG(hwnd, LB_SETCURSEL, MD_ListBox_OnSetCurSel);
            HANDLE_MSG(hwnd, LB_GETSEL, MD_ListBox_OnGetSel);
            HANDLE_MSG(hwnd, LB_GETCURSEL, MD_ListBox_OnGetCurSel);
            HANDLE_MSG(hwnd, LB_GETTEXT, MD_ListBox_OnGetText);
            HANDLE_MSG(hwnd, LB_GETTEXTLEN, MD_ListBox_OnGetTextLen);
            HANDLE_MSG(hwnd, LB_GETCOUNT, MD_ListBox_OnGetCount);
            HANDLE_MSG(hwnd, LB_SELECTSTRING, MD_ListBox_OnSelectString);
            HANDLE_MSG(hwnd, LB_DIR, MD_ListBox_OnDir);
            HANDLE_MSG(hwnd, LB_GETTOPINDEX, MD_ListBox_OnGetTopIndex);
            HANDLE_MSG(hwnd, LB_FINDSTRING, MD_ListBox_OnFindString);
            HANDLE_MSG(hwnd, LB_GETSELCOUNT, MD_ListBox_OnGetSelCount);
            HANDLE_MSG(hwnd, LB_GETSELITEMS, MD_ListBox_OnGetSelItems);
            HANDLE_MSG(hwnd, LB_SETTABSTOPS, MD_ListBox_OnSetTabStops);
            HANDLE_MSG(hwnd, LB_GETHORIZONTALEXTENT, MD_ListBox_OnGetHorizontalExtent);
            HANDLE_MSG(hwnd, LB_SETHORIZONTALEXTENT, MD_ListBox_OnSetHorizontalExtent);
            HANDLE_MSG(hwnd, LB_SETCOLUMNWIDTH, MD_ListBox_OnSetColumnWidth);
            HANDLE_MSG(hwnd, LB_ADDFILE, MD_ListBox_OnAddFile);
            HANDLE_MSG(hwnd, LB_SETTOPINDEX, MD_ListBox_OnSetTopIndex);
            HANDLE_MSG(hwnd, LB_GETITEMRECT, MD_ListBox_OnGetItemRect);
            HANDLE_MSG(hwnd, LB_GETITEMDATA, MD_ListBox_OnGetItemData);
            HANDLE_MSG(hwnd, LB_SETITEMDATA, MD_ListBox_OnSetItemData);
            HANDLE_MSG(hwnd, LB_SELITEMRANGE, MD_ListBox_OnSelItemRange);
            HANDLE_MSG(hwnd, LB_SETANCHORINDEX, MD_ListBox_OnSetAnchorIndex);
            HANDLE_MSG(hwnd, LB_GETANCHORINDEX, MD_ListBox_OnGetAnchorIndex);
            HANDLE_MSG(hwnd, LB_SETCARETINDEX, MD_ListBox_OnSetCaretIndex);
            HANDLE_MSG(hwnd, LB_GETCARETINDEX, MD_ListBox_OnGetCaretIndex);
            HANDLE_MSG(hwnd, LB_SETITEMHEIGHT, MD_ListBox_OnSetItemHeight);
            HANDLE_MSG(hwnd, LB_GETITEMHEIGHT, MD_ListBox_OnGetItemHeight);
            HANDLE_MSG(hwnd, LB_FINDSTRINGEXACT, MD_ListBox_OnFindStringExact);
            HANDLE_MSG(hwnd, LB_SETLOCALE, MD_ListBox_OnSetLocale);
            HANDLE_MSG(hwnd, LB_GETLOCALE, MD_ListBox_OnGetLocale);
            HANDLE_MSG(hwnd, LB_SETCOUNT, MD_ListBox_OnSetCount);
            HANDLE_MSG(hwnd, LB_INITSTORAGE, MD_ListBox_OnInitStorage);
            HANDLE_MSG(hwnd, LB_ITEMFROMPOINT, MD_ListBox_OnItemFromPoint);
        }
    }
    else if (lstrcmpi(szClass, TEXT("COMBOBOX")) == 0)
    {
        switch (uMsg)
        {
            HANDLE_MSG(hwnd, CB_GETEDITSEL, MD_ComboBox_OnGetEditSel);
            HANDLE_MSG(hwnd, CB_LIMITTEXT, MD_ComboBox_OnLimitText);
            HANDLE_MSG(hwnd, CB_SETEDITSEL, MD_ComboBox_OnSetEditSel);
            HANDLE_MSG(hwnd, CB_ADDSTRING, MD_ComboBox_OnAddString);
            HANDLE_MSG(hwnd, CB_DELETESTRING, MD_ComboBox_OnDeleteString);
            HANDLE_MSG(hwnd, CB_DIR, MD_ComboBox_OnDir);
            HANDLE_MSG(hwnd, CB_GETCOUNT, MD_ComboBox_OnGetCount);
            HANDLE_MSG(hwnd, CB_GETCURSEL, MD_ComboBox_GetCurSel);
            HANDLE_MSG(hwnd, CB_GETLBTEXT, MD_ComboBox_GetLBText);
            HANDLE_MSG(hwnd, CB_GETLBTEXTLEN, MD_ComboBox_OnGetLBTextLen);
            HANDLE_MSG(hwnd, CB_INSERTSTRING, MD_ComboBox_OnInsertString);
            HANDLE_MSG(hwnd, CB_RESETCONTENT, MD_ComboBox_OnResetContent);
            HANDLE_MSG(hwnd, CB_FINDSTRING, MD_ComboBox_OnFindString);
            HANDLE_MSG(hwnd, CB_SELECTSTRING, MD_ComboBox_OnSelectString);
            HANDLE_MSG(hwnd, CB_SETCURSEL, MD_ComboBox_OnSetCurSel);
            HANDLE_MSG(hwnd, CB_SHOWDROPDOWN, MD_ComboBox_OnShowDropDown);
            HANDLE_MSG(hwnd, CB_GETITEMDATA, MD_ComboBox_OnGetItemData);
            HANDLE_MSG(hwnd, CB_SETITEMDATA, MD_ComboBox_OnSetItemData);
            HANDLE_MSG(hwnd, CB_GETDROPPEDCONTROLRECT, MD_ComboBox_OnGetDroppedControlRect);
            HANDLE_MSG(hwnd, CB_SETITEMHEIGHT, MD_ComboBox_OnSetItemHeight);
            HANDLE_MSG(hwnd, CB_GETITEMHEIGHT, MD_ComboBox_OnGetItemHeight);
            HANDLE_MSG(hwnd, CB_SETEXTENDEDUI, MD_ComboBox_OnSetExtendedUI);
            HANDLE_MSG(hwnd, CB_GETEXTENDEDUI, MD_ComboBox_OnGetExtendedUI);
            HANDLE_MSG(hwnd, CB_GETDROPPEDSTATE, MD_ComboBox_OnGetDroppedState);
            HANDLE_MSG(hwnd, CB_FINDSTRINGEXACT, MD_ComboBox_OnFindStringExact);
            HANDLE_MSG(hwnd, CB_SETLOCALE, MD_ComboBox_OnSetLocale);
            HANDLE_MSG(hwnd, CB_GETLOCALE, MD_ComboBox_OnGetLocale);
            HANDLE_MSG(hwnd, CB_GETTOPINDEX, MD_ComboBox_OnGetTopIndex);
            HANDLE_MSG(hwnd, CB_SETTOPINDEX, MD_ComboBox_OnSetTopIndex);
            HANDLE_MSG(hwnd, CB_GETHORIZONTALEXTENT, MD_ComboBox_OnGetHorizontalExtent);
            HANDLE_MSG(hwnd, CB_SETHORIZONTALEXTENT, MD_ComboBox_OnSetHorizontalExtent);
            HANDLE_MSG(hwnd, CB_GETDROPPEDWIDTH, MD_ComboBox_OnGetDroppedWidth);
            HANDLE_MSG(hwnd, CB_SETDROPPEDWIDTH, MD_ComboBox_OnSetDroppedWidth);
            HANDLE_MSG(hwnd, CB_INITSTORAGE, MD_ComboBox_OnInitStorage);
        }
    }
    else if (lstrcmpi(szClass, TEXT("SCROLLBAR")) == 0)
    {
        switch (uMsg)
        {
            HANDLE_MSG(hwnd, SBM_SETPOS, MD_ScrollBar_OnSetPos);
            HANDLE_MSG(hwnd, SBM_GETPOS, MD_ScrollBar_OnGetPos);
            HANDLE_MSG(hwnd, SBM_SETRANGE, MD_ScrollBar_OnSetRange);
            HANDLE_MSG(hwnd, SBM_SETRANGEREDRAW, MD_ScrollBar_OnSetRangeRedraw);
            HANDLE_MSG(hwnd, SBM_GETRANGE, MD_ScrollBar_OnGetRange);
            HANDLE_MSG(hwnd, SBM_ENABLE_ARROWS, MD_ScrollBar_OnEnableArrows);
            HANDLE_MSG(hwnd, SBM_SETSCROLLINFO, MD_ScrollBar_OnSetScrollInfo);
            HANDLE_MSG(hwnd, SBM_GETSCROLLINFO, MD_ScrollBar_OnGetScrollInfo);
            HANDLE_MSG(hwnd, SBM_GETSCROLLBARINFO, MD_ScrollBar_OnGetScrollBarInfo);
        }
    }
    switch (uMsg)
    {
        HANDLE_MSG(hwnd, WM_NULL, MD_OnNull);
        HANDLE_MSG(hwnd, WM_CREATE, MD_OnCreate);
        HANDLE_MSG(hwnd, WM_DESTROY, MD_OnDestroy);
        HANDLE_MSG(hwnd, WM_MOVE, MD_OnMove);
        HANDLE_MSG(hwnd, WM_SIZE, MD_OnSize);
        HANDLE_MSG(hwnd, WM_ACTIVATE, MD_OnActivate);
        HANDLE_MSG(hwnd, WM_SETFOCUS, MD_OnSetFocus);
        HANDLE_MSG(hwnd, WM_KILLFOCUS, MD_OnKillFocus);
        HANDLE_MSG(hwnd, WM_ENABLE, MD_OnEnable);
        HANDLE_MSG(hwnd, WM_SETREDRAW, MD_OnSetRedraw);
        HANDLE_MSG(hwnd, WM_SETTEXT, MD_OnSetText);
        HANDLE_MSG(hwnd, WM_GETTEXT, MD_OnGetText);
        HANDLE_MSG(hwnd, WM_GETTEXTLENGTH, MD_OnGetTextLength);
        HANDLE_MSG(hwnd, WM_PAINT, MD_OnPaint);
        HANDLE_MSG(hwnd, WM_CLOSE, MD_OnClose);
#ifndef _WIN32_WCE
        HANDLE_MSG(hwnd, WM_QUERYENDSESSION, MD_OnQueryEndSession);
        HANDLE_MSG(hwnd, WM_QUERYOPEN, MD_OnQueryOpen);
        HANDLE_MSG(hwnd, WM_ENDSESSION, MD_OnEndSession);
#endif
        HANDLE_MSG(hwnd, WM_QUIT, MD_OnQuit);
        HANDLE_MSG(hwnd, WM_ERASEBKGND, MD_OnEraseBkgnd);
        HANDLE_MSG(hwnd, WM_SYSCOLORCHANGE, MD_OnSysColorChange);
        HANDLE_MSG(hwnd, WM_SHOWWINDOW, MD_OnShowWindow);
        HANDLE_MSG(hwnd, WM_WININICHANGE, MD_OnWinIniChange);
        /*HANDLE_MSG(hwnd, WM_SETTINGCHANGE, MD_OnSettingChange);*/
            /* WM_SETTINGCHANGE duplicates WM_WININICHANGE */
        HANDLE_MSG(hwnd, WM_DEVMODECHANGE, MD_OnDevModeChange);
        HANDLE_MSG(hwnd, WM_ACTIVATEAPP, MD_OnActivateApp);
        HANDLE_MSG(hwnd, WM_FONTCHANGE, MD_OnFontChange);
        HANDLE_MSG(hwnd, WM_TIMECHANGE, MD_OnTimeChange);
        HANDLE_MSG(hwnd, WM_CANCELMODE, MD_OnCancelMode);
        HANDLE_MSG(hwnd, WM_SETCURSOR, MD_OnSetCursor);
        HANDLE_MSG(hwnd, WM_MOUSEACTIVATE, MD_OnMouseActivate);
        HANDLE_MSG(hwnd, WM_CHILDACTIVATE, MD_OnChildActivate);
        HANDLE_MSG(hwnd, WM_QUEUESYNC, MD_OnQueueSync);
        HANDLE_MSG(hwnd, WM_GETMINMAXINFO, MD_OnGetMinMaxInfo);
        HANDLE_MSG(hwnd, WM_PAINTICON, MD_OnPaintIcon);
        HANDLE_MSG(hwnd, WM_ICONERASEBKGND, MD_OnIconEraseBkgnd);
        HANDLE_MSG(hwnd, WM_NEXTDLGCTL, MD_OnNextDlgCtl);
        HANDLE_MSG(hwnd, WM_SPOOLERSTATUS, MD_OnSpoolerStatus);
        HANDLE_MSG(hwnd, WM_DRAWITEM, MD_OnDrawItem);
        HANDLE_MSG(hwnd, WM_MEASUREITEM, MD_OnMeasureItem);
        HANDLE_MSG(hwnd, WM_DELETEITEM, MD_OnDeleteItem);
        HANDLE_MSG(hwnd, WM_VKEYTOITEM, MD_OnVkeyToItem);
        HANDLE_MSG(hwnd, WM_CHARTOITEM, MD_OnCharToItem);
        HANDLE_MSG(hwnd, WM_SETFONT, MD_OnSetFont);
        HANDLE_MSG(hwnd, WM_GETFONT, MD_OnGetFont);
        HANDLE_MSG(hwnd, WM_SETHOTKEY, MD_OnSetHotKey);
        HANDLE_MSG(hwnd, WM_GETHOTKEY, MD_OnGetHotKey);
        HANDLE_MSG(hwnd, WM_QUERYDRAGICON, MD_OnQueryDragIcon);
        HANDLE_MSG(hwnd, WM_COMPAREITEM, MD_OnCompareItem);
#if WINVER >= 0x0500
# ifndef _WIN32_WCE
        HANDLE_MSG(hwnd, WM_GETOBJECT, MD_OnGetObject);
# endif
#endif
        HANDLE_MSG(hwnd, WM_COMPACTING, MD_OnCompacting);
        HANDLE_MSG(hwnd, WM_COMMNOTIFY, MD_OnCommNotify);
        HANDLE_MSG(hwnd, WM_WINDOWPOSCHANGING, MD_OnWindowPosChanging);
        HANDLE_MSG(hwnd, WM_WINDOWPOSCHANGED, MD_OnWindowPosChanged);
        HANDLE_MSG(hwnd, WM_POWER, MD_OnPower);
        HANDLE_MSG(hwnd, WM_COPYDATA, MD_OnCopyData);
        HANDLE_MSG(hwnd, WM_CANCELJOURNAL, MD_OnCancelJournal);
#if WINVER >= 0x0400
        HANDLE_MSG(hwnd, WM_NOTIFY, MD_OnNotify);
        HANDLE_MSG(hwnd, WM_INPUTLANGCHANGEREQUEST, MD_OnInputLangChangeRequest);
        HANDLE_MSG(hwnd, WM_INPUTLANGCHANGE, MD_OnInputLangChange);
        HANDLE_MSG(hwnd, WM_TCARD, MD_OnTCard);
        HANDLE_MSG(hwnd, WM_HELP, MD_OnHelp);
        HANDLE_MSG(hwnd, WM_USERCHANGED, MD_OnUserChanged);
        HANDLE_MSG(hwnd, WM_NOTIFYFORMAT, MD_OnNotifyFormat);
        HANDLE_MSG(hwnd, WM_CONTEXTMENU, MD_OnContextMenu);
        HANDLE_MSG(hwnd, WM_STYLECHANGING, MD_OnStyleChanging);
        HANDLE_MSG(hwnd, WM_STYLECHANGED, MD_OnStyleChanged);
        HANDLE_MSG(hwnd, WM_DISPLAYCHANGE, MD_OnDisplayChange);
        HANDLE_MSG(hwnd, WM_GETICON, MD_OnGetIcon);
        HANDLE_MSG(hwnd, WM_SETICON, MD_OnSetIcon);
#endif
        HANDLE_MSG(hwnd, WM_NCCREATE, MD_OnNCCreate);
        HANDLE_MSG(hwnd, WM_NCDESTROY, MD_OnNCDestroy);
        HANDLE_MSG(hwnd, WM_NCCALCSIZE, MD_OnNCCalcSize);
        HANDLE_MSG(hwnd, WM_NCHITTEST, MD_OnNCHitTest);
        HANDLE_MSG(hwnd, WM_NCPAINT, MD_OnNCPaint);
        HANDLE_MSG(hwnd, WM_NCACTIVATE, MD_OnNCActivate);
        HANDLE_MSG(hwnd, WM_GETDLGCODE, MD_OnGetDlgCode);
#ifndef _WIN32_WCE
        HANDLE_MSG(hwnd, WM_SYNCPAINT, MD_OnSyncPaint);
#endif
        HANDLE_MSG(hwnd, WM_NCMOUSEMOVE, MD_OnNCMouseMove);
        HANDLE_MSG(hwnd, WM_NCLBUTTONDOWN, MD_OnNCLButtonDown);
        HANDLE_MSG(hwnd, WM_NCLBUTTONUP, MD_OnNCLButtonUp);
        HANDLE_MSG(hwnd, WM_NCLBUTTONDBLCLK, MD_OnNCLButtonDown);
        HANDLE_MSG(hwnd, WM_NCRBUTTONDOWN, MD_OnNCRButtonDown);
        HANDLE_MSG(hwnd, WM_NCRBUTTONUP, MD_OnNCRButtonUp);
        HANDLE_MSG(hwnd, WM_NCRBUTTONDBLCLK, MD_OnNCRButtonDown);
        HANDLE_MSG(hwnd, WM_NCMBUTTONDOWN, MD_OnNCMButtonDown);
        HANDLE_MSG(hwnd, WM_NCMBUTTONUP, MD_OnNCMButtonUp);
        HANDLE_MSG(hwnd, WM_NCMBUTTONDBLCLK, MD_OnNCMButtonDown);
#if _WIN32_WINNT >= 0x0500
        HANDLE_MSG(hwnd, WM_NCXBUTTONDOWN, MD_OnNCXButtonDown);
        HANDLE_MSG(hwnd, WM_NCXBUTTONUP, MD_OnNCXButtonUp);
        HANDLE_MSG(hwnd, WM_NCXBUTTONDBLCLK, MD_OnNCXButtonDown);
#endif
        HANDLE_MSG(hwnd, WM_KEYDOWN, MD_OnKey);
        HANDLE_MSG(hwnd, WM_KEYUP, MD_OnKey);
        HANDLE_MSG(hwnd, WM_CHAR, MD_OnChar);
        HANDLE_MSG(hwnd, WM_DEADCHAR, MD_OnDeadChar);
        HANDLE_MSG(hwnd, WM_SYSKEYDOWN, MD_OnSysKey);
        HANDLE_MSG(hwnd, WM_SYSKEYUP, MD_OnSysKey);
        HANDLE_MSG(hwnd, WM_SYSCHAR, MD_OnSysChar);
        HANDLE_MSG(hwnd, WM_SYSDEADCHAR, MD_OnSysDeadChar);
#if WINVER >= 0x0400
        HANDLE_MSG(hwnd, WM_IME_STARTCOMPOSITION, MD_OnImeStartComposition);
        HANDLE_MSG(hwnd, WM_IME_ENDCOMPOSITION, MD_OnImeEndComposition);
        HANDLE_MSG(hwnd, WM_IME_COMPOSITION, MD_OnImeComposition);
#endif
        HANDLE_MSG(hwnd, WM_INITDIALOG, MD_OnInitDialog);
        HANDLE_MSG(hwnd, WM_COMMAND, MD_OnCommand);
        HANDLE_MSG(hwnd, WM_SYSCOMMAND, MD_OnSysCommand);
        HANDLE_MSG(hwnd, WM_TIMER, MD_OnTimer);
        HANDLE_MSG(hwnd, WM_HSCROLL, MD_OnHScroll);
        HANDLE_MSG(hwnd, WM_VSCROLL, MD_OnVScroll);
        HANDLE_MSG(hwnd, WM_INITMENU, MD_OnInitMenu);
        HANDLE_MSG(hwnd, WM_INITMENUPOPUP, MD_OnInitMenuPopup);
        HANDLE_MSG(hwnd, WM_MENUSELECT, MD_OnMenuSelect);
        HANDLE_MSG(hwnd, WM_MENUCHAR, MD_OnMenuChar);
        HANDLE_MSG(hwnd, WM_ENTERIDLE, MD_OnEnterIdle);
#if WINVER >= 0x0500
# ifndef _WIN32_WCE
        HANDLE_MSG(hwnd, WM_MENURBUTTONUP, MD_OnMenuRButtonUp);
        HANDLE_MSG(hwnd, WM_MENUDRAG, MD_OnMenuDrag);
        HANDLE_MSG(hwnd, WM_MENUGETOBJECT, MD_OnMenuGetObject);
        HANDLE_MSG(hwnd, WM_UNINITMENUPOPUP, MD_OnUninitMenuPopup);
        HANDLE_MSG(hwnd, WM_MENUCOMMAND, MD_OnMenuCommand);
#  ifndef _WIN32_WCE
#   if _WIN32_WINNT >= 0x0500
        HANDLE_MSG(hwnd, WM_CHANGEUISTATE, MD_OnChangeUIState);
        HANDLE_MSG(hwnd, WM_UPDATEUISTATE, MD_OnUpdateUIState);
        HANDLE_MSG(hwnd, WM_QUERYUISTATE, MD_OnQueryUIState);
#   endif
#  endif
# endif
#endif
        HANDLE_MSG(hwnd, WM_CTLCOLORMSGBOX, MD_OnCtlColor);
        HANDLE_MSG(hwnd, WM_CTLCOLOREDIT, MD_OnCtlColor);
        HANDLE_MSG(hwnd, WM_CTLCOLORLISTBOX, MD_OnCtlColor);
        HANDLE_MSG(hwnd, WM_CTLCOLORBTN, MD_OnCtlColor);
        HANDLE_MSG(hwnd, WM_CTLCOLORDLG, MD_OnCtlColor);
        HANDLE_MSG(hwnd, WM_CTLCOLORSCROLLBAR, MD_OnCtlColor);
        HANDLE_MSG(hwnd, WM_CTLCOLORSTATIC, MD_OnCtlColor);
        HANDLE_MSG(hwnd, WM_MOUSEMOVE, MD_OnMouseMove);
        HANDLE_MSG(hwnd, WM_LBUTTONDOWN, MD_OnLButtonDown);
        HANDLE_MSG(hwnd, WM_LBUTTONUP, MD_OnLButtonUp);
        HANDLE_MSG(hwnd, WM_LBUTTONDBLCLK, MD_OnLButtonDown);
        HANDLE_MSG(hwnd, WM_RBUTTONDOWN, MD_OnRButtonDown);
        HANDLE_MSG(hwnd, WM_RBUTTONUP, MD_OnRButtonUp);
        HANDLE_MSG(hwnd, WM_RBUTTONDBLCLK, MD_OnRButtonDown);
        HANDLE_MSG(hwnd, WM_MBUTTONDOWN, MD_OnMButtonDown);
        HANDLE_MSG(hwnd, WM_MBUTTONUP, MD_OnMButtonUp);
        HANDLE_MSG(hwnd, WM_MBUTTONDBLCLK, MD_OnMButtonDown);
#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
        HANDLE_MSG(hwnd, WM_MOUSEWHEEL, MD_OnMouseWheel);
#endif
#if _WIN32_WINNT >= 0x0500
        HANDLE_MSG(hwnd, WM_XBUTTONDOWN, MD_OnXButtonDown);
        HANDLE_MSG(hwnd, WM_XBUTTONUP, MD_OnXButtonUp);
        HANDLE_MSG(hwnd, WM_XBUTTONDBLCLK, MD_OnXButtonDown);
#endif
        HANDLE_MSG(hwnd, WM_PARENTNOTIFY, MD_OnParentNotify);
        HANDLE_MSG(hwnd, WM_ENTERMENULOOP, MD_OnEnterMenuLoop);
        HANDLE_MSG(hwnd, WM_EXITMENULOOP, MD_OnExitMenuLoop);
#if WINVER >= 0x0400
        HANDLE_MSG(hwnd, WM_NEXTMENU, MD_OnNextMenu);
        HANDLE_MSG(hwnd, WM_SIZING, MD_OnSizing);
        HANDLE_MSG(hwnd, WM_CAPTURECHANGED, MD_OnCaptureChanged);
        HANDLE_MSG(hwnd, WM_MOVING, MD_OnMoving);
        HANDLE_MSG(hwnd, WM_POWERBROADCAST, MD_OnPowerBroadcast);
        HANDLE_MSG(hwnd, WM_DEVICECHANGE, MD_OnDeviceChange);
#endif
        HANDLE_MSG(hwnd, WM_MDICREATE, MD_MDICreate);
        HANDLE_MSG(hwnd, WM_MDIDESTROY, MD_MDIDestroy);
        HANDLE_MSG(hwnd, WM_MDIACTIVATE, MD_MDIActivate);
        HANDLE_MSG(hwnd, WM_MDIRESTORE, MD_MDIRestore);
        HANDLE_MSG(hwnd, WM_MDINEXT, MD_MDINext);
        HANDLE_MSG(hwnd, WM_MDIMAXIMIZE, MD_MDIMaximize);
        HANDLE_MSG(hwnd, WM_MDITILE, MD_MDITile);
        HANDLE_MSG(hwnd, WM_MDICASCADE, MD_MDICascade);
        HANDLE_MSG(hwnd, WM_MDIICONARRANGE, MD_MDIIconArrange);
        HANDLE_MSG(hwnd, WM_MDIGETACTIVE, MD_MDIGetActive);
#ifdef _UNDOCUSER_H
        HANDLE_MSG(hwnd, WM_DROPOBJECT, MD_OnDropObject);
        HANDLE_MSG(hwnd, WM_QUERYDROPOBJECT, MD_OnQueryDropObject);
        HANDLE_MSG(hwnd, WM_BEGINDRAG, MD_OnBeginDrag);
        HANDLE_MSG(hwnd, WM_DRAGLOOP, MD_OnDragLoop);
        HANDLE_MSG(hwnd, WM_DRAGSELECT, MD_OnDragSelect);
        HANDLE_MSG(hwnd, WM_DRAGMOVE, MD_OnDragMove);
#endif
        HANDLE_MSG(hwnd, WM_MDISETMENU, MD_MDISetMenu);
        HANDLE_MSG(hwnd, WM_ENTERSIZEMOVE, MD_OnEnterSizeMove);
        HANDLE_MSG(hwnd, WM_EXITSIZEMOVE, MD_OnExitSizeMove);
        HANDLE_MSG(hwnd, WM_DROPFILES, MD_OnDropFiles);
        HANDLE_MSG(hwnd, WM_MDIREFRESHMENU, MD_MDIRefreshMenu);
#if WINVER >= 0x0400
        HANDLE_MSG(hwnd, WM_IME_SETCONTEXT, MD_OnImeSetContext);
        HANDLE_MSG(hwnd, WM_IME_NOTIFY, MD_OnImeNotify);
        HANDLE_MSG(hwnd, WM_IME_CONTROL, MD_OnImeControl);
        HANDLE_MSG(hwnd, WM_IME_COMPOSITIONFULL, MD_OnImeCompositionFull);
        HANDLE_MSG(hwnd, WM_IME_SELECT, MD_OnImeSelect);
        HANDLE_MSG(hwnd, WM_IME_CHAR, MD_OnImeChar);
#endif
#if WINVER >= 0x0500
        HANDLE_MSG(hwnd, WM_IME_REQUEST, MD_OnImeRequest);
#endif
#if WINVER >= 0x0400
        HANDLE_MSG(hwnd, WM_IME_KEYDOWN, MD_OnImeKey);
        HANDLE_MSG(hwnd, WM_IME_KEYUP, MD_OnImeKey);
#endif
#if (_WIN32_WINNT >= 0x0400) || (WINVER >= 0x0500)
        HANDLE_MSG(hwnd, WM_MOUSEHOVER, MD_OnMouseHover);
        HANDLE_MSG(hwnd, WM_MOUSELEAVE, MD_OnMouseLeave);
#endif
#if WINVER >= 0x0500
        HANDLE_MSG(hwnd, WM_NCMOUSEHOVER, MD_OnNCMouseHover);
        HANDLE_MSG(hwnd, WM_NCMOUSELEAVE, MD_OnNCMouseLeave);
#endif
        HANDLE_MSG(hwnd, WM_CUT, MD_OnCut);
        HANDLE_MSG(hwnd, WM_COPY, MD_OnCopy);
        HANDLE_MSG(hwnd, WM_PASTE, MD_OnPaste);
        HANDLE_MSG(hwnd, WM_CLEAR, MD_OnClear);
        HANDLE_MSG(hwnd, WM_UNDO, MD_OnUndo);
        HANDLE_MSG(hwnd, WM_RENDERFORMAT, MD_OnRenderFormat);
        HANDLE_MSG(hwnd, WM_RENDERALLFORMATS, MD_OnRenderAllFormats);
        HANDLE_MSG(hwnd, WM_DESTROYCLIPBOARD, MD_OnDestroyClipboard);
        HANDLE_MSG(hwnd, WM_DRAWCLIPBOARD, MD_OnDrawClipboard);
        HANDLE_MSG(hwnd, WM_PAINTCLIPBOARD, MD_OnPaintClipboard);
        HANDLE_MSG(hwnd, WM_VSCROLLCLIPBOARD, MD_OnVScrollClipboard);
        HANDLE_MSG(hwnd, WM_SIZECLIPBOARD, MD_OnSizeClipboard);
        HANDLE_MSG(hwnd, WM_ASKCBFORMATNAME, MD_OnAskCBFormatName);
        HANDLE_MSG(hwnd, WM_CHANGECBCHAIN, MD_OnChangeCBChain);
        HANDLE_MSG(hwnd, WM_HSCROLLCLIPBOARD, MD_OnHScrollClipboard);
        HANDLE_MSG(hwnd, WM_QUERYNEWPALETTE, MD_OnQueryNewPalette);
        HANDLE_MSG(hwnd, WM_PALETTEISCHANGING, MD_OnPaletteIsChanging);
        HANDLE_MSG(hwnd, WM_PALETTECHANGED, MD_OnPaletteChanged);
        HANDLE_MSG(hwnd, WM_HOTKEY, MD_OnHotKey);
#if WINVER >= 0x0400
        HANDLE_MSG(hwnd, WM_PRINT, MD_OnPrint);
        HANDLE_MSG(hwnd, WM_PRINTCLIENT, MD_OnPrintClient);
#endif
#if _WIN32_WINNT >= 0x0500
        HANDLE_MSG(hwnd, WM_APPCOMMAND, MD_OnAppCommand);
#endif
        default:
        {
            if (WM_USER <= uMsg && uMsg <= 0x7FFF)
            {
                return MD_OnUser(hwnd, uMsg, wParam, lParam);
            }
            if (WM_APP <= uMsg && uMsg <= 0xBFFF)
            {
                return MD_OnApp(hwnd, uMsg, wParam, lParam);
            }
            return MD_OnUnknown(hwnd, uMsg, wParam, lParam);
        }
    }
    return 0;
}

static __inline LRESULT MSGDUMP_API
MD_msgresult(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT lResult)
{
#define DEFINE_RESULT(WM_) case WM_: MSGDUMP_TPRINTF(TEXT("%s") TEXT(#WM_) TEXT(": hwnd:%p, lResult:%p\n"), \
                                                     MSGDUMP_PREFIX, (void *)hwnd, lResult); break
    TCHAR szClass[32];
    szClass[0] = 0;
    GetClassName(hwnd, szClass, ARRAYSIZE(szClass));
    if (lstrcmpi(szClass, TEXT("EDIT")) == 0)
    {
        switch (uMsg)
        {
            DEFINE_RESULT(EM_GETSEL);
            DEFINE_RESULT(EM_SETSEL);
            DEFINE_RESULT(EM_GETRECT);
            DEFINE_RESULT(EM_SETRECT);
            DEFINE_RESULT(EM_SETRECTNP);
            DEFINE_RESULT(EM_SCROLL);
            DEFINE_RESULT(EM_LINESCROLL);
            DEFINE_RESULT(EM_SCROLLCARET );
            DEFINE_RESULT(EM_GETMODIFY);
            DEFINE_RESULT(EM_SETMODIFY);
            DEFINE_RESULT(EM_GETLINECOUNT);
            DEFINE_RESULT(EM_LINEINDEX);
            DEFINE_RESULT(EM_SETHANDLE);
            DEFINE_RESULT(EM_GETHANDLE);
            DEFINE_RESULT(EM_GETTHUMB);
            DEFINE_RESULT(EM_LINELENGTH);
            DEFINE_RESULT(EM_REPLACESEL);
            DEFINE_RESULT(EM_GETLINE);
            DEFINE_RESULT(EM_CANUNDO);
            DEFINE_RESULT(EM_UNDO);
            DEFINE_RESULT(EM_FMTLINES);
            DEFINE_RESULT(EM_LINEFROMCHAR);
            DEFINE_RESULT(EM_SETTABSTOPS);
            DEFINE_RESULT(EM_SETPASSWORDCHAR);
            DEFINE_RESULT(EM_EMPTYUNDOBUFFER);
            DEFINE_RESULT(EM_GETFIRSTVISIBLELINE);
            DEFINE_RESULT(EM_SETREADONLY);
            DEFINE_RESULT(EM_SETWORDBREAKPROC);
            DEFINE_RESULT(EM_GETWORDBREAKPROC);
            DEFINE_RESULT(EM_GETPASSWORDCHAR);
            DEFINE_RESULT(EM_SETMARGINS);
            DEFINE_RESULT(EM_GETMARGINS);
            DEFINE_RESULT(EM_SETLIMITTEXT);
            DEFINE_RESULT(EM_GETLIMITTEXT);
            DEFINE_RESULT(EM_POSFROMCHAR);
            DEFINE_RESULT(EM_CHARFROMPOS);
            DEFINE_RESULT(EM_SETIMESTATUS);
            DEFINE_RESULT(EM_GETIMESTATUS);
        }
    }
    else if (lstrcmpi(szClass, TEXT("STATIC")) == 0)
    {
        switch (uMsg)
        {
            DEFINE_RESULT(STM_SETICON);
            DEFINE_RESULT(STM_GETICON);
            DEFINE_RESULT(STM_SETIMAGE);
            DEFINE_RESULT(STM_GETIMAGE);
        }
    }
    else if (lstrcmpi(szClass, TEXT("LISTBOX")) == 0)
    {
        switch (uMsg)
        {
            DEFINE_RESULT(LB_ADDSTRING);
            DEFINE_RESULT(LB_INSERTSTRING);
            DEFINE_RESULT(LB_DELETESTRING);
            DEFINE_RESULT(LB_SELITEMRANGEEX);
            DEFINE_RESULT(LB_RESETCONTENT);
            DEFINE_RESULT(LB_SETSEL);
            DEFINE_RESULT(LB_SETCURSEL);
            DEFINE_RESULT(LB_GETSEL);
            DEFINE_RESULT(LB_GETCURSEL);
            DEFINE_RESULT(LB_GETTEXT);
            DEFINE_RESULT(LB_GETTEXTLEN);
            DEFINE_RESULT(LB_GETCOUNT);
            DEFINE_RESULT(LB_SELECTSTRING);
            DEFINE_RESULT(LB_DIR);
            DEFINE_RESULT(LB_GETTOPINDEX);
            DEFINE_RESULT(LB_FINDSTRING);
            DEFINE_RESULT(LB_GETSELCOUNT);
            DEFINE_RESULT(LB_GETSELITEMS);
            DEFINE_RESULT(LB_SETTABSTOPS);
            DEFINE_RESULT(LB_GETHORIZONTALEXTENT);
            DEFINE_RESULT(LB_SETHORIZONTALEXTENT);
            DEFINE_RESULT(LB_SETCOLUMNWIDTH);
            DEFINE_RESULT(LB_ADDFILE);
            DEFINE_RESULT(LB_SETTOPINDEX);
            DEFINE_RESULT(LB_GETITEMRECT);
            DEFINE_RESULT(LB_GETITEMDATA);
            DEFINE_RESULT(LB_SETITEMDATA);
            DEFINE_RESULT(LB_SELITEMRANGE);
            DEFINE_RESULT(LB_SETANCHORINDEX);
            DEFINE_RESULT(LB_GETANCHORINDEX);
            DEFINE_RESULT(LB_SETCARETINDEX);
            DEFINE_RESULT(LB_GETCARETINDEX);
            DEFINE_RESULT(LB_SETITEMHEIGHT);
            DEFINE_RESULT(LB_GETITEMHEIGHT);
            DEFINE_RESULT(LB_FINDSTRINGEXACT);
            DEFINE_RESULT(LB_SETLOCALE);
            DEFINE_RESULT(LB_GETLOCALE);
            DEFINE_RESULT(LB_SETCOUNT);
            DEFINE_RESULT(LB_INITSTORAGE);
            DEFINE_RESULT(LB_ITEMFROMPOINT);
        }
    }
    else if (lstrcmpi(szClass, TEXT("COMBOBOX")) == 0)
    {
        switch (uMsg)
        {
            DEFINE_RESULT(CB_GETEDITSEL);
            DEFINE_RESULT(CB_LIMITTEXT);
            DEFINE_RESULT(CB_SETEDITSEL);
            DEFINE_RESULT(CB_ADDSTRING);
            DEFINE_RESULT(CB_DELETESTRING);
            DEFINE_RESULT(CB_DIR);
            DEFINE_RESULT(CB_GETCOUNT);
            DEFINE_RESULT(CB_GETCURSEL);
            DEFINE_RESULT(CB_GETLBTEXT);
            DEFINE_RESULT(CB_GETLBTEXTLEN);
            DEFINE_RESULT(CB_INSERTSTRING);
            DEFINE_RESULT(CB_RESETCONTENT);
            DEFINE_RESULT(CB_FINDSTRING);
            DEFINE_RESULT(CB_SELECTSTRING);
            DEFINE_RESULT(CB_SETCURSEL);
            DEFINE_RESULT(CB_SHOWDROPDOWN);
            DEFINE_RESULT(CB_GETITEMDATA);
            DEFINE_RESULT(CB_SETITEMDATA);
            DEFINE_RESULT(CB_GETDROPPEDCONTROLRECT);
            DEFINE_RESULT(CB_SETITEMHEIGHT);
            DEFINE_RESULT(CB_GETITEMHEIGHT);
            DEFINE_RESULT(CB_SETEXTENDEDUI);
            DEFINE_RESULT(CB_GETEXTENDEDUI);
            DEFINE_RESULT(CB_GETDROPPEDSTATE);
            DEFINE_RESULT(CB_FINDSTRINGEXACT);
            DEFINE_RESULT(CB_SETLOCALE);
            DEFINE_RESULT(CB_GETLOCALE);
            DEFINE_RESULT(CB_GETTOPINDEX);
            DEFINE_RESULT(CB_SETTOPINDEX);
            DEFINE_RESULT(CB_GETHORIZONTALEXTENT);
            DEFINE_RESULT(CB_SETHORIZONTALEXTENT);
            DEFINE_RESULT(CB_GETDROPPEDWIDTH);
            DEFINE_RESULT(CB_SETDROPPEDWIDTH);
            DEFINE_RESULT(CB_INITSTORAGE);
        }
    }
    else if (lstrcmpi(szClass, TEXT("SCROLLBAR")) == 0)
    {
        switch (uMsg)
        {
            DEFINE_RESULT(SBM_SETPOS);
            DEFINE_RESULT(SBM_GETPOS);
            DEFINE_RESULT(SBM_SETRANGE);
            DEFINE_RESULT(SBM_SETRANGEREDRAW);
            DEFINE_RESULT(SBM_GETRANGE);
            DEFINE_RESULT(SBM_ENABLE_ARROWS);
            DEFINE_RESULT(SBM_SETSCROLLINFO);
            DEFINE_RESULT(SBM_GETSCROLLINFO);
            DEFINE_RESULT(SBM_GETSCROLLBARINFO);
        }
    }
    switch (uMsg)
    {
    DEFINE_RESULT(WM_NULL);
    DEFINE_RESULT(WM_CREATE);
    DEFINE_RESULT(WM_DESTROY);
    DEFINE_RESULT(WM_MOVE);
    DEFINE_RESULT(WM_SIZE);
    DEFINE_RESULT(WM_ACTIVATE);
    DEFINE_RESULT(WM_SETFOCUS);
    DEFINE_RESULT(WM_KILLFOCUS);
    DEFINE_RESULT(WM_ENABLE);
    DEFINE_RESULT(WM_SETREDRAW);
    DEFINE_RESULT(WM_SETTEXT);
    DEFINE_RESULT(WM_GETTEXT);
    DEFINE_RESULT(WM_GETTEXTLENGTH);
    DEFINE_RESULT(WM_PAINT);
    DEFINE_RESULT(WM_CLOSE);
#ifndef _WIN32_WCE
    DEFINE_RESULT(WM_QUERYENDSESSION);
    DEFINE_RESULT(WM_QUERYOPEN);
    DEFINE_RESULT(WM_ENDSESSION);
#endif
    DEFINE_RESULT(WM_QUIT);
    DEFINE_RESULT(WM_ERASEBKGND);
    DEFINE_RESULT(WM_SYSCOLORCHANGE);
    DEFINE_RESULT(WM_SHOWWINDOW);
    DEFINE_RESULT(WM_WININICHANGE);
    //DEFINE_RESULT(WM_SETTINGCHANGE);    // same as WM_WININICHANGE
    DEFINE_RESULT(WM_DEVMODECHANGE);
    DEFINE_RESULT(WM_ACTIVATEAPP);
    DEFINE_RESULT(WM_FONTCHANGE);
    DEFINE_RESULT(WM_TIMECHANGE);
    DEFINE_RESULT(WM_CANCELMODE);
    DEFINE_RESULT(WM_SETCURSOR);
    DEFINE_RESULT(WM_MOUSEACTIVATE);
    DEFINE_RESULT(WM_CHILDACTIVATE);
    DEFINE_RESULT(WM_QUEUESYNC);
    DEFINE_RESULT(WM_GETMINMAXINFO);
    DEFINE_RESULT(WM_PAINTICON);
    DEFINE_RESULT(WM_ICONERASEBKGND);
    DEFINE_RESULT(WM_NEXTDLGCTL);
    DEFINE_RESULT(WM_SPOOLERSTATUS);
    DEFINE_RESULT(WM_DRAWITEM);
    DEFINE_RESULT(WM_MEASUREITEM);
    DEFINE_RESULT(WM_DELETEITEM);
    DEFINE_RESULT(WM_VKEYTOITEM);
    DEFINE_RESULT(WM_CHARTOITEM);
    DEFINE_RESULT(WM_SETFONT);
    DEFINE_RESULT(WM_GETFONT);
    DEFINE_RESULT(WM_SETHOTKEY);
    DEFINE_RESULT(WM_GETHOTKEY);
    DEFINE_RESULT(WM_QUERYDRAGICON);
    DEFINE_RESULT(WM_COMPAREITEM);
#ifndef _WIN32_WCE
    DEFINE_RESULT(WM_GETOBJECT);
#endif
    DEFINE_RESULT(WM_COMPACTING);
    DEFINE_RESULT(WM_COMMNOTIFY);
    DEFINE_RESULT(WM_WINDOWPOSCHANGING);
    DEFINE_RESULT(WM_WINDOWPOSCHANGED);
    DEFINE_RESULT(WM_POWER);
    DEFINE_RESULT(WM_COPYDATA);
    DEFINE_RESULT(WM_CANCELJOURNAL);
    DEFINE_RESULT(WM_NOTIFY);
    DEFINE_RESULT(WM_INPUTLANGCHANGEREQUEST);
    DEFINE_RESULT(WM_INPUTLANGCHANGE);
    DEFINE_RESULT(WM_TCARD);
    DEFINE_RESULT(WM_HELP);
    DEFINE_RESULT(WM_USERCHANGED);
    DEFINE_RESULT(WM_NOTIFYFORMAT);
    DEFINE_RESULT(WM_CONTEXTMENU);
    DEFINE_RESULT(WM_STYLECHANGING);
    DEFINE_RESULT(WM_STYLECHANGED);
    DEFINE_RESULT(WM_DISPLAYCHANGE);
    DEFINE_RESULT(WM_GETICON);
    DEFINE_RESULT(WM_SETICON);
    DEFINE_RESULT(WM_NCCREATE);
    DEFINE_RESULT(WM_NCDESTROY);
    DEFINE_RESULT(WM_NCCALCSIZE);
    DEFINE_RESULT(WM_NCHITTEST);
    DEFINE_RESULT(WM_NCPAINT);
    DEFINE_RESULT(WM_NCACTIVATE);
    DEFINE_RESULT(WM_GETDLGCODE);
#ifndef _WIN32_WCE
    DEFINE_RESULT(WM_SYNCPAINT);
#endif
    DEFINE_RESULT(WM_NCMOUSEMOVE);
    DEFINE_RESULT(WM_NCLBUTTONDOWN);
    DEFINE_RESULT(WM_NCLBUTTONUP);
    DEFINE_RESULT(WM_NCLBUTTONDBLCLK);
    DEFINE_RESULT(WM_NCRBUTTONDOWN);
    DEFINE_RESULT(WM_NCRBUTTONUP);
    DEFINE_RESULT(WM_NCRBUTTONDBLCLK);
    DEFINE_RESULT(WM_NCMBUTTONDOWN);
    DEFINE_RESULT(WM_NCMBUTTONUP);
    DEFINE_RESULT(WM_NCMBUTTONDBLCLK);
    DEFINE_RESULT(WM_NCXBUTTONDOWN);
    DEFINE_RESULT(WM_NCXBUTTONUP);
    DEFINE_RESULT(WM_NCXBUTTONDBLCLK);
#ifdef WM_INPUT_DEVICE_CHANGE
    DEFINE_RESULT(WM_INPUT_DEVICE_CHANGE);
#endif
    DEFINE_RESULT(WM_INPUT);
    DEFINE_RESULT(WM_KEYDOWN);
    DEFINE_RESULT(WM_KEYUP);
    DEFINE_RESULT(WM_CHAR);
    DEFINE_RESULT(WM_DEADCHAR);
    DEFINE_RESULT(WM_SYSKEYDOWN);
    DEFINE_RESULT(WM_SYSKEYUP);
    DEFINE_RESULT(WM_SYSCHAR);
    DEFINE_RESULT(WM_SYSDEADCHAR);
    DEFINE_RESULT(WM_UNICHAR);
    DEFINE_RESULT(WM_IME_STARTCOMPOSITION);
    DEFINE_RESULT(WM_IME_ENDCOMPOSITION);
    DEFINE_RESULT(WM_IME_COMPOSITION);
    DEFINE_RESULT(WM_INITDIALOG);
    DEFINE_RESULT(WM_COMMAND);
    DEFINE_RESULT(WM_SYSCOMMAND);
    DEFINE_RESULT(WM_TIMER);
    DEFINE_RESULT(WM_HSCROLL);
    DEFINE_RESULT(WM_VSCROLL);
    DEFINE_RESULT(WM_INITMENU);
    DEFINE_RESULT(WM_INITMENUPOPUP);
    DEFINE_RESULT(WM_MENUSELECT);
#if _WIN32_WINNT >= 0x0601
    DEFINE_RESULT(WM_GESTURE);
    DEFINE_RESULT(WM_GESTURENOTIFY);
#endif
    DEFINE_RESULT(WM_MENUCHAR);
    DEFINE_RESULT(WM_ENTERIDLE);
#ifndef _WIN32_WCE
    DEFINE_RESULT(WM_MENURBUTTONUP);
    DEFINE_RESULT(WM_MENUDRAG);
    DEFINE_RESULT(WM_MENUGETOBJECT);
    DEFINE_RESULT(WM_UNINITMENUPOPUP);
    DEFINE_RESULT(WM_MENUCOMMAND);
    DEFINE_RESULT(WM_CHANGEUISTATE);
    DEFINE_RESULT(WM_UPDATEUISTATE);
    DEFINE_RESULT(WM_QUERYUISTATE);
#endif
    DEFINE_RESULT(WM_CTLCOLORMSGBOX);
    DEFINE_RESULT(WM_CTLCOLOREDIT);
    DEFINE_RESULT(WM_CTLCOLORLISTBOX);
    DEFINE_RESULT(WM_CTLCOLORBTN);
    DEFINE_RESULT(WM_CTLCOLORDLG);
    DEFINE_RESULT(WM_CTLCOLORSCROLLBAR);
    DEFINE_RESULT(WM_CTLCOLORSTATIC);
    DEFINE_RESULT(WM_MOUSEMOVE);
    DEFINE_RESULT(WM_LBUTTONDOWN);
    DEFINE_RESULT(WM_LBUTTONUP);
    DEFINE_RESULT(WM_LBUTTONDBLCLK);
    DEFINE_RESULT(WM_RBUTTONDOWN);
    DEFINE_RESULT(WM_RBUTTONUP);
    DEFINE_RESULT(WM_RBUTTONDBLCLK);
    DEFINE_RESULT(WM_MBUTTONDOWN);
    DEFINE_RESULT(WM_MBUTTONUP);
    DEFINE_RESULT(WM_MBUTTONDBLCLK);
    DEFINE_RESULT(WM_MOUSEWHEEL);
    DEFINE_RESULT(WM_XBUTTONDOWN);
    DEFINE_RESULT(WM_XBUTTONUP);
    DEFINE_RESULT(WM_XBUTTONDBLCLK);
#if _WIN32_WINNT >= 0x0600
    DEFINE_RESULT(WM_MOUSEHWHEEL);
#endif
    DEFINE_RESULT(WM_PARENTNOTIFY);
    DEFINE_RESULT(WM_ENTERMENULOOP);
    DEFINE_RESULT(WM_EXITMENULOOP);
    DEFINE_RESULT(WM_NEXTMENU);
    DEFINE_RESULT(WM_SIZING);
    DEFINE_RESULT(WM_CAPTURECHANGED);
    DEFINE_RESULT(WM_MOVING);
    DEFINE_RESULT(WM_POWERBROADCAST);
    DEFINE_RESULT(WM_DEVICECHANGE);
    DEFINE_RESULT(WM_MDICREATE);
    DEFINE_RESULT(WM_MDIDESTROY);
    DEFINE_RESULT(WM_MDIACTIVATE);
    DEFINE_RESULT(WM_MDIRESTORE);
    DEFINE_RESULT(WM_MDINEXT);
    DEFINE_RESULT(WM_MDIMAXIMIZE);
    DEFINE_RESULT(WM_MDITILE);
    DEFINE_RESULT(WM_MDICASCADE);
    DEFINE_RESULT(WM_MDIICONARRANGE);
    DEFINE_RESULT(WM_MDIGETACTIVE);
    DEFINE_RESULT(WM_MDISETMENU);
    DEFINE_RESULT(WM_ENTERSIZEMOVE);
    DEFINE_RESULT(WM_EXITSIZEMOVE);
    DEFINE_RESULT(WM_DROPFILES);
    DEFINE_RESULT(WM_MDIREFRESHMENU);
#if WINVER >= 0x0602
    DEFINE_RESULT(WM_POINTERDEVICECHANGE);
    DEFINE_RESULT(WM_POINTERDEVICEINRANGE);
    DEFINE_RESULT(WM_POINTERDEVICEOUTOFRANGE);
#endif
#if WINVER >= 0x0601
    DEFINE_RESULT(WM_TOUCH);
#endif
#if WINVER >= 0x0602
    DEFINE_RESULT(WM_NCPOINTERUPDATE);
    DEFINE_RESULT(WM_NCPOINTERDOWN);
    DEFINE_RESULT(WM_NCPOINTERUP);
    DEFINE_RESULT(WM_POINTERUPDATE);
    DEFINE_RESULT(WM_POINTERDOWN);
    DEFINE_RESULT(WM_POINTERUP);
    DEFINE_RESULT(WM_POINTERENTER);
    DEFINE_RESULT(WM_POINTERLEAVE);
    DEFINE_RESULT(WM_POINTERACTIVATE);
    DEFINE_RESULT(WM_POINTERCAPTURECHANGED);
    DEFINE_RESULT(WM_TOUCHHITTESTING);
    DEFINE_RESULT(WM_POINTERWHEEL);
    DEFINE_RESULT(WM_POINTERHWHEEL);
#endif
    DEFINE_RESULT(WM_IME_SETCONTEXT);
    DEFINE_RESULT(WM_IME_NOTIFY);
    DEFINE_RESULT(WM_IME_CONTROL);
    DEFINE_RESULT(WM_IME_COMPOSITIONFULL);
    DEFINE_RESULT(WM_IME_SELECT);
    DEFINE_RESULT(WM_IME_CHAR);
    DEFINE_RESULT(WM_IME_REQUEST);
    DEFINE_RESULT(WM_IME_KEYDOWN);
    DEFINE_RESULT(WM_IME_KEYUP);
    DEFINE_RESULT(WM_MOUSEHOVER);
    DEFINE_RESULT(WM_MOUSELEAVE);
    DEFINE_RESULT(WM_NCMOUSEHOVER);
    DEFINE_RESULT(WM_NCMOUSELEAVE);
    DEFINE_RESULT(WM_WTSSESSION_CHANGE);
#if WINVER >= 0x0601
    DEFINE_RESULT(WM_DPICHANGED);
#endif
    DEFINE_RESULT(WM_CUT);
    DEFINE_RESULT(WM_COPY);
    DEFINE_RESULT(WM_PASTE);
    DEFINE_RESULT(WM_CLEAR);
    DEFINE_RESULT(WM_UNDO);
    DEFINE_RESULT(WM_RENDERFORMAT);
    DEFINE_RESULT(WM_RENDERALLFORMATS);
    DEFINE_RESULT(WM_DESTROYCLIPBOARD);
    DEFINE_RESULT(WM_DRAWCLIPBOARD);
    DEFINE_RESULT(WM_PAINTCLIPBOARD);
    DEFINE_RESULT(WM_VSCROLLCLIPBOARD);
    DEFINE_RESULT(WM_SIZECLIPBOARD);
    DEFINE_RESULT(WM_ASKCBFORMATNAME);
    DEFINE_RESULT(WM_CHANGECBCHAIN);
    DEFINE_RESULT(WM_HSCROLLCLIPBOARD);
    DEFINE_RESULT(WM_QUERYNEWPALETTE);
    DEFINE_RESULT(WM_PALETTEISCHANGING);
    DEFINE_RESULT(WM_PALETTECHANGED);
    DEFINE_RESULT(WM_HOTKEY);
    DEFINE_RESULT(WM_PRINT);
    DEFINE_RESULT(WM_PRINTCLIENT);
    DEFINE_RESULT(WM_APPCOMMAND);
    DEFINE_RESULT(WM_THEMECHANGED);
#ifdef WM_CLIPBOARDUPDATE
    DEFINE_RESULT(WM_CLIPBOARDUPDATE);
#endif
#if _WIN32_WINNT >= 0x0600
    DEFINE_RESULT(WM_DWMCOMPOSITIONCHANGED);
    DEFINE_RESULT(WM_DWMNCRENDERINGCHANGED);
    DEFINE_RESULT(WM_DWMCOLORIZATIONCOLORCHANGED);
    DEFINE_RESULT(WM_DWMWINDOWMAXIMIZEDCHANGE);
#endif
#if _WIN32_WINNT >= 0x0601
    DEFINE_RESULT(WM_DWMSENDICONICTHUMBNAIL);
    DEFINE_RESULT(WM_DWMSENDICONICLIVEPREVIEWBITMAP);
#endif
#if WINVER >= 0x0600
    DEFINE_RESULT(WM_GETTITLEBARINFOEX);
#endif
    default:
        if (WM_USER <= uMsg && uMsg <= 0x7FFF)
        {
            MSGDUMP_TPRINTF(TEXT("%sWM_USER+%u(hwnd:%p, lResult:%p)\n"),
                            MSGDUMP_PREFIX, uMsg - WM_USER, (void *)hwnd, (void *)lResult);
        }
        else if (WM_APP <= uMsg && uMsg <= 0xBFFF)
        {
            MSGDUMP_TPRINTF(TEXT("%sWM_APP+%u(hwnd:%p, lResult:%p)\n"),
                            MSGDUMP_PREFIX, uMsg - WM_APP, (void *)hwnd, (void *)lResult);
        }
        else
        {
            MSGDUMP_TPRINTF(TEXT("%sWM_%u(hwnd:%p, lResult:%p)\n"),
                            MSGDUMP_PREFIX, uMsg, (void *)hwnd, (void *)lResult);
        }
#undef DEFINE_RESULT
    }
    return 0;
}

#endif
