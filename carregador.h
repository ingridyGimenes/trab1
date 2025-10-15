#ifndef CARREGADOR_H
#define CARREGADOR_H

// Tipo opaco
typedef void* Carregador;

// Cria um novo carregador com identificador id
Carregador cria_carregador(int id);

// Adiciona uma forma ao carregador (usa a pilha interna)
void adiciona_forma(Carregador c, void* forma);

// Remove a forma do topo (retorna ponteiro)
void* retira_forma(Carregador c);

// Retorna a forma do topo (sem remover)
void* topo_carregador(Carregador c);

// Retorna o id do carregador
int get_id_carregador(Carregador c);

// Retorna quantas formas há no carregador
int qtd_formas_carregador(Carregador c);

// Mostra informações do carregador
void imprime_carregador(Carregador c);

// Esvazia e libera memória
void esvazia_carregador(Carregador c);
void desaloca_carregador(Carregador c);

#endif
