#ifndef GEO_IO_H
#define GEO_IO_H

#include <stdio.h>
#include "../tads_gerais/fila.h"
#include "../formas/forma.h"

/**
 * Lê um arquivo .geo já ABERTO e enfileira as formas criadas no "chão"
 * (na ordem em que aparecem no arquivo).
 *
 * Suporta comandos:
 *  - c i x y r corb corp
 *  - r i x y w h corb corp
 *  - l i x1 y1 x2 y2 cor
 *  - t i x y corb corp a texto... (texto vai até o fim da linha)
 *  - ts fFamily fWeight fSize (afeta textos subsequentes)
 *
 * Retorna: número de formas carregadas.
 * Em caso de erro de parsing, imprime no stderr e continua onde possível.
 */
int geo_ler(FILE *geo, FILA fila_chao);

#endif /* GEO_IO_H */
