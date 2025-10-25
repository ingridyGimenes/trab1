#pragma once

// Gera um SVG a partir de uma Fila de FORMA.
// Largura/altura do canvas são apenas dimensões do viewport do SVG (não escala formas).
// Retorna 0 em sucesso, -1 em erro.
int svg_dump_fila(const char* path_svg, void* fila_formas, double width, double height);
