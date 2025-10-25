#pragma once

typedef void* CIRCLE;

CIRCLE criaCirculoInterno(int id, double x, double y, double r, const char* corb, const char* corp);

int getIdCirculo(CIRCLE c);
double getXCirculo(CIRCLE c);
double getYCirculo(CIRCLE c);
double getRaioCirculo(CIRCLE c);
const char* getCorBordaCirculo(CIRCLE c);
const char* getCorPreenchimentoCirculo(CIRCLE c);
void setIdCirculo(CIRCLE c, int id);
void setYCirculo(CIRCLE c, double y);
void setXCirculo(CIRCLE c, double x);
void setCorBordaCirculo(CIRCLE c, const char* novaCor);
void setCorPreenchimentoCirculo(CIRCLE c, const char* novaCor);
void setRCirculo(CIRCLE c, double r);
double areaCirculo(CIRCLE c);
CIRCLE clonaCirculo(CIRCLE c, int novoId);
void inverteCores_cir(CIRCLE c);
void destruir_circulo(CIRCLE c);