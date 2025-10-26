#pragma once
#include "../tads_gerais/fila.h"

/**
 * @typedef CARREGADOR
 * @brief Tipo opaco para representar o carregador responsável por gerenciar
 *        o carregamento inicial de formas e estilo global de texto.
 */
typedef void* CARREGADOR;

/**
 * @brief Lê um arquivo .geo e insere as formas criadas na fila "chao" na ordem em que aparecem.
 *
 * @param caminhoGeo Caminho para o arquivo .geo.
 * @param filaChao   Fila onde as formas serão inseridas.
 *
 * @note A função interpreta comandos de criação de formas e adiciona cada forma lida
 *       ao final da fila, mantendo a ordem original.
 */
void carregarGeo(const char* caminhoGeo, FILA filaChao);

/**
 * @brief Define o estilo global de texto usado para as formas de texto subsequentes.
 *
 * @param family Família da fonte (ex: "sans", "serif").
 * @param weight Peso da fonte (ex: "normal", "bold").
 * @param size   Tamanho da fonte.
 *
 * @note Corresponde ao comando `ts` no arquivo .geo.
 */
void defineEstiloTexto(const char* family, const char* weight, const char* size);

/**
 * @brief Libera os recursos internos do carregador, incluindo dados de estilo global.
 */
void destruirCarregador(void);
