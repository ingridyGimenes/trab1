#ifndef pilha_h
#define pilha_h

typedef void *Pilha;

void* cria_pilha();

bool add_na_pilha(void* pilha, void* valor);

void* retira_da_pilha(void* pilha);

void imprime_pilha(void* pilha);

void* esvazia_pilha(void* pilha);

void* primeiro_pilha(void* pilha);

void* segundo_pilha(void* pilha);

int tamanho_pilha(void* pilha);

bool pilha_vazia(void* pilha);

#endif