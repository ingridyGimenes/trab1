#pragma once
#include "fila.h"

// Lê o arquivo .geo e insere as formas na fila chão
void carregarGeo(const char* caminhoGeo, FILA filaChao);

// Função para liberar recursos do carregador (se necessário)
void destruirCarregador(void);

// Define estilo global de texto (ts)
void defineEstiloTexto(const char* family, const char* weight, const char* size);
