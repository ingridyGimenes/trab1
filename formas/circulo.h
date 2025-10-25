#ifndef CIRCULO_H
#define CIRCULO_H

typedef void* CIRCLE;
typedef circulo Circulo;

// Funções internas chamadas por formas.c
CIRCLE criaCirculoInterno(int id, double x, double y, double r, const char* corb, const char* corp);


int getIdCirculo(CIRCLE c);
double getXCirculo(CIRCLE c);
double getYCirculo(CIRCLE c);
double getRaioCirculo(CIRCLE c);

const char* getCorBordaCirculo(CIRCLE c);
const char* getCorPreenchimentoCirculo(CIRCLE c);

void setIdCirculo( CIRCLE c, int id);
void setPosCirculo(CIRCLE c, double x, double y);
void setYCirculo(CIRCLE c, double y);
void setXCirculo(CIRCLE c, double x);
void setCorBordaCirculo(CIRCLE c, const char* novaCor);
void setCorPreenchimentoCirculo(CIRCLE c, const char* novaCor);
void inverteCores_cir(CIRCLE c);

double areaCirculo(CIRCLE c);
CIRCLE clonaCirculo(CIRCLE c, int novoId);
void destroiCirculo(CIRCLE c);

#endif
