#pragma once
#include <stdbool.h>

/**
 * @typedef FORMA
 * @brief Tipo genérico opaco para representar qualquer forma geométrica
 *        (círculo, retângulo, linha ou texto).
 */
typedef void* FORMA;

/*
 * @brief Cria uma nova forma genérica a partir de um tipo e de um objeto.
 * @param tipo Caractere que identifica o tipo da forma:
 *        - 'c' - círculo
 *        - 'r' - retângulo
 *        - 'l' - linha
 *        - 't' - texto
 * @param objeto Ponteiro para o objeto específico da forma.
 * @return Ponteiro para a forma criada.
 */
FORMA criaForma(char tipo, void* objeto);

/**
 * @brief Retorna o tipo da forma armazenada.
 * @param f Ponteiro para a forma.
 * @return Caractere do tipo da forma ('c', 'r', 'l', 't').
 */
char tipoForma(FORMA f);

/**
 * @brief Retorna o ponteiro para o objeto interno associado à forma.
 * @param f Ponteiro para a forma.
 * @return Ponteiro genérico (void*) para o objeto específico.
 */
void* objetoForma(FORMA f);

/**
 * @brief Libera os recursos associados à forma.
 * @param f Ponteiro para a forma a ser destruída.
 */
void destruirForma(FORMA f);


/** 
 * @brief Verifica se duas formas geométricas se intersectam.
 * @param A Ponteiro para a primeira forma.
 * @param B Ponteiro para a segunda forma.
 * @return true se as formas tiverem interseção; false caso contrário.
 */
bool formasIntersectam(FORMA A, FORMA B);

/**
 * @brief Calcula a área de uma forma genérica.
 * @param F Ponteiro para a forma.
 * @return Área da forma (em unidades quadradas).
 */
double areaForma(FORMA F);



/**
 * @brief Define a cor de borda da forma B igual à cor de preenchimento da forma A.
 * @param A Ponteiro para a forma fonte (cor de preenchimento).
 * @param B Ponteiro para a forma destino (cor de borda).
 */
void bordaDeBRecebeCorpDeA(FORMA A, FORMA B);

/**
 * @brief Cria um clone da forma A com cores de borda e preenchimento trocadas.
 * @param A Ponteiro para a forma a ser clonada.
 * @param novoId Novo identificador a ser atribuído ao clone.
 * @return Ponteiro para a forma clonada com cores invertidas.
 */
FORMA clonaFormaComCoresTrocadas(FORMA A, int novoId);
