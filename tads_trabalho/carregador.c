#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "carregador.h"
#include "/workspaces/trab1/formas/circulo.h"
#include "/workspaces/trab1/formas/retangulo.h"
#include "/workspaces/trab1/formas/linha.h"
#include "/workspaces/trab1/formas/texto.h"
#include "/workspaces/trab1/formas/forma.h"

static char* estiloFamily = NULL;
static char* estiloWeight = NULL;
static char* estiloSize = NULL;

void defineEstiloTexto(const char* family, const char* weight, const char* size) {
    free(estiloFamily);
    free(estiloWeight);
    free(estiloSize);

    estiloFamily = strdup(family);
    estiloWeight = strdup(weight);
    estiloSize = strdup(size);
}

void carregarGeo(const char* caminhoGeo, FILA filaChao) {
    FILA* geo = fopen(caminhoGeo, "r");
    if (!geo) {
        fprintf(stderr, "Erro ao abrir arquivo GEO: %s\n", caminhoGeo);
        exit(1);
    }

    char comando[8];
    while (fscanf(geo, "%s", comando) != EOF) {
        if (strcmp(comando, "c") == 0) {
            int id;
            double x, y, r;
            char corB[64], corP[64];
            fscanf(geo, "%d %lf %lf %lf %s %s", &id, &x, &y, &r, corB, corP);
            CIRCULO c = criaCirculo(id, x, y, r, corB, corP);
            FORMA f = criaFormaCirculo(c);
            pushFila(filaChao, f);
        }

        else if (strcmp(comando, "r") == 0) {
            int id;
            double x, y, w, h;
            char corB[64], corP[64];
            fscanf(geo, "%d %lf %lf %lf %lf %s %s", &id, &x, &y, &w, &h, corB, corP);
            RETANGULO r_ = criaRetangulo(id, x, y, w, h, corB, corP);
            FORMA f = criaFormaRetangulo(r_);
            pushFila(filaChao, f);
        }

        else if (strcmp(comando, "l") == 0) {
            int id;
            double x1, y1, x2, y2;
            char cor[64];
            fscanf(geo, "%d %lf %lf %lf %lf %s", &id, &x1, &y1, &x2, &y2, cor);
            LINHA l = criaLinha(id, x1, y1, x2, y2, cor);
            FORMA f = criaFormaLinha(l);
            pushFila(filaChao, f);
        }

        else if (strcmp(comando, "t") == 0) {
            int id;
            double x, y;
            char corB[64], corP[64], ancora[8];
            char txt[256];

            fscanf(geo, "%d %lf %lf %s %s %s", &id, &x, &y, corB, corP, ancora);
            fgets(txt, sizeof(txt), geo);
            txt[strcspn(txt, "\n")] = 0; // remove \n

            TEXTO t = criaTexto(id, x, y, corB, corP, ancora, &txt);
            FORMA f = criaFormaTexto(t);
            pushFila(filaChao, f);
        }

        else if (strcmp(comando, "ts") == 0) {
            char fam[64], weight[64], size[64];
            fscanf(geo, "%s %s %s", fam, weight, size);
            defineEstiloTexto(fam, weight, size);
        }
    }

    fclose(geo);
}

void destruirCarregador(void) {
    free(estiloFamily);
    free(estiloWeight);
    free(estiloSize);
}
