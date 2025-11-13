#pragma once

// Tipo opaco: não exponha struct neste .h
typedef void* CIRCULO;

/**
 * @brief Cria um círculo cuja âncora é o centro (x,y).
 * @param id   Identificador único da forma.
 * @param x    Coordenada x do centro.
 * @param y    Coordenada y do centro.
 * @param r    Raio (deve ser >= 0).
 * @param corB Cor da borda (SVG). Ver notas sobre ownership abaixo.
 * @param corP Cor de preenchimento (SVG). Ver notas sobre ownership abaixo.
 * @return Um handle CIRCULO ou NULL em caso de falha (ex.: alocação inválida ou parâmetros ilegais).
 *
 * @note Contrato de strings: esta implementação (defina aqui!)
 *       - (opção A) COPIA internamente corB/corP (ex.: strdup). Em destruirCirculo, as cópias são liberadas.
 *       - (opção B) NÃO copia: o chamador deve manter corB/corP válidas enquanto o círculo existir.
 * @pre r >= 0.0
 */
CIRCULO criaCirculo(int id, double x, double y, double r, const char* corB, const char* corP);

/** Getters: não modificam o objeto. Retornam valores default se c == NULL. */
int    getIdCirculo(const CIRCULO c);
double getXCirculo(const CIRCULO c);
double getYCirculo(const CIRCULO c);
double getRaioCirculo(const CIRCULO c);

/**
 * @brief Retorna as cores. O ponteiro retornado permanece válido enquanto o CIRCULO existir.
 *        Ver contrato de ownership em criaCirculo.
 */
const char* getCorBordaCirculo(const CIRCULO c);
const char* getCorPreenchimentoCirculo(const CIRCULO c);

/** Setters: ignoram a chamada se c == NULL. Validações são aplicadas (ex.: r >= 0). */
void setIdCirculo(CIRCULO c, int id);
void setXCirculo(CIRCULO c, double x);
void setYCirculo(CIRCULO c, double y);
void setRaioCirculo(CIRCULO c, double r);

/**
 * @brief Define a cor da borda/preenchimento.
 * @note Se a implementação copiar strings, elas serão duplicadas; caso contrário, apenas referenciadas.
 */
void setCorBordaCirculo(CIRCULO c, const char* novaCor);
void setCorPreenchimentoCirculo(CIRCULO c, const char* novaCor);

/**
 * @brief Calcula a área do círculo.
 * @return Área (π r^2). Retorna 0.0 se c == NULL ou r <= 0.
 */
double calculaAreaCirculo(const CIRCULO c);

/**
 * @brief Cria um clone idêntico com novoId (cores, posição, raio, etc.).
 * @param c      Círculo de origem.
 * @param novoId Identificador do clone.
 * @return Novo CIRCULO ou NULL em caso de falha.
 * @pre c != NULL
 */
CIRCULO clonaCirculo(const CIRCULO c, int novoId);

/**
 * @brief Troca cor de borda com cor de preenchimento (in-place).
 * @pre c != NULL
 */
void inverterCoresCirculo(CIRCULO c);

/**
 * @brief Libera recursos do círculo e anula o ponteiro.
 * @param c Ponteiro para o handle. Se *c for NULL, não faz nada. Ao final, *c = NULL.
 */
void destruirCirculo(CIRCULO* c);
