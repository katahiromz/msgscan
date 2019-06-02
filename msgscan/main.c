// main.c --- Windows message dumper
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>.
// This file is public domain software.
#include <windows.h>
#include <stdio.h>
#include "common.h"

extern FILE *g_fp;

BOOL
DoCreateWindow(HWND hwndTarget,
               const char *class_name,
               const char *text,
               const char *filename);

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

void OnTimer(HWND hwnd, UINT id)
{
    KillTimer(hwnd, id);
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

int main(int argc, char **argv)
{
    char *endptr;
    HWND hwndTarget = NULL;
    const char *class_name = NULL;
    const char *text = NULL;
    const char *filename = NULL;
    int i;
    MSG msg;

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
