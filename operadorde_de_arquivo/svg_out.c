#include "svg_out.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ============================================================
   ADAPTADORES de acesso aos TADs de forma
   -> MAPEIE estes "getters" para as suas funções reais
   ============================================================ */

static char get_tipo(FORMA F) {
    /* seu forma.h provavelmente já tem isso */
    return tipoForma(F);
}

/* ------ Círculo ------ */
// TODO: mapeie para seus getters reais
static double C_x(void* C) { /* getXCirculo(C) */ extern double getXCirculo(void*); return getXCirculo(C); }
static double C_y(void* C) { extern double getYCirculo(void*); return getYCirculo(C); }
static double C_r(void* C) { extern double getRaioCirculo(void*); return getRaioCirculo(C); }
static const char* C_corb(void* C){ extern const char* getCorBordaCirculo(void*); return getCorBordaCirculo(C); }
static const char* C_corp(void* C){ extern const char* getCorPreenchimentoCirculo(void*); return getCorPreenchimentoCirculo(C); }

/* ------ Retângulo ------ */
static double R_x(void* R) { extern double getXRetangulo(void*); return getXRetangulo(R); }
static double R_y(void* R) { extern double getYRetangulo(void*); return getYRetangulo(R); }
static double R_w(void* R) { extern double getLarguraRetangulo(void*); return getLarguraRetangulo(R); }
static double R_h(void* R) { extern double getAlturaRetangulo(void*); return getAlturaRetangulo(R); }
static const char* R_corb(void* R){ extern const char* getCorBordaRetangulo(void*); return getCorBordaRetangulo(R); }
static const char* R_corp(void* R){ extern const char* getCorPreenchimentoRetangulo(void*); return getCorPreenchimentoRetangulo(R); }

/* ------ Linha ------ */
static double L_x1(void* L){ extern double getX1Linha(void*); return getX1Linha(L); }
static double L_y1(void* L){ extern double getY1Linha(void*); return getY1Linha(L); }
static double L_x2(void* L){ extern double getX2Linha(void*); return getX2Linha(L); }
static double L_y2(void* L){ extern double getY2Linha(void*); return getY2Linha(L); }
static const char* L_cor(void* L){ extern const char* getCorLinha(void*); return getCorLinha(L); }

/* ------ Texto ------ */
static double T_x(void* T){ extern double getXTexto(void*); return getXTexto(T); }
static double T_y(void* T){ extern double getYTexto(void*); return getYTexto(T); }
static char   T_anc(void* T){ extern char getAncoraTexto(void*); return getAncoraTexto(T); }
static const char* T_txt(void* T){ extern const char* getConteudoTexto(void*); return getConteudoTexto(T); }
static const char* T_corb(void* T){ extern const char* getCorBordaTexto(void*); return getCorBordaTexto(T); }
static const char* T_corp(void* T){ extern const char* getCorPreenchimentoTexto(void*); return getCorPreenchimentoTexto(T); }
/* Se você tiver estilo (family/weight/size), adicione aqui e no draw. */

/* Pega ponteiro interno do objeto conforme seu TAD de forma */
static void* F_obj(FORMA F) {
    /* alguns projetos têm objetoForma(F); se existir, use-o. */
    extern void* objetoForma(FORMA);
    return objetoForma(F);
}

/* ============================================================
   Contexto SVG
   ============================================================ */
struct svg_ctx {
    FILE* out;
    double minX, minY, maxX, maxY;
    double width, height;
};

static void svg_header(FILE* f, double w, double h, double minX, double minY, double maxX, double maxY) {
    fprintf(f,
        "<svg xmlns=\"http://www.w3.org/2000/svg\" "
        "width=\"%.2f\" height=\"%.2f\" viewBox=\"%.2f %.2f %.2f %.2f\">\n",
        w, h, minX, minY, maxX - minX, maxY - minY);
    fprintf(f, "  <defs>\n");
    fprintf(f, "    <marker id=\"arrow\" markerWidth=\"6\" markerHeight=\"6\" refX=\"6\" refY=\"3\" orient=\"auto\">\n");
    fprintf(f, "      <path d=\"M0,0 L6,3 L0,6 Z\" />\n");
    fprintf(f, "    </marker>\n");
    fprintf(f, "  </defs>\n");
    /* fundo quadriculado leve (opcional) */
    fprintf(f, "  <rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" fill=\"white\" />\n",
            minX, minY, maxX-minX, maxY-minY);
}

SVG svg_begin(FILE* out, double minX, double minY, double maxX, double maxY,
              double width, double height) {
    if (!out) return NULL;
    if (width  <= 0) width  = (maxX - minX);
    if (height <= 0) height = (maxY - minY);

    struct svg_ctx* s = (struct svg_ctx*)calloc(1, sizeof(*s));
    s->out = out;
    s->minX = minX; s->minY = minY; s->maxX = maxX; s->maxY = maxY;
    s->width = width; s->height = height;

    svg_header(out, width, height, minX, minY, maxX, maxY);
    return s;
}

