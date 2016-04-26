#include <stdio.h>
#include <ctype.h>
#include <strings.h>
#include <windows.h>
#include <commctrl.h>

#include "resource.h"
#include "str_type.h"
#include "investor.h"

const char * wndClassName = "wndClass";

const int wndWidth = 350;
const int wndHeight = 250;

typedef enum INPUT_ERROR{
    INPUT_OK,
    INPUT_INVALID_NAME,
    INPUT_INVALID_SURNAME,
    INPUT_INVALID_BIRHDATE,
    INPUT_INVALID_INVESTMENT,
    INPUT_INVALID_PROJ_NUM
} INPUT_ERROR;

WNDPROC OldButtonProc;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DlgProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam);
void GetDesktopResolution(int * w, int * h);
INPUT_ERROR getInputErrCode(HWND hEdits[EDITS_NUM]);
const char * getInputErrMsg(INPUT_ERROR code);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = wndClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc)){
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    int desktopW, desktopH;
    GetDesktopResolution(&desktopW, &desktopH);

    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,
        wndClassName,
        "Investor validator",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        (desktopW - wndWidth)/2, (desktopH - wndHeight)/2 , wndWidth, wndHeight,
        NULL, NULL, hInstance, NULL);

    if(NULL == hwnd){
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);


    while(GetMessage(&Msg, NULL, 0, 0)){
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    return Msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
    HINSTANCE hInstance = GetModuleHandle(NULL);

    static HWND hEdits[EDITS_NUM];
    static const TCHAR * lebels[] = {
        L"Name:",
        L"Surname:",
        L"Bithdate:",
        L"Investment:",
        L"Projects number:"

    };

    static const char * editsText[] = {
        "Mykyta",
        "Sylchenko",
        "1998-04-07",
        "12.3",
        "4"

    };

    int w = 150, staticsW = 130, h = 25, dst = 5, x = 25, y = 25, count = 0;
    switch(msg){
    case WM_CREATE:
        for(;count < EDITS_NUM; count++){
            CreateWindowW(
                          L"static",
                          lebels[count],
                          WS_CHILD | WS_VISIBLE,
                          x, y + (h + dst)*count, staticsW, h,
                          hwnd,
                          STATIC_NAME_ID + count,
                          hInstance,
                          NULL);

            hEdits[count] = CreateWindowEx(0,
                          "edit",
                          editsText[count],
                          WS_CHILD | WS_VISIBLE | WS_BORDER,
                          x + staticsW + dst, y + (h + dst)*count, w, h,
                          hwnd,
                          EDIT_NAME_ID + count,
                          hInstance,
                          NULL);
        }
        CreateWindowW(
                      L"button",
                      L"Confirm",
                      WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
                      x + staticsW + dst, y + (h + dst)*count, w, h,
                      hwnd,
                      BUTTON_CONFIRM_ID,
                      hInstance,
                      NULL);
        break;
    case WM_COMMAND:
        switch(LOWORD(wParam)){
        case BUTTON_CONFIRM_ID:
            ;INPUT_ERROR inputErr = getInputErrCode(hEdits);
            if(inputErr == INPUT_OK){
                char buff[128];
                Investor_t inv = Investor_new();
                GetWindowText(hEdits[0], buff, sizeof(buff));
                strcpy(inv->name, buff);
                GetWindowText(hEdits[1], buff, sizeof(buff));
                strcpy(inv->surname, buff);
                GetWindowText(hEdits[2], buff, sizeof(buff));
                strcpy(inv->birthdate, buff);
                GetWindowText(hEdits[3], buff, sizeof(buff));
                inv->investment = atof(buff);
                GetWindowText(hEdits[4], buff, sizeof(buff));
                inv->projectsNumber = atoi(buff);

                COPYDATASTRUCT cd;
                cd.dwData = 0;
                cd.cbData = sizeof(Investor_s);
                cd.lpData = inv;

                HWND hwndDlg = DialogBoxParam(hInstance, "dlgName", hwnd, DlgProc, &cd);
            }else{
                MessageBox(hwnd, getInputErrMsg(inputErr), "Message box", MB_ICONEXCLAMATION | MB_OK);
            }
            break;
        }
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK DlgProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam){
    switch(msg){
    case WM_INITDIALOG:
        ;PCOPYDATASTRUCT cd = (PCOPYDATASTRUCT)lParam;
        Investor_t inv = (Investor_t)cd->lpData;

        HWND nameStatic = GetDlgItem(hwndDlg, DLG_STATIC_DELIVERED_NAME_ID);
        HWND surnameStatic = GetDlgItem(hwndDlg, DLG_STATIC_DELIVERED_SURNAME_ID);
        HWND birthdateStatic = GetDlgItem(hwndDlg, DLG_STATIC_DELIVERED_BIRTHDATE_ID);
        HWND investmentStatic = GetDlgItem(hwndDlg, DLG_STATIC_DELIVERED_INVESTMENT_ID);
        HWND prjNumStatic = GetDlgItem(hwndDlg, DLG_STATIC_DELIVERED_PROJECTS_NUMBER_ID);

        SetWindowText(nameStatic, inv->name);
        SetWindowText(surnameStatic, inv->surname);
        SetWindowText(birthdateStatic, inv->birthdate);

        char buff[64];
        sprintf(buff, "%f", inv->investment);
        SetWindowText(investmentStatic, buff);
        itoa(inv->projectsNumber, buff, 10);
        SetWindowText(prjNumStatic, buff);

        Investor_delete(inv);
        break;
    case WM_CLOSE:
        EndDialog(hwndDlg, 0);
        break;

    case WM_COMMAND:
        switch(LOWORD(wParam)){
        case DLG_BUTTON_OK_ID:
            EndDialog(hwndDlg, 0);
            break;
        }
        break;
    }
    return 0;
}

void GetDesktopResolution(int * w, int * h){
    RECT desktop;
    HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    *w = desktop.right;
    *h = desktop.bottom;
}

INPUT_ERROR getInputErrCode(HWND hEdits[EDITS_NUM]){
    char buff[128];

    GetWindowText(hEdits[0], buff, sizeof(buff));
    if(!isAlpha(buff)) return INPUT_INVALID_NAME;

    GetWindowText(hEdits[1], buff, sizeof(buff));
    if(!isAlpha(buff)) return INPUT_INVALID_SURNAME;

    GetWindowText(hEdits[2], buff, sizeof(buff));
    if(!isDate(buff)) return INPUT_INVALID_BIRHDATE;

    GetWindowText(hEdits[3], buff, sizeof(buff));
    if(!isNonNegatReal(buff)) return INPUT_INVALID_INVESTMENT;

    GetWindowText(hEdits[4], buff, sizeof(buff));
    if(!isNonNegatInteger(buff)) return INPUT_INVALID_PROJ_NUM;

    return INPUT_OK;
}

const char * getInputErrMsg(INPUT_ERROR code){
    static const char * repr[] = {
        "Ok",
        "Invalid name",
        "Invalid surname",
        "Invalid birthdate",
        "Invalid investment",
        "Invalid projects number"
    };
    return repr[code];
}
