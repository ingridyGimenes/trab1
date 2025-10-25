#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fila.h"

/*
  IMPLEMENTAÇÃO DE FILA ENCADEADA GENÉRICA (FIFO)
  - Nós e a struct Fila ficam APENAS neste .c (não expor no .h)
  - Interface: cria_fila, destruir_fila, esvazia_fila, add_na_fila, remove_da_fila,
               primeiro_fila, segundo_fila, tamanho_fila, fila_vazia, imprime_fila
*/

typedef struct NO {
    void *dado;
    struct NO *prox;
} no;

typedef no* pont_no;

typedef struct {
    pont_no comeco; // frente da fila (deonde removemos)
    pont_no fim;    // traseira da fila (onde inserimos)
    int tam;
} Fila;

typedef Fila* pont_f;

// --- Criação / Destruição ---------------------------------------------------

void* cria_fila(void) {
    pont_f f = (pont_f) malloc(sizeof(Fila));
    if (f == NULL) {
        fprintf(stderr, "erro ao alocar memoria da fila\n");
        return NULL;
    }
    f->comeco = NULL;
    f->fim = NULL;
    f->tam = 0;
    return (void*) f;
}

/* Esvazia todos os nós. Aceita destrutor opcional para os dados. */
void esvazia_fila(void* fila, void (*destrutor)(void*)) {
    if (fila == NULL) return;
    pont_f f = (pont_f) fila;
    pont_no aux = f->comeco;
    while (aux) {
        pont_no proximo = aux->prox;
        if (destrutor) destrutor(aux->dado);
        free(aux);
        aux = proximo;
    }
    f->comeco = NULL;
    f->fim = NULL;
    f->tam = 0;
}

/* Destrói a fila inteira (nós + struct Fila). */
void destruir_fila(void* fila, void (*destrutor)(void*)) {
    if (fila == NULL) return;
    esvazia_fila(fila, destrutor);
    free((pont_f) fila);
}

// --- Operações básicas ------------------------------------------------------

bool add_na_fila(void* fila, void* valor) {
    if (fila == NULL) {
        fprintf(stderr, "fila nula em add_na_fila\n");
        return false;
    }
    pont_f f = (pont_f) fila;
    pont_no temp = (pont_no) malloc(sizeof(no));
    if (temp == NULL) {
        fprintf(stderr, "erro ao alocar novo no\n");
        return false;
    }
    temp->dado = valor;
    temp->prox = NULL;

    if (f->fim == NULL) { // fila estava vazia
        f->comeco = temp;
        f->fim = temp;
    } else {
        f->fim->prox = temp;
        f->fim = temp;
    }
    f->tam++;
    return true;
}

void* remove_da_fila(void* fila) {
    if (fila == NULL) {
        fprintf(stderr, "fila nula em remove_da_fila\n");
        return NULL;
    }
    pont_f f = (pont_f) fila;
    if (f->comeco == NULL) { // vazia
        return NULL;
    }

    pont_no aux = f->comeco;
    void* valor = aux->dado;
    f->comeco = aux->prox;
    if (f->comeco == NULL) {
        f->fim = NULL; // ficou vazia
    }
    aux->prox = NULL; // higiene
    free(aux);
    f->tam--;
    return valor;
}

void* primeiro_fila(void* fila) { // frente
    if (fila == NULL) return NULL;
    pont_f f = (pont_f) fila;
    return f->comeco ? f->comeco->dado : NULL;
}

void* segundo_fila(void* fila) { // segundo elemento
    if (fila == NULL) return NULL;
    pont_f f = (pont_f) fila;
    return (f->comeco && f->comeco->prox) ? f->comeco->prox->dado : NULL;
}

int tamanho_fila(void* fila) {
    if (fila == NULL) return 0;
    pont_f f = (pont_f) fila;
    return f->tam;
}

bool fila_vazia(void* fila) {
    if (fila == NULL) return true;
    pont_f f = (pont_f) fila;
    return (f->comeco == NULL);
}

// --- Debug ------------------------------------------------------------------

void imprime_fila(void* fila, void (*print_dado)(const void*)) {
    if (fila == NULL) {
        printf("fila: (nula)\n");
        return;
    }
    pont_f f = (pont_f) fila;
    pont_no aux = f->comeco;
    printf("fila (tam=%d): ", f->tam);
    while (aux) {
        if (print_dado) print_dado(aux->dado);
        else printf("%p", aux->dado);
        if (aux->prox) printf(" -> ");
        aux = aux->prox;
    }
    printf("\n");
}
