#ifndef fila_h
#define fila_h

typedef void *Fila;

void cria_fila();

void add_na_fila(void* fila, void* valor);

void* remove_da_fila(void* fila);

void imprime_fila(void* fila);

void* esvazia_fila(void* fila);

int tamanho_fila(void* fila);

void* primeiro_fila(void* fila);

void* segundo_fila(void* fila);

#endif