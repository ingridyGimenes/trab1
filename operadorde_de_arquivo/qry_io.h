#pragma once
#include <stdio.h>
#include "../tads_gerais/fila.h"
#include "../operadorde_de_arquivo/svg_out.h"

/* Vincula o contexto SVG usado para anotações do .qry (setas de dsp[v] e asteriscos do calc).
   Deve ser chamado pelo main.c após abrir o SVG do .qry (ex.: svg_begin_qry(...)).
*/
void qry_bind_svg(SVG sctx);

/* Executa um arquivo .qry sobre:
   - fila_chao: FILA com as formas do chão (já carregada ao fim do .geo)
   - txt_out  : arquivo de saída textual (.txt) para logs/relatórios
   Observações:
   - Se um SVG estiver vinculado via qry_bind_svg(), comandos 'dsp ... v' serão anotados
     no SVG e 'calc' marcará as posições de esmagamento com asterisco.
   Retorna: número de instruções executadas.
*/
int qry_executar(FILE *qry, FILA fila_chao, FILE *txt_out);
