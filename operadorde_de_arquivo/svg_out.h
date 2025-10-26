#pragma once
#include <stdio.h>
#include "../tads_gerais/fila.h"
#include "../formas/forma.h"

/**
 * @brief Inicia um documento SVG definindo a viewport.
 *
 * @param svg    Ponteiro para arquivo aberto para escrita do SVG.
 * @param width  Largura sugerida da viewport.
 * @param height Altura sugerida da viewport.
 *
 * @note A função escreve o cabeçalho e a tag raiz do SVG.
 */
void svg_begin(FILE* svg, double width, double height);

/**
 * @brief Finaliza o documento SVG.
 *
 * @param svg Ponteiro para o arquivo SVG.
 *
 * @note Fecha a tag raiz do SVG e garante término apropriado do documento.
 */
void svg_end(FILE* svg);

/**
 * @brief Desenha todas as formas presentes na fila "chao" sem alterar a ordem final.
 *
 * @param svg  Ponteiro para o arquivo SVG.
 * @param chao Fila contendo as formas a serem desenhadas.
 *
 * @details Para preservar a ordem, cada elemento é desenhado, removido da frente
 *          e reenfileirado ao final, mantendo a sequência original ao término.
 */
void svg_draw_chao(FILE* svg, FILA chao);

/**
 * @brief Desenha um asterisco vermelho em uma coordenada.
 *
 * @param svg  Ponteiro para o arquivo SVG.
 * @param x    Coordenada x do centro do asterisco.
 * @param y    Coordenada y do centro do asterisco.
 * @param size Comprimento dos “braços” do asterisco.
 */
void svg_mark_asterisk(FILE* svg, double x, double y, double size);

/**
 * @brief Desenha uma caixa de marcação de disparo (tracejada) e, opcionalmente, um rótulo.
 *
 * @param svg   Ponteiro para o arquivo SVG.
 * @param x     Coordenada x do canto inferior esquerdo da caixa.
 * @param y     Coordenada y do canto inferior esquerdo da caixa.
 * @param w     Largura da caixa.
 * @param h     Altura da caixa.
 * @param label Texto do rótulo; se for NULL, nada é rotulado.
 */
void svg_mark_shot_box(FILE* svg, double x, double y, double w, double h, const char* label);
