#pragma once
#include <stdio.h>
#include "../tads_gerais/fila.h"

/* Lê e processa um arquivo .geo, criando as formas e
   enfileirando-as no chão (FILA) na ordem de criação.

   Parâmetros:
     - geo       : FILE* já aberto do .geo
     - fila_chao : FILA onde as formas criadas serão inseridas
     - txt_out   : (opcional) logs textuais

   Retorna: número de instruções (linhas de comando) executadas.
*/
int geo_executar(FILE *geo, FILA fila_chao, FILE *txt_out);
