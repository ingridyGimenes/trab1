#pragma once

typedef void* TEXTO;
typedef void* ESTILO;

ESTILO criaEstilo(const char* family, const char* weight, const char* size);
void destruirEstilo(ESTILO e);

const char* getFamily(ESTILO e);
const char* getWeight(ESTILO e);
const char* getSize(ESTILO e);

void setFamily(ESTILO e, const char* family);
void setWeight(ESTILO e, const char* weight);
void setSize(ESTILO e, const char* size);

TEXTO criaTexto(int id, double x, double y, const char* corB, const char* corP, char ancora, const char* conteudo, ESTILO estilo);
void destruirTexto(TEXTO t);

int getIdTexto(TEXTO t);
double getXTexto(TEXTO t);
double getYTexto(TEXTO t);
const char* getCorBordaTexto(TEXTO t);
const char* getCorPreenchimentoTexto(TEXTO t);
char getAncoraTexto(TEXTO t);
const char* getConteudoTexto(TEXTO t);
ESTILO getEstiloTexto(TEXTO t);

void setIdTexto(TEXTO t, int id);
void setXTexto(TEXTO t, double x);
void setYTexto(TEXTO t, double y);
void setCorBordaTexto(TEXTO t, const char* corB);
void setCorPreenchimentoTexto(TEXTO t, const char* corP);
void setAncoraTexto(TEXTO t, char ancora);
void setConteudoTexto(TEXTO t, const char* conteudo);
void setEstiloTexto(TEXTO t, ESTILO estilo);

double calculaAreaTexto(TEXTO t);
TEXTO clonaTexto(TEXTO t, int novoId);
void inverterCoresTexto(TEXTO t);
