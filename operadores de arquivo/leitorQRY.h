#ifndef PROCESSADOR_QRY_H
#define PROCESSADOR_QRY_H

#include "/workspaces/trab1/tads_gerais/fila.h"
#include "/workspaces/trab1/formas/circulo.h"
#include "/workspaces/trab1/formas/retangulo.h"
#include "/workspaces/trab1/formas/linha.h"
#include "/workspaces/trab1/formas/texto.h"
#include "/workspaces/trab1/tads_trabalho/disparador.h"
#include "/workspaces/trab1/tads_trabalho/carregador.h"
#include "/workspaces/trab1/tads_trabalho/processador.h"

// Executa os comandos do arquivo .qry, modificando o estado do jogo
void processarQry(const char *nomeArquivo, Fila *chao);

#endif
