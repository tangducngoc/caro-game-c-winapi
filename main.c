#include <windows.h>
#include <stdlib.h>
#include <time.h>
//code made by:TangDucNgoc(Himawari) it free to use!!
#define BOARD_SIZE 15
#define CELL_SIZE 40
#define ID_NEWGAME 1
#define ID_PVP 2
#define ID_PVAI 3
#define ID_EXIT 4

int board[BOARD_SIZE][BOARD_SIZE];
int turn = 1; // 1 = X, -1 = O
int win = 0;
int vsAI = 0;

void ResetGame() {
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            board[i][j] = 0;
    turn = 1;
    win = 0;
}

int CheckWin(int r, int c) {
    int dirs[4][2] = {{1,0},{0,1},{1,1},{1,-1}};
    for (int d=0; d<4; d++) {
        int count=1;
        for (int i=1;i<5;i++){
            int nr=r+i*dirs[d][0], nc=c+i*dirs[d][1];
            if(nr<0||nr>=BOARD_SIZE||nc<0||nc>=BOARD_SIZE) break;
            if(board[nr][nc]==board[r][c]) count++; else break;
        }
        for (int i=1;i<5;i++){
            int nr=r-i*dirs[d][0], nc=c-i*dirs[d][1];
            if(nr<0||nr>=BOARD_SIZE||nc<0||nc>=BOARD_SIZE) break;
            if(board[nr][nc]==board[r][c]) count++; else break;
        }
        if(count>=5) return 1;
    }
    return 0;
}

void AITurn(HWND hwnd) {
    int empty[BOARD_SIZE * BOARD_SIZE][2];
    int count = 0;
    for (int i=0; i<BOARD_SIZE; i++)
        for (int j=0; j<BOARD_SIZE; j++)
            if (board[i][j] == 0) {
                empty[count][0] = i;
                empty[count][1] = j;
                count++;
            }

    if (count == 0) return;

    srand(time(NULL));
    int r = rand() % count;
    board[empty[r][0]][empty[r][1]] = -1;
    if (CheckWin(empty[r][0], empty[r][1])) {
        win = -1;
        MessageBox(hwnd, "May thang!", "Ket qua", MB_OK);
    }
}

void DrawBoard(HDC hdc) {
    // Yellow background
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 180));
    RECT rect = {0, 0, BOARD_SIZE*CELL_SIZE, BOARD_SIZE*CELL_SIZE};
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);

    // Black line
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0,0,0));
    SelectObject(hdc, hPen);
    for (int i=0;i<=BOARD_SIZE;i++){
        MoveToEx(hdc, i*CELL_SIZE, 0, NULL);
        LineTo(hdc, i*CELL_SIZE, BOARD_SIZE*CELL_SIZE);
        MoveToEx(hdc, 0, i*CELL_SIZE, NULL);
        LineTo(hdc, BOARD_SIZE*CELL_SIZE, i*CELL_SIZE);
    }
    DeleteObject(hPen);

    // Font to
   HFONT hFont = CreateFont(
    24, 0, 0, 0, FW_BOLD,
    FALSE, FALSE, FALSE,
    DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
    CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
    VARIABLE_PITCH, TEXT("Arial"));

    SelectObject(hdc, hFont);

    // X (red) O(blue)
    for (int i=0;i<BOARD_SIZE;i++){
        for (int j=0;j<BOARD_SIZE;j++){
            if (board[i][j] == 1) {
                SetTextColor(hdc, RGB(255,0,0));
                TextOut(hdc, j*CELL_SIZE + 12, i*CELL_SIZE + 8, "X", 1);
            } else if (board[i][j] == -1) {
                SetTextColor(hdc, RGB(0,0,255));
                TextOut(hdc, j*CELL_SIZE + 12, i*CELL_SIZE + 8, "O", 1);
            }
        }
    }

    DeleteObject(hFont);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
            case ID_NEWGAME:
                ResetGame();
                InvalidateRect(hwnd, NULL, TRUE);
                break;
            case ID_PVP:
                vsAI = 0;
                ResetGame();
                InvalidateRect(hwnd, NULL, TRUE);
                break;
            case ID_PVAI:
                vsAI = 1;
                ResetGame();
                InvalidateRect(hwnd, NULL, TRUE);
                break;
            case ID_EXIT:
                PostQuitMessage(0);
                break;
        }
        break;

    case WM_LBUTTONDOWN:
        if (win) break;
        {
            int x = LOWORD(lParam) / CELL_SIZE;
            int y = HIWORD(lParam) / CELL_SIZE;
            if (x < BOARD_SIZE && y < BOARD_SIZE && board[y][x] == 0) {
                board[y][x] = turn;
                if (CheckWin(y, x)) {
                    win = turn;
                    if (turn == 1)
                        MessageBox(hwnd, "Nguoi choi X thang!", "Ket qua", MB_OK);
                    else
                        MessageBox(hwnd, "Nguoi choi O thang!", "Ket qua", MB_OK);
                } else {
                    turn = -turn;
                    if (vsAI && turn == -1 && !win) {
                        AITurn(hwnd);
                        turn = -turn;
                    }
                }
                InvalidateRect(hwnd, NULL, TRUE);
            }
        }
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            DrawBoard(hdc);
            EndPaint(hwnd, &ps);
        }
        break;

    case WM_CREATE:
        {
            HMENU hMenu = CreateMenu();
            HMENU hSubMenu = CreatePopupMenu();
            AppendMenu(hSubMenu, MF_STRING, ID_NEWGAME, "Choi lai");
            AppendMenu(hSubMenu, MF_STRING, ID_PVP, "Choi 2 nguoi");
            AppendMenu(hSubMenu, MF_STRING, ID_PVAI, "Choi voi may");
            AppendMenu(hSubMenu, MF_STRING, ID_EXIT, "Thoat");
            AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, "Tuy chon");
            SetMenu(hwnd, hMenu);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "CaroGame";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    HWND hwnd = CreateWindow("CaroGame", "Caro don gian", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, BOARD_SIZE*CELL_SIZE+16, BOARD_SIZE*CELL_SIZE+59,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}
