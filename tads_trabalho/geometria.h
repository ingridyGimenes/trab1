#pragma once
#include <stdbool.h>
#include "../formas/forma.h"

/**
 * @brief Verifica se duas formas geométricas se intersectam.
 *
 * @param A Ponteiro para a primeira forma.
 * @param B Ponteiro para a segunda forma.
 * @return true se as formas se intersectarem; false caso contrário.
 */
bool geometriaFormasIntersectam(FORMA A, FORMA B);

/**
 * @brief Calcula a área de uma forma genérica.
 *
 * @param F Ponteiro para a forma.
 * @return Valor da área da forma.
 *
 * @note Para linhas e textos, a área segue as convenções:
 *       - Linha: 2.0 * comprimento
 *       - Texto: 20.0 * número de caracteres.
 */
double geometriaAreaForma(FORMA F);

/**
 * @brief Cria um clone de uma forma com as cores de borda e preenchimento invertidas.
 *
 * @param F Ponteiro para a forma original.
 * @param novoId Identificador a ser atribuído ao clone.
 * @return Ponteiro para a forma clonada com cores invertidas.
 */
FORMA geometriaClonaFormaComCoresTrocadas(FORMA F, int novoId);

/**
 * @brief Faz a cor de borda de B receber a cor de preenchimento de A.
 *
 * @param A Ponteiro para a forma fonte (cor de preenchimento).
 * @param B Ponteiro para a forma destino (cor de borda).
 */
void geometriaBordaDeBRecebeCorpDeA(FORMA A, FORMA B);
