#pragma once
#include <stdio.h>
#include "../tads_gerais/fila.h"

/* Executa um arquivo .qry sobre:
   - fila_chao: FILA com as formas do chão (já carregada ao fim do .geo)
   - txt_out  : arquivo de saída textual (.txt) para logs/relatórios
   Retorna: número de instruções executadas.
*/
int qry_executar(FILE *qry, FILA fila_chao, FILE *txt_out);
