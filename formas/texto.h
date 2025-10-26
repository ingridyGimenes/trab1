#pragma once

/**
 * @typedef TEXTO
 * @brief Tipo opaco para representar um texto posicionado no plano cartesiano.
 */
typedef void* TEXTO;

/**
 * @typedef ESTILO
 * @brief Tipo opaco para representar o estilo aplicado a um texto (família, peso e tamanho da fonte).
 */
typedef void* ESTILO;

/**
 * @brief Cria um novo estilo de texto.
 * @param family Família da fonte (ex: "sans", "serif").
 * @param weight Peso da fonte (ex: "normal", "bold").
 * @param size Tamanho da fonte.
 * @return Ponteiro para o estilo criado.
 */
ESTILO criaEstilo(const char* family, const char* weight, const char* size);

/**
 * @brief Libera os recursos associados a um estilo.
 * @param e Ponteiro para o estilo a ser destruído.
 */
void destruirEstilo(ESTILO e);

/**
 * @brief Retorna a família da fonte do estilo.
 * @param e Ponteiro para o estilo.
 * @return String com a família da fonte.
 */
const char* getFamily(ESTILO e);

/**
 * @brief Retorna o peso da fonte do estilo.
 * @param e Ponteiro para o estilo.
 * @return String com o peso da fonte.
 */
const char* getWeight(ESTILO e);

/**
 * @brief Retorna o tamanho da fonte do estilo.
 * @param e Ponteiro para o estilo.
 * @return String com o tamanho da fonte.
 */
const char* getSize(ESTILO e);

/**
 * @brief Define uma nova família de fonte no estilo.
 * @param e Ponteiro para o estilo.
 * @param family Nova família de fonte.
 */
void setFamily(ESTILO e, const char* family);

/**
 * @brief Define um novo peso de fonte no estilo.
 * @param e Ponteiro para o estilo.
 * @param weight Novo peso de fonte.
 */
void setWeight(ESTILO e, const char* weight);

/**
 * @brief Define um novo tamanho de fonte no estilo.
 * @param e Ponteiro para o estilo.
 * @param size Novo tamanho de fonte.
 */
void setSize(ESTILO e, const char* size);

/**
 * @brief Cria um novo texto com posição, cores, âncora, conteúdo e estilo.
 * @param id Identificador único do texto.
 * @param x Coordenada x do texto.
 * @param y Coordenada y do texto.
 * @param corB Cor da borda.
 * @param corP Cor de preenchimento.
 * @param ancora Tipo de âncora ('i' início, 'm' meio, 'f' fim).
 * @param conteudo Texto a ser exibido.
 * @param estilo Estilo aplicado ao texto.
 * @return Ponteiro para o texto criado.
 */
TEXTO criaTexto(int id, double x, double y, const char* corB, const char* corP, char ancora, const char* conteudo, ESTILO estilo);

/**
 * @brief Libera os recursos associados a um texto.
 * @param t Ponteiro para o texto a ser destruído.
 */
void destruirTexto(TEXTO t);

/**
 * @brief Retorna o identificador do texto.
 * @param t Ponteiro para o texto.
 * @return Identificador numérico.
 */
int getIdTexto(TEXTO t);

/**
 * @brief Retorna a coordenada x do texto.
 * @param t Ponteiro para o texto.
 * @return Coordenada x.
 */
double getXTexto(TEXTO t);

/**
 * @brief Retorna a coordenada y do texto.
 * @param t Ponteiro para o texto.
 * @return Coordenada y.
 */
double getYTexto(TEXTO t);

/**
 * @brief Retorna a cor da borda do texto.
 * @param t Ponteiro para o texto.
 * @return String com a cor da borda.
 */
const char* getCorBordaTexto(TEXTO t);

/**
 * @brief Retorna a cor de preenchimento do texto.
 * @param t Ponteiro para o texto.
 * @return String com a cor de preenchimento.
 */
const char* getCorPreenchimentoTexto(TEXTO t);

/**
 * @brief Retorna o tipo de âncora do texto.
 * @param t Ponteiro para o texto.
 * @return Caractere representando a âncora ('i', 'm' ou 'f').
 */
char getAncoraTexto(TEXTO t);

/**
 * @brief Retorna o conteúdo do texto.
 * @param t Ponteiro para o texto.
 * @return String com o conteúdo do texto.
 */
const char* getConteudoTexto(TEXTO t);

/**
 * @brief Retorna o estilo do texto.
 * @param t Ponteiro para o texto.
 * @return Ponteiro para o estilo.
 */
ESTILO getEstiloTexto(TEXTO t);

/**
 * @brief Define um novo identificador para o texto.
 * @param t Ponteiro para o texto.
 * @param id Novo identificador.
 */
void setIdTexto(TEXTO t, int id);

/**
 * @brief Define uma nova coordenada x para o texto.
 * @param t Ponteiro para o texto.
 * @param x Novo valor de x.
 */
void setXTexto(TEXTO t, double x);

/**
 * @brief Define uma nova coordenada y para o texto.
 * @param t Ponteiro para o texto.
 * @param y Novo valor de y.
 */
void setYTexto(TEXTO t, double y);

/**
 * @brief Define uma nova cor de borda para o texto.
 * @param t Ponteiro para o texto.
 * @param corB Nova cor de borda.
 */
void setCorBordaTexto(TEXTO t, const char* corB);

/**
 * @brief Define uma nova cor de preenchimento para o texto.
 * @param t Ponteiro para o texto.
 * @param corP Nova cor de preenchimento.
 */
void setCorPreenchimentoTexto(TEXTO t, const char* corP);

/**
 * @brief Define uma nova âncora para o texto.
 * @param t Ponteiro para o texto.
 * @param ancora Novo tipo de âncora ('i', 'm' ou 'f').
 */
void setAncoraTexto(TEXTO t, char ancora);

/**
 * @brief Define um novo conteúdo para o texto.
 * @param t Ponteiro para o texto.
 * @param conteudo Nova string de conteúdo.
 */
void setConteudoTexto(TEXTO t, const char* conteudo);

/**
 * @brief Define um novo estilo para o texto.
 * @param t Ponteiro para o texto.
 * @param estilo Novo estilo.
 */
void setEstiloTexto(TEXTO t, ESTILO estilo);

/**
 * @brief Calcula a área equivalente de um texto (20.0 * número de caracteres).
 * @param t Ponteiro para o texto.
 * @return Valor da área.
 */
double calculaAreaTexto(TEXTO t);

/**
 * @brief Cria um clone de um texto existente com um novo identificador.
 * @param t Ponteiro para o texto original.
 * @param novoId Novo identificador.
 * @return Ponteiro para o texto clonado.
 */
TEXTO clonaTexto(TEXTO t, int novoId);

/**
 * @brief Inverte a cor de borda e de preenchimento de um texto.
 * @param t Ponteiro para o texto.
 */
void inverterCoresTexto(TEXTO t);
