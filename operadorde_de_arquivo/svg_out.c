#include "svg_out.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>

#include "../tads_gerais/fila.h"
#include "../formas/forma.h"
#include "../formas/circulo.h"
#include "../formas/retangulo.h"
#include "../formas/linha.h"
#include "../formas/texto.h"


/* ===========================================================
   ADAPTAÇÃO DE APIS (caso nomes difiram nos seus TADs)
   -----------------------------------------------------------
   Se seus getters tiverem nomes diferentes, ajuste APENAS
   esta seção.
   =========================================================== */

/* ---- CÍRCULO ----
   Esperado:
     int    getIdCirculo(CIRCULO);
     double getXCirculo(CIRCULO), getYCirculo(CIRCULO);
     double getRaioCirculo(CIRCULO);
     const char* getCorbCirculo(CIRCULO);
     const char* getCorpCirculo(CIRCULO);
*/

/* ---- RETÂNGULO ----
   Esperado:
     int    getIdRetangulo(RETANGULO);
     double getXRetangulo(RETANGULO), getYRetangulo(RETANGULO);
     double getLarguraRetangulo(RETANGULO), getAlturaRetangulo(RETANGULO);
     const char* getCorbRetangulo(RETANGULO);
     const char* getCorpRetangulo(RETANGULO);
*/

/* ---- LINHA ----
   Esperado:
     int    getIdLinha(LINHA);
     double getX1Linha(LINHA), getY1Linha(LINHA);
     double getX2Linha(LINHA), getY2Linha(LINHA);
     const char* getCorLinha(LINHA);
*/

/* ---- TEXTO ----
   Esperado (mínimo):
     int    getIdTexto(TEXTO);
     double getXTexto(TEXTO), getYTexto(TEXTO);
     char   getAncoraTexto(TEXTO);     // 'i','m','f'
     const char* getCorbTexto(TEXTO);
     const char* getCorpTexto(TEXTO);
     const char* getConteudoTexto(TEXTO);

   Se seu TEXTO tem estilo (family/weight/size), ative -DTEXTO_TEM_ESTILO e forneça:
     const char* getFamilyTexto(TEXTO);
     const char* getWeightTexto(TEXTO); // "n","b","b+","l"
     const char* getSizeTexto(TEXTO);   // "12", "14", etc
*/

/* ===========================================================
   UTIL
   =========================================================== */

static void svg_printf(FILE* f, const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(f, fmt, ap);
    va_end(ap);
}

static void xml_escape_into(const char* s, char* out, size_t outcap) {
    /* Escapa &, <, > e " para segurança básica no SVG. */
    size_t j = 0;
    for (size_t i = 0; s && s[i] && j + 6 < outcap; ++i) {
        char c = s[i];
        if (c == '&')      j += snprintf(out + j, outcap - j, "&amp;");
        else if (c == '<') j += snprintf(out + j, outcap - j, "&lt;");
        else if (c == '>') j += snprintf(out + j, outcap - j, "&gt;");
        else if (c == '"') j += snprintf(out + j, outcap - j, "&quot;");
        else out[j++] = c;
    }
    if (j < outcap) out[j] = '\0';
}

/* Converte peso "n|b|b+|l" em SVG font-weight numérico aproximado. */
static const char* weight_to_svg(const char* w) {
    if (!w) return "400";
    if (strcmp(w,"b")==0)  return "700";
    if (strcmp(w,"b+")==0) return "800";
    if (strcmp(w,"l")==0)  return "300";
    return "400"; /* n */
}

/* Converte family "sans|serif|cursive" para CSS genérico. */
static const char* family_to_svg(const char* fam) {
    if (!fam) return "sans-serif";
    if (strcmp(fam,"sans")==0)   return "sans-serif";
    if (strcmp(fam,"serif")==0)  return "serif";
    if (strcmp(fam,"cursive")==0)return "cursive";
    return "sans-serif";
}

/* text-anchor a partir de âncora 'i'|'m'|'f' */
static const char* anchor_to_text_anchor(char a) {
    if (a == 'm') return "middle";
    if (a == 'f') return "end";
    return "start"; /* 'i' */
}

/* ===========================================================
   DESENHO POR TIPO
   =========================================================== */

static void draw_circulo(FILE* svg, CIRCULO c) {
    double x = getXCirculo(c);
    double y = getYCirculo(c);
    double r = getRaioCirculo(c);
    const char* corb = getCorbCirculo(c);
    const char* corp = getCorpCirculo(c);
    svg_printf(svg,
        "<circle cx=\"%.3f\" cy=\"%.3f\" r=\"%.3f\" stroke=\"%s\" fill=\"%s\" />\n",
        x, y, r, corb?corb:"black", corp?corp:"none");
}

static void draw_retangulo(FILE* svg, RETANGULO r) {
    double x = getXRetangulo(r);
    double y = getYRetangulo(r);
    double w = getLarguraRetangulo(r);
    double h = getAlturaRetangulo(r);
    const char* corb = getCorbRetangulo(r);
    const char* corp = getCorpRetangulo(r);
    svg_printf(svg,
        "<rect x=\"%.3f\" y=\"%.3f\" width=\"%.3f\" height=\"%.3f\" stroke=\"%s\" fill=\"%s\" />\n",
        x, y, w, h, corb?corb:"black", corp?corp:"none");
}

