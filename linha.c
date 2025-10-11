#include "linha.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    int id;
    int x1, y1, x2, y2;
    char* cor;
}linha;

LINHA cria_linha(int i, int x_1, int x_2, int y_1, int y_2, char* corL){
    linha* l = malloc(sizeof(linha));

    if(l==NULL){
        printf("erro ao criar linha");
        exit(1);
    }

    l->id = i;
    l->x1 = x_1;
    l->x2 = x_2;
    l->y1 = y_1;
    l->y2 = y_2;

    l->cor = (char*) malloc(strlen(corL)+1);
    if(l->cor==NULL){
        printf("erro ao alocar cor da linha");
        exit(1);
    }
    strcpy(l->cor, corL);

    return ((linha*)l);

}

int get_id_linha(LINHA L){  
    linha* l = ((linha*)L);
    return l->id;
}

int get_codx1_linha(LINHA L){  
    linha* l = ((linha*)L);
    return l->x1;
}


int get_codx2_linha(LINHA L){  
    linha* l = ((linha*)L);
    return l->x2;
}

int get_cody1_linha(LINHA L){  
    linha* l = ((linha*)L);
    return l->y1;
}


int get_cody2_linha(LINHA L){  
    linha* l = ((linha*)L);
    return l->y2;
}

char* get_cor_linha(LINHA L){
     linha* l = ((linha*)L);
     return l->cor;
}




void set_codx1_linha(LINHA L, int x){  
    linha* l = ((linha*)L);
    l->x1 = x;
}


void set_codx2_linha(LINHA L, int x){  
    linha* l = ((linha*)L);
    l->x2 = x;
}

void set_cody1_linha(LINHA L, int y){  
    linha* l = ((linha*)L);
    l->y1 = y;
}


void set_cody2_linha(LINHA L, int y){  
    linha* l = ((linha*)L);
    l->y2 = y;
}

void set_cor_linha(LINHA L, char* corL){
     linha* l = ((linha*)L);
     strcpy(l->cor, corL);
}