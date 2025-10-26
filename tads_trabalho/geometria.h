#pragma once
#include <stdbool.h>
#include "../formas/forma.h"

/**
 * Verifica se duas formas geométricas se intersectam.
 * Regras de interseção implementadas:
 *  - c–c, r–r, c–r
 *  - l–l
 *  - Texto tratado como segmento horizontal de comprimento 10*|t|:
 *      âncora 'i': [ (x,y) .. (x+cl,y) ]
 *      âncora 'f': [ (x-cl,y) .. (x,y) ]
 *      âncora 'm': [ (x-cl/2,y) .. (x+cl/2,y) ]
 *    Suporta t–t (seg–seg), t–l (seg–seg), t–r (seg–ret), t–c (seg–círc).
 */
bool geometriaFormasIntersectam(FORMA A, FORMA B);

/**
 * Calcula a área de uma forma genérica.
 * Convenções do projeto:
 *  - Círculo: π r²
 *  - Retângulo: w * h
 *  - Linha: 2.0 * comprimento
 *  - Texto: 20.0 * número de caracteres
 */
double geometriaAreaForma(FORMA F);

/**
 * Cria um clone de uma forma com as cores de borda e preenchimento invertidas.
 * Para linha (que não tem preenchimento), apenas clona.
 */
FORMA geometriaClonaFormaComCoresTrocadas(FORMA F, int novoId);

/**
 * Faz a cor de borda de B receber a cor de preenchimento de A.
 * (Para linha, usa-se a cor da linha.)
 */
void geometriaBordaDeBRecebeCorpDeA(FORMA A, FORMA B);

/**
 * Retorna a "âncora" da forma para marcação no SVG:
 *  - Círculo: centro
 *  - Retângulo: canto inferior esquerdo (âncora)
 *  - Texto: sua âncora (x,y)
 *  - Linha: ponto médio
 */
void geometriaAncoraDaForma(FORMA f, double *x, double *y);
