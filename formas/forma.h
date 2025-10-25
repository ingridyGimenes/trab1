#pragma once
#include <stdbool.h>

typedef void* FORMA;  // Wrapper opaco

// Criação / acesso
FORMA cria_forma(char tipo, void* objeto);
char  tipo_forma(FORMA f);
void* objeto_forma(FORMA f);

// Interseção e área
bool   formas_intersectam(FORMA A, FORMA B);
double area_forma(FORMA F);

// Regras de interação
void   borda_de_B_recebe_corp_de_A(FORMA A, FORMA B);
FORMA  clona_forma_com_cores_trocadas(FORMA A, int novoId);

// Liberação
void destruir_forma(FORMA F);
