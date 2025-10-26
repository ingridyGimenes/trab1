#pragma once
#include <stdio.h>
#include <stdbool.h>
#include "../tads_gerais/fila.h"

/**
 * @file geo_io.h
 * @brief Leitura do arquivo .geo e povoamento da Fila "chão" com as formas na ordem de criação.
 *
 * Regras do PDF:
 * - Comandos suportados: c, r, l, t, ts (estilo de texto para comandos t subsequentes).
 * - A função NÃO desenha SVG; só lê e empilha as formas no "chão".
 * - A ordem de enfileiramento é a ordem de aparição no .geo.
 *
 * Erros de parsing são reportados via stderr com linha/coluna sempre que possível.
 */

/**
 * @brief Lê um arquivo .geo do caminho informado e enfileira as formas na FILA chao.
 * @param caminho_geo Caminho do arquivo .geo.
 * @param chao FILA onde as formas serão enfileiradas (na ordem de criação).
 * @return true em sucesso; false em erro (mensagens no stderr).
 */
bool geo_ler_arquivo(const char* caminho_geo, FILA chao);

/**
 * @brief Lê de um stream já aberto (.geo) e enfileira as formas na FILA chao.
 * @param fin Stream aberto para leitura (não é fechado pela função).
 * @param chao FILA onde as formas serão enfileiradas.
 * @return true em sucesso; false em erro.
 */
bool geo_ler_stream(FILE* fin, FILA chao);
