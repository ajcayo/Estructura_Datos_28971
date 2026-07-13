#pragma once
#include "ComunWin.h"
#include <vector>
#include <string>

const int RADIO = 20;
const int NIVEL_ALTO = 80;

void dibujarLinea(HDC hdc, int x1, int y1, int x2, int y2);
int anchoNodoTexto(int numClaves);
void dibujarNodoMultivia(HDC hdc, const std::vector<int>& claves, int x, int y);
