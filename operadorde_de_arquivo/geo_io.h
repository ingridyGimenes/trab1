#pragma once
#include <stdbool.h>

// O loader é opaco (nenhuma struct exposta)
typedef void* GEO_LOADER;

// fila do chão chega de fora (sua Fila*)
// formas serão carregadas nela como FORMA (wrapper do geometria/forma.h)
GEO_LOADER geo_loader_create(void* fila_chao);

// Lê e insere as formas do .geo no "chão".
// Retorna true se ok, false se falhar (erro de arquivo ou sintaxe).
bool geo_loader_load_file(GEO_LOADER gl, const char* filepath);

// Desaloca o loader (não mexe na fila do chão).
void geo_loader_destroy(GEO_LOADER gl);
