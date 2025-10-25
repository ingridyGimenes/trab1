#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pilha.h"

/*
  IMPLEMENTAÇÃO DE PILHA ENCADEADA GENÉRICA
  - Nós e a struct Pilha ficam APENAS neste .c (proibido expor no .h)
  - Retornos e mensagens padronizados
  - Funções extras utilitárias: topo, destruir, esvaziar com destrutor opcional
*/

typedef struct NO {
    void *dado;
    struct NO *prox;
} no;

typedef no* pont_no;

typedef struct {
    pont_no comeco; // topo da pilha
    int tam;
} Pilha;

typedef Pilha* pont_p;

// --- Criação / Destruição ---------------------------------------------------

void* cria_pilha(void) {
    pont_p p = (pont_p) malloc(sizeof(Pilha));
    if (p == NULL) {
        fprintf(stderr, "erro ao alocar pilha\n");
        return NULL;
    }
    p->comeco = NULL;
    p->tam = 0;
    return (void*) p;
}

/*
 * Esvazia todos os nós da pilha.
 * Se for passado um destrutor (func que recebe void*), aplica ao dado antes de liberar o nó.
 */
void esvazia_pilha(void* pilha, void (*destrutor)(void*)) {
    if (pilha == NULL) return;
    pont_p p = (pont_p) pilha;
    pont_no aux = p->comeco;
    while (aux) {
        pont_no proximo = aux->prox;
        if (destrutor) destrutor(aux->dado);
        free(aux);
        aux = proximo;
    }
    p->comeco = NULL;
    p->tam = 0;
}

/*
 * Destrói a pilha por completo (nós + struct Pilha).
 * Aceita destrutor opcional para os dados.
 */
void destruir_pilha(void* pilha, void (*destrutor)(void*)) {
    if (pilha == NULL) return;
    esvazia_pilha(pilha, destrutor);
    free((pont_p) pilha);
}

// --- Operações básicas ------------------------------------------------------

bool add_na_pilha(void* pilha, void* valor) {
    if (pilha == NULL) {
        fprintf(stderr, "pilha nula em add_na_pilha\n");
        return false;
    }
    pont_p p = (pont_p) pilha;
    pont_no temp = (pont_no) malloc(sizeof(no));
    if (temp == NULL) {
        fprintf(stderr, "erro ao alocar novo no\n");
        return false;
    }
    temp->dado = valor;
    temp->prox = p->comeco;
    p->comeco = temp;
    p->tam++;
    return true;
}

void* retira_da_pilha(void* pilha) {
    if (pilha == NULL) {
        fprintf(stderr, "pilha nula em retira_da_pilha\n");
        return NULL;
    }
    pont_p p = (pont_p) pilha;
    pont_no aux = p->comeco;
    if (aux == NULL) {
        // pilha vazia não é erro fatal; apenas retorna NULL
        return NULL;
    }
    void* valor = aux->dado;
    p->comeco = aux->prox;
    aux->prox = NULL; // higiene
    free(aux);
    p->tam--;
    return valor;
}

void* primeiro_pilha(void* pilha) { // topo
    if (pilha == NULL) return NULL;
    pont_p p = (pont_p) pilha;
    return p->comeco ? p->comeco->dado : NULL;
}

void* segundo_pilha(void* pilha) { // segundo elemento a partir do topo
    if (pilha == NULL) return NULL;
    pont_p p = (pont_p) pilha;
    return (p->comeco && p->comeco->prox) ? p->comeco->prox->dado : NULL;
}

int tamanho_pilha(void* pilha) {
    if (pilha == NULL) return 0;
    pont_p p = (pont_p) pilha;
    return p->tam;
}

bool pilha_vazia(void* pilha) {
    if (pilha == NULL) return true;
    pont_p p = (pont_p) pilha;
    return (p->comeco == NULL);
}

// --- Debug ------------------------------------------------------------------

void imprime_pilha(void* pilha, void (*print_dado)(const void*)) {
    if (pilha == NULL) {
        printf("pilha: (nula)\n");
        return;
    }
    pont_p p = (pont_p) pilha;
    pont_no aux = p->comeco;
    printf("pilha (tam=%d): ", p->tam);
    while (aux) {
        if (print_dado) print_dado(aux->dado);
        else printf("%p", aux->dado);
        if (aux->prox) printf(" -> ");
        aux = aux->prox;
    }
    printf("\n");
}
