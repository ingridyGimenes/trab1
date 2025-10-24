#include <stdio.h>
#include <stdlib.h>
#include "disparador.h"
#include "carregador.h"

struct disparador {
    int id;
    double x;
    double y;
    Carregador* esquerdo;
    Carregador* direito;
    void* forma_em_disparo;
};

// ------------------------------------------------------------

Disparador* cria_disparador(int id) {
    Disparador* d = (Disparador*) malloc(sizeof(Disparador));
    if (d == NULL) {
        fprintf(stderr, "Erro: falha ao alocar disparador.\n");
        return NULL;
    }

    d->id = id;
    d->x = 0;
    d->y = 0;
    d->esquerdo = NULL;
    d->direito = NULL;
    d->forma_em_disparo = NULL;
    return d;
}

void posiciona_disparador(Disparador* d, double x, double y) {
    if (d == NULL) return;
    d->x = x;
    d->y = y;
}

void conecta_carregadores(Disparador* d, Carregador* cesq, Carregador* cdir) {
    if (d == NULL) return;
    d->esquerdo = cesq;
    d->direito = cdir;
}

/**
 * Função auxiliar: devolve forma de disparo ao topo de um carregador.
 */
static void devolver_para_carregador(Carregador* c, void* forma) {
    if (c != NULL && forma != NULL) {
        adiciona_forma_carregador(c, forma);
    }
}

void* shift_disparador(Disparador* d, char lado) {
    if (d == NULL) return NULL;

    void* atual = d->forma_em_disparo;

    // devolve forma atual (se houver)
    if (atual != NULL) {
        if (lado == 'e')
            devolver_para_carregador(d->direito, atual);
        else if (lado == 'd')
            devolver_para_carregador(d->esquerdo, atual);
    }

    // pega nova forma do carregador correspondente
    if (lado == 'e') {
        d->forma_em_disparo = remove_forma_carregador(d->esquerdo);
    } else if (lado == 'd') {
        d->forma_em_disparo = remove_forma_carregador(d->direito);
    } else {
        fprintf(stderr, "Aviso: lado inválido no shift_disparador (%c)\n", lado);
        d->forma_em_disparo = NULL;
    }

    return d->forma_em_disparo;
}

void* disparar(Disparador* d, double dx, double dy, char modo) {
    if (!d || !d->forma_em_disparo)
        return NULL;

    void* forma = d->forma_em_disparo;
    d->forma_em_disparo = NULL;

    // desloca a forma (caso o módulo de forma trate posição)
    // mover_forma(forma, d->x + dx, d->y + dy);

    if (modo == 'v') {
        // registrar visualmente (SVG ou log)
        // svg_registra_disparo(d, forma, dx, dy);
    }

    return forma;
}

double get_x_disparador(Disparador* d) {
    if (d == NULL) return 0.0;
    return d->x;
}

double get_y_disparador(Disparador* d) {
    if (d == NULL) return 0.0;
    return d->y;
}

int get_id_disparador(Disparador* d) {
    if (d == NULL) return -1;
    return d->id;
}

void libera_disparador(Disparador* d) {
    if (d == NULL) return;
    // Não libera os carregadores nem as formas
    free(d);
}
