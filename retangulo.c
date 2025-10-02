#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "retangulo.h"

typedef struct{
    int i; //id
    int x; //cordenada x
    int y; //cordenada y
    float w; //largura
    float h; //altura
    char *cor_b; //cor de borda
    char *cor_p; //cor do preenchimento
}retangulo;

RETANGULO cria_retangulo(int i, int x, int y, float w, float h, char cor_b, char cor_p){
    retangulo *r = (retangulo*)malloc(sizeof(retangulo));
    if(r){
        r->i = i;
        r->x = x;
        r->y = y;
        r->w = w;
        r->h = h;
        r->cor_b = (char*)malloc(sizeof(cor_b)*(strlen(cor_b)+1));
        r->cor_p = (char*)malloc(sizeof(cor_p)*(strlen(cor_p)+1));
        if(r->cor_b == NULL || r->cor_p == NULL){
            printf("erro ao definir cores");
            free(r->cor_b);
            free(r->cor_p);
            exit(1);
        }else{
            strcpy(r->cor_b, cor_b);
            strcpy(r->cor_p, cor_p);
        }
        
        return r;


    }else{
        printf("erro ao criar o retangulo");
        exit(1);
    } 
}

int le_id_ret(RETANGULO r){
        
        retangulo *ret = ((retangulo*)r);
        return ret->i;
    }

int cord_x_ret(RETANGULO r){
    retangulo *ret = ((retangulo*)r);
    return ret->x;
} 

int cord_y_ret(RETANGULO r){
    retangulo *ret = ((retangulo*)r);
    return ret->y;
}    

int area_ret(RETANGULO r){
     retangulo *ret = ((retangulo*)r);
     return ret->h*ret->w;
}

int width_ret(RETANGULO r){
     retangulo *ret = ((retangulo*)r);
     return ret->w;
}

int heigth_ret(RETANGULO r){
     retangulo *ret = ((retangulo*)r);
     return ret->h;
}

