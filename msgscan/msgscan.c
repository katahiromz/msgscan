// msgscan.c --- Windows message dumper
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>.
// This file is public domain software.
#include "targetver.h"
#include <windows.h>
#include <windowsx.h>
#include <psapi.h>
#include <shlwapi.h>
#include <strsafe.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN64
    #define VERSION_INFO "msgscan64 ver.0.3 by katahiromz"
#else
    #define VERSION_INFO "msgscan32 ver.0.3 by katahiromz"
#endif

void show_version(void)
{
    fprintf(stderr, "%s\n", VERSION_INFO);
}

void show_help(void)
{
#ifdef _WIN64
    printf("Usage: msgscan64 --handle <HWND> [output-file]\n");
    printf("Usage: msgscan64 --class \"window class\" [output-file]\n");
    printf("Usage: msgscan64 --class \"window class\" --text \"window text\" [output-file]\n");
#else
    printf("Usage: msgscan32 --handle <HWND> [output-file]\n");
    printf("Usage: msgscan32 --class \"window class\" [output-file]\n");
    printf("Usage: msgscan32 --class \"window class\" --text \"window text\" [output-file]\n");
#endif
    printf("\n");
    printf("You can stop this program by Ctrl+C.\n");
}

typedef BOOL (APIENTRY *INSTALL_PROC)(HWND hwndNotify, HWND hwndTarget);
typedef BOOL (APIENTRY *UNINSTALL_PROC)(void);

HINSTANCE g_hInstance = NULL;
HINSTANCE g_hinstDLL = NULL;
HWND g_hwndNotify = NULL;
HWND g_hwndTarget = NULL;
const char *g_filename = NULL;
FILE *g_fp = NULL;

INSTALL_PROC InstallSendProc = NULL;
INSTALL_PROC InstallSendRetProc = NULL;
INSTALL_PROC InstallSendPostProc = NULL;
UNINSTALL_PROC UninstallSendProc = NULL;
UNINSTALL_PROC UninstallSendRetProc = NULL;
UNINSTALL_PROC UninstallPostProc = NULL;

BOOL EnableProcessPriviledge(LPCTSTR pszSE_)
{
    BOOL f;
    HANDLE hProcess;
    HANDLE hToken;
    LUID luid;
    TOKEN_PRIVILEGES tp;
    
    f = FALSE;
    hProcess = GetCurrentProcess();
    if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
    {
        if (LookupPrivilegeValue(NULL, pszSE_, &luid))
        {
            tp.PrivilegeCount = 1;
            tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            tp.Privileges[0].Luid = luid;
            f = AdjustTokenPrivileges(hToken, FALSE, &tp, 0, NULL, NULL);
        }
        CloseHandle(hToken);
    }
    return f;
}

BOOL DoUnhook(HWND hwnd)
{
    if (g_hinstDLL)
    {
        UninstallSendProc();
        UninstallSendRetProc();
        UninstallPostProc();
        FreeLibrary(g_hinstDLL);
        g_hinstDLL = NULL;
    }
    return TRUE;
}

BOOL DoHook(HWND hwnd, HWND hwndTarget)
{
    TCHAR szPath[MAX_PATH];
    GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath));
    LPTSTR pch = PathFindFileName(szPath);
    if (pch && *pch)
        *pch = 0;

#ifdef _WIN64
    PathAppend(szPath, TEXT("MsgGet64.dll"));
#else
    PathAppend(szPath, TEXT("MsgGet32.dll"));
