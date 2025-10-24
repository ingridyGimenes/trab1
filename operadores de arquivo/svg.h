#ifndef SVG_H
#define SVG_H

#include <stdio.h>
#include "/workspaces/trab1/tads_gerais/fila.h"

/*
 * Gera SVGs para o projeto Bocha Geométrica.
 *
 * Contrato mínimo esperado das "formas" (dependências):
 * - A fila (Fila *chao) armazena ponteiros void* para as formas.
 * - Cada forma deve ter uma função que permita imprimir sua
 *   representação SVG em um FILE*:
 *     void forma_svg_imprime(FILE *out, void *forma);
 *   (ou funções por tipo, caso prefira: circulo_svg_imprime(...), etc.)
 *
 * - Deve existir uma rotina que identifica o tipo de forma, se
 *   necessário:
 *     char forma_get_tipo(void *forma); // 'c','r','l','t' etc
 *
 * Se você tiver funções com nomes diferentes, ajuste as chamadas em svg.c.
 */

/* inicia/termina um arquivo SVG (escreve cabeçalho e rodapé) */
void svg_inicia(FILE *f, double width, double height);
void svg_termina(FILE *f);

/* Gera um svg a partir do conteúdo da fila 'chao'.
   O arquivo gerado terá o nome especificado em nomeArquivo. */
void svg_gera_do_chao(const char *nomeArquivo, Fila *chao, double width, double height);

/* Registra uma anotação (asterisco vermelho) para ser impressa no próximo SVG.
   Usar em 'calc' quando uma forma é esmagada:
     svg_anotar_esmagado(x, y);
*/
void svg_anotar_esmagado(double x, double y);

/* Limpa a lista de anotações (após gerar o svg) */
void svg_limpa_anotacoes(void);

#endif /* SVG_H */
