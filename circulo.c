#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "circulo.h"

#define PI  3.14159

typedef struct{
 int id;
 int x, y;
 double raio;
 char *cor_b, *cor_p;
}circulo;

CIRCLE cria_circulo(int ID, int cod_x, int cod_y, double r, char cor_P, char cor_B){
    circulo *C = (circulo*)malloc(sizeof(circulo));
    if(C){
        C->id = ID;
        C->x = cod_x;
        C->y = cod_y;
        C->raio = r;

        C->cor_b = (char*)malloc(strlen(cor_B)+1);

            if(C->cor_b == NULL){
                printf("erro ao alocar cor");
                exit(1);
            }else strcpy(C->cor_b, cor_B);

        C->cor_p = (char*)malloc(strlen(cor_P)+1);

            if(C->cor_p == NULL){
                printf("erro ao alocar cor");
                exit(1);
            }else strcpy(C->cor_p, cor_P);


    }else{
        printf("erro na criacao do circulo");
        exit(1);
    }
}

