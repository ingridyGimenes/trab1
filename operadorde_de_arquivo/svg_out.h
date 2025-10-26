#pragma once
#include <stdio.h>
#include "../tads_gerais/fila.h"
#include "../formas/forma.h"

/* Contexto simples para escrever SVG */
typedef struct svg_ctx* SVG;

/* Abre um SVG com viewport/coord (minX,minY,maxX,maxY). 
   Se width/height <= 0, usa (maxX-minX) e (maxY-minY). */
SVG svg_begin(FILE* out, double minX, double minY, double maxX, double maxY,
              double width, double height);

/* Desenha todas as formas que estão na FILA (sem consumir a fila). */
void svg_draw_fila(SVG s, FILA f);

/* Desenha UMA forma (útil se você quer granularidade). */
void svg_draw_forma(SVG s, FORMA F);

/* Anota “dimensões do disparo” (para dsp com flag v).
   Desenha uma seta do (x0,y0) ao (x1,y1) e escreve rótulos. */
void svg_note_disparo(SVG s, double x0, double y0, double x1, double y1);

/* Marca um esmagamento com um asterisco vermelho no ponto (x,y). */
void svg_mark_esmagado(SVG s, double x, double y);

/* Fecha o SVG (escreve </svg>) */
void svg_end(SVG s);
