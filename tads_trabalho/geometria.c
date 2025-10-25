// geometria.c — versão final com nomes padronizados dos TADs

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "geometria.h"

#include "/workspaces/trab1/formas/forma.h"
#include "/workspaces/trab1/formas/circulo.h"
#include "/workspaces/trab1/formas/retangulo.h"
#include "/workspaces/trab1/formas/linha.h"
#include "/workspaces/trab1/formas/texto.h"

// ----------------------------------------------------------------------------
// Wrapper FORMA
// ----------------------------------------------------------------------------
typedef struct {
    char  tipo;
    void* obj;
} FormaWrap;

static inline FormaWrap* W(FORMA f){ return (FormaWrap*)f; }

FORMA criaForma(char tipo, void* objeto){
    FormaWrap* w = malloc(sizeof(FormaWrap));
    if(!w) return NULL;
    w->tipo = tipo;
    w->obj  = objeto;
    return (FORMA)w;
}

char tipoForma(FORMA f){ return f ? W(f)->tipo : '\0'; }
void* objetoForma(FORMA f){ return f ? W(f)->obj : NULL; }
void destruirForma(FORMA f){ if(f) free(W(f)); }

// ----------------------------------------------------------------------------
// Utilitários geométricos
// ----------------------------------------------------------------------------
static double clampd(double v, double a, double b){
    return v < a ? a : (v > b ? b : v);
}

static double orient(double ax,double ay,double bx,double by,double cx,double cy){
    return (bx-ax)*(cy-ay) - (by-ay)*(cx-ax);
}

static bool onSegment(double ax,double ay,double bx,double by,double px,double py){
    return (px >= fmin(ax,bx) && px <= fmax(ax,bx) &&
            py >= fmin(ay,by) && py <= fmax(ay,by));
}

static bool segIntersect(double a1x,double a1y,double a2x,double a2y,
                         double b1x,double b1y,double b2x,double b2y){
    double o1 = orient(a1x,a1y,a2x,a2y,b1x,b1y);
    double o2 = orient(a1x,a1y,a2x,a2y,b2x,b2y);
    double o3 = orient(b1x,b1y,b2x,b2y,a1x,a1y);
    double o4 = orient(b1x,b1y,b2x,b2y,a2x,a2y);

    if ((o1>0 && o2<0 || o1<0 && o2>0) && (o3>0 && o4<0 || o3<0 && o4>0)) return true;
    if (o1 == 0 && onSegment(a1x,a1y,a2x,a2y,b1x,b1y)) return true;
    if (o2 == 0 && onSegment(a1x,a1y,a2x,a2y,b2x,b2y)) return true;
    if (o3 == 0 && onSegment(b1x,b1y,b2x,b2y,a1x,a1y)) return true;
    if (o4 == 0 && onSegment(b1x,b1y,b2x,b2y,a2x,a2y)) return true;
    return false;
}

static double dist2PontoSegmento(double px,double py,
                                 double ax,double ay,double bx,double by){
    double vx = bx-ax, vy = by-ay;
    double wx = px-ax, wy = py-ay;
    double c1 = vx*wx + vy*wy;
    if (c1 <= 0) return (px-ax)*(px-ax)+(py-ay)*(py-ay);
    double c2 = vx*vx + vy*vy;
    if (c2 <= c1) return (px-bx)*(px-bx)+(py-by)*(py-by);
    double t = c1 / c2;
    double qx = ax + t*vx, qy = ay + t*vy;
    return (px-qx)*(px-qx)+(py-qy)*(py-qy);
}

// ----------------------------------------------------------------------------
// Texto -> Segmento
// ----------------------------------------------------------------------------
static void textoParaSegmento(FORMA T, double* x1,double* y1,double* x2,double* y2){
    char ancora = getAncoraTexto(objetoForma(T));
    double x = getXTexto(objetoForma(T));
    double y = getYTexto(objetoForma(T));
    const char* txt = getConteudoTexto(objetoForma(T));
    size_t len = txt ? strlen(txt) : 0;
    double cl = 10.0 * (double)len;

    if (ancora == 'i') { *x1 = x;       *y1 = y; *x2 = x+cl; *y2 = y; }
    else if (ancora == 'f'){ *x1 = x-cl; *y1 = y; *x2 = x;    *y2 = y; }
    else { *x1 = x - cl/2.0; *y1 = y; *x2 = x + cl/2.0; *y2 = y; }
}

