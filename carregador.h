#ifndef CARREGADOR_H
#define CARREGADOR_H


typedef void* Carregador;

Carregador cria_carregador(int id);


void adiciona_forma(Carregador c, void* forma);


void* retira_forma(Carregador c);


void* topo_carregador(Carregador c);

int get_id_carregador(Carregador c);


int qtd_formas_carregador(Carregador c);


void imprime_carregador(Carregador c);


void esvazia_carregador(Carregador c);
void desaloca_carregador(Carregador c);

#endif
