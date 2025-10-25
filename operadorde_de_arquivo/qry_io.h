#pragma once
#include <stdio.h>
#include "../tads_gerais/fila.h"

/**
 * Executa um arquivo .qry já aberto.
 *
 * Args:
 *   qry        : FILE* para o .qry
 *   fila_chao  : FILA com as formas “no chão”
 *   txt_out    : FILE* onde escrever a saída textual
 *
 * Retorna: número de instruções executadas.
 *
 * Implementa os comandos: pd, lc, atch, shft, dsp, rjd, calc
 * conforme o enunciado da Bocha Geométrica.
 *
 * Observação: por limitações da API atual, o comando `dsp` apenas
 * registra a posição alvo no TXT e coloca a forma na arena.
 * Se for fornecida uma API para reposicionar a âncora da forma,
 * atualizo o código para aplicar o deslocamento de fato.
 */
int qry_executar(FILE *qry, FILA fila_chao, FILE *txt_out);
