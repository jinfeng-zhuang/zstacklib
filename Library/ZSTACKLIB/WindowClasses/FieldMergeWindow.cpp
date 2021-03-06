#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>

#include <stdio.h>

#include <zstack/ringbuf.h>
#include <zstack/log.h>
#include <zstack/yuv.h>
#include <zstack/platform.h>

extern int YUVWindow_Register(HINSTANCE hInstance);

static struct {
    HWND edit_fieldA;
    HWND edit_fieldB;
    CHAR content_fieldA[32];
    CHAR content_fieldB[32];
    HWND button_prev;
    HWND button_next;
    HWND yuv;
    HWND match;
} database;

struct db {
    int top;
    int bottom;
    int match;
    unsigned char *merged_buffer;
};

static unsigned int worker = 0;

// TODO
// should be here, otherwise it will be overwritten by stack, which is using in worker thread
static struct db db;
static struct YUVWindowStruct r;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HBITMAP hPaper;
    static HDC hDeck;
    static HBRUSH hBrush;
    static struct ring ringbuffer = {0, 0, 0, 0};
    
    HDC hdcWindow;
    PAINTSTRUCT ps;
    HINSTANCE hInstance;
    int do_update = 0;

    hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

    //TRACE;

    switch (uMsg)
    {
    case WM_CREATE:

        YUVWindow_Register(hInstance);

        //--------------------------------------------------------
        database.edit_fieldA = CreateWindowEx(
            0,
            TEXT("EDIT"),
            NULL,
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            0, 0, 50, 20,
            hWnd,
            NULL,
            hInstance,
            NULL);

        database.edit_fieldB = CreateWindowEx(
            0,
            TEXT("EDIT"),
            NULL,
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            50, 0, 50, 20,
            hWnd,
            NULL,
            hInstance,
            NULL);

        database.button_prev = CreateWindowEx(
            0,
            TEXT("Button"),
            TEXT("<<"),
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            100, 0, 50, 20,
            hWnd,
            NULL,
            hInstance,
            NULL);

        database.button_next = CreateWindowEx(
            0,
            TEXT("Button"),
            TEXT(">>"),
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            150, 0, 50, 20,
            hWnd,
            NULL,
            hInstance,
            NULL);

        database.match = CreateWindowEx(
            0,
            TEXT("Button"),
            TEXT("Match"),
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            250, 0, 50, 20,
            hWnd,
            NULL,
            hInstance,
            NULL);

        database.yuv = CreateWindowEx(
            0,
            TEXT("YUVWindow"),
            NULL,
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            0, 30, 720, 576,
            hWnd,
            NULL,
            hInstance,
            NULL);

        break;
    case WM_SIZE:
        //GetClientRect(hWnd, &rect);

        break;

    case WM_PAINT:
        hdcWindow = BeginPaint(hWnd, &ps);

        EndPaint(hWnd, &ps);
        break;

    case WM_COMMAND:
        GetWindowTextA(database.edit_fieldA, database.content_fieldA, sizeof(database.content_fieldA));
        GetWindowTextA(database.edit_fieldB, database.content_fieldB, sizeof(database.content_fieldB));

        db.top = atoi(database.content_fieldA);
        db.bottom = atoi(database.content_fieldB);
        db.match = 0;

        do_update = 1;

        if ((HWND)lParam == database.button_prev) {
            if (db.bottom > 0)
                db.bottom--;
            _snprintf(database.content_fieldB, sizeof(database.content_fieldB), "%d", db.bottom);
            SetWindowTextA(database.edit_fieldB, database.content_fieldB);

            do_update = 1;
        }
        else if ((HWND)lParam == database.button_next) {
            db.bottom++;
            _snprintf(database.content_fieldB, sizeof(database.content_fieldB), "%d", db.bottom);
            SetWindowTextA(database.edit_fieldB, database.content_fieldB);

            do_update = 1;
        }
        else if ((HWND)lParam == database.match) {
            db.top++;
            _snprintf(database.content_fieldA, sizeof(database.content_fieldA), "%d", db.top);
            SetWindowTextA(database.edit_fieldA, database.content_fieldA);
            db.match = 1;
        }

        if (do_update && worker) {
            PostThreadMessage(worker, WM_USER, MSG_SET_DATA, (WPARAM)&db);
        }
        break;

    case WM_USER:
        if (wParam == MSG_SET_HANDLE) {
            worker = lParam;
        }
        else if (wParam == MSG_SET_DATA) {
            memcpy(&r, (void *)lParam, sizeof(struct YUVWindowStruct));
            PostMessage(database.yuv, WM_USER, (WPARAM)&r, NULL);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

int FieldMergeWindow_Register(HINSTANCE hInstance)
{
    WNDCLASSEX wce = { 0 };

    wce.cbSize = sizeof(wce);
    wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wce.hCursor = LoadCursor(NULL, IDC_ARROW);
    wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wce.hInstance = hInstance;
    wce.lpfnWndProc = WindowProc;
    wce.lpszClassName = TEXT("FieldMergeWindow");
    wce.style = CS_HREDRAW | CS_VREDRAW;
    if (!RegisterClassEx(&wce)) {
        return 1;
    }

    return 0;
}

