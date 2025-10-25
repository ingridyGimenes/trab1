#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "geometria.h"  
#include "/workspaces/trab1/formas/forma.h"   // <<< ver cabeçalho sugerido abaixo
#include "/workspaces/trab1/formas/circulo.h"
#include "/workspaces/trab1/formas/retangulo.h"
#include "/workspaces/trab1/formas/linha.h"
#include "/workspaces/trab1/formas/texto.h"
 

/*
  GEOMETRIA — dispatcher de interseções e áreas entre formas do jogo
  ------------------------------------------------------------------
  IMPORTANTE (regra do trabalho): não declarar struct no .h. Por isso, este .c
  mantém uma struct opaca e o .h expõe apenas typedefs void* e funções.

  Cabeçalho SUGERIDO (geometria.h):
  ---------------------------------
  #pragma once
  #include <stdbool.h>

  typedef void* FORMA;  // wrapper opaco { char tipo; void* obj }

  // criação / utilidades
  FORMA cria_forma(char tipo, void* objeto); // objeto é o TAD concreto (RETANGULO, CIRCULO, ...)
  char  tipo_forma(FORMA f);
  void* objeto_forma(FORMA f);

  // interseção & área
  bool   formas_intersectam(FORMA A, FORMA B);    
  double area_forma(FORMA F);

  // regras específicas para o processador
  void   borda_de_B_recebe_corp_de_A(FORMA A, FORMA B); // corb(B) <- corp(A)
  FORMA  clona_forma_com_cores_trocadas(FORMA A, int novoId);

  // destruição
  void   destruir_forma(FORMA F); // libera wrapper; NÃO libera o objeto concreto

  Observação: se preferir que destruir_forma tb libere o objeto concreto, troque
  a política e deixe claro no comentário do .h. Aqui, manterei "não libera o
  objeto concreto" para permitir reaproveitar a mesma instância em outros lugares.
*/

// -----------------------------------------------------------------------------
// Wrapper opaco (somente neste .c)
// -----------------------------------------------------------------------------

typedef struct {
    char  tipo; // 'r','c','l','t'
    void* obj;  // ponteiro real para o TAD da forma
} FormaWrap;

static inline FormaWrap* asW(FORMA f) { return (FormaWrap*) f; }

FORMA cria_forma(char tipo, void* objeto) {
    FormaWrap* w = (FormaWrap*) malloc(sizeof(FormaWrap));
    if (!w) return NULL;
    w->tipo = tipo;
    w->obj  = objeto;
    return (FORMA) w;
}

char tipo_forma(FORMA f) {
    return f ? asW(f)->tipo : '\0';
}

void* objeto_forma(FORMA f) {
    return f ? asW(f)->obj : NULL;
}

void destruir_forma(FORMA f) {
    if (!f) return;
    free(asW(f));
}

// -----------------------------------------------------------------------------
// Utilitários de geometria básicos (segmento etc.)
// -----------------------------------------------------------------------------

static double clampd(double v, double a, double b) {
    return v < a ? a : (v > b ? b : v);
}

static double dist2_ponto_segmento(double px, double py,
                                   double ax, double ay,
                                   double bx, double by) {
    double vx = bx - ax, vy = by - ay;
    double wx = px - ax, wy = py - ay;
    double c1 = vx*wx + vy*wy;
    if (c1 <= 0) return (px-ax)*(px-ax) + (py-ay)*(py-ay);
    double c2 = vx*vx + vy*vy;
    if (c2 <= c1) return (px-bx)*(px-bx) + (py-by)*(py-by);
    double t = c1 / c2;
    double qx = ax + t*vx, qy = ay + t*vy;
    return (px-qx)*(px-qx) + (py-qy)*(py-qy);
}

static double orient(double ax,double ay,double bx,double by,double cx,double cy){
    return (bx-ax)*(cy-ay) - (by-ay)*(cx-ax);
}

static bool on_segment(double ax,double ay,double bx,double by,double px,double py){
    return (px >= fmin(ax,bx) && px <= fmax(ax,bx) &&
            py >= fmin(ay,by) && py <= fmax(ay,by));
}

