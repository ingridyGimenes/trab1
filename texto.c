#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "texto.h"

typedef struct{
    char* family;
    char* weight;
    char* size;
}estilo;

typedef struct{

    int id;
    double x;
    double y;
    char *corB;
    char *corP;
    char *ancora;
    char *txto;
    estilo txtstyle;

}texto;

Estilo cria_estilo(char* Family, char* Weight, char* Size ){

        estilo *ts = (estilo*)malloc(sizeof(estilo));
            if( ts == NULL){
                printf("erro ao alocar memoria");
                exit(1);
            }

        ts->family = (char*)malloc(strlen(Family)+1);
            if(ts->family == NULL){
                printf("erro a alocar family");
                exit(1);
            }
        strcpy(ts->family, "" );

        ts->weight = (char*)malloc(strlen(Weight)+1);
            if(ts->weight == NULL){
                printf("erro a alocar weight");
                exit(1);
            }
        strcpy(ts->weight, "" );

        ts->size = (char*)malloc(strlen(Size)+1);
            if(ts->size == NULL){
                printf("erro a alocar size");
                exit(1);
            }
        strcpy(ts->size, "" );

        return ts;
}

TEXTO cria_texto( int i, double codX, double codY, char *cor_b, char *cor_p, char *a, char *TXTO){
    texto *txt = (texto*)malloc(sizeof(texto));
        
        if(txt == NULL){
            printf("erro ao alocar do txt");
        }
     
    txt->id = i;
    txt->x = codX;
    txt->y = codY;

    txt->corB = (char*)malloc(strlen(cor_b)+1);
            if(txt->corB == NULL){
                printf("erro ao alocar corB");
                exit(1);
            }
        strcpy(txt->corB, cor_b);


    txt->corP = (char*)malloc(strlen(cor_p)+1);
        if(txt->corP == NULL){
            printf("erro ao alocar corP");
            exit(1);
        }
     strcpy(txt->corP, cor_p);


    txt->ancora = (char*)malloc(strlen(a)+1);
            if(txt->ancora == NULL){
                printf("erro ao alocar ancora");
                exit(1);
            }
     strcpy(txt->ancora, a);

     txt->txto = (char*)malloc(strlen(TXTO)+1);
            if(txt->txto == NULL){
                printf("erro ao alocar txto");
                exit(1);
            }
        strcpy(txt->txto, TXTO);
}


int get_id_txt(TEXTO t){
    texto *txt = ((texto*)t);
    return txt->id;
}

double get_codx_txt(TEXTO t){
    texto *txt = ((texto*)t);
    return txt->x;
}

double get_cody_txt(TEXTO t){
    texto *txt = ((texto*)t);
    return txt->y;
}

char* get_corB_txt(TEXTO t){
    texto *txt = ((texto*)t);
    return txt->corB;
}

char* get_corP_txt(TEXTO t){
    texto *txt = ((texto*)t);
    return txt->corP;
}

char* get_ancora_txt(TEXTO t){
    texto *txt = ((texto*)t);
    return txt->ancora;
}

char* get_txto_txt(TEXTO t){
    texto *txt = ((texto*)t);
    return txt->txto;
}

char* getFamily(Estilo e){
    estilo* es = ((estilo*)e);
    return es->family;
}

char* getWeight(Estilo e){
    estilo* es = ((estilo*)e);
    return es->weight;
}

char* getSize(Estilo e){
    estilo* es = ((estilo*)e);
    return es->size;
}


void set_codx_txt(TEXTO t, double Cx){
    texto* T = ((texto*)t);
    T->x = Cx;
}

void set_cody_txt(TEXTO t, double Cy){
     texto* T = ((texto*)t);
    T->x = Cy;
}

void set_corB_txt(TEXTO t, char* cor_b){
     texto* T = ((texto*)t);
    strcpy(T->corB, cor_b);
}

void set_corP_txt(TEXTO t, char* cor_p){
     texto* T = ((texto*)t);
    strcpy(T->corB, cor_p);
}

void set_ancora_txt(TEXTO t, char* anc){
    texto* T = ((texto*)t);
    strcpy(T->corB, anc);
}

void set_txto_txt(TEXTO t, char* cont){
     texto* T = ((texto*)t);
    strcpy(T->txto, cont);
}

void setFamily(Estilo e, char* familia){
     estilo* E = ((estilo*)e);
   strcpy(E->family, familia);
}

void setWeight(Estilo e, char* wgt){
    estilo* E = ((estilo*)e);
    strcpy(E->weight, wgt);
}

void setSize(Estilo e, char* sz){
     estilo* E = ((estilo*)e);
    strcpy(E->size, sz);
}
