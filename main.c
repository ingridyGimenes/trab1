// main.c — integração completa: GEO -> QRY -> SVG (+ TXT de log)
//
// uso:
//   ./app entrada.geo entrada.qry saida.svg [saida.txt]
//
// obs: o .txt é opcional (default "saida.txt")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Fila/Pilha e módulos de arquivo
#include "fila.h"
#include "geo_io.h"
#include "qry_io.h"
#include "svg_out.h"

// Processamento e formas
#include "processador.h"
#include "forma.h"      // FORMA, tipo_forma, objeto_forma

// TADs concretos (para destruir o objeto real)
#include "retangulo.h"
#include "circulo.h"
#include "linha.h"
#include "texto.h"

// -----------------------------------------------------------------------------
// util: destrói completamente uma FORMA (objeto concreto + wrapper)
// -----------------------------------------------------------------------------
static void destruir_forma_total(FORMA f) {
    if (!f) return;
    char  t = tipo_forma(f);
    void* o = objeto_forma(f);

    // Libera o objeto concreto conforme o tipo
    switch (t) {
        case 'r': excluir_retangulo(o); break;   // alias: exclui_retangulo
        case 'c': destruir_circulo(o);  break;   // alias: destroiCirculo
        case 'l': destruir_linha(o);    break;
        case 't': destruir_texto(o);    break;
        default: /* tipo desconhecido: nada */   break;
    }

    // Libera o wrapper FORMA
    destruir_forma(f);
}

// esvazia por completo uma fila de FORMA (destruindo o conteúdo)
static void esvazia_fila_formas(void* fila) {
    while (!fila_vazia(fila)) {
        FORMA f = (FORMA) remove_da_fila(fila);
        destruir_forma_total(f);
    }
}

// gerador de ids para clones (callback passado ao processador)
static int next_id(void) {
    static int base = 10000;
    return base++;
}

// -----------------------------------------------------------------------------
// main
// -----------------------------------------------------------------------------
int main(int argc, char** argv) {
    if (argc < 4) {
        fprintf(stderr, "uso: %s <entrada.geo> <entrada.qry> <saida.svg> [saida.txt]\n", argv[0]);
        return 1;
    }

    const char* path_geo = argv[1];
    const char* path_qry = argv[2];
    const char* path_svg = argv[3];
    const char* path_txt = (argc >= 5 ? argv[4] : "saida.txt");

    // Estruturas principais
    void* chao  = cria_fila();  // Fila* de FORMA (formas “deitadas”)
    void* arena = cria_fila();  // Fila* de FORMA (formas “no ar”)
    if (!chao || !arena) {
        fprintf(stderr, "erro: falha ao criar filas (chao/arena)\n");
        return 1;
    }

    Processador* proc = cria_processador();
    if (!proc) {
        fprintf(stderr, "erro: falha ao criar processador\n");
        return 1;
    }

    // 1) Carrega o .geo no chão
    GEO_LOADER gl = geo_loader_create(chao);
    if (!gl) {
        fprintf(stderr, "erro: geo_loader_create\n");
        return 1;
    }
    if (!geo_loader_load_file(gl, path_geo)) {
        fprintf(stderr, "erro ao ler .geo: %s\n", path_geo);
        geo_loader_destroy(gl);
        esvazia_fila_formas(chao);
        esvazia_fila_formas(arena);
        libera_processador(proc);
        return 1;
    }
    geo_loader_destroy(gl);

    // 2) Executa o .qry (gera .txt de log das operações)
    QRY_EXECUTOR qe = qry_exec_create(chao, arena, proc, path_txt);
    if (!qe) {
        fprintf(stderr, "erro: qry_exec_create (%s)\n", path_txt);
        esvazia_fila_formas(chao);
        esvazia_fila_formas(arena);
        libera_processador(proc);
        return 1;
    }
    if (!qry_exec_run_file(qe, path_qry)) {
        fprintf(stderr, "erro ao executar .qry: %s\n", path_qry);
        // continua, mas avisa — o .qry pode ter executado parcialmente
    }
    qry_exec_destroy(qe);

    // 3) Gera o SVG a partir do CHÃO (estado final após o QRY)
    //    Ajuste o tamanho conforme sua arena (viewport):
    if (svg_dump_fila(path_svg, chao, 1000.0, 800.0) != 0) {
        fprintf(stderr, "erro ao gerar SVG: %s\n", path_svg);
        // continua, mas avisa
    }

    // 4) Resumo no stdout (opcional)
    printf("Pontuação total: %.2f\n", get_pontuacao_total(proc));
    printf("Total de clones: %d\n",   get_total_clones(proc));
    printf("Total esmagadas: %d\n",   get_total_esmagadas(proc));
    printf("Formas no chão:  %d\n",   tamanho_fila(chao));
    printf("Formas na arena: %d\n",   tamanho_fila(arena));

    // 5) Limpeza completa
    esvazia_fila_formas(arena);
    esvazia_fila_formas(chao);
    // (se você tiver destrutores das filas/pilhas, chame-os aqui)
    libera_processador(proc);

    return 0;
}
