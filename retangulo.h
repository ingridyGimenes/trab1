#ifndef retangulo_h
#define retangulo_h

typedef void* RETANGULO;

void* cria_retangulo(int i, double x, double y, double w, double h, char* cor_b, char* cor_p);

int le_id_ret(RETANGULO r);

double get_cord_x_ret(RETANGULO r);

double get_cord_y_ret(RETANGULO r);

double get_width_ret(RETANGULO r);

double get_heigth_ret(RETANGULO r);

char* get_corB_ret(RETANGULO r);

char* get_corP_ret(RETANGULO r);

double calcula_area_ret(RETANGULO r);

void setIdRetangulo(RETANGULO r, int id);

void set_cordX_ret(RETANGULO r, double cordX);

void set_cordY_ret(RETANGULO r, double cordY);

void set_height_ret(RETANGULO r, double H);

void set_width_ret(RETANGULO r, double W);

void set_corB_ret(RETANGULO r, char* corB);

void set_corP_ret(RETANGULO r, char* corP);

void exclui_retangulo(RETANGULO r);

RETANGULO clonaRetangulo(RETANGULO r, int novoId);

#endif