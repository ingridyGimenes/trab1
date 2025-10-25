#pragma once
#include "../tads_gerais/fila.h"

typedef void* CARREGADOR;

// Lê o arquivo .geo e insere as formas na fila "chao" (na ordem de criação)
void carregarGeo(const char* caminhoGeo, FILA filaChao);

// Define o estilo global de texto (comando 'ts' no .geo)
void defineEstiloTexto(const char* family, const char* weight, const char* size);

// Libera recursos internos do carregador (estilo global)
void destruirCarregador(void);
