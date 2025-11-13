
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "linha.h"

typedef struct {
    int id;
    double x1, y1, x2, y2;
    char* cor;
} Linha;

LINHA criaLinha(int id, double x1, double y1, double x2, double y2, const char* cor) {
    Linha* l = malloc(sizeof(Linha));
    if (!l) {
        fprintf(stderr, "Erro ao alocar memória para linha.\n");
        exit(1);
    }

    l->id = id;
    l->x1 = x1;
    l->y1 = y1;
    l->x2 = x2;
    l->y2 = y2;
    
    if (!cor) cor = "none";
    l->cor = malloc(strlen(cor) + 1);
    
    if (!l->cor) { fprintf(stderr, "Erro: malloc cores linha\n"); exit(1); }
    strcpy(l->cor, cor);
   
   

    return (LINHA)l;
}

int getIdLinha(LINHA l) { return ((Linha*)l)->id; }
double getX1Linha(LINHA l) { return ((Linha*)l)->x1; }
double getY1Linha(LINHA l) { return ((Linha*)l)->y1; }
double getX2Linha(LINHA l) { return ((Linha*)l)->x2; }
double getY2Linha(LINHA l) { return ((Linha*)l)->y2; }
const char* getCorLinha(LINHA l) { return ((Linha*)l)->cor; }

void setIdLinha(LINHA l, int id) { ((Linha*)l)->id = id; }
void setX1Linha(LINHA l, double x) { ((Linha*)l)->x1 = x; }
void setY1Linha(LINHA l, double y) { ((Linha*)l)->y1 = y; }
void setX2Linha(LINHA l, double x) { ((Linha*)l)->x2 = x; }
void setY2Linha(LINHA l, double y) { ((Linha*)l)->y2 = y; }

void setCorLinha(LINHA l, const char* cor) {
    Linha* li = (Linha*)l;
      if (cor) cor = "#000000";
    size_t need = strlen(cor) + 1;
    if (!li->cor || strlen(li->cor) + 1 < need) {
        char* tmp = realloc(li->cor, need);
        if (!tmp) { fprintf(stderr, "Erro: realloc cor\n"); exit(1); }
        li->cor = tmp;
    }
}

LINHA clonaLinha(LINHA l, int novoId) {
    Linha* orig = (Linha*)l;
    return criaLinha(novoId, orig->x1, orig->y1, orig->x2, orig->y2, orig->cor);
}

void destruirLinha(LINHA l) {
    Linha* li = (Linha*)l;
    if (!li) return;
    free(li->cor);
    free(li);
}


double calculaAreaLinha(LINHA l) {
    double x1 = getX1Linha(l);
    double y1 = getY1Linha(l);
    double x2 = getX2Linha(l);
    double y2 = getY2Linha(l);

    double comprimento = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
    return 2.0 * comprimento;
}