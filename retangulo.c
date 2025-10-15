#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "retangulo.h"

typedef struct{
    int i; //id
    double x; //cordenada x
    double y; //cordenada y
    double w; //largura
    double h; //altura
    char *cor_b; //cor de borda
    char *cor_p; //cor do preenchimento
}retangulo;

void* cria_retangulo(int i, double x, double y, double w, double h, char* cor_b, char* cor_p){
    retangulo *r = (retangulo*)malloc(sizeof(retangulo));
    if(r){
        r->i = i;
        r->x = x;
        r->y = y;
        r->w = w;
        r->h = h;
        r->cor_b = strdup(cor_b);
        r->cor_p = strdup(cor_p);
        
        return (RETANGULO)r;


    }else{
        printf("erro ao criar o retangulo");
        exit(1);
    } 
}

int le_id_ret(RETANGULO r){
        
        retangulo *ret = ((retangulo*)r);
        return ret->i;
    }

double get_cord_x_ret(RETANGULO r){
    retangulo *ret = ((retangulo*)r);
    return ret->x;
} 

double get_cord_y_ret(RETANGULO r){
    retangulo *ret = ((retangulo*)r);
    return ret->y;
}    



double get_width_ret(RETANGULO r){
     retangulo *ret = ((retangulo*)r);
     return ret->w;
}

double get_heigth_ret(RETANGULO r){
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

double calcula_area_ret(RETANGULO r){
     retangulo *ret = ((retangulo*)r);
     return ret->h*ret->w;
}

void setIdRetangulo(RETANGULO r, int id){
    retangulo* ret = (retangulo*)r;
    ret->i = id;
}

void set_cordX_ret(RETANGULO r, double cordX){
    retangulo *ret = ((retangulo*)r);
    ret->x = cordX;
}

void set_cordY_ret(RETANGULO r, double cordY){
    retangulo *ret = ((retangulo*)r);
    ret->y = cordY;
}

void set_height_ret(RETANGULO r, double H){
    retangulo *ret = ((retangulo*)r);
    ret->h = H;
}

void set_width_ret(RETANGULO r, double W){
    retangulo *ret = ((retangulo*)r);
    ret->w = W;
}

void set_corB_ret(RETANGULO r, char* corB){
    retangulo *ret = ((retangulo*)r);
    free(ret->cor_b);
    ret->cor_b = strdup(corB);
}

void set_corP_ret(RETANGULO r, char* corP){
    retangulo *ret = ((retangulo*)r);
    free(ret->cor_p);
    ret->cor_p = strdup(corP);
}

void exclui_retangulo(RETANGULO r){
    retangulo *ret = (retangulo*)r;
    if(ret == NULL){
        printf("retangulo nn existe");
        return;
    }

    free(ret->cor_b);
    free(ret->cor_p);
    free(ret);
}

RETANGULO clonaRetangulo(RETANGULO r, int novoId) {
   retangulo* orig = (retangulo*)r;
    retangulo* novo = (retangulo*)malloc(sizeof(retangulo));

    novo->i = novoId;
    novo->x = orig->x;
    novo->y = orig->y;
    novo->w = orig->w;
    novo->h = orig->h;

    // Troca as cores da borda e do preenchimento
    novo->cor_b = strdup(orig->cor_p);
    novo->cor_p = strdup(orig->cor_b);

    return (RETANGULO)novo;
}