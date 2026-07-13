#include <windows.h>

LRESULT CALLBACK Ventana(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nCmd)
{
    WNDCLASS wc = {};

    wc.lpfnWndProc = Ventana;
    wc.hInstance = hInst;
    wc.lpszClassName = "Tablero";

    RegisterClass(&wc);

    CreateWindow(
        "Tablero",
        "Tablero de Ajedrez",
        WS_OVERLAPPEDWINDOW,
        100, 100, 520, 540,
        NULL, NULL, hInst, NULL
    );

    ShowWindow(GetActiveWindow(), nCmd);

    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK Ventana(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        int tam = 60;

        for(int fila = 0; fila < 8; fila++)
        {
            for(int col = 0; col < 8; col++)
            {
                HBRUSH brocha;

                if((fila + col) % 2 == 0)
                    brocha = CreateSolidBrush(RGB(255,255,255));
                else
                    brocha = CreateSolidBrush(RGB(80,80,80));

                RECT cuadro;

                cuadro.left   = col * tam;
                cuadro.top    = fila * tam;
                cuadro.right  = cuadro.left + tam;
                cuadro.bottom = cuadro.top + tam;

                FillRect(hdc, &cuadro, brocha);

                DeleteObject(brocha);
            }
        }

        EndPaint(hwnd, &ps);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}