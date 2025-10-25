#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "geometria.h"
#include "../formas/circulo.h"
#include "../formas/retangulo.h"
#include "../formas/linha.h"
#include "../formas/texto.h"
#include "../formas/forma.h"

// ---------- util local ----------

static double clamp(double v, double lo, double hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

// ---------- interseções ----------

static bool intersecaoCirculoCirculo(CIRCULO a, CIRCULO b) {
    double dx = getXCirculo(a) - getXCirculo(b);
    double dy = getYCirculo(a) - getYCirculo(b);
    double dist2 = dx*dx + dy*dy;
    double r = getRaioCirculo(a) + getRaioCirculo(b);
    return dist2 <= r*r;
}

static bool intersecaoRetanguloRetangulo(RETANGULO A, RETANGULO B) {
    double ax = getXRetangulo(A);
    double ay = getYRetangulo(A);
    double aw = getLarguraRetangulo(A);
    double ah = getAlturaRetangulo(A);

    double bx = getXRetangulo(B);
    double by = getYRetangulo(B);
    double bw = getLarguraRetangulo(B);
    double bh = getAlturaRetangulo(B);

    // AABBs se sobrepõem?
    return !(ax + aw < bx || bx + bw < ax || ay + ah < by || by + bh < ay);
}

static bool intersecaoCirculoRetangulo(CIRCULO c, RETANGULO r) {
    double cx = getXCirculo(c);
    double cy = getYCirculo(c);
    double cr = getRaioCirculo(c);

    double rx = getXRetangulo(r);
    double ry = getYRetangulo(r);
    double rw = getLarguraRetangulo(r);
    double rh = getAlturaRetangulo(r);

    double closestX = clamp(cx, rx, rx + rw);
    double closestY = clamp(cy, ry, ry + rh);

    double dx = cx - closestX;
    double dy = cy - closestY;
    return (dx*dx + dy*dy) <= cr*cr;
}

static bool intersecaoLinhaLinha(LINHA A, LINHA B) {
    // Segmento A: (x1,y1)-(x2,y2)
    double x1 = getX1Linha(A), y1 = getY1Linha(A);
    double x2 = getX2Linha(A), y2 = getY2Linha(A);
    // Segmento B: (x3,y3)-(x4,y4)
    double x3 = getX1Linha(B), y3 = getY1Linha(B);
    double x4 = getX2Linha(B), y4 = getY2Linha(B);

    double den = (x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4);
    if (den == 0.0) return false; // paralelos ou coincidentes (ignoramos coincidentes aqui)

    double t = ((x1 - x3)*(y3 - y4) - (y1 - y3)*(x3 - x4)) / den;
    double u = ((x1 - x3)*(y1 - y2) - (y1 - y3)*(x1 - x2)) / den;

    return (t >= 0 && t <= 1 && u >= 0 && u <= 1);
}

// Heurística simples para texto: tratamos como um "ponto" grosso (10x10)
static bool intersecaoTextoTexto(TEXTO A, TEXTO B) {
    double ax = getXTexto(A), ay = getYTexto(A);
    double bx = getXTexto(B), by = getYTexto(B);
    return (fabs(ax - bx) < 10.0 && fabs(ay - by) < 10.0);
}

bool geometriaFormasIntersectam(FORMA A, FORMA B) {
    if (!A || !B) return false;

    char tA = tipoForma(A);
    char tB = tipoForma(B);
    void* fA = objetoForma(A);
    void* fB = objetoForma(B);

    // c-c
    if (tA == 'c' && tB == 'c') {
        return intersecaoCirculoCirculo((CIRCULO)fA, (CIRCULO)fB);
    }
    // r-r
    if (tA == 'r' && tB == 'r') {
        return intersecaoRetanguloRetangulo((RETANGULO)fA, (RETANGULO)fB);
    }
    // c-r / r-c
    if (tA == 'c' && tB == 'r') {
        return intersecaoCirculoRetangulo((CIRCULO)fA, (RETANGULO)fB);
    }
    if (tA == 'r' && tB == 'c') {
        return intersecaoCirculoRetangulo((CIRCULO)fB, (RETANGULO)fA);
    }
    // l-l
    if (tA == 'l' && tB == 'l') {
        return intersecaoLinhaLinha((LINHA)fA, (LINHA)fB);
    }
    // t-t (heurístico)
    if (tA == 't' && tB == 't') {
        return intersecaoTextoTexto((TEXTO)fA, (TEXTO)fB);
    }

    // Outras combinações (texto com outros, linha com áreas) não tratadas
    // Poderia-se expandir com bounding-box de texto e teste linha-retângulo etc.
    return false;
}

// ---------- área ----------

double geometriaAreaForma(FORMA F) {
    if (!F) return 0.0;
    char t = tipoForma(F);
    void* o = objetoForma(F);

    switch (t) {
        case 'c': 
            return calculaAreaCirculo(o);
        
        case 'r': 
            return calculaAreaRetangulo(o);
        
        case 't':
              return calculaAreaTexto(o);
        case 'l':
            return calculaAreaLinha(o);
        default:
            return 0.0; // Sem área útil
    }
}

// ---------- clonagem + cores ----------

FORMA geometriaClonaFormaComCoresTrocadas(FORMA F, int novoId) {
    if (!F) return NULL;
    char t = tipoForma(F);
    void* o = objetoForma(F);

    if (t == 'c') {
        CIRCULO clone = clonaCirculo((CIRCULO)o, novoId);
        inverterCoresCirculo(clone);
        return criaForma('c', clone);
    }
    if (t == 'r') {
        RETANGULO clone = clonaRetangulo((RETANGULO)o, novoId);
        inverterCoresRetangulo(clone);
        return criaForma('r', clone);
    }
    if (t == 't') {
        TEXTO clone = clonaTexto((TEXTO)o, novoId);
        inverterCoresTexto(clone);
        return criaForma('t', clone);
    }
    if (t == 'l') {
        // linha não possui preenchimento/borda; apenas clonamos
        LINHA clone = clonaLinha((LINHA)o, novoId);
        return criaForma('l', clone);
    }

    return NULL;
}

void geometriaBordaDeBRecebeCorpDeA(FORMA A, FORMA B) {
    if (!A || !B) return;

    char tA = tipoForma(A);
    char tB = tipoForma(B);
    void* oA = objetoForma(A);
    void* oB = objetoForma(B);

    const char* corp = NULL;
    if (tA == 'c') corp = getCorPreenchimentoCirculo(oA);
    else if (tA == 'r') corp = getCorPreenchimentoRetangulo(oA);
    else if (tA == 't') corp = getCorPreenchimentoTexto(oA);
    // linha não tem preenchimento → ignoramos

    if (!corp) return;

    // “Borda de B recebe corp de A”
    if (tB == 'c') setCorBordaCirculo(oB, corp);
    else if (tB == 'r') setCorBordaRetangulo(oB, corp);
    else if (tB == 't') setCorBordaTexto(oB, corp);
    else if (tB == 'l') setCorLinha(oB, corp); // para linha, usamos a cor da linha
}