static bool seg_intersect(double a1x,double a1y,double a2x,double a2y,
                          double b1x,double b1y,double b2x,double b2y){
    double o1 = orient(a1x,a1y,a2x,a2y,b1x,b1y);
    double o2 = orient(a1x,a1y,a2x,a2y,b2x,b2y);
    double o3 = orient(b1x,b1y,b2x,b2y,a1x,a1y);
    double o4 = orient(b1x,b1y,b2x,b2y,a2x,a2y);

    if ((o1>0 && o2<0 || o1<0 && o2>0) && (o3>0 && o4<0 || o3<0 && o4>0)) return true;

    if (o1 == 0 && on_segment(a1x,a1y,a2x,a2y,b1x,b1y)) return true;
    if (o2 == 0 && on_segment(a1x,a1y,a2x,a2y,b2x,b2y)) return true;
    if (o3 == 0 && on_segment(b1x,b1y,b2x,b2y,a1x,a1y)) return true;
    if (o4 == 0 && on_segment(b1x,b1y,b2x,b2y,a2x,a2y)) return true;

    return false;
}

// -----------------------------------------------------------------------------
// Acesso aos TADs concretos (assumindo nomes de funções; ajuste aos seus .h)
// -----------------------------------------------------------------------------
// Retângulo
static inline double RX(FORMA r){ return get_x_ret(objeto_forma(r)); }
static inline double RY(FORMA r){ return get_y_ret(objeto_forma(r)); }
static inline double RW(FORMA r){ return get_w_ret(objeto_forma(r)); }
static inline double RH(FORMA r){ return get_h_ret(objeto_forma(r)); }

// Círculo (assuma seus getters: get_x_circ, get_y_circ, get_r_circ)
static inline double CX(FORMA c){ return get_x_circ(objeto_forma(c)); }
static inline double CY(FORMA c){ return get_y_circ(objeto_forma(c)); }
static inline double CR(FORMA c){ return get_r_circ(objeto_forma(c)); }

// Linha (assuma getters: get_x1_lin, get_y1_lin, get_x2_lin, get_y2_lin)
static inline double L1X(FORMA l){ return get_x1_lin(objeto_forma(l)); }
static inline double L1Y(FORMA l){ return get_y1_lin(objeto_forma(l)); }
static inline double L2X(FORMA l){ return get_x2_lin(objeto_forma(l)); }
static inline double L2Y(FORMA l){ return get_y2_lin(objeto_forma(l)); }

// Texto (assuma getters: get_x_txt, get_y_txt, get_ancora_txt('i'|'m'|'f'), get_len_txt)
static inline double TX(FORMA t){ return get_x_txt(objeto_forma(t)); }
static inline double TY(FORMA t){ return get_y_txt(objeto_forma(t)); }
static inline char   TA(FORMA t){ return get_ancora_txt(objeto_forma(t)); }
static inline int    TL(FORMA t){ return get_len_txt(objeto_forma(t)); }

static void texto_para_segmento(FORMA t, double* x1,double* y1,double* x2,double* y2){
    double xt = TX(t), yt = TY(t); char a = TA(t); int len = TL(t);
    double cl = 10.0 * (double)len; // conforme enunciado
    if (a == 'i') {
        *x1 = xt; *y1 = yt; *x2 = xt + cl; *y2 = yt;
    } else if (a == 'f') {
        *x1 = xt - cl; *y1 = yt; *x2 = xt; *y2 = yt;
    } else { // 'm'
        *x1 = xt - cl/2.0; *y1 = yt; *x2 = xt + cl/2.0; *y2 = yt;
    }
}

// -----------------------------------------------------------------------------
// Interseções específicas
// -----------------------------------------------------------------------------

static bool ret_ret(FORMA A, FORMA B){
    double ax1 = RX(A), ay1 = RY(A), ax2 = RX(A)+RW(A), ay2 = RY(A)+RH(A);
    double bx1 = RX(B), by1 = RY(B), bx2 = RX(B)+RW(B), by2 = RY(B)+RH(B);
    bool sep = (ax2 < bx1) || (bx2 < ax1) || (ay2 < by1) || (by2 < ay1);
    return !sep; // borda conta como colisão
}

static bool circ_circ(FORMA A, FORMA B){
    double dx = CX(A) - CX(B);
    double dy = CY(A) - CY(B);
    double rr = CR(A) + CR(B);
    return (dx*dx + dy*dy) <= rr*rr;
}

static bool ret_circ(FORMA R, FORMA C){
    double cx = CX(C), cy = CY(C), r = CR(C);
    double rx = RX(R), ry = RY(R), rw = RW(R), rh = RH(R);
    double nx = clampd(cx, rx, rx+rw);
    double ny = clampd(cy, ry, ry+rh);
    double dx = cx - nx, dy = cy - ny;
    return (dx*dx + dy*dy) <= r*r;
}

static bool seg_seg(FORMA A, FORMA B){
    return seg_intersect(L1X(A),L1Y(A),L2X(A),L2Y(A), L1X(B),L1Y(B),L2X(B),L2Y(B));
}

