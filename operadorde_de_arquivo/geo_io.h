#ifndef GEO_IO_H
#define GEO_IO_H

#include <stdio.h>
#include "../tads_gerais/fila.h"
#include "../formas/forma.h"

/**
 * @brief Lê um arquivo .geo já aberto e enfileira as formas criadas no "chão"
 *        na ordem em que aparecem.
 *
 * @details Reconhece os seguintes comandos:
 *          - c i x y r corb corp
 *          - r i x y w h corb corp
 *          - l i x1 y1 x2 y2 cor
 *          - t i x y corb corp a texto...  (o conteúdo do texto segue até o fim da linha)
 *          - ts fFamily fWeight fSize      (altera o estilo aplicado a textos subsequentes)
 *
 * @param geo        Ponteiro para o arquivo .geo já aberto em modo leitura.
 * @param fila_chao  Fila onde as formas lidas serão enfileiradas, preservando a ordem.
 *
 * @return Número total de formas carregadas.
 *
 * @note O estilo definido por "ts" permanece ativo até nova instrução "ts".
 * @note Cada forma criada é empacotada em uma FORMA e inserida em @p fila_chao.
 * @warning Em erros de parsing, mensagens são escritas em stderr e a leitura tenta prosseguir.
 * @warning IDs, cores e dimensões são assumidos válidos conforme especificação dos comandos.
 */
int geo_ler(FILE *geo, FILA fila_chao);

#endif /* GEO_IO_H */
