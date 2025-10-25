#pragma once

typedef void* CIRCULO;

CIRCULO criaCirculo(int id, double x, double y, double r, const char* corB, const char* corP);

int getIdCirculo(CIRCULO c);
double getXCirculo(CIRCULO c);
double getYCirculo(CIRCULO c);
double getRaioCirculo(CIRCULO c);
const char* getCorBordaCirculo(CIRCULO c);
const char* getCorPreenchimentoCirculo(CIRCULO c);

void setIdCirculo(CIRCULO c, int id);
void setXCirculo(CIRCULO c, double x);
void setYCirculo(CIRCULO c, double y);
void setRaioCirculo(CIRCULO c, double r);
void setCorBordaCirculo(CIRCULO c, const char* novaCor);
void setCorPreenchimentoCirculo(CIRCULO c, const char* novaCor);

double calculaAreaCirculo(CIRCULO c);
CIRCULO clonaCirculo(CIRCULO c, int novoId);
void inverterCoresCirculo(CIRCULO c);

void destruirCirculo(CIRCULO c);