static bool ret_seg(FORMA R, FORMA S){
    double rx = RX(R), ry = RY(R), rw = RW(R), rh = RH(R);
    double x1 = L1X(S), y1 = L1Y(S), x2 = L2X(S), y2 = L2Y(S);
    if (x1 >= rx && x1 <= rx+rw && y1 >= ry && y1 <= ry+rh) return true;
    if (x2 >= rx && x2 <= rx+rw && y2 >= ry && y2 <= ry+rh) return true;
    // 4 arestas
    if (seg_intersect(x1,y1,x2,y2, rx,ry, rx+rw,ry)) return true;           // baixo
    if (seg_intersect(x1,y1,x2,y2, rx,ry+rh, rx+rw,ry+rh)) return true;     // cima
    if (seg_intersect(x1,y1,x2,y2, rx,ry, rx,ry+rh)) return true;           // esquerda
    if (seg_intersect(x1,y1,x2,y2, rx+rw,ry, rx+rw,ry+rh)) return true;     // direita
    return false;
}

static bool circ_seg(FORMA C, FORMA S){
    double d2 = dist2_ponto_segmento(CX(C), CY(C), L1X(S),L1Y(S), L2X(S),L2Y(S));
    return d2 <= CR(C)*CR(C);
}

static bool ret_texto(FORMA R, FORMA T){
    double x1,y1,x2,y2; texto_para_segmento(T, &x1,&y1,&x2,&y2);
    FORMA tmp = cria_forma('l', NULL); // reuso do teste segmento
    asW(tmp)->obj = NULL; // objeto não é necessário para seg_intersect
    // usamos diretamente seg_intersect com as coordenadas calculadas
    (void)tmp; // só para deixar claro que não usamos obj
    return ret_seg(R, cria_forma('l', NULL)), // para coerência de assinatura (não usada)
           seg_intersect(x1,y1,x2,y2, RX(R),RY(R), RX(R)+RW(R),RY(R)) ||
           seg_intersect(x1,y1,x2,y2, RX(R),RY(R)+RH(R), RX(R)+RW(R),RY(R)+RH(R)) ||
           seg_intersect(x1,y1,x2,y2, RX(R),RY(R), RX(R),RY(R)+RH(R)) ||
           seg_intersect(x1,y1,x2,y2, RX(R)+RW(R),RY(R), RX(R)+RW(R),RY(R)+RH(R)) ||
           (x1>=RX(R) && x1<=RX(R)+RW(R) && y1>=RY(R) && y1<=RY(R)+RH(R)) ||
           (x2>=RX(R) && x2<=RX(R)+RW(R) && y2>=RY(R) && y2<=RY(R)+RH(R));
}

static bool circ_texto(FORMA C, FORMA T){
    double x1,y1,x2,y2; texto_para_segmento(T, &x1,&y1,&x2,&y2);
    double d2 = dist2_ponto_segmento(CX(C), CY(C), x1,y1, x2,y2);
    return d2 <= CR(C)*CR(C);
}

static bool seg_texto(FORMA S, FORMA T){
    double x1,y1,x2,y2; texto_para_segmento(T, &x1,&y1,&x2,&y2);
    return seg_intersect(L1X(S),L1Y(S),L2X(S),L2Y(S), x1,y1,x2,y2);
}

bool formas_intersectam(FORMA A, FORMA B) {
    if (!A || !B) return false;
    char a = tipo_forma(A), b = tipo_forma(B);

    if (a == 'r' && b == 'r') return ret_ret(A,B);
    if (a == 'c' && b == 'c') return circ_circ(A,B);

    if (a == 'r' && b == 'c') return ret_circ(A,B);
    if (a == 'c' && b == 'r') return ret_circ(B,A);

    if (a == 'l' && b == 'l') return seg_seg(A,B);

    if (a == 'r' && b == 'l') return ret_seg(A,B);
    if (a == 'l' && b == 'r') return ret_seg(B,A);

    if (a == 'c' && b == 'l') return circ_seg(A,B);
    if (a == 'l' && b == 'c') return circ_seg(B,A);

    if (a == 'r' && b == 't') return ret_texto(A,B);
    if (a == 't' && b == 'r') return ret_texto(B,A);

    if (a == 'c' && b == 't') return circ_texto(A,B);
    if (a == 't' && b == 'c') return circ_texto(B,A);

    if (a == 'l' && b == 't') return seg_texto(A,B);
    if (a == 't' && b == 'l') return seg_texto(B,A);

    if (a == 't' && b == 't') {
        double x1a,y1a,x2a,y2a; texto_para_segmento(A,&x1a,&y1a,&x2a,&y2a);
        double x1b,y1b,x2b,y2b; texto_para_segmento(B,&x1b,&y1b,&x2b,&y2b);
        return seg_intersect(x1a,y1a,x2a,y2a, x1b,y1b,x2b,y2b);
    }

    return false;
}

