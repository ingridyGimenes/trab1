#ifndef CARREGADOR_ARQUIVOS_H
#define CARREGADOR_ARQUIVOS_H

#include "/workspaces/trab1/tads_gerais/fila.h"
#include "/workspaces/trab1/formas/circulo.h"
#include "/workspaces/trab1/formas/retangulo.h"
#include "/workspaces/trab1/formas/linha.h"
#include "/workspaces/trab1/formas/texto.h"

// Lê um arquivo .geo e insere as formas geométricas em uma fila (o chão)
void carregarGeo(const char *nomeArquivo, Fila *chao);

#endif
