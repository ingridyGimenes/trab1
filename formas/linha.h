#pragma once

typedef void* LINHA;

LINHA criaLinha(int id, double x1, double y1, double x2, double y2, const char* cor);

int getIdLinha(LINHA l);
double getX1Linha(LINHA l);
double getY1Linha(LINHA l);
double getX2Linha(LINHA l);
double getY2Linha(LINHA l);
const char* getCorLinha(LINHA l);

void setIdLinha(LINHA l, int id);
void setX1Linha(LINHA l, double x);
void setY1Linha(LINHA l, double y);
void setX2Linha(LINHA l, double x);
void setY2Linha(LINHA l, double y);
void setCorLinha(LINHA l, const char* cor);

LINHA clonaLinha(LINHA l, int novoId);
void destruirLinha(LINHA l);
