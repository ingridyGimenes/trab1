#pragma once

/**
 * @typedef RETANGULO
 * @brief Tipo opaco para representar um retângulo no plano cartesiano.
 */
typedef void* RETANGULO;

/**
 * @brief Cria um retângulo com posição, dimensões e cores.
 * @param id Identificador único do retângulo.
 * @param x Coordenada x do canto inferior esquerdo.
 * @param y Coordenada y do canto inferior esquerdo.
 * @param largura Largura do retângulo.
 * @param altura Altura do retângulo.
 * @param corB Cor da borda.
 * @param corP Cor de preenchimento.
 * @return Ponteiro para o retângulo criado.
 */
RETANGULO criaRetangulo(int id, double x, double y, double largura, double altura, const char* corB, const char* corP);

/**
 * @brief Retorna o identificador do retângulo.
 * @param r Ponteiro para o retângulo.
 * @return Identificador numérico.
 */
int getIdRetangulo(RETANGULO r);

/**
 * @brief Retorna a coordenada x do canto inferior esquerdo do retângulo.
 * @param r Ponteiro para o retângulo.
 * @return Coordenada x.
 */
double getXRetangulo(RETANGULO r);

/**
 * @brief Retorna a coordenada y do canto inferior esquerdo do retângulo.
 * @param r Ponteiro para o retângulo.
 * @return Coordenada y.
 */
double getYRetangulo(RETANGULO r);

/**
 * @brief Retorna a largura do retângulo.
 * @param r Ponteiro para o retângulo.
 * @return Largura.
 */
double getLarguraRetangulo(RETANGULO r);

/**
 * @brief Retorna a altura do retângulo.
 * @param r Ponteiro para o retângulo.
 * @return Altura.
 */
double getAlturaRetangulo(RETANGULO r);

/**
 * @brief Retorna a cor da borda do retângulo.
 * @param r Ponteiro para o retângulo.
 * @return String com a cor da borda.
 */
const char* getCorBordaRetangulo(RETANGULO r);

/**
 * @brief Retorna a cor de preenchimento do retângulo.
 * @param r Ponteiro para o retângulo.
 * @return String com a cor de preenchimento.
 */
const char* getCorPreenchimentoRetangulo(RETANGULO r);

/**
 * @brief Define um novo identificador para o retângulo.
 * @param r Ponteiro para o retângulo.
 * @param id Novo identificador.
 */
void setIdRetangulo(RETANGULO r, int id);

/**
 * @brief Define uma nova coordenada x para o canto inferior esquerdo do retângulo.
 * @param r Ponteiro para o retângulo.
 * @param x Novo valor de x.
 */
void setXRetangulo(RETANGULO r, double x);

/**
 * @brief Define uma nova coordenada y para o canto inferior esquerdo do retângulo.
 * @param r Ponteiro para o retângulo.
 * @param y Novo valor de y.
 */
void setYRetangulo(RETANGULO r, double y);

/**
 * @brief Define uma nova largura para o retângulo.
 * @param r Ponteiro para o retângulo.
 * @param w Nova largura.
 */
void setLarguraRetangulo(RETANGULO r, double w);

/**
 * @brief Define uma nova altura para o retângulo.
 * @param r Ponteiro para o retângulo.
 * @param h Nova altura.
 */
void setAlturaRetangulo(RETANGULO r, double h);

/**
 * @brief Define uma nova cor de borda para o retângulo.
 * @param r Ponteiro para o retângulo.
 * @param corB Nova cor da borda.
 */
void setCorBordaRetangulo(RETANGULO r, const char* corB);

/**
 * @brief Define uma nova cor de preenchimento para o retângulo.
 * @param r Ponteiro para o retângulo.
 * @param corP Nova cor de preenchimento.
 */
void setCorPreenchimentoRetangulo(RETANGULO r, const char* corP);

/**
 * @brief Calcula a área do retângulo.
 * @param r Ponteiro para o retângulo.
 * @return Valor da área.
 */
double calculaAreaRetangulo(RETANGULO r);

/**
 * @brief Cria um clone de um retângulo existente com um novo identificador.
 * @param r Ponteiro para o retângulo original.
 * @param novoId Novo identificador.
 * @return Ponteiro para o novo retângulo clonada.
 */
RETANGULO clonaRetangulo(RETANGULO r, int novoId);

/**
 * @brief Inverte a cor de borda e de preenchimento do retângulo.
 * @param r Ponteiro para o retângulo.
 */
void inverterCoresRetangulo(RETANGULO r);

/**
 * @brief Libera os recursos associados ao retângulo.
 * @param r Ponteiro para o retângulo a ser destruído.
 */
void destruirRetangulo(RETANGULO r);
