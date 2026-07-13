#include <windows.h>
#include <stdio.h>

typedef int  (*FuncAncho)();
typedef int  (*FuncAlto)();
typedef void (*FuncDibujar)(HWND hwnd);

LRESULT CALLBACK ProcedimientoVentana(HWND hwnd, UINT mensaje, WPARAM wParam, LPARAM lParam)
{
    switch (mensaje)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            HINSTANCE dll = LoadLibraryA("ArbolPantalla.dll");
            if (dll != NULL)
            {
                FuncDibujar dibujar = (FuncDibujar) GetProcAddress(dll, "DibujarArbolEnVentana");
                if (dibujar != NULL)
                {
                    dibujar(hwnd);
                }
                FreeLibrary(dll);
            }

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_KEYDOWN:
        {
            if (wParam == VK_ESCAPE)
            {
                PostQuitMessage(0);
            }
            return 0;
        }

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProcA(hwnd, mensaje, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE instancia, HINSTANCE instanciaAnterior,
                   LPSTR parametros, int mostrar)
{
    int ancho = 1200;
    int alto  = 700;

    HINSTANCE dll = LoadLibraryA("ArbolPantalla.dll");
    if (dll != NULL)
    {
        FuncAncho obtenerAncho = (FuncAncho) GetProcAddress(dll, "ObtenerAnchoPantalla");
        FuncAlto  obtenerAlto  = (FuncAlto)  GetProcAddress(dll, "ObtenerAltoPantalla");
        if (obtenerAncho != NULL) ancho = obtenerAncho();
        if (obtenerAlto  != NULL) alto  = obtenerAlto();
        FreeLibrary(dll);
    }

    WNDCLASSA claseVentana;
    claseVentana.style         = CS_HREDRAW | CS_VREDRAW;
    claseVentana.lpfnWndProc   = ProcedimientoVentana;
    claseVentana.cbClsExtra    = 0;
    claseVentana.cbWndExtra    = 0;
    claseVentana.hInstance     = instancia;
    claseVentana.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    claseVentana.hCursor       = LoadCursor(NULL, IDC_ARROW);
    claseVentana.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    claseVentana.lpszMenuName  = NULL;
    claseVentana.lpszClassName = "VentanaArbol";

    RegisterClassA(&claseVentana);

    HWND ventana = CreateWindowA(
        "VentanaArbol",
        "Arbol Binario - Datos de Pantalla",
        WS_OVERLAPPEDWINDOW,
        0, 0, ancho, alto,
        NULL, NULL, instancia, NULL
    );

    ShowWindow(ventana, SW_SHOWMAXIMIZED);
    UpdateWindow(ventana);

    MSG mensaje;
    while (GetMessageA(&mensaje, NULL, 0, 0))
    {
        TranslateMessage(&mensaje);
        DispatchMessageA(&mensaje);
    }

    return (int) mensaje.wParam;
}
