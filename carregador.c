#include <stdio.h>
#include <stdlib.h>
#include "carregador.h"
#include "pilha.h"

typedef struct {
    int id;
    void* pilhaFormas; 
} CarregadorStruct;

Carregador cria_carregador(int id) {
    CarregadorStruct* c = (CarregadorStruct*) malloc(sizeof(CarregadorStruct));
    if (!c) {
        printf("Erro ao alocar carregador!\n");
        exit(1);
    }

    c->id = id;
    c->pilhaFormas = cria_pilha();
    return (void*) c;
}

void adiciona_forma(Carregador c, void* forma) {
    CarregadorStruct* car = (CarregadorStruct*) c;
    add_na_pilha(car->pilhaFormas, forma);
}

void* retira_forma(Carregador c) {
    CarregadorStruct* car = (CarregadorStruct*) c;
    return retira_da_pilha(car->pilhaFormas);
}

void* topo_carregador(Carregador c) {
    CarregadorStruct* car = (CarregadorStruct*) c;
    return primeiro_pilha(car->pilhaFormas);
}

int get_id_carregador(Carregador c) {
    return ((CarregadorStruct*)c)->id;
}

int qtd_formas_carregador(Carregador c) {
    CarregadorStruct* car = (CarregadorStruct*) c;
    return tamanho_pilha(car->pilhaFormas);
}

void imprime_carregador(Carregador c) {
    CarregadorStruct* car = (CarregadorStruct*) c;
    printf("Carregador %d:\n", car->id);
    printf("  Quantidade de formas: %d\n", tamanho_pilha(car->pilhaFormas));
    printf("  Conteúdo da pilha (endereços): ");
    imprime_pilha(car->pilhaFormas);
}

void esvazia_carregador(Carregador c) {
    CarregadorStruct* car = (CarregadorStruct*) c;
    esvazia_pilha(car->pilhaFormas);
}

void desaloca_carregador(Carregador c) {
    CarregadorStruct* car = (CarregadorStruct*) c;
    esvazia_pilha(car->pilhaFormas);
    free(car->pilhaFormas);
    free(car);
}
