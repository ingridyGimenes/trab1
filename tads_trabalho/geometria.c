#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "geometria.h"
#include "/workspaces/trab1/formas/circulo.h"
#include "/workspaces/trab1/formas/retangulo.h"
#include "/workspaces/trab1/formas/linha.h"
#include "/workspaces/trab1/formas/texto.h"

#define M_PI 3.14159

bool geometriaFormasIntersectam(FORMA A, FORMA B) {
    if (!A || !B) return false;

    char tA = tipoForma(A);
    char tB = tipoForma(B);
    void* fA = objetoForma(A);
    void* fB = objetoForma(B);

    // --- Círculo x Círculo ---
    if (tA == 'c' && tB == 'c') {
        double dx = getXCirculo(fA) - getXCirculo(fB);
        double dy = getYCirculo(fA) - getYCirculo(fB);
        double dist = sqrt(dx*dx + dy*dy);
        return dist <= (getRaioCirculo(fA) + getRaioCirculo(fB));
    }

    // --- Retângulo x Retângulo ---
    if (tA == 'r' && tB == 'r') {
        double ax = get_cord_x_ret(fA);
        double ay = get_cord_y_ret(fA);
        double aw = get_width_ret(fA);
        double ah = get_heigth_ret(fA);

        double bx = get_cord_x_ret(fB);
        double by = get_cord_y_ret(fB);
        double bw = get_width_ret(fB);
        double bh = get_heigth_ret(fB);

        return !(ax + aw < bx || bx + bw < ax || ay + ah < by || by + bh < ay);
    }

    // --- Círculo x Retângulo ---
    if ((tA == 'c' && tB == 'r') || (tA == 'r' && tB == 'c')) {
        void* circulo = (tA == 'c') ? fA : fB;
        void* ret = (tA == 'r') ? fA : fB;

        double cx = getXCirculo(circulo);
        double cy = getYCirculo(circulo);
        double r = getRaioCirculo(circulo);

        double rx = get_cord_x_ret(ret);
        double ry = get_cord_y_ret(ret);
        double rw = get_width_ret(ret);
        double rh = get_heigth_ret(ret);

        double closestX = fmax(rx, fmin(cx, rx + rw));
        double closestY = fmax(ry, fmin(cy, ry + rh));

        double dx = cx - closestX;
        double dy = cy - closestY;

        return (dx*dx + dy*dy) <= (r*r);
    }

    // --- Linha x Linha ---
    if (tA == 'l' && tB == 'l') {
        // Algoritmo de interseção de segmentos de linha
        double x1 = get_codx1_linha(fA), y1 = get_cody1_linha(fA);
        double x2 = get_codx2_linha(fA), y2 = get_cody2_linha(fA);
        double x3 = get_codx1_linha(fB), y3 = get_cody1_linha(fB);
        double x4 = get_codx2_linha(fB), y4 = get_cody2_linha(fB);

        double den = (x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4);
        if (den == 0) return false; // paralelas ou coincidentes

        double t = ((x1 - x3)*(y3 - y4) - (y1 - y3)*(x3 - x4)) / den;
        double u = ((x1 - x3)*(y1 - y2) - (y1 - y3)*(x1 - x2)) / den;

        return (t >= 0 && t <= 1 && u >= 0 && u <= 1);
    }

    // --- Texto x Texto ---
    if (tA == 't' && tB == 't') {
        // simplificação: considera bounding box
        double ax = get_codx_txt(fA);
        double ay = get_cody_txt(fA);
        double bx = get_codx_txt(fB);
        double by = get_cody_txt(fB);

        return (fabs(ax - bx) < 10 && fabs(ay - by) < 10);
    }

    return false;
}

double geometriaAreaForma(FORMA F) {
    if (!F) return 0.0;
    char t = tipoForma(F);
    void* f = objetoForma(F);

    switch (t) {
        case 'c':
            return M_PI * pow(getRaioCirculo(f), 2);
        case 'r':
            return get_width_ret(f) * get_heigth_ret(f);
        case 't':
            return 0.0; // Texto não tem área real
        case 'l':
            return 0.0; // Linha também não
        default:
            return 0.0;
    }
}

FORMA geometriaClonaFormaComCoresTrocadas(FORMA F, int novoId) {
    if (!F) return NULL;
    char t = tipoForma(F);
    void* f = objetoForma(F);

    if (t == 'c') {
        CIRCULO clone = clonaCirculo(f, novoId);
        inverteCores_cir(clone);
        return criaForma('c', clone);
    }

    if (t == 'r') {
        RETANGULO clone = clonaRetangulo(f, novoId);
        inverteCores_ret(clone);
        return criaForma('r', clone);
    }

    if (t == 'l') {
        LINHA clone = clona_linha(f, novoId);
        return criaForma('l', clone);
    }

    if (t == 't') {
        TEXTO clone = clona_texto(f, novoId);
        return criaForma('t', clone);
    }

    return NULL;
}

void geometriaBordaDeBRecebeCorpDeA(FORMA A, FORMA B) {
    if (!A || !B) return;
    char tA = tipoForma(A);
    char tB = tipoForma(B);

    void* fA = objetoForma(A);
    void* fB = objetoForma(B);

    const char* corP = NULL;
    if (tA == 'c') corP = getCorPreenchimentoCirculo(fA);
    else if (tA == 'r') corP = get_corP_ret(fA);
    else if (tA == 't') corP = get_corP_txt(fA);

    if (!corP) return;

    if (tB == 'c') setCorBordaCirculo(fB, corP);
    else if (tB == 'r') set_corB_ret(fB, (char*)corP);
    else if (tB == 't') set_corB_txt(fB, (char*)corP);
}
