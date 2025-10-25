#include <stdio.h>
#include <stdlib.h>
#include "../tads_gerais/pilha.h"
#include "../tads_gerais/fila.h"
#include "../formas/forma.h"
#include "disparador.h"

/*
 * Este módulo simula o "disparo" das formas:
 * - retira elementos do carregador (PILHA)
 * - insere na arena (FILA)
 * - não cria cópias, só move os ponteiros
 * - usado pelos comandos do .qry (pd, dsp, shft, etc.)
 */

void dispararForma(PILHA carregador, FILA arena, int quantidade) {
    if (!carregador || !arena || quantidade <= 0) return;

    for (int i = 0; i < quantidade; i++) {
        if (pilhaVazia(carregador)) {
            // pilha acabou
            break;
        }
        FORMA f = removeDaPilha(carregador);
        insereNaFila(arena, f);
    }
}

void dispararTudo(PILHA carregador, FILA arena) {
    if (!carregador || !arena) return;

    while (!pilhaVazia(carregador)) {
        FORMA f = removeDaPilha(carregador);
        insereNaFila(arena, f);
    }
}
