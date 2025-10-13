#ifndef pilha_h
#define pilha_h


void cria_pilha();

void add_na_pilha(void* pilha, void* valor);

void* retira_da_pilha(void* pilha);

void imprime_pilha(void* pilha);

#endif