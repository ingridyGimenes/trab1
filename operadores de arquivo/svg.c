#include "svg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- Implementação interna das anotações --- */
typedef struct {
    double x, y;
} Anotacao;

static Anotacao *anotacoes = NULL;
static size_t anot_n = 0;
static size_t anot_cap = 0;

void svg_anotar_esmagado(double x, double y) {
    if (anot_n + 1 > anot_cap) {
        size_t nova = (anot_cap == 0) ? 8 : anot_cap * 2;
        Anotacao *tmp = realloc(anotacoes, nova * sizeof(Anotacao));
        if (!tmp) return; /* falha silenciosa (pode trocar por erro) */
        anotacoes = tmp;
        anot_cap = nova;
    }
    anotacoes[anot_n].x = x;
    anotacoes[anot_n].y = y;
    anot_n++;
}

void svg_limpa_anotacoes(void) {
    free(anotacoes);
    anotacoes = NULL;
    anot_n = 0;
    anot_cap = 0;
}

/* --- Cabeçalho e rodapé SVG --- */
void svg_inicia(FILE *f, double width, double height) {
    fprintf(f, "<?xml version=\"1.0\" standalone=\"no\"?>\n");
    fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" ");
    fprintf(f, "version=\"1.1\" viewBox=\"0 0 %.2f %.2f\" ", width, height);
    fprintf(f, "width=\"%.2f\" height=\"%.2f\">\n", width, height);
    /* fundo branco para visualização */
    fprintf(f, "<rect x=\"0\" y=\"0\" width=\"%.2f\" height=\"%.2f\" fill=\"white\" />\n", width, height);
}

void svg_termina(FILE *f) {
    fprintf(f, "</svg>\n");
}

/* --- Função utilitária: imprime as anotações (asteriscos vermelhos) --- */
static void svg_imprime_anotacoes(FILE *f) {
    for (size_t i = 0; i < anot_n; ++i) {
        double x = anotacoes[i].x;
        double y = anotacoes[i].y;
        /* desenha um pequeno asterisco com linhas */
        double s = 6.0;
        fprintf(f, "<g stroke=\"red\" stroke-width=\"1.5\">\n");
        fprintf(f, "  <line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" />\n", x - s, y, x + s, y);
        fprintf(f, "  <line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" />\n", x, y - s, x, y + s);
        fprintf(f, "  <line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" />\n", x - s*0.7, y - s*0.7, x + s*0.7, y + s*0.7);
        fprintf(f, "  <line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" />\n", x - s*0.7, y + s*0.7, x + s*0.7, y - s*0.7);
        fprintf(f, "</g>\n");
    }
}

/* --- Integração com as formas --- 
   Este trecho supõe que existe:
     char forma_get_tipo(void *f); // 'c','r','l','t'
     void circulo_svg_imprime(FILE*, void*);
     void retangulo_svg_imprime(FILE*, void*);
     void linha_svg_imprime(FILE*, void*);
     void texto_svg_imprime(FILE*, void*);
   Ajuste os nomes conforme suas implementações.
*/

extern char forma_get_tipo(void *f);
extern void circulo_svg_imprime(FILE *out, void *f);
extern void retangulo_svg_imprime(FILE *out, void *f);
extern void linha_svg_imprime(FILE *out, void *f);
extern void texto_svg_imprime(FILE *out, void *f);

/* --- Gera SVG a partir da fila 'chao' sem esvaziá-la.
   Observação: como a API da sua fila pode não permitir iteração
   não-destrutiva, aqui assumimos que existe uma função
     void fila_iterar(Fila *fila, void (*fn)(void*, void*), void *ctx);
   Se não tiver, podemos adaptar (por ex. desenfileirar e reenfileirar).
*/
static void imprime_forma_no_svg(FILE *out, void *forma) {
    char tipo = forma_get_tipo(forma);
    switch (tipo) {
        case 'c': circulo_svg_imprime(out, forma); break;
        case 'r': retangulo_svg_imprime(out, forma); break;
        case 'l': linha_svg_imprime(out, forma); break;
        case 't': texto_svg_imprime(out, forma); break;
        default:
            /* se for um tipo desconhecido, tentar imprimir baseado em tipo */
            break;
    }
}

/* Se sua fila NÃO tem função de iterar sem remover, usamos a estratégia
   desenfileirar+reenfileirar para percorrer sem alterar a ordem final.
   A implementação abaixo assume que sua fila implementa:
     void *desenfileira(Fila *f);
     void enfileira(Fila *f, void *d);
     int fila_vazia(Fila *f);
   Ajuste os nomes conforme seu TAD Fila.
*/

/* Nomes de funções que podem diferir no seu projeto; ajuste se preciso */
extern void *desenfileira(Fila *f);
extern void enfileira(Fila *f, void *d);
extern int fila_vazia(Fila *f);

void svg_gera_do_chao(const char *nomeArquivo, Fila *chao, double width, double height) {
    FILE *out = fopen(nomeArquivo, "w");
    if (!out) {
        fprintf(stderr, "Erro: nao foi possivel criar %s\n", nomeArquivo);
        return;
    }

    svg_inicia(out, width, height);

    /* percorre a fila sem perder a ordem: desenfileira e reenfileira */
    size_t n = 0;
    /* para descobrir o tamanho sem acesso direto vamos percorrer até voltar a cabeça:
       assumimos que a fila não é circular desconhecida — para segurança, deslocamos até encontrar
       todas as formas: verificamos fila_vazia antes e usamos contador com limite alto (proteção). */

    /* técnica: se fila vazia, nada a fazer */
    if (!fila_vazia(chao)) {
        /* primeiro pass: desenfileirar tudo, imprimir e reenfileirar */
        /* NOTA: este loop conserva a ordem original */
        while (!fila_vazia(chao)) {
            void *f = desenfileira(chao);
            if (f == NULL) break;
            /* imprime a forma */
            imprime_forma_no_svg(out, f);
            /* reenfileira para manter o chão */
            enfileira(chao, f);
            n++;
            /* segurança: limite razoável para evitar loop infinito */
            if (n > 2000000) break;
        }
    }

    /* imprime as anotações (asteriscos vermelhos) */
    svg_imprime_anotacoes(out);

    svg_termina(out);
    fclose(out);

    /* após gerar o SVG, habitualmente queremos limpar anotações */
    svg_limpa_anotacoes();
}
