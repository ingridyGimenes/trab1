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

int get_cord_x_ret(RETANGULO r){
    retangulo *ret = ((retangulo*)r);
    return ret->x;
} 

int get_cord_y_ret(RETANGULO r){
    retangulo *ret = ((retangulo*)r);
    return ret->y;
}    



int get_width_ret(RETANGULO r){
     retangulo *ret = ((retangulo*)r);
     return ret->w;
}

int get_heigth_ret(RETANGULO r){
     retangulo *ret = ((retangulo*)r);
     return ret->h;
}

char* get_corB_ret(RETANGULO r){
    retangulo *ret = ((retangulo*)r);
    return ret->cor_b;
}

char* get_corP_ret(RETANGULO r){
    retangulo *ret = ((retangulo*)r);
    return ret->cor_p;
}

int calcula_area_ret(RETANGULO r){
     retangulo *ret = ((retangulo*)r);
     return ret->h*ret->w;
}

void set_cordX_ret(RETANGULO r, int cordX){
    retangulo *ret = ((retangulo*)r);
    ret->x = cordX;
}

void set_cordY_ret(RETANGULO r, int cordY){
    retangulo *ret = ((retangulo*)r);
    ret->y = cordY;
}

void set_height_ret(RETANGULO r, float H){
    retangulo *ret = ((retangulo*)r);
    ret->h = H;
}

void set_width_ret(RETANGULO r, float W){
    retangulo *ret = ((retangulo*)r);
    ret->w = W;
}

void set_corB_ret(RETANGULO r, char* corB){
    retangulo *ret = ((retangulo*)r);
    strcpy(ret->cor_b, corB);
}

void set_corP_ret(RETANGULO r, char* corP){
    retangulo *ret = ((retangulo*)r);
    strcpy(ret->cor_p, corP);
}