#include <stdio.h>
#include <stdlib.h>
#include "carregador.h"
#include "pilha.h"

struct carregador {
    int id;
    Pilha* formas;  // pilha de formas geométricas (void*)
};

// ------------------------------------------------------------

Carregador* cria_carregador(int id) {
    Carregador* c = (Carregador*) malloc(sizeof(Carregador));
    if (c == NULL) {
        fprintf(stderr, "Erro: falha ao alocar carregador.\n");
        return NULL;
    }

    c->formas = cria_pilha();
    if (c->formas == NULL) {
        free(c);
        return NULL;
    }

    c->id = id;
    return c;
}

int get_id_carregador(Carregador* c) {
    if (c == NULL) return -1;
    return c->id;
}

bool adiciona_forma_carregador(Carregador* c, void* forma) {
    if (c == NULL) return false;
    return add_na_pilha(c->formas, forma);
}

void* remove_forma_carregador(Carregador* c) {
    if (c == NULL) return NULL;
    return retira_da_pilha(c->formas);
}

void* topo_carregador(Carregador* c) {
    if (c == NULL) return NULL;
    return primeiro_pilha(c->formas);
}

bool carregador_vazio(Carregador* c) {
    if (c == NULL) return true;
    return pilha_vazia(c->formas);
}

int tamanho_carregador(Carregador* c) {
    if (c == NULL) return 0;
    return tamanho_pilha(c->formas);
}

void libera_carregador(Carregador* c) {
    if (c == NULL) return;
    esvazia_pilha(c->formas);
    free(c);
}
