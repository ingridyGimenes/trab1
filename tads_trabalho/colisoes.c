#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "colisoes.h"

// ------------------------------------------------------------
// Círculo - Círculo
// ------------------------------------------------------------
bool sobrepoe_circulo_circulo(Circulo* c1, Circulo* c2) {
    double dx = c1->x - c2->x;
    double dy = c1->y - c2->y;
    double dist = sqrt(dx*dx + dy*dy);
    return dist <= (c1->r + c2->r);
}

// ------------------------------------------------------------
// Retângulo - Retângulo
// ------------------------------------------------------------
bool sobrepoe_retangulo_retangulo(Retangulo* r1, Retangulo* r2) {
    return !(r1->x + r1->w < r2->x ||
             r2->x + r2->w < r1->x ||
             r1->y + r1->h < r2->y ||
             r2->y + r2->h < r1->y);
}

// ------------------------------------------------------------
// Círculo - Retângulo
// ------------------------------------------------------------
bool sobrepoe_circulo_retangulo(Circulo* c, Retangulo* r) {
    double maisProxX = fmax(r->x, fmin(c->x, r->x + r->w));
    double maisProxY = fmax(r->y, fmin(c->y, r->y + r->h));

    double dx = c->x - maisProxX;
    double dy = c->y - maisProxY;

    return (dx*dx + dy*dy) <= (c->r * c->r);
}

// ------------------------------------------------------------
// Linha - Linha (interseção de segmentos)
// ------------------------------------------------------------
static double det(double x1, double y1, double x2, double y2, double x3, double y3) {
    return (x2 - x1)*(y3 - y1) - (y2 - y1)*(x3 - x1);
}

bool sobrepoe_linha_linha(Linha* l1, Linha* l2) {
    double d1 = det(l1->x1, l1->y1, l1->x2, l1->y2, l2->x1, l2->y1);
    double d2 = det(l1->x1, l1->y1, l1->x2, l1->y2, l2->x2, l2->y2);
    double d3 = det(l2->x1, l2->y1, l2->x2, l2->y2, l1->x1, l1->y1);
    double d4 = det(l2->x1, l2->y1, l2->x2, l2->y2, l1->x2, l1->y2);

    return ((d1*d2) < 0 && (d3*d4) < 0);
}

// ------------------------------------------------------------
// Texto → Segmento equivalente
// ------------------------------------------------------------
void texto_para_segmento(Texto* t, double* x1, double* y1, double* x2, double* y2) {
    double comprimento = 10.0 * strlen(t->txto);
    if (t->a == 'i') {
        *x1 = t->x;
        *y1 = t->y;
        *x2 = t->x + comprimento;
        *y2 = t->y;
    } else if (t->a == 'f') {
        *x1 = t->x - comprimento;
        *y1 = t->y;
        *x2 = t->x;
        *y2 = t->y;
    } else { // 'm'
        *x1 = t->x - comprimento / 2.0;
        *y1 = t->y;
        *x2 = t->x + comprimento / 2.0;
        *y2 = t->y;
    }
}


