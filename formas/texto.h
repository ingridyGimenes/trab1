#ifndef texto_h
#define texto_h

typedef void *TEXTO;
typedef void *Estilo;
typedef texto Texto;


Estilo cria_estilo(char* Family, char* Weight, char* Size );

TEXTO cria_texto( int i, double codX, double codY, char *cor_b, char *cor_p, char *a, char *TXTO);


int get_id_txt(TEXTO t);

double get_codx_txt(TEXTO t);

double get_cody_txt(TEXTO t);

char* get_corB_txt(TEXTO t);

char* get_corP_txt(TEXTO t);

char* get_ancora_txt(TEXTO t);

char* get_txto_txt(TEXTO t);

char* getFamily(Estilo e);

char* getWeight(Estilo e);

char* getSize(Estilo e);


void set_codx_txt(TEXTO t, double Cx);

void set_cody_txt(TEXTO t, double Cy);

void set_corB_txt(TEXTO t, char* cor_b);

void set_corP_txt(TEXTO t, char* cor_p);

void set_ancora_txt(TEXTO t, char* anc);

void set_txto_txt(TEXTO t, char* cont);

void setFamily(Estilo e, char* familia);

void setWeight(Estilo e, char* wgt);

void setSize(Estilo e, char* sz);



#endif


