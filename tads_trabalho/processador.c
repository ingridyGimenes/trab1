#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "processador.h"
#include "/workspaces/trab1/tads_gerais/fila.h"
#include "geometria.h"   // dispatcher genérico de interseção & áreas (ver notas)
#include "/workspaces/trab1/formas/forma.h"
/* --------------------------------------------------------------------------
   PROCESSADOR
   - Mantém pontuação e contadores
   - Processa a FILA "arena" de FORMAS (cada item: {tipo, ptr}) na ordem de lançamento
   - Aplica as regras do enunciado para pares (i, i+1)

   Requisitos de integração:
   - A Fila deve conter ponteiros para objetos "FORMA" (wrapper opaco do jogo),
     definidos em geometria.h:

       typedef struct {
           char  tipo;  // 'r','c','l','t'
           void *obj;   // ponteiro real para o TAD específico
       } Forma;

   - geometria.h deve expor estas funções (implementadas por você nos TADs específicos):
       bool    formas_intersectam(const Forma* A, const Forma* B);
       double  area_forma(const Forma* F);
       void    borda_de_B_recebe_corp_de_A(Forma* A, Forma* B); // muda cor da borda de B
       Forma*  clona_forma_com_cores_trocadas(const Forma* A, int novoId);
       void    destruir_forma(Forma* F); // desaloca a forma concreta + o wrapper

   - O gerenciador de IDs deve fornecer um próximo id para a clonagem:
       int     next_id(void);
   -------------------------------------------------------------------------- */

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

static void devolve_ao_chao(void* chao, Forma* f) {
    if (!f) return;
    add_na_fila(chao, f);
}

/*
 * Regras do enunciado, por par (I,J), na ordem de lançamento:
 * - Se sobrepõe:
 *   - area(I) < area(J): I é destruído (pontua area(I)), J volta ao chão
 *   - area(I) > area(J): I muda a cor da BORDA de J para corp(I), ambos voltam ao chão
 *                        e cria-se um CLONE de I com cores trocadas (entra após I e J)
 *   - áreas iguais: ambos voltam ao chão sem alterações
 * - Se NÃO sobrepõe: ambos voltam ao chão
 */

double processa_formas(Processador* p, void* arena, void* chao, int (*next_id)(void)) {
    if (!p || !arena || !chao) return 0.0;

    double area_esmagada_rodada = 0.0;

    // retira primeiro
    Forma* I = (Forma*) remove_da_fila(arena);

    while (I) {
        Forma* J = (Forma*) remove_da_fila(arena); // pode ser NULL se sobrar um

        if (!J) {
            // ímpar: não há par para I → volta ao chão na mesma ordem relativa
            devolve_ao_chao(chao, I);
            I = NULL;
            break;
        }

        bool colide = formas_intersectam(I, J);
        if (colide) {
            double aI = area_forma(I);
            double aJ = area_forma(J);

            if (aI < aJ) {
                // I esmagado
                area_esmagada_rodada += aI;
                p->total_esmagadas++;
                devolve_ao_chao(chao, J);
                destruir_forma(I);

            } else if (aI > aJ) {
                // I altera borda de J; ambos voltam; clona I com cores trocadas
                area_esmagada_rodada += aJ;
                p->total_esmagadas++;

                borda_de_B_recebe_corp_de_A(I, J);
                devolve_ao_chao(chao, I);
                devolve_ao_chao(chao, J);

                int novoId = next_id ? next_id() : 0; // se não houver, 0 (ajuste se preferir exigir)
                Forma* clone = clona_forma_com_cores_trocadas(I, novoId);
                if (clone) {
                    devolve_ao_chao(chao, clone);
                    p->total_clones++;
                }
            } else {
                // áreas iguais → sem esmagamento
                devolve_ao_chao(chao, I);
                devolve_ao_chao(chao, J);
            }
        } else {
            // sem sobreposição → ambos voltam ao chão
            devolve_ao_chao(chao, I);
            devolve_ao_chao(chao, J);
        }

        // próximo par
        I = (Forma*) remove_da_fila(arena);
    }

    p->pontuacao_total += area_esmagada_rodada;
    return area_esmagada_rodada;
}

// getters

double get_pontuacao_total(Processador* p) {
    return p ? p->pontuacao_total : 0.0;
}

int get_total_clones(Processador* p) {
    return p ? p->total_clones : 0;
}

int get_total_esmagadas(Processador* p) {
    return p ? p->total_esmagadas : 0;
}

void libera_processador(Processador* p) {
    if (p) free(p);
}