#endif
    g_hinstDLL = LoadLibrary(szPath);
    if (!g_hinstDLL)
    {
        return FALSE;
    }

    InstallSendProc = (INSTALL_PROC)GetProcAddress(g_hinstDLL, "InstallSendProc");
    InstallSendRetProc = (INSTALL_PROC)GetProcAddress(g_hinstDLL, "InstallSendRetProc");
    InstallSendPostProc = (INSTALL_PROC)GetProcAddress(g_hinstDLL, "InstallPostProc");
    UninstallSendProc = (UNINSTALL_PROC)GetProcAddress(g_hinstDLL, "UninstallSendProc");
    UninstallSendRetProc = (UNINSTALL_PROC)GetProcAddress(g_hinstDLL, "UninstallSendRetProc");
    UninstallPostProc = (UNINSTALL_PROC)GetProcAddress(g_hinstDLL, "UninstallPostProc");

    if (!InstallSendProc ||
        !InstallSendRetProc ||
        !InstallSendPostProc ||
        !UninstallSendProc ||
        !UninstallSendRetProc ||
        !UninstallPostProc)
    {
        FreeLibrary(g_hinstDLL);
        g_hinstDLL = NULL;
        return FALSE;
    }

    if (!InstallSendProc(hwnd, hwndTarget) ||
        !InstallSendRetProc(hwnd, hwndTarget) ||
        !InstallSendPostProc(hwnd, hwndTarget))
    {
        UninstallSendProc();
        UninstallSendRetProc();
        UninstallPostProc();
        FreeLibrary(g_hinstDLL);
        g_hinstDLL = NULL;
        return FALSE;
    }

    return TRUE;
}

BOOL DoData(HWND hwnd, LPTSTR psz)
{
    INT ich;

    ich = lstrlen(psz);
    if (ich && psz[ich - 1] == '\n')
    {
        psz[ich - 1] = 0;
        if ((ich - 1) && psz[ich - 2] == '\r')
        {
            psz[ich - 2] = 0;
        }
    }

    if (g_fp)
    {
        fprintf(g_fp, "%s\n", psz);
    }

    if (strstr(psz, "R: WM_NCDESTROY") == psz)
    {
        DestroyWindow(hwnd);
    }
}

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    if (!DoHook(hwnd, g_hwndTarget))
    {
        fprintf(stderr, "ERROR: Unable to hook\n");
        return FALSE;
    }

    fprintf(stderr, "Press Ctrl+C to exit.\n");
    return TRUE;
}

void OnDestroy(HWND hwnd)
{
    DoUnhook(hwnd);

    if (g_filename && g_fp)
    {
        fclose(g_fp);
        g_fp = NULL;
    }

    PostQuitMessage(0);
}

BOOL OnCopyData(HWND hwnd, HWND hwndFrom, PCOPYDATASTRUCT pcds)
{
    LPTSTR psz = (LPTSTR)pcds->lpData;
    return DoData(hwnd, psz);
}

LRESULT CALLBACK
WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
        HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
        HANDLE_MSG(hwnd, WM_COPYDATA, OnCopyData);
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

