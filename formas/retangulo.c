#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "retangulo.h"

typedef struct {
    int i;              // id
    double x;           // coordenada x
    double y;           // coordenada y
    double w;           // largura
    double h;           // altura
    char *cor_b;        // cor da borda
    char *cor_p;        // cor do preenchimento
} retangulo;

// Função auxiliar para duplicar strings com segurança
static char* dupstr(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char* d = (char*)malloc(len);
    if (d) memcpy(d, s, len);
    return d;
}

// -----------------------------------------------------------------------------
// Criação / destruição
// -----------------------------------------------------------------------------

RETANGULO cria_retangulo(int i, double x, double y, double w, double h,
                         const char* cor_b, const char* cor_p) {
    retangulo *r = (retangulo*)malloc(sizeof(retangulo));
    if (!r) {
        fprintf(stderr, "Erro ao criar retângulo\n");
        return NULL;
    }

    r->i = i;
    r->x = x;
    r->y = y;
    r->w = w;
    r->h = h;
    r->cor_b = dupstr(cor_b);
    r->cor_p = dupstr(cor_p);

    if ((cor_b && !r->cor_b) || (cor_p && !r->cor_p)) {
        free(r->cor_b);
        free(r->cor_p);
        free(r);
        return NULL;
    }

    return (RETANGULO)r;
}

void excluir_retangulo(RETANGULO r) {
    retangulo *ret = (retangulo*)r;
    if (!ret) return;
    free(ret->cor_b);
    free(ret->cor_p);
    free(ret);
}

// Alias compatível com versões antigas
void exclui_retangulo(RETANGULO r) {
    excluir_retangulo(r);
}

// -----------------------------------------------------------------------------
// Getters originais
// -----------------------------------------------------------------------------

int le_id_ret(RETANGULO r) {
    return ((retangulo*)r)->i;
}

double get_cord_x_ret(RETANGULO r) {
    return ((retangulo*)r)->x;
}

double get_cord_y_ret(RETANGULO r) {
    return ((retangulo*)r)->y;
}

double get_width_ret(RETANGULO r) {
    return ((retangulo*)r)->w;
}

double get_heigth_ret(RETANGULO r) {
    return ((retangulo*)r)->h;
}

char* get_corB_ret(RETANGULO r) {
    return ((retangulo*)r)->cor_b;
}

char* get_corP_ret(RETANGULO r) {
    return ((retangulo*)r)->cor_p;
}

double calcula_area_ret(RETANGULO r) {
    retangulo *ret = (retangulo*)r;
    return ret->w * ret->h;
}

// -----------------------------------------------------------------------------
// Setters originais
// -----------------------------------------------------------------------------

void setIdRetangulo(RETANGULO r, int id) {
    ((retangulo*)r)->i = id;
}

void set_cordX_ret(RETANGULO r, double cordX) {
    ((retangulo*)r)->x = cordX;
}

void set_cordY_ret(RETANGULO r, double cordY) {
    ((retangulo*)r)->y = cordY;
}

void set_height_ret(RETANGULO r, double H) {
    ((retangulo*)r)->h = H;
}

void set_width_ret(RETANGULO r, double W) {
    ((retangulo*)r)->w = W;
}

void set_corB_ret(RETANGULO r, const char* corB) {
    retangulo *ret = (retangulo*)r;
    char* novo = dupstr(corB);
    if (!corB || novo) {
        free(ret->cor_b);
        ret->cor_b = novo;
    }
}

void set_corP_ret(RETANGULO r, const char* corP) {
    retangulo *ret = (retangulo*)r;
    char* novo = dupstr(corP);
    if (!corP || novo) {
        free(ret->cor_p);
        ret->cor_p = novo;
    }
}

// -----------------------------------------------------------------------------
// Utilitários
// -----------------------------------------------------------------------------

RETANGULO clona_retangulo(RETANGULO r, int novoId) {
    retangulo* orig = (retangulo*)r;
    if (!orig) return NULL;

    retangulo* novo = (retangulo*)malloc(sizeof(retangulo));
    if (!novo) return NULL;

    novo->i = novoId;
    novo->x = orig->x;
    novo->y = orig->y;
    novo->w = orig->w;
    novo->h = orig->h;
    novo->cor_b = orig->cor_b ? dupstr(orig->cor_b) : NULL;
    novo->cor_p = orig->cor_p ? dupstr(orig->cor_p) : NULL;

    if ((orig->cor_b && !novo->cor_b) || (orig->cor_p && !novo->cor_p)) {
        free(novo->cor_b);
        free(novo->cor_p);
        free(novo);
        return NULL;
    }

    return (RETANGULO)novo;
}

void inverteCores_ret(RETANGULO c) {
    retangulo* ret = (retangulo*)c;
    if (!ret) return;
    char* tmp = ret->cor_b;
    ret->cor_b = ret->cor_p;
    ret->cor_p = tmp;
}

// -----------------------------------------------------------------------------
// Aliases para geometria.c
// -----------------------------------------------------------------------------

int         get_id_ret(RETANGULO r) { return le_id_ret(r); }
double      get_x_ret(RETANGULO r)  { return get_cord_x_ret(r); }
double      get_y_ret(RETANGULO r)  { return get_cord_y_ret(r); }
double      get_w_ret(RETANGULO r)  { return get_width_ret(r); }
double      get_h_ret(RETANGULO r)  { return get_heigth_ret(r); }
const char* get_corb_ret(RETANGULO r){ return get_corB_ret(r); }
const char* get_corp_ret(RETANGULO r){ return get_corP_ret(r); }
void        set_corb_ret(RETANGULO r, const char* c){ set_corB_ret(r, c); }
void        set_corp_ret(RETANGULO r, const char* c){ set_corP_ret(r, c); }
