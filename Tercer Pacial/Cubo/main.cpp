#include <windows.h>
#include <string>
#include <stack>

const int CASILLA_SIZE = 40;  
const int ESPACIO_CARAS = 10; 
const int CARA_SIZE = CASILLA_SIZE * 3; 

const int WIN_WIDTH  = 650; 
const int WIN_HEIGHT = 600;

struct OffsetCara { 
    int x; 
    int y; 
};

OffsetCara offsets[6];

struct Boton {
    RECT rect;
    std::string etiqueta;
    char comando;
};

Boton botones[7]; 

class CuboMagico {
private:
    std::stack<char> historialMovimientos;

public:
    int cubo[6][3][3];

    CuboMagico() { 
        inicializarCubo(); 
    }

    void inicializarCubo() {
        for (int cara = 0; cara < 6; cara++) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    cubo[cara][i][j] = cara; 
                }
            }
        }
        while(!historialMovimientos.empty()) {
            historialMovimientos.pop();
        }
    }

    void rotarMatrizCaraHorario(int cara) {
        int temp[3][3];
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                temp[j][2 - i] = cubo[cara][i][j];
            }
        }
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                cubo[cara][i][j] = temp[i][j];
            }
        }
    }

    void rotarMatrizCaraAntiHorario(int cara) {
        rotarMatrizCaraHorario(cara);
        rotarMatrizCaraHorario(cara);
        rotarMatrizCaraHorario(cara);
    }

    // --- GIROS HORARIOS CORREGIDOS (Con buffers seguros) ---
    void girarArriba(bool registrar = true) {
        rotarMatrizCaraHorario(0);
        int t2[3], t3[3], t4[3], t1[3];
        for(int i=0; i<3; i++) { t2[i]=cubo[2][0][i]; t3[i]=cubo[3][0][i]; t4[i]=cubo[4][0][i]; t1[i]=cubo[1][0][i]; }
        for(int i=0; i<3; i++) { cubo[2][0][i]=t3[i]; cubo[3][0][i]=t4[i]; cubo[4][0][i]=t1[i]; cubo[1][0][i]=t2[i]; }
        if (registrar) historialMovimientos.push('U');
    }

    void girarIzquierda(bool registrar = true) {
        rotarMatrizCaraHorario(1);
        int t0[3], t2[3], t5[3], t4[3];
        for(int i=0; i<3; i++) { t0[i]=cubo[0][i][0]; t2[i]=cubo[2][i][0]; t5[i]=cubo[5][i][0]; t4[i]=cubo[4][2-i][2]; }
        for(int i=0; i<3; i++) { cubo[0][i][0]=t4[i]; cubo[2][i][0]=t0[i]; cubo[5][i][0]=t2[i]; cubo[4][2-i][2]=t5[i]; }
        if (registrar) historialMovimientos.push('L');
    }

    void girarFrontal(bool registrar = true) {
        rotarMatrizCaraHorario(2);
        int t0[3], t3[3], t5[3], t1[3];
        for(int i=0; i<3; i++) { t0[i]=cubo[0][2][i]; t3[i]=cubo[3][i][0]; t5[i]=cubo[5][0][2-i]; t1[i]=cubo[1][2-i][2]; }
        for(int i=0; i<3; i++) { cubo[3][i][0]=t0[i]; cubo[5][0][2-i]=t3[i]; cubo[1][2-i][2]=t5[i]; cubo[0][2][i]=t1[i]; }
        if (registrar) historialMovimientos.push('F');
    }

    void girarDerecha(bool registrar = true) {
        rotarMatrizCaraHorario(3);
        int t0[3], t4[3], t5[3], t2[3];
        for(int i=0; i<3; i++) { t0[i]=cubo[0][i][2]; t4[i]=cubo[4][2-i][0]; t5[i]=cubo[5][i][2]; t2[i]=cubo[2][i][2]; }
        for(int i=0; i<3; i++) { cubo[0][i][2]=t2[i]; cubo[4][2-i][0]=t0[i]; cubo[5][i][2]=t4[i]; cubo[2][i][2]=t5[i]; }
        if (registrar) historialMovimientos.push('R');
    }

    void girarAtras(bool registrar = true) {
        rotarMatrizCaraHorario(4);
        int t0[3], t1[3], t5[3], t3[3];
        for(int i=0; i<3; i++) { t0[i]=cubo[0][0][i]; t1[i]=cubo[1][2-i][0]; t5[i]=cubo[5][2][2-i]; t3[i]=cubo[3][i][2]; }
        for(int i=0; i<3; i++) { cubo[1][2-i][0]=t0[i]; cubo[5][2][2-i]=t1[i]; cubo[3][i][2]=t5[i]; cubo[0][0][i]=t3[i]; }
        if (registrar) historialMovimientos.push('B');
    }

    void girarAbajo(bool registrar = true) {
        rotarMatrizCaraHorario(5);
        int t2[3], t1[3], t4[3], t3[3];
        for(int i=0; i<3; i++) { t2[i]=cubo[2][2][i]; t1[i]=cubo[1][2][i]; t4[i]=cubo[4][2][i]; t3[i]=cubo[3][2][i]; }
        for(int i=0; i<3; i++) { cubo[2][2][i]=t1[i]; cubo[1][2][i]=t4[i]; cubo[4][2][i]=t3[i]; cubo[3][2][i]=t2[i]; }
        if (registrar) historialMovimientos.push('D');
    }

    // --- GIROS ANTI-HORARIOS (Para deshacer perfectamente) ---
    void deshacerU() {
        rotarMatrizCaraAntiHorario(0);
        int t2[3], t3[3], t4[3], t1[3];
        for(int i=0; i<3; i++) { t2[i]=cubo[2][0][i]; t3[i]=cubo[3][0][i]; t4[i]=cubo[4][0][i]; t1[i]=cubo[1][0][i]; }
        for(int i=0; i<3; i++) { cubo[2][0][i]=t1[i]; cubo[3][0][i]=t2[i]; cubo[4][0][i]=t3[i]; cubo[1][0][i]=t4[i]; }
    }
    void deshacerL() {
        rotarMatrizCaraAntiHorario(1);
        int t0[3], t2[3], t5[3], t4[3];
        for(int i=0; i<3; i++) { t0[i]=cubo[0][i][0]; t2[i]=cubo[2][i][0]; t5[i]=cubo[5][i][0]; t4[i]=cubo[4][2-i][2]; }
        for(int i=0; i<3; i++) { cubo[0][i][0]=t2[i]; cubo[2][i][0]=t5[i]; cubo[5][i][0]=t4[i]; cubo[4][2-i][2]=t0[i]; }
    }
    void deshacerF() {
        rotarMatrizCaraAntiHorario(2);
        int t0[3], t3[3], t5[3], t1[3];
        for(int i=0; i<3; i++) { t0[i]=cubo[0][2][i]; t3[i]=cubo[3][i][0]; t5[i]=cubo[5][0][2-i]; t1[i]=cubo[1][2-i][2]; }
        for(int i=0; i<3; i++) { cubo[0][2][i]=t3[i]; cubo[3][i][0]=t5[i]; cubo[5][0][2-i]=t1[i]; cubo[1][2-i][2]=t0[i]; }
    }
    void deshacerR() {
        rotarMatrizCaraAntiHorario(3);
        int t0[3], t4[3], t5[3], t2[3];
        for(int i=0; i<3; i++) { t0[i]=cubo[0][i][2]; t4[i]=cubo[4][2-i][0]; t5[i]=cubo[5][i][2]; t2[i]=cubo[2][i][2]; }
        for(int i=0; i<3; i++) { cubo[0][i][2]=t4[i]; cubo[4][2-i][0]=t5[i]; cubo[5][i][2]=t2[i]; cubo[2][i][2]=t0[i]; }
    }
    void deshacerB() {
        rotarMatrizCaraAntiHorario(4);
        int t0[3], t1[3], t5[3], t3[3];
        for(int i=0; i<3; i++) { t0[i]=cubo[0][0][i]; t1[i]=cubo[1][2-i][0]; t5[i]=cubo[5][2][2-i]; t3[i]=cubo[3][i][2]; }
        for(int i=0; i<3; i++) { cubo[0][0][i]=t1[i]; cubo[1][2-i][0]=t5[i]; cubo[5][2][2-i]=t3[i]; cubo[3][i][2]=t0[i]; }
    }
    void deshacerD() {
        rotarMatrizCaraAntiHorario(5);
        int t2[3], t1[3], t4[3], t3[3];
        for(int i=0; i<3; i++) { t2[i]=cubo[2][2][i]; t1[i]=cubo[1][2][i]; t4[i]=cubo[4][2][i]; t3[i]=cubo[3][2][i]; }
        for(int i=0; i<3; i++) { cubo[2][2][i]=t3[i]; cubo[1][2][i]=t2[i]; cubo[4][2][i]=t1[i]; cubo[3][2][i]=t4[i]; }
    }

    void resolverPasoAPaso() {
        if (historialMovimientos.empty()) return;

        char ultimoMov = historialMovimientos.top();
        historialMovimientos.pop();

        switch (ultimoMov) {
            case 'U': deshacerU(); break;
            case 'L': deshacerL(); break;
            case 'F': deshacerF(); break;
            case 'R': deshacerR(); break;
            case 'B': deshacerB(); break;
            case 'D': deshacerD(); break;
        }
    }
    
    bool estaResuelto() { 
        return historialMovimientos.empty(); 
    }
};

