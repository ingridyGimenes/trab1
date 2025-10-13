#ifndef retangulo_h
#define retangulo_h

typedef void *RETANGULO;

RETANGULO cria_retangulo(int i, int x, int y, float w, float h, char cor_b, char cor_p);

int le_id_ret(RETANGULO r);

int get_cord_x_ret(RETANGULO r);

int get_cord_y_ret(RETANGULO r);

int get_width_ret(RETANGULO r);

int get_heigth_ret(RETANGULO r);

char* get_corB_ret(RETANGULO r);

char* get_corP_ret(RETANGULO r);

int calcula_area_ret(RETANGULO r);

void set_cordX_ret(RETANGULO r, int cordX);

void set_cordY_ret(RETANGULO r, int cordY);

void set_height_ret(RETANGULO r, float H);

void set_width_ret(RETANGULO r, float W);

void set_corB_ret(RETANGULO r, char* corB);

void set_corP_ret(RETANGULO r, char* corP);

void exclui_retangulo(RETANGULO r);

#endif