// main.c - Bocha Geométrica (C99)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "tads_gerais/fila.h"
#include "operadorde_de_arquivo/geo_io.h"    // int geo_executar(FILE*, FILA, FILE*)
#include "operadorde_de_arquivo/qry_io.h"    // qry_bind_svg(...), int qry_executar(FILE*, FILA, FILE*)
#include "operadorde_de_arquivo/svg_out.h"   // svg_begin/svg_end/svg_draw_fila

// --- helpers de caminho/nome -----------------------------------------------

static const char* path_sep(void) {
#ifdef _WIN32
    return "\\";
#else
    return "/";
#endif
}

static void join2(char *dst, size_t sz, const char *a, const char *b) {
    if (!a || !*a) { snprintf(dst, sz, "%s", b ? b : ""); return; }
    if (!b || !*b) { snprintf(dst, sz, "%s", a); return; }
    size_t la = strlen(a);
    int need_sep = (la > 0 && a[la-1] != '/' && a[la-1] != '\\');
    snprintf(dst, sz, "%s%s%s", a, need_sep ? path_sep() : "", b);
}

static void basename_no_ext(char *dst, size_t sz, const char *filename) {
    const char *base = filename;
    for (const char *p = filename; p && *p; ++p) {
        if (*p == '/' || *p == '\\') base = p + 1;
    }
    snprintf(dst, sz, "%s", base);
    char *dot = strrchr(dst, '.');
    if (dot) *dot = '\0';
}

// --- CLI --------------------------------------------------------------------

typedef struct {
    const char *bed;  // -e
    const char *geo;  // -f
    const char *bsd;  // -o
    const char *qry;  // -q (opcional)
} Args;

static void uso(const char *prog) {
    fprintf(stderr,
        "Uso: %s -e <dir_entrada> -f <arq.geo> -o <dir_saida> [-q <arq.qry>]\n"
        "Ex.: %s -e ./in -f cidade.geo -o ./out -q a.qry\n",
        prog, prog);
}

static int parse_args(int argc, char **argv, Args *out) {
    memset(out, 0, sizeof(*out));
    for (int i = 1; i < argc; ++i) {
        if      (strcmp(argv[i], "-e") == 0 && i + 1 < argc) out->bed = argv[++i];
        else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) out->geo = argv[++i];
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) out->bsd = argv[++i];
        else if (strcmp(argv[i], "-q") == 0 && i + 1 < argc) out->qry = argv[++i];
        else { fprintf(stderr, "Parâmetro inválido ou faltando valor: %s\n", argv[i]); return 0; }
    }
    if (!out->bed || !out->geo || !out->bsd) return 0;
    return 1;
}

// --- programa ---------------------------------------------------------------

int main(int argc, char **argv) {
    Args args;
    if (!parse_args(argc, argv, &args)) {
        uso(argv[0]);
        return 1;
    }

    // Caminhos de entrada
    char geo_path[1024], qry_path[1024];
    join2(geo_path, sizeof geo_path, args.bed, args.geo);
    if (args.qry) join2(qry_path, sizeof qry_path, args.bed, args.qry);

    // Abre .geo
    FILE *fgeo = fopen(geo_path, "r");
    if (!fgeo) {
        fprintf(stderr, "Erro ao abrir GEO '%s': %s\n", geo_path, strerror(errno));
        return 2;
    }

    // Fila do chão e leitura do .geo
    FILA chao = criaFila();
    if (!chao) {
        fprintf(stderr, "Falha ao criar fila do chao.\n");
        fclose(fgeo);
        return 2;
    }

    int nformas = geo_executar(fgeo, chao, NULL);
    (void)nformas;
    fclose(fgeo);

    // SVG inicial <bsd>/<base_geo>.svg
    char base_geo[256], svg_inicial_path[1024];
    basename_no_ext(base_geo, sizeof base_geo, args.geo);
    {
        char svg_name[512];
        snprintf(svg_name, sizeof svg_name, "%s.svg", base_geo);
        join2(svg_inicial_path, sizeof svg_inicial_path, args.bsd, svg_name);
    }

    FILE *fsvg_ini = fopen(svg_inicial_path, "w");
    if (!fsvg_ini) {
        fprintf(stderr, "Erro ao criar SVG inicial '%s': %s\n", svg_inicial_path, strerror(errno));
        return 3;
    }
    SVG s0 = svg_begin(fsvg_ini, 0.0, 0.0, 1000.0, 1000.0, 1000.0, 1000.0);
    svg_draw_fila(s0, chao);
    svg_end(s0);
    fclose(fsvg_ini);

    // Se não há .qry, finaliza aqui
    if (!args.qry) {
        return 0;
    }

    // Abre .qry
    FILE *fqry = fopen(qry_path, "r");
    if (!fqry) {
        fprintf(stderr, "Erro ao abrir QRY '%s': %s\n", qry_path, strerror(errno));
        return 4;
    }

    // Saídas do .qry: <bsd>/<base_geo>-<base_qry>.txt e .svg
    char base_qry[256];
    basename_no_ext(base_qry, sizeof base_qry, args.qry);

    char out_txt_path[1024], out_svg_path[1024];
    {
        char name_txt[512], name_svg[512];
        snprintf(name_txt, sizeof name_txt, "%s-%s.txt", base_geo, base_qry);
        snprintf(name_svg, sizeof name_svg, "%s-%s.svg", base_geo, base_qry);
        join2(out_txt_path, sizeof out_txt_path, args.bsd, name_txt);
        join2(out_svg_path, sizeof out_svg_path, args.bsd, name_svg);
    }

    FILE *ftxt = fopen(out_txt_path, "w");
    if (!ftxt) {
        fprintf(stderr, "Erro ao criar TXT '%s': %s\n", out_txt_path, strerror(errno));
        fclose(fqry);
        return 5;
    }

    // >>> ABRE o SVG do .qry ANTES de executar as instruções e VINCULA no qry_io
    FILE *fsvg_qry = fopen(out_svg_path, "w");
    if (!fsvg_qry) {
        fprintf(stderr, "Erro ao criar SVG do .qry '%s': %s\n", out_svg_path, strerror(errno));
        // Sem SVG: ainda assim executamos o .qry (só não haverá anotações gráficas)
    }

    SVG sctx_qry = NULL;
    if (fsvg_qry) {
        sctx_qry = svg_begin(fsvg_qry, 0.0, 0.0, 1000.0, 1000.0, 1000.0, 1000.0);
        qry_bind_svg(sctx_qry);  // <- permite 'dsp ... v' e asteriscos do 'calc'
    } else {
        qry_bind_svg(NULL);
    }

    // Executa o .qry (TXT sai em ftxt; anotações gráficas saem no sctx_qry se houver)
    int ninstr = qry_executar(fqry, chao, ftxt);
    (void)ninstr;
    fclose(fqry);
    fclose(ftxt);

    // Desenha o chão final no MESMO SVG e fecha
    if (sctx_qry) {
        svg_draw_fila(sctx_qry, chao);
        svg_end(sctx_qry);
        fclose(fsvg_qry);
    }

    return 0;
}
