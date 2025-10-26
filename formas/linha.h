#pragma once

/**
 * @typedef LINHA
 * @brief Tipo opaco para representar uma linha no plano cartesiano.
 */
typedef void* LINHA;

/**
 * @brief Cria uma linha com coordenadas iniciais e finais e cor.
 * @param id Identificador único da linha.
 * @param x1 Coordenada x do ponto inicial.
 * @param y1 Coordenada y do ponto inicial.
 * @param x2 Coordenada x do ponto final.
 * @param y2 Coordenada y do ponto final.
 * @param cor Cor da linha.
 * @return Ponteiro para a linha criada.
 */
LINHA criaLinha(int id, double x1, double y1, double x2, double y2, const char* cor);

/**
 * @brief Retorna o identificador da linha.
 * @param l Ponteiro para a linha.
 * @return Identificador numérico.
 */
int getIdLinha(LINHA l);

/**
 * @brief Retorna a coordenada x do ponto inicial da linha.
 * @param l Ponteiro para a linha.
 * @return Coordenada x inicial.
 */
double getX1Linha(LINHA l);

/**
 * @brief Retorna a coordenada y do ponto inicial da linha.
 * @param l Ponteiro para a linha.
 * @return Coordenada y inicial.
 */
double getY1Linha(LINHA l);

/**
 * @brief Retorna a coordenada x do ponto final da linha.
 * @param l Ponteiro para a linha.
 * @return Coordenada x final.
 */
double getX2Linha(LINHA l);

/**
 * @brief Retorna a coordenada y do ponto final da linha.
 * @param l Ponteiro para a linha.
 * @return Coordenada y final.
 */
double getY2Linha(LINHA l);

/**
 * @brief Retorna a cor da linha.
 * @param l Ponteiro para a linha.
 * @return String com a cor.
 */
const char* getCorLinha(LINHA l);

/**
 * @brief Define um novo identificador para a linha.
 * @param l Ponteiro para a linha.
 * @param id Novo identificador.
 */
void setIdLinha(LINHA l, int id);

/**
 * @brief Define uma nova coordenada x para o ponto inicial da linha.
 * @param l Ponteiro para a linha.
 * @param x Novo valor de x inicial.
 */
void setX1Linha(LINHA l, double x);

/**
 * @brief Define uma nova coordenada y para o ponto inicial da linha.
 * @param l Ponteiro para a linha.
 * @param y Novo valor de y inicial.
 */
void setY1Linha(LINHA l, double y);

/**
 * @brief Define uma nova coordenada x para o ponto final da linha.
 * @param l Ponteiro para a linha.
 * @param x Novo valor de x final.
 */
void setX2Linha(LINHA l, double x);

/**
 * @brief Define uma nova coordenada y para o ponto final da linha.
 * @param l Ponteiro para a linha.
 * @param y Novo valor de y final.
 */
void setY2Linha(LINHA l, double y);

/**
 * @brief Define uma nova cor para a linha.
 * @param l Ponteiro para a linha.
 * @param cor Nova cor.
 */
void setCorLinha(LINHA l, const char* cor);

/**
 * @brief Cria um clone de uma linha existente com um novo identificador.
 * @param l Ponteiro para a linha original.
 * @param novoId Novo identificador.
 * @return Ponteiro para a nova linha clonada.
 */
LINHA clonaLinha(LINHA l, int novoId);

/**
 * @brief Libera os recursos associados à linha.
 * @param l Ponteiro para a linha a ser destruída.
 */
void destruirLinha(LINHA l);

/**
 * @brief Calcula a área equivalente da linha (2.0 * comprimento).
 * @param l Ponteiro para a linha.
 * @return Valor da área calculada.
 */
double calculaAreaLinha(LINHA l);
