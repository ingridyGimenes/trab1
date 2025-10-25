// geometria.h (dispatcher de colisões e áreas)
#pragma once
#include <stdbool.h>
#include "/workspaces/trab1/formas/circulo.h"
#include "/workspaces/trab1/formas/retangulo.h"
#include "/workspaces/trab1/formas/linha.h"
#include "/workspaces/trab1/formas/texto.h"

// interseções específicas
bool ret_intersecta_ret(RETANGULO a, RETANGULO b);
bool circ_intersecta_circ(CIRCULO a, CIRCULO b);
bool ret_intersecta_circ(RETANGULO r, CIRCULO c);
bool seg_intersecta_seg(LINHA a, LINHA b);

bool ret_intersecta_seg(RETANGULO r, LINHA s);
bool circ_intersecta_seg(CIRCULO c, LINHA s);

// helpers para TEXTO convertendo pra segmento
bool ret_intersecta_texto(RETANGULO r, TEXTO t);
bool circ_intersecta_texto(CIRCULO c, TEXTO t);
bool seg_intersecta_texto(LINHA s, TEXTO t);

// área “efetiva” para regra de esmagamento:
double area_ret(RETANGULO r);          // w*h
double area_circ(CIRCULO c);           // pi*r^2
double area_linha(LINHA l);            // 2.0 * comprimento
double area_texto(TEXTO t);            // 20.0 * |t|
