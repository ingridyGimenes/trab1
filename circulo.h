#ifndef circulo_h
#define circulo_h

typedef void *CIRCLE;

CIRCLE cria_circulo(int ID, int cod_x, int cod_y, double r, char cor_P, char cor_B);

int get_id_circ(CIRCLE c);

int get_cordx_cir(CIRCLE c);

int get_cordy_cir(CIRCLE c);

int get_raio_cir(CIRCLE c);

char* get_corB_cir(CIRCLE c);

char* get_corP_cir(CIRCLE c);

void set_x_cir(CIRCLE c, int x);

void set_y_cir(CIRCLE c, int Y);

void set_raio_cir(CIRCLE c, int r);

void set_corB_cir(CIRCLE c, char* corB);

void set_corP_cir(CIRCLE c, char* corP);

double calcula_area_circulo(CIRCLE c);

#endif