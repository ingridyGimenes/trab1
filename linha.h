#ifndef linha_h
#define  linha_h

typedef void *LINHA;

LINHA cria_linha(int i, int x_1, int x_2, int y_1, int y_2, char* corL);

int get_id_linha(LINHA L);

int get_codx1_linha(LINHA L);


int get_codx2_linha(LINHA L);

int get_cody1_linha(LINHA L);


int get_cody2_linha(LINHA L);

char* get_cor_linha(LINHA L);




void set_codx1_linha(LINHA L, int x);


void set_codx2_linha(LINHA L, int x);

void set_cody1_linha(LINHA L, int y);


void set_cody2_linha(LINHA L, int y);

void set_cor_linha(LINHA L, char* corL);


#endif