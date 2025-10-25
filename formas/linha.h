#pragma once

typedef void* LINHA;

LINHA cria_linha(int i, double x_1, double x_2, double y_1, double y_2, char* corL);

int get_id_linha(LINHA L);
double get_codx1_linha(LINHA L);
double get_codx2_linha(LINHA L);
double get_cody1_linha(LINHA L);
double get_cody2_linha(LINHA L);
char* get_cor_linha(LINHA L);
void set_codx1_linha(LINHA L, double x);
void set_codx2_linha(LINHA L, double x);
void set_cody1_linha(LINHA L, double y);
void set_cody2_linha(LINHA L, double y);
void set_cor_linha(LINHA L, char* corL);
LINHA clona_linha(LINHA L, int novo_id);
void destruir_linha(LINHA L);