/* ============================================================
   Desenho de cada tipo
   ============================================================ */
static void draw_circle(SVG s, void* C) {
    fprintf(s->out,
        "  <circle cx=\"%.2f\" cy=\"%.2f\" r=\"%.2f\" stroke=\"%s\" fill=\"%s\" />\n",
        C_x(C), C_y(C), C_r(C), C_corb(C), C_corp(C));
}

static void draw_rect(SVG s, void* R) {
    fprintf(s->out,
        "  <rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" stroke=\"%s\" fill=\"%s\" />\n",
        R_x(R), R_y(R), R_w(R), R_h(R), R_corb(R), R_corp(R));
}

static void draw_line(SVG s, void* L) {
    fprintf(s->out,
        "  <line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"%s\" stroke-width=\"2\" />\n",
        L_x1(L), L_y1(L), L_x2(L), L_y2(L), L_cor(L));
}

static void draw_text(SVG s, void* T) {
    /* âncora: 'i' (start), 'm' (middle), 'f' (end) */
    char a = T_anc(T);
    const char* anchor = (a=='m') ? "middle" : (a=='f' ? "end" : "start");
    fprintf(s->out,
        "  <text x=\"%.2f\" y=\"%.2f\" fill=\"%s\" stroke=\"%s\" text-anchor=\"%s\">",
        T_x(T), T_y(T), T_corp(T), T_corb(T), anchor);
    /* escape básico (aqui assumimos que o txt não tem chars especiais) */
    fputs(T_txt(T), s->out);
    fputs("</text>\n", s->out);

    /* Se quiser desenhar também o “segmento induzido” (útil para colisão), descomente:
    size_t len = strlen(T_txt(T));
    double cl = 10.0 * (double)len;
    double x1, y1, x2, y2;
    if (a=='i'){ x1=T_x(T); y1=T_y(T); x2=T_x(T)+cl; y2=T_y(T); }
    else if(a=='f'){ x1=T_x(T)-cl; y1=T_y(T); x2=T_x(T); y2=T_y(T); }
    else { x1=T_x(T)-cl/2; y1=T_y(T); x2=T_x(T)+cl/2; y2=T_y(T); }
    fprintf(s->out,"  <line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"gray\" stroke-dasharray=\"4,3\"/>\n",
            x1,y1,x2,y2);
    */
}

/* ============================================================
   API pública
   ============================================================ */

void svg_draw_forma(SVG s, FORMA F) {
    if (!s || !F) return;
    char t = get_tipo(F);
    void* o = F_obj(F);
    switch (t) {
        case 'c': draw_circle(s, o); break;
        case 'r': draw_rect(s, o);   break;
        case 'l': draw_line(s, o);   break;
        case 't': draw_text(s, o);   break;
        default:
            /* desconhecido: ignore ou marque */
            fprintf(s->out, "  <!-- forma desconhecida: %c -->\n", t);
            break;
    }
}


void svg_draw_fila(SVG s, FILA f) {
    if (!s || !f) return;
    int n = tamanhoFila(f);
    for (int i = 0; i < n; ++i) {
        void* elem = removeDaFila(f);
        if (elem) {
            svg_draw_forma(s, (FORMA)elem);
            insereNaFila(f, elem);
        }
    }
}


void svg_note_disparo(SVG s, double x0, double y0, double x1, double y1) {
    if (!s) return;
    double dx = x1 - x0, dy = y1 - y0;
    double dist = sqrt(dx*dx + dy*dy);
    fprintf(s->out,
        "  <line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" "
        "stroke=\"black\" stroke-dasharray=\"5,3\" marker-end=\"url(#arrow)\"/>\n",
        x0,y0,x1,y1);
    fprintf(s->out,
        "  <text x=\"%.2f\" y=\"%.2f\" fill=\"black\">dx=%.2f, dy=%.2f, |d|=%.2f</text>\n",
        (x0+x1)/2.0, (y0+y1)/2.0 - 5.0, dx, dy, dist);
}

void svg_mark_esmagado(SVG s, double x, double y) {
    if (!s) return;
    /* asterisco simples com duas linhas cruzadas + círculo vermelho pequeno */
    fprintf(s->out,
        "  <circle cx=\"%.2f\" cy=\"%.2f\" r=\"3\" fill=\"red\" />\n", x, y);
    fprintf(s->out,
        "  <line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"red\"/>\n",
        x-6, y-6, x+6, y+6);
    fprintf(s->out,
        "  <line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"red\"/>\n",
        x-6, y+6, x+6, y-6);
}

void svg_end(SVG s) {
    if (!s) return;
    fprintf(s->out, "</svg>\n");
    free(s);
}