static void draw_linha(FILE* svg, LINHA l) {
    double x1 = getX1Linha(l), y1 = getY1Linha(l);
    double x2 = getX2Linha(l), y2 = getY2Linha(l);
    const char* cor = getCorLinha(l);
    /* largura “2.0” é compatível com a convenção de área (2 * comprimento) */
    svg_printf(svg,
        "<line x1=\"%.3f\" y1=\"%.3f\" x2=\"%.3f\" y2=\"%.3f\" stroke=\"%s\" stroke-width=\"2\" />\n",
        x1, y1, x2, y2, cor?cor:"black");
}

static void draw_texto(FILE* svg, TEXTO t) {
    double x = getXTexto(t);
    double y = getYTexto(t);
    char a = getAncoraTexto(t);
    const char* corb = getCorbTexto(t);
    const char* corp = getCorpTexto(t);
    const char* conteudo = getConteudoTexto(t);

    char esc[4096];
    xml_escape_into(conteudo ? conteudo : "", esc, sizeof esc);

#ifdef TEXTO_TEM_ESTILO
    const char* fam = family_to_svg(getFamilyTexto(t));
    const char* w   = weight_to_svg(getWeightTexto(t));
    const char* sz  = getSizeTexto(t) ? getSizeTexto(t) : "12";
    svg_printf(svg,
        "<text x=\"%.3f\" y=\"%.3f\" text-anchor=\"%s\" stroke=\"%s\" fill=\"%s\" "
        "font-family=\"%s\" font-weight=\"%s\" font-size=\"%s\">%s</text>\n",
        x, y, anchor_to_text_anchor(a), corb?corb:"black", corp?corp:"black",
        fam, w, sz, esc);
#else
    svg_printf(svg,
        "<text x=\"%.3f\" y=\"%.3f\" text-anchor=\"%s\" stroke=\"%s\" fill=\"%s\">%s</text>\n",
        x, y, anchor_to_text_anchor(a), corb?corb:"black", corp?corp:"black", esc);
#endif
}

/* ===========================================================
   DISPATCHER
   =========================================================== */

static void draw_forma(FILE* svg, FORMA f) {
    if (!f) return;
    char t = tipoForma(f);
    void* obj = objetoForma(f);
    switch (t) {
        case 'c': draw_circulo(svg, (CIRCULO)obj);   break;
        case 'r': draw_retangulo(svg, (RETANGULO)obj); break;
        case 'l': draw_linha(svg, (LINHA)obj);       break;
        case 't': draw_texto(svg, (TEXTO)obj);       break;
        default:
            /* tipo desconhecido: ignore */
            break;
    }
}

/* ===========================================================
   API PÚBLICA
   =========================================================== */

void svg_begin(FILE* svg, double width, double height) {
    if (!svg) return;
    if (width <= 0)  width  = 1000;
    if (height <= 0) height = 1000;

    svg_printf(svg,
        "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" "
        "width=\"%.0f\" height=\"%.0f\" viewBox=\"0 0 %.0f %.0f\">\n",
        width, height, width, height);

    /* fundo opcional branco translúcido para facilitar visualização */
    svg_printf(svg,
        "  <rect x=\"0\" y=\"0\" width=\"100%%\" height=\"100%%\" fill=\"white\" fill-opacity=\"0.95\"/>\n");
}

void svg_end(FILE* svg) {
    if (!svg) return;
    svg_printf(svg, "</svg>\n");
}

/* percorre a fila SEM perder ordem */
void svg_draw_chao(FILE* svg, FILA chao) {
    if (!svg || !chao) return;

    int n = tamanho_fila(chao); /* se seu TAD não tiver, faça um laço até “dar a volta” */
    if (n <= 0) return;

    /* Safe loop: remove e recoloca */
    for (int i = 0; i < n; ++i) {
        FORMA f = remove_da_fila(chao);
        draw_forma(svg, f);
        add_na_fila(chao, f);
    }
}

/* Asterisco vermelho simples (X + +) centrado em (x,y) */
void svg_mark_asterisk(FILE* svg, double x, double y, double size) {
    if (!svg) return;
    if (size <= 0) size = 6.0;
    double s = size;

    svg_printf(svg,
      "<g stroke=\"red\" stroke-width=\"1.5\">\n"
      "  <line x1=\"%.3f\" y1=\"%.3f\" x2=\"%.3f\" y2=\"%.3f\" />\n"
      "  <line x1=\"%.3f\" y1=\"%.3f\" x2=\"%.3f\" y2=\"%.3f\" />\n"
      "  <line x1=\"%.3f\" y1=\"%.3f\" x2=\"%.3f\" y2=\"%.3f\" />\n"
      "  <line x1=\"%.3f\" y1=\"%.3f\" x2=\"%.3f\" y2=\"%.3f\" />\n"
      "</g>\n",
      x-s, y-s, x+s, y+s,
      x-s, y+s, x+s, y-s,
      x, y-s, x, y+s,
      x-s, y, x+s, y
    );
}

/* Caixa/medida de disparo (tracejada) + rótulo opcional. */
void svg_mark_shot_box(FILE* svg, double x, double y, double w, double h, const char* label) {
    if (!svg) return;
    if (w < 0) { x += w; w = -w; }
    if (h < 0) { y += h; h = -h; }

    svg_printf(svg,
      "<rect x=\"%.3f\" y=\"%.3f\" width=\"%.3f\" height=\"%.3f\" "
      "fill=\"none\" stroke=\"#444\" stroke-dasharray=\"4,3\"/>\n",
      x, y, w, h);

    if (label && *label) {
        char esc[1024]; xml_escape_into(label, esc, sizeof esc);
        svg_printf(svg,
          "<text x=\"%.3f\" y=\"%.3f\" fill=\"#444\" font-size=\"10\" "
          "font-family=\"sans-serif\">%s</text>\n",
          x + 3, y - 3, esc);
    }
}