// ----------------------------------------------------------------------------
// Interseções
// ----------------------------------------------------------------------------
bool formasIntersectam(FORMA A, FORMA B){
    if(!A || !B) return false;
    char a = tipoForma(A), b = tipoForma(B);

    // Retângulo - Retângulo
    if (a=='r' && b=='r'){
        double ax1=getXRetangulo(objetoForma(A));
        double ay1=getYRetangulo(objetoForma(A));
        double ax2=ax1+getLarguraRetangulo(objetoForma(A));
        double ay2=ay1+getAlturaRetangulo(objetoForma(A));
        double bx1=getXRetangulo(objetoForma(B));
        double by1=getYRetangulo(objetoForma(B));
        double bx2=bx1+getLarguraRetangulo(objetoForma(B));
        double by2=by1+getAlturaRetangulo(objetoForma(B));
        return !(ax2<bx1 || bx2<ax1 || ay2<by1 || by2<ay1);
    }

    // Círculo - Círculo
    if (a=='c' && b=='c'){
        double dx=getXCirculo(objetoForma(A))-getXCirculo(objetoForma(B));
        double dy=getYCirculo(objetoForma(A))-getYCirculo(objetoForma(B));
        double rr=getRaioCirculo(objetoForma(A))+getRaioCirculo(objetoForma(B));
        return (dx*dx+dy*dy)<=rr*rr;
    }

    // Retângulo - Círculo
    if ((a=='r' && b=='c') || (a=='c' && b=='r')){
        FORMA R = a=='r'?A:B; FORMA C = a=='c'?A:B;
        double cx=getXCirculo(objetoForma(C)), cy=getYCirculo(objetoForma(C)), r=getRaioCirculo(objetoForma(C));
        double rx=getXRetangulo(objetoForma(R)), ry=getYRetangulo(objetoForma(R));
        double rw=getLarguraRetangulo(objetoForma(R)), rh=getAlturaRetangulo(objetoForma(R));
        double nx = clampd(cx, rx, rx+rw);
        double ny = clampd(cy, ry, ry+rh);
        double dx = cx-nx, dy = cy-ny;
        return (dx*dx+dy*dy)<=r*r;
    }

    // Linha - Linha
    if (a=='l' && b=='l'){
        return segIntersect(
            getX1Linha(objetoForma(A)),getY1Linha(objetoForma(A)),
            getX2Linha(objetoForma(A)),getY2Linha(objetoForma(A)),
            getX1Linha(objetoForma(B)),getY1Linha(objetoForma(B)),
            getX2Linha(objetoForma(B)),getY2Linha(objetoForma(B))
        );
    }

    // Texto - qualquer (converte em segmento)
    if (a=='t' || b=='t'){
        double ax1,ay1,ax2,ay2,bx1,by1,bx2,by2;
        if (a=='t') textoParaSegmento(A,&ax1,&ay1,&ax2,&ay2);
        else {
            ax1=getX1Linha(objetoForma(A));
            ay1=getY1Linha(objetoForma(A));
            ax2=getX2Linha(objetoForma(A));
            ay2=getY2Linha(objetoForma(A));
        }
        if (b=='t') textoParaSegmento(B,&bx1,&by1,&bx2,&by2);
        else {
            bx1=getX1Linha(objetoForma(B));
            by1=getY1Linha(objetoForma(B));
            bx2=getX2Linha(objetoForma(B));
            by2=getY2Linha(objetoForma(B));
        }
        return segIntersect(ax1,ay1,ax2,ay2,bx1,by1,bx2,by2);
    }

    return false;
}

// ----------------------------------------------------------------------------
// Área
// ----------------------------------------------------------------------------
double areaForma(FORMA F){
    if(!F) return 0.0;
    char t = tipoForma(F);
    if (t=='r') return calculaAreaRetangulo(objetoForma(F));
    if (t=='c') return calculaAreaCirculo(objetoForma(F));
    if (t=='l'){
        double dx = getX2Linha(objetoForma(F)) - getX1Linha(objetoForma(F));
        double dy = getY2Linha(objetoForma(F)) - getY1Linha(objetoForma(F));
        return 2.0 * sqrt(dx*dx + dy*dy);
    }
    if (t=='t') return calculaAreaTexto(objetoForma(F));
    return 0.0;
}

// ----------------------------------------------------------------------------
// Bordas e clonagem
// ----------------------------------------------------------------------------
void bordaDeBRecebeCorpDeA(FORMA A, FORMA B){
    const char* cor = NULL;
    switch(tipoForma(A)){
        case 'r': cor = getCorPreenchimentoRetangulo(objetoForma(A)); break;
        case 'c': cor = getCorPreenchimentoCirculo(objetoForma(A)); break;
        case 'l': cor = getCorLinha(objetoForma(A)); break;
        case 't': cor = getCorPreenchimentoTexto(objetoForma(A)); break;
    }

    switch(tipoForma(B)){
        case 'r': setCorBordaRetangulo(objetoForma(B), cor); break;
        case 'c': setCorBordaCirculo(objetoForma(B), cor); break;
        case 'l': setCorLinha(objetoForma(B), cor); break;
        case 't': setCorBordaTexto(objetoForma(B), cor); break;
    }
}

FORMA clonaFormaComCoresTrocadas(FORMA A, int novoId){
    if(!A) return NULL;
    switch(tipoForma(A)){
        case 'r': {
            RETANGULO r = clonaRetangulo(objetoForma(A), novoId);
            inverterCoresRetangulo(r);
            return criaForma('r', r);
        }
        case 'c': {
            CIRCULO c = clonaCirculo(objetoForma(A), novoId);
            inverterCoresCirculo(c);
            return criaForma('c', c);
        }
        case 'l': {
            LINHA l = clonaLinha(objetoForma(A), novoId);
            return criaForma('l', l);
        }
        case 't': {
            TEXTO t = clonaTexto(objetoForma(A), novoId);
            inverterCoresTexto(t);
            return criaForma('t', t);
        }
    }
    return NULL;
}
