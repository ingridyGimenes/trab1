
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <stdbool.h>

#include "geometria.h"
#include "../formas/circulo.h"
#include "../formas/retangulo.h"
#include "../formas/linha.h"
#include "../formas/texto.h"
#include "../formas/forma.h"

/* ================= util local ================= */

static double clamp(double v, double lo, double hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

/* distância ponto-segmento (ao quadrado) */
static double dist2_ponto_seg(double px, double py, double x1, double y1, double x2, double y2) {
    double vx = x2 - x1, vy = y2 - y1;
    double wx = px - x1, wy = py - y1;
    double c1 = vx*wx + vy*wy;
    if (c1 <= 0.0) {
        double dx = px - x1, dy = py - y1;
        return dx*dx + dy*dy;
    }
    double c2 = vx*vx + vy*vy;
    if (c2 <= c1) {
        double dx = px - x2, dy = py - y2;
        return dx*dx + dy*dy;
    }
    double t = c1 / c2;
    double projx = x1 + t*vx, projy = y1 + t*vy;
    double dx = px - projx, dy = py - projy;
    return dx*dx + dy*dy;
}

/* orientação para interseção de segmentos */
static int orient(double ax, double ay, double bx, double by, double cx, double cy) {
    double v = (bx-ax)*(cy-ay) - (by-ay)*(cx-ax);
    if (v > 0) return 1;
    if (v < 0) return -1;
    return 0;
}

static bool between(double a, double b, double c) {
    return fmin(a,b) - 1e-9 <= c && c <= fmax(a,b) + 1e-9;
}

/* interseção segmento-segmento */
static bool seg_seg_intersect(double x1, double y1, double x2, double y2,
                              double x3, double y3, double x4, double y4) {
    int o1 = orient(x1,y1,x2,y2,x3,y3);
    int o2 = orient(x1,y1,x2,y2,x4,y4);
    int o3 = orient(x3,y3,x4,y4,x1,y1);
    int o4 = orient(x3,y3,x4,y4,x2,y2);

    if (o1 != o2 && o3 != o4) return true;

    /* casos colineares */
    if (o1 == 0 && between(x1,x2,x3) && between(y1,y2,y3)) return true;
    if (o2 == 0 && between(x1,x2,x4) && between(y1,y2,y4)) return true;
    if (o3 == 0 && between(x3,x4,x1) && between(y3,y4,y1)) return true;
    if (o4 == 0 && between(x3,x4,x2) && between(y3,y4,y2)) return true;
    return false;
}

/* ================= interseções básicas ================= */

static bool intersecaoCirculoCirculo(CIRCULO a, CIRCULO b) {
    double dx = getXCirculo(a) - getXCirculo(b);
    double dy = getYCirculo(a) - getYCirculo(b);
    double dist2 = dx*dx + dy*dy;
    double r = getRaioCirculo(a) + getRaioCirculo(b);
    return dist2 <= r*r + 1e-9;
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

    return !(ax + aw < bx - 1e-9 || bx + bw < ax - 1e-9 ||
             ay + ah < by - 1e-9 || by + bh < ay - 1e-9);
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
    return (dx*dx + dy*dy) <= cr*cr + 1e-9;
}

/* ================= linha/segmento contra outros ================= */

static bool intersecaoLinhaLinha(LINHA A, LINHA B) {
    double x1 = getX1Linha(A), y1 = getY1Linha(A);
    double x2 = getX2Linha(A), y2 = getY2Linha(A);
    double x3 = getX1Linha(B), y3 = getY1Linha(B);
    double x4 = getX2Linha(B), y4 = getY2Linha(B);
    return seg_seg_intersect(x1,y1,x2,y2,x3,y3,x4,y4);
}

static bool seg_ret_intersect(double x1, double y1, double x2, double y2,
                              double rx, double ry, double rw, double rh) {
    /* 1) um extremo dentro */
    if ((x1 >= rx-1e-9 && x1 <= rx+rw+1e-9 && y1 >= ry-1e-9 && y1 <= ry+rh+1e-9) ||
        (x2 >= rx-1e-9 && x2 <= rx+rw+1e-9 && y2 >= ry-1e-9 && y2 <= ry+rh+1e-9))
        return true;

    /* 2) intersecta alguma borda */
    double rx2 = rx + rw, ry2 = ry + rh;
    if (seg_seg_intersect(x1,y1,x2,y2, rx,ry, rx2,ry))   return true; /* topo */
    if (seg_seg_intersect(x1,y1,x2,y2, rx,ry2,rx2,ry2))  return true; /* base */
    if (seg_seg_intersect(x1,y1,x2,y2, rx,ry, rx,ry2))   return true; /* esq */
    if (seg_seg_intersect(x1,y1,x2,y2, rx2,ry, rx2,ry2)) return true; /* dir */

    return false;
}

static bool seg_circ_intersect(double x1, double y1, double x2, double y2,
                               double cx, double cy, double r) {
    double d2 = dist2_ponto_seg(cx, cy, x1, y1, x2, y2);
    return d2 <= r*r + 1e-9;
}

/* ================= texto como segmento (PDF) ================= */

static void texto_segmento(TEXTO T, double *x1, double *y1, double *x2, double *y2) {
    double xt = getXTexto(T), yt = getYTexto(T);
    const char *conteudo = getConteudoTexto(T); /* troque se seu getter difere */
    size_t len = conteudo ? strlen(conteudo) : 0;
    double cl = 10.0 * (double)len;

    char anc = getAncoraTexto(T); /* 'i', 'm', 'f' */

    if (anc == 'i' || anc == 'I') {
        *x1 = xt;      *y1 = yt;
        *x2 = xt + cl; *y2 = yt;
    } else if (anc == 'f' || anc == 'F') {
        *x1 = xt - cl; *y1 = yt;
        *x2 = xt;      *y2 = yt;
    } else { /* 'm' */
        *x1 = xt - cl/2.0; *y1 = yt;
        *x2 = xt + cl/2.0; *y2 = yt;
    }
}

/* ================= API principal ================= */

bool geometriaFormasIntersectam(FORMA A, FORMA B) {
    if (!A || !B) return false;

    char tA = tipoForma(A);
    char tB = tipoForma(B);
    void* fA = objetoForma(A);
    void* fB = objetoForma(B);

    /* c-c */
    if (tA == 'c' && tB == 'c') {
        return intersecaoCirculoCirculo((CIRCULO)fA, (CIRCULO)fB);
    }
    /* r-r */
    if (tA == 'r' && tB == 'r') {
        return intersecaoRetanguloRetangulo((RETANGULO)fA, (RETANGULO)fB);
    }
    /* c-r / r-c */
    if (tA == 'c' && tB == 'r') {
        return intersecaoCirculoRetangulo((CIRCULO)fA, (RETANGULO)fB);
    }
    if (tA == 'r' && tB == 'c') {
        return intersecaoCirculoRetangulo((CIRCULO)fB, (RETANGULO)fA);
    }
    /* l-l */
    if (tA == 'l' && tB == 'l') {
        return intersecaoLinhaLinha((LINHA)fA, (LINHA)fB);
    }

    /* ===== Texto como segmento contra os outros ===== */
    if (tA == 't' || tB == 't') {
        double sx1, sy1, sx2, sy2;
        if (tA == 't') texto_segmento((TEXTO)fA, &sx1, &sy1, &sx2, &sy2);
        else           texto_segmento((TEXTO)fB, &sx1, &sy1, &sx2, &sy2);

        char to = (tA == 't') ? tB : tA; /* tipo “do outro” */

        if (to == 't') {
            /* t-t vira seg-seg */
            double tx1, ty1, tx2, ty2;
            if (tA == 't') texto_segmento((TEXTO)fB, &tx1, &ty1, &tx2, &ty2);
            else           texto_segmento((TEXTO)fA, &tx1, &ty1, &tx2, &ty2);
            return seg_seg_intersect(sx1,sy1,sx2,sy2, tx1,ty1,tx2,ty2);
        }
        if (to == 'l') {
            double lx1, ly1, lx2, ly2;
            if (tB == 'l') { lx1 = getX1Linha((LINHA)fB); ly1 = getY1Linha((LINHA)fB);
                             lx2 = getX2Linha((LINHA)fB); ly2 = getY2Linha((LINHA)fB); }
            else           { lx1 = getX1Linha((LINHA)fA); ly1 = getY1Linha((LINHA)fA);
                             lx2 = getX2Linha((LINHA)fA); ly2 = getY2Linha((LINHA)fA); }
            return seg_seg_intersect(sx1,sy1,sx2,sy2, lx1,ly1,lx2,ly2);
        }
        if (to == 'r') {
            double rx, ry, rw, rh;
            if (tB == 'r') { rx = getXRetangulo((RETANGULO)fB);
                             ry = getYRetangulo((RETANGULO)fB);
                             rw = getLarguraRetangulo((RETANGULO)fB);
                             rh = getAlturaRetangulo((RETANGULO)fB); }
            else           { rx = getXRetangulo((RETANGULO)fA);
                             ry = getYRetangulo((RETANGULO)fA);
                             rw = getLarguraRetangulo((RETANGULO)fA);
                             rh = getAlturaRetangulo((RETANGULO)fA); }
            return seg_ret_intersect(sx1,sy1,sx2,sy2, rx,ry,rw,rh);
        }
        if (to == 'c') {
            double cx, cy, cr;
            if (tB == 'c') { cx = getXCirculo((CIRCULO)fB);
                             cy = getYCirculo((CIRCULO)fB);
                             cr = getRaioCirculo((CIRCULO)fB); }
            else           { cx = getXCirculo((CIRCULO)fA);
                             cy = getYCirculo((CIRCULO)fA);
                             cr = getRaioCirculo((CIRCULO)fA); }
            return seg_circ_intersect(sx1,sy1,sx2,sy2, cx,cy,cr);
        }
        /* tipo não esperado: sem colisão */
        return false;
    }

    /* ===== Linha com outros (reuso dos helpers de segmento) ===== */
    if (tA == 'l' || tB == 'l') {
        double sx1, sy1, sx2, sy2;
        if (tA == 'l') { sx1 = getX1Linha((LINHA)fA); sy1 = getY1Linha((LINHA)fA);
                         sx2 = getX2Linha((LINHA)fA); sy2 = getY2Linha((LINHA)fA); }
        else           { sx1 = getX1Linha((LINHA)fB); sy1 = getY1Linha((LINHA)fB);
                         sx2 = getX2Linha((LINHA)fB); sy2 = getY2Linha((LINHA)fB); }

        char to = (tA == 'l') ? tB : tA;

        if (to == 'r') {
            double rx, ry, rw, rh;
            if (tB == 'r') { rx = getXRetangulo((RETANGULO)fB);
                             ry = getYRetangulo((RETANGULO)fB);
                             rw = getLarguraRetangulo((RETANGULO)fB);
                             rh = getAlturaRetangulo((RETANGULO)fB); }
            else           { rx = getXRetangulo((RETANGULO)fA);
                             ry = getYRetangulo((RETANGULO)fA);
                             rw = getLarguraRetangulo((RETANGULO)fA);
                             rh = getAlturaRetangulo((RETANGULO)fA); }
            return seg_ret_intersect(sx1,sy1,sx2,sy2, rx,ry,rw,rh);
        }
        if (to == 'c') {
            double cx, cy, cr;
            if (tB == 'c') { cx = getXCirculo((CIRCULO)fB);
                             cy = getYCirculo((CIRCULO)fB);
                             cr = getRaioCirculo((CIRCULO)fB); }
            else           { cx = getXCirculo((CIRCULO)fA);
                             cy = getYCirculo((CIRCULO)fA);
                             cr = getRaioCirculo((CIRCULO)fA); }
            return seg_circ_intersect(sx1,sy1,sx2,sy2, cx,cy,cr);
        }
        /* l-l já foi tratado; l-t foi tratado no bloco do texto */
    }

    /* combinação não tratada */
    return false;
}

/* ================= área ================= */

double geometriaAreaForma(FORMA F) {
    if (!F) return 0.0;
    char t = tipoForma(F);
    void* o = objetoForma(F);

    switch (t) {
        case 'c': return calculaAreaCirculo(o);
        case 'r': return calculaAreaRetangulo(o);
        case 't': return calculaAreaTexto(o);  /* deve retornar 20.0 * |t| */
        case 'l': return calculaAreaLinha(o);  /* deve retornar 2.0 * comprimento */
        default:  return 0.0;
    }
}

/* ================= clonagem + cores ================= */

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
        LINHA clone = clonaLinha((LINHA)o, novoId); /* linha não tem cores p/ inverter */
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
    if (tA == 'c')      corp = getCorPreenchimentoCirculo(oA);
    else if (tA == 'r') corp = getCorPreenchimentoRetangulo(oA);
    else if (tA == 't') corp = getCorPreenchimentoTexto(oA);
    /* linha não tem preenchimento */

    if (!corp) return;

    if (tB == 'c')      setCorBordaCirculo(oB, corp);
    else if (tB == 'r') setCorBordaRetangulo(oB, corp);
    else if (tB == 't') setCorBordaTexto(oB, corp);
    else if (tB == 'l') setCorLinha(oB, corp); /* usamos cor da linha */
}

/* ================= âncora para o callback do asterisco ================= */

void geometriaAncoraDaForma(FORMA f, double *x, double *y) {
    if (!x || !y) return;
    if (!f) { *x = 0.0; *y = 0.0; return; }

    char t = tipoForma(f);
    void *o = objetoForma(f);

    switch (t) {
        case 'c':
            *x = getXCirculo(o);
            *y = getYCirculo(o);
            break;
        case 'r':
            *x = getXRetangulo(o);
            *y = getYRetangulo(o);
            break;
        case 't':
            *x = getXTexto(o);
            *y = getYTexto(o);
            break;
        case 'l': {
            double x1 = getX1Linha(o), y1 = getY1Linha(o);
            double x2 = getX2Linha(o), y2 = getY2Linha(o);
            *x = (x1 + x2) * 0.5;
            *y = (y1 + y2) * 0.5;
        } break;
        default:
            *x = 0.0; *y = 0.0;
    }
}