// -----------------------------------------------------------------------------
// Áreas
// -----------------------------------------------------------------------------

static double area_ret(FORMA R){ return calcula_area_ret(objeto_forma(R)); }
static double area_circ_(FORMA C){ double r = CR(C); return M_PI*r*r; }
static double area_linha_(FORMA L){
    double dx = L2X(L)-L1X(L), dy = L2Y(L)-L1Y(L);
    double len = sqrt(dx*dx + dy*dy);
    return 2.0 * len; // conforme enunciado
}
static double area_texto_(FORMA T){ return 20.0 * (double) TL(T); }

double area_forma(FORMA F){
    if (!F) return 0.0;
    switch (tipo_forma(F)){
        case 'r': return area_ret(F);
        case 'c': return area_circ_(F);
        case 'l': return area_linha_(F);
        case 't': return area_texto_(F);
        default:  return 0.0;
    }
}

// -----------------------------------------------------------------------------
// Alteração de cor e clonagem com cores trocadas
// -----------------------------------------------------------------------------

// corb(B) <- corp(A)
void borda_de_B_recebe_corp_de_A(FORMA A, FORMA B){
    if (!A || !B) return;
    char a = tipo_forma(A), b = tipo_forma(B);

    // precisamos de setters/ getters de cor em cada TAD
    if (b == 'r') {
        if (a == 'r') set_corb_ret(objeto_forma(B), get_corp_ret(objeto_forma(A)));
        else if (a == 'c') set_corb_ret(objeto_forma(B), get_corp_circ(objeto_forma(A)));
        else if (a == 't') set_corb_ret(objeto_forma(B), get_corp_txt(objeto_forma(A)));
        else if (a == 'l') set_corb_ret(objeto_forma(B), get_cor_lin(objeto_forma(A))); // linha tem 1 cor
    } else if (b == 'c') {
        if (a == 'r') set_corb_circ(objeto_forma(B), get_corp_ret(objeto_forma(A)));
        else if (a == 'c') set_corb_circ(objeto_forma(B), get_corp_circ(objeto_forma(A)));
        else if (a == 't') set_corb_circ(objeto_forma(B), get_corp_txt(objeto_forma(A)));
        else if (a == 'l') set_corb_circ(objeto_forma(B), get_cor_lin(objeto_forma(A)));
    } else if (b == 't') {
        if (a == 'r') set_corb_txt(objeto_forma(B), get_corp_ret(objeto_forma(A)));
        else if (a == 'c') set_corb_txt(objeto_forma(B), get_corp_circ(objeto_forma(A)));
        else if (a == 't') set_corb_txt(objeto_forma(B), get_corp_txt(objeto_forma(A)));
        else if (a == 'l') set_corb_txt(objeto_forma(B), get_cor_lin(objeto_forma(A)));
    } else if (b == 'l') {
        // linha só tem uma cor; se quiser mudar, substitua sua cor pela corp(A)
        if      (a == 'r') set_cor_lin(objeto_forma(B), get_corp_ret(objeto_forma(A)));
        else if (a == 'c') set_cor_lin(objeto_forma(B), get_corp_circ(objeto_forma(A)));
        else if (a == 't') set_cor_lin(objeto_forma(B), get_corp_txt(objeto_forma(A)));
        else if (a == 'l') set_cor_lin(objeto_forma(B), get_cor_lin(objeto_forma(A)));
    }
}

FORMA clona_forma_com_cores_trocadas(FORMA A, int novoId){
    if (!A) return NULL;
    char a = tipo_forma(A);
    void* obj = objeto_forma(A);

    if (a == 'r') {
        // retângulo: clone + inverter cores
        void* clone = clona_retangulo(obj, novoId);
        if (clone) inverte_cores_ret(clone);
        return cria_forma('r', clone);
    } else if (a == 'c') {
        void* clone = clona_circulo(obj, novoId);
        if (clone) inverte_cores_circ(clone);
        return cria_forma('c', clone);
    } else if (a == 't') {
        void* clone = clona_texto(obj, novoId);
        if (clone) inverte_cores_txt(clone);
        return cria_forma('t', clone);
    } else if (a == 'l') {
        // linha não tem corp/corb distintos; apenas duplica
        void* clone = clona_linha(obj, novoId);
        return cria_forma('l', clone);
    }
    return NULL;
}
