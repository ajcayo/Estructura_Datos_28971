#include "GraficoUtils.h"

void dibujarLinea(HDC hdc, int x1, int y1, int x2, int y2){
    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);
}

int anchoNodoTexto(int numClaves){
    return 30 * numClaves + 10;
}

void dibujarNodoMultivia(HDC hdc, const std::vector<int>& claves, int x, int y){
    int ancho = anchoNodoTexto((int)claves.size());
    int alto = 30;
    RECT rect = {x - ancho/2, y - alto/2, x + ancho/2, y + alto/2};
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

    int cellW = ancho / (int)claves.size();
    for(size_t i = 0; i < claves.size(); i++){
        RECT celda = {rect.left + (int)i*cellW, rect.top, rect.left + (int)(i+1)*cellW, rect.bottom};
        if(i > 0) dibujarLinea(hdc, celda.left, rect.top, celda.left, rect.bottom);
        std::string s = std::to_string(claves[i]);
        DrawTextA(hdc, s.c_str(), -1, &celda, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}
