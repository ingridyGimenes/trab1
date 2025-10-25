#pragma once

typedef void* RETANGULO;

RETANGULO criaRetangulo(int id, double x, double y, double largura, double altura, const char* corB, const char* corP);

int getIdRetangulo(RETANGULO r);
double getXRetangulo(RETANGULO r);
double getYRetangulo(RETANGULO r);
double getLarguraRetangulo(RETANGULO r);
double getAlturaRetangulo(RETANGULO r);
const char* getCorBordaRetangulo(RETANGULO r);
const char* getCorPreenchimentoRetangulo(RETANGULO r);

void setIdRetangulo(RETANGULO r, int id);
void setXRetangulo(RETANGULO r, double x);
void setYRetangulo(RETANGULO r, double y);
void setLarguraRetangulo(RETANGULO r, double w);
void setAlturaRetangulo(RETANGULO r, double h);
void setCorBordaRetangulo(RETANGULO r, const char* corB);
void setCorPreenchimentoRetangulo(RETANGULO r, const char* corP);

double calculaAreaRetangulo(RETANGULO r);
RETANGULO clonaRetangulo(RETANGULO r, int novoId);
void inverterCoresRetangulo(RETANGULO r);

void destruirRetangulo(RETANGULO r);
