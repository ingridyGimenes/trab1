#ifndef COLISOES_H
#define COLISOES_H

#include <stdbool.h>

// Tipos genéricos das formas
#include "/workspaces/trab1/formas/circulo.h"
#include "/workspaces/trab1/formas/retangulo.h"
#include "/workspaces/trab1/formas/linha.h"
#include "/workspaces/trab1/formas/texto.h"

/*
 * Funções de verificação de sobreposição entre formas geométricas
 * (Bocha Geométrica - Estrutura de Dados I - UEL)
 *
 * Cada função retorna true se as formas se sobrepõem, false caso contrário.
 */

// Testes diretos entre tipos
bool sobrepoe_circulo_circulo(Circulo* c1, Circulo* c2);
bool sobrepoe_retangulo_retangulo(Retangulo* r1, Retangulo* r2);
bool sobrepoe_circulo_retangulo(Circulo* c, Retangulo* r);
bool sobrepoe_linha_linha(Linha* l1, Linha* l2);

// Texto tratado como segmento horizontal
void texto_para_segmento(Texto* t, double* x1, double* y1, double* x2, double* y2);


#endif
