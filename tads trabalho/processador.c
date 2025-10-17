#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "processador.h"
#include "fila.h"



struct processador {
    double pontuacao_total;
    int total_clones;
    int total_esmagadas;
};



Processador* cria_processador(void) {
    Processador* p = (Processador*) malloc(sizeof(Processador));
    if (!p) {
        fprintf(stderr, "Erro: falha ao alocar processador.\n");
        return NULL;
    }

    p->pontuacao_total = 0.0;
    p->total_clones = 0;
    p->total_esmagadas = 0;
    return p;
}

/**
 * Determina se há sobreposição entre duas formas genéricas.
 * (Simples — aqui apenas exemplificando círculos e retângulos;
 * em um projeto completo, haveria função polimórfica para cada tipo.)
 */
static bool sobrepoe_formas(void* f1, void* f2, char tipo1, char tipo2) {
    if (tipo1 == 'c' && tipo2 == 'c')
        return sobrepoe_circulo((Circulo*)f1, (Circulo*)f2);

    // outros tipos (retângulo, linha, texto) seriam verificados aqui
    return false;
}

/**
 * Calcula área de uma forma genérica.
 */
static double area_forma(void* f, char tipo) {
    if (tipo == 'c')
        return area_circulo((Circulo*)f);
    // outros tipos também poderiam ser incluídos aqui
    return 0.0;
}

/**
 * Cria clone de uma forma genérica (apenas círculo por enquanto).
 */
static void* clona_forma(void* f, char tipo, int novo_id) {
    if (tipo == 'c')
        return clona_circulo((Circulo*)f, novo_id);
    return NULL;
}

/**
 * Processa a fila da arena conforme as regras do enunciado.
 */
double processa_formas(Processador* p, Fila* arena, Fila* chao) {
    if (!p || !arena || fila_vazia(arena)) return 0.0;

    double area_esmagada_rodada = 0.0;
    int proximo_id_clone = 10000; // exemplo de base para IDs de clones

    void* f1 = remove_da_fila(arena);
    void* f2 = NULL;

    while (!fila_vazia(arena)) {
        f2 = remove_da_fila(arena);

        // para simplificação, vamos supor que ambas são círculos
        if (sobrepoe_circulo((Circulo*)f1, (Circulo*)f2)) {
            double a1 = area_circulo((Circulo*)f1);
            double a2 = area_circulo((Circulo*)f2);

            if (a1 < a2) {
                // f1 é esmagado
                area_esmagada_rodada += a1;
                p->total_esmagadas++;
                // f2 volta para o chão
                insere_na_fila(chao, f2);
                libera_circulo((Circulo*)f1);
            } else if (a1 > a2) {
                // f2 tem borda trocada e ambos voltam para o chão
                area_esmagada_rodada += a2;
                p->total_esmagadas++;

                // altera a cor de borda de f2 para a de preenchimento de f1
                Circulo* c1 = (Circulo*)f1;
                Circulo* c2 = (Circulo*)f2;
                free(c2->corb);
                c2->corb = strdup(c1->corp);

                insere_na_fila(chao, f1);
                insere_na_fila(chao, f2);

                // cria clone com cores trocadas
                Circulo* clone = clona_circulo(c1, proximo_id_clone++);
                insere_na_fila(chao, clone);
                p->total_clones++;
            } else {
                // mesma área → sem esmagamento
                insere_na_fila(chao, f1);
                insere_na_fila(chao, f2);
            }
        } else {
            // sem sobreposição → voltam para o chão
            insere_na_fila(chao, f1);
            insere_na_fila(chao, f2);
        }

        // prepara próximo par
        if (!fila_vazia(arena))
            f1 = remove_da_fila(arena);
        else
            f1 = NULL;
    }

    p->pontuacao_total += area_esmagada_rodada;
    return area_esmagada_rodada;
}

double get_pontuacao_total(Processador* p) {
    if (!p) return 0.0;
    return p->pontuacao_total;
}

int get_total_clones(Processador* p) {
    if (!p) return 0;
    return p->total_clones;
}

int get_total_esmagadas(Processador* p) {
    if (!p) return 0;
    return p->total_esmagadas;
}

void libera_processador(Processador* p) {
    if (!p) return;
    free(p);
}