CuboMagico miCubo;

COLORREF getRGBColor(int colorId) {
    switch (colorId) {
        case 0: return RGB(255, 255, 255); 
        case 1: return RGB(0, 155, 72);    
        case 2: return RGB(183, 18, 52);   
        case 3: return RGB(0, 70, 173);    
        case 4: return RGB(255, 88, 0);    
        case 5: return RGB(255, 213, 0);   
        default: return RGB(128, 128, 128);
    }
}

void dibujarCuboGrafico(HDC hdc) {
    for (int cara = 0; cara < 6; cara++) {
        int startX = offsets[cara].x;
        int startY = offsets[cara].y;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                int x1 = startX + j * CASILLA_SIZE;
                int y1 = startY + i * CASILLA_SIZE;
                HBRUSH brush = CreateSolidBrush(getRGBColor(miCubo.cubo[cara][i][j]));
                HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
                HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
                HPEN oldPen = (HPEN)SelectObject(hdc, pen);

                Rectangle(hdc, x1, y1, x1 + CASILLA_SIZE, y1 + CASILLA_SIZE);

                SelectObject(hdc, oldBrush); 
                SelectObject(hdc, oldPen);
                DeleteObject(brush); 
                DeleteObject(pen);
            }
        }
    }

    SetBkMode(hdc, TRANSPARENT);
    for (int i = 0; i < 7; i++) {
        HBRUSH brushBoton = CreateSolidBrush(i == 6 ? RGB(180, 240, 180) : RGB(220, 220, 220));
        HPEN penBoton = CreatePen(PS_SOLID, i == 6 ? 2 : 1, i == 6 ? RGB(0, 150, 0) : RGB(100, 100, 100));
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brushBoton);
        HPEN oldPen = (HPEN)SelectObject(hdc, penBoton);

        RoundRect(hdc, botones[i].rect.left, botones[i].rect.top, botones[i].rect.right, botones[i].rect.bottom, 8, 8);
        DrawTextA(hdc, botones[i].etiqueta.c_str(), -1, &botones[i].rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        SelectObject(hdc, oldBrush); 
        SelectObject(hdc, oldPen);
        DeleteObject(brushBoton); 
        DeleteObject(penBoton);
    }

    std::string info = "Presiona RESOLVER repetidamente para ordenar paso a paso mediante LIFO.";
    TextOutA(hdc, 20, WIN_HEIGHT - 120, info.c_str(), (int)info.size());
}

