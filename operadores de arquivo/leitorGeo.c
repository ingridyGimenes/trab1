#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leitorGeo.h"

// --------------------------------------------------------------
// Função auxiliar: lê o próximo comando e cria o tipo correto de forma
// --------------------------------------------------------------
void carregarGeo(const char *nomeArquivo, Fila *chao) {
    //cria um leitor 
    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", nomeArquivo);
        exit(1);
    }

    char comando[10];

    // leitura linha a linha
    while (fscanf(arquivo, "%s", comando) != EOF) {
        // ----------------------------------------------------------
        // Círculo
        // c i x y r corb corp
        // ----------------------------------------------------------
        if (strcmp(comando, "c") == 0) {
            int id;
            double x, y, r;
            char corb[30], corp[30];

            fscanf(arquivo, "%d %lf %lf %lf %s %s", &id, &x, &y, &r, corb, corp);

            Circulo *c = cria_circulo(id, x, y, r, corb, corp);
            add_na_fila(chao, c);
        }

        // ----------------------------------------------------------
        // Retângulo
        // r i x y w h corb corp
        // ----------------------------------------------------------
        else if (strcmp(comando, "r") == 0) {
            int id;
            double x, y, w, h;
            char corb[30], corp[30];

            fscanf(arquivo, "%d %lf %lf %lf %lf %s %s", &id, &x, &y, &w, &h, corb, corp);

            Retangulo *r = cria_retangulo(id, x, y, w, h, corb, corp);
            add_na_fila(chao, r);

        }

        // ----------------------------------------------------------
        // Linha
        // l i x1 y1 x2 y2 cor
        // ----------------------------------------------------------
        else if (strcmp(comando, "l") == 0) {
            int id;
            double x1, y1, x2, y2;
            char cor[30];

            fscanf(arquivo, "%d %lf %lf %lf %lf %s", &id, &x1, &y1, &x2, &y2, cor);

            Linha *l = cria_linha(id, x1, y1, x2, y2, cor);
            add_na_fila(chao, l);
        }

        // ----------------------------------------------------------
        // Texto
        // t i x y corb corp a txto
        // ----------------------------------------------------------
        else if (strcmp(comando, "t") == 0) {
            int id;
            double x, y;
            char corb[30], corp[30], ancora;
            char texto[200];

            fscanf(arquivo, "%d %lf %lf %s %s %c", &id, &x, &y, corb, corp, &ancora);
            fgets(texto, sizeof(texto), arquivo); // lê o resto da linha (texto pode ter espaços)

            // remove quebra de linha se houver
            texto[strcspn(texto, "\n")] = '\0';

            Texto *t = cria_texto(id, x, y, corb, corp, ancora, texto);
            add_na_fila(chao, t);
        }

        // ----------------------------------------------------------
        // Comando desconhecido
        // ----------------------------------------------------------
        else {
            fprintf(stderr, "Comando desconhecido: %s\n", comando);
            // descarta linha
            char lixo[200];
            fgets(lixo, sizeof(lixo), arquivo);
        }
    }

    fclose(arquivo);
}
