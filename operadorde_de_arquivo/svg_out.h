#pragma once
#include <stdio.h>
#include "../tads_gerais/fila.h"
#include "../formas/forma.h"

/* Inicia um SVG com viewport dado.
 * width/height são apenas hints; se não souber, use algo como 1000x1000.
 */
void svg_begin(FILE* svg, double width, double height);

/* Finaliza o arquivo SVG (fecha a tag raiz). */
void svg_end(FILE* svg);

/* Desenha todas as FORMAS existentes na Fila `chao` SEM alterar sua ordem.
 * Para preservar a fila, a função remove cada elemento e re-enfileira ao final.
 */
void svg_draw_chao(FILE* svg, FILA chao);

/* Anota um asterisco vermelho (p.ex., posição de forma esmagada).
 * Tamanho controla o “comprimento” dos braços do asterisco.
 */
void svg_mark_asterisk(FILE* svg, double x, double y, double size);

/* Anota uma “caixa”/medida de disparo (quando flag `v` do dsp).
 * Desenha um retângulo tracejado e (opcionalmente) um rótulo.
 * Se label for NULL, não imprime texto.
 */
void svg_mark_shot_box(FILE* svg, double x, double y, double w, double h, const char* label);
