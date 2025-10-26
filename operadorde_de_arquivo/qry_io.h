#pragma once
#include <stdio.h>
#include "../tads_gerais/fila.h"

/**
 * @brief Executa os comandos de um arquivo .qry já aberto, manipulando as formas
 *        na fila do "chão" e registrando a saída textual.
 *
 * @param qry       Ponteiro para o arquivo .qry aberto para leitura.
 * @param fila_chao Fila contendo as formas disponíveis no "chão".
 * @param txt_out   Ponteiro para o arquivo de saída textual onde os relatórios serão escritos.
 *
 * @return Número total de instruções executadas.
 *
 * @details Comandos suportados:
 *          - pd   : posiciona disparador
 *          - lc   : carrega formas do chão em um carregador
 *          - atch : acopla carregadores a um disparador
 *          - shft : rota a carga para a posição de disparo (esquerda/direita)
 *          - dsp  : dispara a forma posicionada
 *          - rjd  : executa rajada de disparos (shft + dsp iterativos)
 *          - calc : processa sobreposições na arena e pontuação
 *
 * @note Para cada comando, as informações requeridas são registradas em @p txt_out.
 * @note O comando de estilo de texto (ts) pertence ao processamento do .geo; aqui utilizam-se os dados já carregados.
 *
 * 
 */
int qry_executar(FILE *qry, FILA fila_chao, FILE *txt_out);