BOOL NormalizeNTPath(TCHAR *pszDest, size_t cchDestMax, LPCTSTR pszSrc)
{
    INT cchSrc = lstrlen(pszSrc);
    TCHAR szNTPath[MAX_PATH], szDrive[MAX_PATH] = TEXT("A:");
    TCHAR cDrive;

    StringCbCopy(pszDest, cchDestMax, pszSrc);

    for (cDrive = 'A'; cDrive <= 'Z'; ++cDrive)
    {
        szDrive[0] = cDrive;
        szNTPath[0] = 0;
        if (QueryDosDevice(szDrive, szNTPath, ARRAYSIZE(szNTPath)))
        {
            INT cchLen = lstrlen(szNTPath);
            if (memcmp(szNTPath, pszSrc, cchLen * sizeof(TCHAR)) == 0)
            {
                StringCchCopy(pszDest, cchDestMax, szDrive);
                StringCchCat(pszDest, cchDestMax, &pszSrc[cchLen]);
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL GetFileNameFromProcess(LPTSTR pszPath, DWORD cchPath, HANDLE hProcess)
{
    TCHAR szPath[MAX_PATH];
    BOOL ret = GetProcessImageFileName(hProcess, szPath, ARRAYSIZE(szPath));
    NormalizeNTPath(pszPath, cchPath, szPath);
    return ret;
}

BOOL GetFileNameFromPID(LPTSTR pszPath, DWORD cchPath, DWORD pid)
{
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (!hProcess)
        return FALSE;

    BOOL ret = GetFileNameFromProcess(pszPath, cchPath, hProcess);
    CloseHandle(hProcess);
    return ret;
}

typedef BOOL (WINAPI *ISWOW64PROCESS)(HANDLE, PBOOL);

BOOL IsWow64(HANDLE hProcess)
{
    HINSTANCE hKernel32 = GetModuleHandleA("kernel32");
    ISWOW64PROCESS pIsWow64Process;
    pIsWow64Process = (ISWOW64PROCESS)GetProcAddress(hKernel32, "IsWow64Process");
    if (pIsWow64Process)
    {
        BOOL ret = FALSE;
        (*pIsWow64Process)(hProcess, &ret);
        return ret;
    }
    return FALSE;
}

BOOL CheckTarget(HWND hwndTarget)
{
    TCHAR szTargetPath[MAX_PATH];
    DWORD pid = 0;
    INT bits = 0;
    DWORD dwBinType;
    HANDLE hProcess;

    GetWindowThreadProcessId(hwndTarget, &pid);

    GetFileNameFromPID(szTargetPath, ARRAYSIZE(szTargetPath), pid);

    BOOL bIsWow64 = FALSE;
    hProcess = OpenProcess(GENERIC_READ, TRUE, pid);
    if (hProcess)
    {
        if (IsWow64(hProcess))
        {
            bIsWow64 = TRUE;
        }
        CloseHandle(hProcess);
    }

    if (bIsWow64)
    {
        bits = 32;
    }
    else if (GetBinaryType(szTargetPath, &dwBinType) && dwBinType == SCS_32BIT_BINARY)
    {
        bits = 32;
    }
    else
    {
        bits = 64;
    }

#ifdef _WIN64
    if (bits == 64)
        return TRUE;
#else
    if (bits == 32)
        return TRUE;
#endif

    fprintf(stderr, "ERROR: Bits mismatched\n");
    return FALSE;
}

typedef struct FIND_WND
{
    const char *class_name;
    const char *text;
    HWND hwndFound;
} FIND_WND;

static BOOL CALLBACK
EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    char szClass[128], szText[128];
    FIND_WND *find = (FIND_WND *)lParam;

    GetClassNameA(hwnd, szClass, ARRAYSIZE(szClass));
    GetWindowTextA(hwnd, szText, ARRAYSIZE(szText));

    if (find->class_name)
    {
        if (lstrcmpiA(szClass, find->class_name) != 0)
        {
            return TRUE;
        }
    }

    if (find->text)
    {
        if (lstrcmpiA(szText, find->text) != 0)
        {
            return TRUE;
        }
    }

    find->hwndFound = hwnd;
    return FALSE;
}

BOOL
DoCreateWindow(HWND hwndTarget,
               const char *class_name,
               const char *text,
               const char *filename)
{
    WNDCLASS wc;
    HWND hwnd;
    FIND_WND find;

    if (hwndTarget)
    {
        g_hwndTarget = hwndTarget;
    }
    else
    {
        g_hwndTarget = FindWindowA(class_name, text);
        if (!g_hwndTarget)
        {
            find.class_name = class_name;
            find.text = text;
            find.hwndFound = NULL;
            EnumWindows(EnumWindowsProc, (LPARAM)&find);
            g_hwndTarget = find.hwndFound;
        }
    }

    if (!IsWindow(g_hwndTarget))
    {
        fprintf(stderr, "ERROR: invalid window handle: %p\n", (void *)g_hwndTarget);
        return FALSE;
    }

    if (!CheckTarget(g_hwndTarget))
    {
        return FALSE;
    }

    g_filename = filename;
    if (g_filename)
    {
        g_fp = fopen(g_filename, "w");
        if (!g_fp)
        {
            fprintf(stderr, "ERROR: Unable to open file '%s'\n", g_filename);
            return FALSE;
        }
    }
    else
    {
        g_fp = stdout;
    }

    ZeroMemory(&wc, sizeof(wc));
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
    wc.lpszClassName = TEXT(VERSION_INFO);
    if (!RegisterClass(&wc))
    {
        fprintf(stderr, "ERROR: RegisterClass failed\n");
        return FALSE;
    }

    hwnd = CreateWindow(wc.lpszClassName, wc.lpszClassName, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 80, 80, NULL, NULL, wc.hInstance, NULL);
    if (!hwnd)
    {
        fprintf(stderr, "ERROR: CreateWindow failed\n");
        return FALSE;
    }

    //ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    g_hwndNotify = hwnd;

    return TRUE;
}

static BOOL WINAPI HandlerRoutine(DWORD dwCtrlType)
{
    DestroyWindow(g_hwndNotify);

    DoUnhook(g_hwndNotify);

    if (g_filename && g_fp)
    {
        fclose(g_fp);
        g_fp = NULL;
    }

    return FALSE;
}

int main(int argc, char **argv)
{
    char *endptr;
    HWND hwndTarget = NULL;
    const char *class_name = NULL;
    const char *text = NULL;
    const char *filename = NULL;
    int i;
    MSG msg;

    SetConsoleCtrlHandler(HandlerRoutine, TRUE);

    g_hInstance = GetModuleHandle(NULL);
    EnableProcessPriviledge(SE_DEBUG_NAME);

    if (argc <= 1)
    {
        show_help();
        return EXIT_FAILURE;
    }

    for (i = 1; i < argc; ++i)
    {
        const char *arg = argv[i];

        if (strcmp(arg, "--help") == 0)
        {
            show_help();
            return EXIT_SUCCESS;
        }

        if (strcmp(arg, "--version") == 0)
        {
            show_version();
            return EXIT_SUCCESS;
        }

        if (strcmp(arg, "--handle") == 0)
        {
            if (i + 1 >= argc)
            {
                fprintf(stderr, "ERROR: Option '--handle' needs an operand\n");
                show_help();
                return EXIT_FAILURE;
            }

            arg = argv[++i];
            hwndTarget = (HWND)(ULONG_PTR)strtoul(arg, &endptr, 16);
            if (*endptr != 0 || !IsWindow(hwndTarget))
            {
                hwndTarget = (HWND)(ULONG_PTR)strtoul(arg, &endptr, 10);
                if (*endptr != 0 || !IsWindow(hwndTarget))
                {
                    fprintf(stderr, "ERROR: '%s' is not window handle\n", arg);
                    return EXIT_FAILURE;
                }
            }
            continue;
        }

        if (strcmp(arg, "--class") == 0)
        {
            if (i + 1 >= argc)
            {
                fprintf(stderr, "ERROR: Option '--class' needs an operand\n");
                show_help();
                return EXIT_FAILURE;
            }
            class_name = argv[++i];
            continue;
        }

        if (strcmp(arg, "--text") == 0)
        {
            if (i + 1 >= argc)
            {
                fprintf(stderr, "ERROR: Option '--text' needs an operand\n");
                show_help();
                return EXIT_FAILURE;
            }
            text = argv[++i];
            continue;
        }

        if (arg[0] == '-')
        {
            fprintf(stderr, "ERROR: Invalid option '%s'\n", arg);
            return EXIT_FAILURE;
        }

        if (!filename)
        {
            filename = arg;
        }
        else
        {
            fprintf(stderr, "ERROR: Multiple output specified\n");
            return EXIT_FAILURE;
        }
    }

    if (!DoCreateWindow(hwndTarget, class_name, text, filename))
        return EXIT_FAILURE;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return EXIT_SUCCESS;
}