void inicializarEstructuras() {
    offsets[0].x = CARA_SIZE + ESPACIO_CARAS * 2;                 offsets[0].y = ESPACIO_CARAS;
    offsets[1].x = ESPACIO_CARAS;                                 offsets[1].y = CARA_SIZE + ESPACIO_CARAS * 2;
    offsets[2].x = CARA_SIZE + ESPACIO_CARAS * 2;                 offsets[2].y = CARA_SIZE + ESPACIO_CARAS * 2;
    offsets[3].x = (CARA_SIZE * 2) + ESPACIO_CARAS * 3;           offsets[3].y = CARA_SIZE + ESPACIO_CARAS * 2;
    offsets[4].x = (CARA_SIZE * 3) + ESPACIO_CARAS * 4;           offsets[4].y = CARA_SIZE + ESPACIO_CARAS * 2;
    offsets[5].x = CARA_SIZE + ESPACIO_CARAS * 2;                 offsets[5].y = (CARA_SIZE * 2) + ESPACIO_CARAS * 3;

    int btnWidth = 75, btnHeight = 35, startX = 15, startY = WIN_HEIGHT - 80;

    botones[0].etiqueta = "Girar U"; botones[0].comando = 'U';
    botones[1].etiqueta = "Girar L"; botones[1].comando = 'L';
    botones[2].etiqueta = "Girar F"; botones[2].comando = 'F';
    botones[3].etiqueta = "Girar R"; botones[3].comando = 'R';
    botones[4].etiqueta = "Girar B"; botones[4].comando = 'B';
    botones[5].etiqueta = "Girar D"; botones[5].comando = 'D';

    for (int i = 0; i < 6; i++) {
        botones[i].rect.left = startX + i * (btnWidth + 8);
        botones[i].rect.top = startY;
        botones[i].rect.right = botones[i].rect.left + btnWidth;
        botones[i].rect.bottom = botones[i].rect.top + btnHeight;
    }

    botones[6].rect.left = botones[5].rect.right + 12;
    botones[6].rect.top = startY - 2; 
    botones[6].rect.right = botones[6].rect.left + 95;
    botones[6].rect.bottom = botones[5].rect.bottom + 2;
    botones[6].etiqueta = "RESOLVER";
    botones[6].comando = 'S'; 
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rc; 
            GetClientRect(hwnd, &rc);
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

            HBRUSH fondo = CreateSolidBrush(RGB(245, 245, 245));
            FillRect(memDC, &rc, fondo); 
            DeleteObject(fondo);

            dibujarCuboGrafico(memDC);

            BitBlt(hdc, 0, 0, rc.right, rc.bottom, memDC, 0, 0, SRCCOPY);
            SelectObject(memDC, oldBitmap); 
            DeleteObject(memBitmap); 
            DeleteDC(memDC);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_LBUTTONDOWN: {
            int x = LOWORD(lParam), y = HIWORD(lParam);
            for (int i = 0; i < 7; i++) {
                if (x >= botones[i].rect.left && x <= botones[i].rect.right &&
                    y >= botones[i].rect.top && y <= botones[i].rect.bottom) {
                    
                    if (botones[i].comando == 'S') {
                        miCubo.resolverPasoAPaso();
                    } else {
                        switch (botones[i].comando) {
                            case 'U': miCubo.girarArriba(); break;
                            case 'L': miCubo.girarIzquierda(); break;
                            case 'F': miCubo.girarFrontal(); break;
                            case 'R': miCubo.girarDerecha(); break;
                            case 'B': miCubo.girarAtras(); break;
                            case 'D': miCubo.girarAbajo(); break;
                        }
                    }
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                }
            }
            return 0;
        }
        case WM_DESTROY: 
            PostQuitMessage(0); 
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int main() {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    const char CLASS_NAME[] = "VentanaCuboRubikPasoAPaso";
    inicializarEstructuras();

    WNDCLASS wc = { };
    wc.lpfnWndProc = WndProc; 
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME; 
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Cubo Rubik LIFO - Paso a Paso",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, WIN_WIDTH, WIN_HEIGHT,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;
    ShowWindow(hwnd, SW_SHOW);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) { 
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    }
    return 0;
}




