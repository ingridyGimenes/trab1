// main.c - Bocha Geométrica (C99)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "tads_gerais/fila.h"
#include "operadorde_de_arquivo/geo_io.h"   // int geo_ler(FILE*, FILA)
#include "operadorde_de_arquivo/qry_io.h"      // int qry_executar(FILE*, FILA, FILE*)
#include "operadorde_de_arquivo/svg_out.h"      // svg_begin/svg_end/svg_draw_chao

// --- helpers de caminho/nome -----------------------------------------------

/**
 * @brief Retorna o separador de diretório de acordo com o sistema.
 * @return "\\" no Windows; "/" nos demais sistemas.
 */
static const char* path_sep(void) {
#ifdef _WIN32
    return "\\";
#else
    return "/";
#endif
}

/**
 * @brief Concatena dois segmentos de caminho em dst, inserindo separador se necessário.
 * @param dst Buffer de destino.
 * @param sz  Tamanho do buffer de destino.
 * @param a   Primeiro segmento (ex.: diretório).
 * @param b   Segundo segmento (ex.: arquivo).
 *
 * @note Não duplica o separador se @p a já terminar com '/' ou '\\'.
 */
static void join2(char *dst, size_t sz, const char *a, const char *b) {
    // monta "a/ b" (sem duplicar separador)
    if (!a || !*a) { snprintf(dst, sz, "%s", b ? b : ""); return; }
    if (!b || !*b) { snprintf(dst, sz, "%s", a); return; }
    size_t la = strlen(a);
    int need_sep = (la > 0 && a[la-1] != '/' && a[la-1] != '\\');
    snprintf(dst, sz, "%s%s%s", a, need_sep ? path_sep() : "", b);
}

/**
 * @brief Extrai o nome-base sem extensão de um caminho de arquivo.
 * @param dst Buffer de destino para o nome-base.
 * @param sz  Tamanho do buffer de destino.
 * @param filename Caminho do arquivo (pode conter diretórios).
 *
 * @note Ex.: "dir/a.geo" -> "a".
 */
static void basename_no_ext(char *dst, size_t sz, const char *filename) {
    // pega nome base sem extensão: ex. "dir/a.geo" -> "a"
    const char *base = filename;
    for (const char *p = filename; p && *p; ++p) {
        if (*p == '/' || *p == '\\') base = p + 1;
    }
    snprintf(dst, sz, "%s", base);
    char *dot = strrchr(dst, '.');
    if (dot) *dot = '\0';
}

// --- CLI --------------------------------------------------------------------

/**
 * @brief Estrutura com os argumentos de linha de comando.
 * @param bed Diretório-base de entrada (-e).
 * @param geo Nome do arquivo .geo sob o diretório de entrada (-f).
 * @param bsd Diretório-base de saída (-o).
 * @param qry Nome do arquivo .qry sob o diretório de entrada (-q, opcional).
 */
typedef struct {
    const char *bed;     // -e (entrada base)
    const char *geo;     // -f (arquivo .geo dentro de bed)
    const char *bsd;     // -o (saída base)
    const char *qry;     // -q (arquivo .qry dentro de bed) [opcional]
} Args;

/**
 * @brief Imprime instruções de uso na saída de erro.
 * @param prog Nome do executável (argv[0]).
 */
static void uso(const char *prog) {
    fprintf(stderr,
        "Uso: %s -e <dir_entrada> -f <arq.geo> -o <dir_saida> [-q <arq.qry>]\n"
        "Ex.: %s -e ./in -f cidade.geo -o ./out -q a.qry\n",
        prog, prog);
}

/**
 * @brief Faz o parsing dos argumentos de linha de comando.
 * @param argc Contador de argumentos.
 * @param argv Vetor de argumentos.
 * @param out  Estrutura de saída com os parâmetros preenchidos.
 * @return 1 em caso de sucesso; 0 se faltarem argumentos obrigatórios ou houver erro.
 *
 * @note Exige -e, -f e -o; -q é opcional.
 */
static int parse_args(int argc, char **argv, Args *out) {
    memset(out, 0, sizeof(*out));
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-e") == 0 && i + 1 < argc) out->bed = argv[++i];
        else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) out->geo = argv[++i];
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) out->bsd = argv[++i];
        else if (strcmp(argv[i], "-q") == 0 && i + 1 < argc) out->qry = argv[++i];
        else { fprintf(stderr, "Parâmetro inválido ou faltando valor: %s\n", argv[i]); return 0; }
    }
    if (!out->bed || !out->geo || !out->bsd) return 0;
    return 1;
}

// --- programa ---------------------------------------------------------------

/**
 * @brief Programa principal: carrega o .geo, gera o SVG inicial e, se fornecido .qry,
 *        executa as instruções, gera o TXT e o SVG final.
 *
 * @param argc Contador de argumentos de linha de comando.
 * @param argv Vetor de argumentos de linha de comando.
 * @return 0 em sucesso; códigos positivos em caso de erro de abertura de arquivos ou criação de saídas.
 *
 * @details Fluxo:
 *          1) Lê -e, -f, -o (e -q opcional).
 *          2) Abre e processa o .geo em uma FILA "chao".
 *          3) Gera <base_geo>.svg no diretório de saída.
 *          4) Se houver .qry:
 *             - Executa as instruções, escrevendo <base_geo>-<base_qry>.txt
 *             - Gera <base_geo>-<base_qry>.svg com o chão final.
 */
int main(int argc, char **argv) {
    Args args;
    if (!parse_args(argc, argv, &args)) {
        uso(argv[0]);
        return 1;
    }

    // Caminhos de entrada (.geo e .qry ficam sob -e)
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

    int nformas = geo_ler(fgeo, chao);
    (void)nformas; // pode ser útil para logs
    fclose(fgeo);

    // Saída: SVG inicial = <bsd>/<base_geo>.svg
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
    svg_begin(fsvg_ini, 1000.0, 1000.0);
    svg_draw_chao(fsvg_ini, chao);
    svg_end(fsvg_ini);
    fclose(fsvg_ini);

    // Se não há .qry, finaliza aqui
    if (!args.qry) {
        // libera fila do chão (usuário deve prover destruirFila/esvaziaFila em outro ponto, se desejar)
        return 0;
    }

    // Abre .qry
    FILE *fqry = fopen(qry_path, "r");
    if (!fqry) {
        fprintf(stderr, "Erro ao abrir QRY '%s': %s\n", qry_path, strerror(errno));
        return 4;
    }

    // Arquivos de saída após .qry:
    //   <bsd>/<base_geo>-<base_qry>.txt
    //   <bsd>/<base_geo>-<base_qry>.svg
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

    // Executa o .qry (gera relatórios no TXT)
    int ninstr = qry_executar(fqry, chao, ftxt);
    (void)ninstr; // o próprio qry_executar pode registrar o total; aqui só garantimos chamada.
    fclose(fqry);
    fclose(ftxt);

    // Gera SVG final do chão após o processamento do .qry
    FILE *fsvg_final = fopen(out_svg_path, "w");
    if (!fsvg_final) {
        fprintf(stderr, "Erro ao criar SVG final '%s': %s\n", out_svg_path, strerror(errno));
        return 6;
    }
    svg_begin(fsvg_final, 1000.0, 1000.0);
    svg_draw_chao(fsvg_final, chao);
    svg_end(fsvg_final);
    fclose(fsvg_final);

    return 0;
}
