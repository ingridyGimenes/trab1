#include <stdio.h>
#include <stdlib.h>
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

int get_id_circ(CIRCLE c){
    circulo *cir = ((circulo*)c);
    return cir->id;
}

int get_cordx_cir(CIRCLE c){
    circulo *cir = ((circulo*)c);
    return cir->x;
}

int get_cordy_cir(CIRCLE c){
    circulo *cir = ((circulo*)c);
    return cir->y;
}

int get_raio_cir(CIRCLE c){
    circulo *cir = ((circulo*)c);
    return cir->raio;
}

char* get_corB_cir(CIRCLE c){
     circulo *cir = ((circulo*)c);
    return cir->cor_b;
}

char* get_corP_cir(CIRCLE c){
     circulo *cir = ((circulo*)c);
    return cir->cor_p;
}

void set_x_cir(CIRCLE c, int x){
    circulo *cir = ((circulo*)c);
    cir->x = x;
}

void set_y_cir(CIRCLE c, int Y){
    circulo *cir = ((circulo*)c);
    cir->y = Y;
}

void set_raio_cir(CIRCLE c, int r){
    circulo *cir = ((circulo*)c);
    cir->raio = r;
}

void set_corB_cir(CIRCLE c, char* corB){
    circulo *cir = ((circulo*)c);
    strcpy(cir->cor_b, corB);
}

void set_corP_cir(CIRCLE c, char* corP){
    circulo *cir = ((circulo*)c);
    strcpy(cir->cor_p, corP);
}

double calcula_area_circulo(CIRCLE c){
    circulo *cir = ((circulo*)c);
    return PI*(cir->raio*cir->raio);
}