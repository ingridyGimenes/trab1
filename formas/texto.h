#pragma once
typedef void* TEXTO;
typedef void* Estilo;

Estilo cria_estilo(const char* family, const char* weight, const char* size);

void   destruir_estilo(Estilo e);
char*  getFamily(Estilo e);
char*  getWeight(Estilo e);
char*  getSize(Estilo e);
void   setFamily(Estilo e, const char* familia);
void   setWeight(Estilo e, const char* wgt);
void   setSize(Estilo e, const char* sz);

TEXTO cria_texto(int id, double x, double y, const char *cor_b, const char *cor_p, char ancora, const char *conteudo);

void destruir_texto(TEXTO t);
int     get_id_txt(TEXTO t);
double  get_codx_txt(TEXTO t);
double  get_cody_txt(TEXTO t);
char*   get_corB_txt(TEXTO t);
char*   get_corP_txt(TEXTO t);
char    get_ancora_txt(TEXTO t);
char*   get_txto_txt(TEXTO t);
int     get_len_txt(TEXTO t);
void set_codx_txt(TEXTO t, double x);
void set_cody_txt(TEXTO t, double y);
void set_corB_txt(TEXTO t, const char* cor_b);
void set_corP_txt(TEXTO t, const char* cor_p);
void set_ancora_txt(TEXTO t, char ancora);
void set_txto_txt(TEXTO t, const char* cont);
double calcula_area_txt(TEXTO t);
TEXTO  clona_texto(TEXTO t, int novoId);
void   inverte_cores_txt(TEXTO t);
