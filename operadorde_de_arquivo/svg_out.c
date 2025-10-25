#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "svg_out.h"

#include "/workspaces/trab1/tads_gerais/fila.h"       // add_na_fila
#include "/workspaces/trab1/formas/forma.h"   
#include "/workspaces/trab1/formas/circulo.h"
#include "/workspaces/trab1/formas/retangulo.h"
#include "/workspaces/trab1/formas/linha.h"
#include "/workspaces/trab1/formas/texto.h"

static void svg_begin(FILE* out, double w, double h){
    fprintf(out, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%.2f\" height=\"%.2f\" viewBox=\"0 0 %.2f %.2f\">\n",
            w, h, w, h);
    fprintf(out, "  <rect x=\"0\" y=\"0\" width=\"100%%\" height=\"100%%\" fill=\"white\"/>\n");
}

static void svg_end(FILE* out){
    fprintf(out, "</svg>\n");
}

static void draw_ret(FILE* out, RETANGULO r){
    double x = get_x_ret(r), y = get_y_ret(r), w = get_w_ret(r), h = get_h_ret(r);
    const char* cb = get_corb_ret(r);
    const char* cp = get_corp_ret(r);
    fprintf(out, "  <rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" stroke=\"%s\" fill=\"%s\"/>\n",
            x, y, w, h, cb?cb:"black", cp?cp:"none");
}

static void draw_circ(FILE* out, CIRCLE c){
    double x = get_x_circ(c), y = get_y_circ(c), r = get_r_circ(c);
    const char* cb = get_corb_circ(c);
    const char* cp = get_corp_circ(c);
    fprintf(out, "  <circle cx=\"%.2f\" cy=\"%.2f\" r=\"%.2f\" stroke=\"%s\" fill=\"%s\"/>\n",
            x, y, r, cb?cb:"black", cp?cp:"none");
}

static void draw_lin(FILE* out, LINHA l){
    double x1 = get_x1_lin(l), y1 = get_y1_lin(l), x2 = get_x2_lin(l), y2 = get_y2_lin(l);
    const char* c = get_cor_lin(l);
    fprintf(out, "  <line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"%s\"/>\n",
            x1, y1, x2, y2, c?c:"black");
}

static void draw_txt(FILE* out, TEXTO t){
    double x = get_x_txt(t), y = get_y_txt(t);
    const char* cb = get_corB_txt(t);
    const char* cp = get_corP_txt(t);
    const char* content = get_txto_txt(t);
    char anc = get_ancora_txt(t); // 'i','m','f'
    const char* anchor = (anc=='i'?"start":(anc=='f'?"end":"middle"));
    // estilo básico; se quiser, leia family/size do seu TAD
    fprintf(out, "  <text x=\"%.2f\" y=\"%.2f\" text-anchor=\"%s\" font-family=\"sans-serif\" font-size=\"12\" "
                 "stroke=\"%s\" fill=\"%s\">%s</text>\n",
            x, y, anchor, cb?cb:"black", cp?cp:"black", content?content:"");
}

int svg_dump_fila(const char* path_svg, void* fila_formas, double width, double height){
    if(!path_svg || !fila_formas) return -1;
    FILE* out = fopen(path_svg, "w");
    if(!out) return -1;

    svg_begin(out, width, height);

    // percorre a fila sem destruí-la: vamos remover cada item e recolocar no fim
    int n = tamanho_fila(fila_formas);
    for(int i=0;i<n;i++){
        FORMA F = (FORMA) remove_da_fila(fila_formas);
        char t = tipo_forma(F);
        void* obj = objeto_forma(F);

        if      (t=='r') draw_ret(out, (RETANGULO)obj);
        else if (t=='c') draw_circ(out, (CIRCLE)obj);
        else if (t=='l') draw_lin(out, (LINHA)obj);
        else if (t=='t') draw_txt(out, (TEXTO)obj);

        add_na_fila(fila_formas, F); // volta pra fila
    }

    svg_end(out);
    fclose(out);
    return 0;
}
