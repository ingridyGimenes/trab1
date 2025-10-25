#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "carregador.h"
#include "../formas/circulo.h"
#include "../formas/retangulo.h"
#include "../formas/linha.h"
#include "../formas/texto.h"
#include "../formas/forma.h"
#include "../tads_gerais/fila.h"

// -----------------------------------------------------------------------------
// Estado interno do módulo (estilo global do texto definido por 'ts')
// -----------------------------------------------------------------------------
static ESTILO gEstilo = NULL;

// Helpers de string (internos ao carregador)
static void lstrip(char* s) {
    if (!s) return;
    size_t i = 0, j = 0;
    while (s[i] == ' ' || s[i] == '\t') i++;
    if (i) {
        while (s[i]) s[j++] = s[i++];
        s[j] = '\0';
    }
}

static void rstrip(char* s) {
    if (!s) return;
    size_t n = strlen(s);
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r' || s[n-1] == ' ' || s[n-1] == '\t'))
        s[--n] = '\0';
}

static void strip(char* s) {
    rstrip(s);
    lstrip(s);
}

static void strip_quotes(char* s) {
    if (!s) return;
    size_t n = strlen(s);
    if (n >= 2 && ((s[0] == '"' && s[n-1] == '"') || (s[0] == '\'' && s[n-1] == '\''))) {
        // remove aspas envolventes
        memmove(s, s+1, n-2);
        s[n-2] = '\0';
    }
}

// -----------------------------------------------------------------------------
// API pública
// -----------------------------------------------------------------------------
void defineEstiloTexto(const char* family, const char* weight, const char* size) {
    // recria estilo global sempre que 'ts' aparece
    if (gEstilo) destruirEstilo(gEstilo);
    gEstilo = criaEstilo(family ? family : "sans", weight ? weight : "normal", size ? size : "12px");
}

void destruirCarregador(void) {
    if (gEstilo) {
        destruirEstilo(gEstilo);
        gEstilo = NULL;
    }
}

void carregarGeo(const char* caminhoGeo, FILA filaChao) {
    if (!caminhoGeo || !filaChao) {
        fprintf(stderr, "[carregador] parâmetros inválidos.\n");
        return;
    }

    FILE* geo = fopen(caminhoGeo, "r");
    if (!geo) {
        fprintf(stderr, "[carregador] erro ao abrir GEO: %s\n", caminhoGeo);
        exit(1);
    }

    // Se não houve 'ts' no arquivo, garanta um estilo padrão
    if (!gEstilo) {
        gEstilo = criaEstilo("sans", "normal", "12px");
    }

    char cmd[8];
    // buffers auxiliares
    char corB[128], corP[128], corL[128];
    char ancoraStr[8];

    while (fscanf(geo, "%7s", cmd) != EOF) {

        // --------------------------
        // círculo: c id x y r corB corP
        // --------------------------
        if (strcmp(cmd, "c") == 0) {
            int id; double x, y, r;
            if (fscanf(geo, "%d %lf %lf %lf %127s %127s", &id, &x, &y, &r, corB, corP) != 6) {
                fprintf(stderr, "[carregador] linha 'c' malformada.\n");
                break;
            }
            CIRCULO c = criaCirculo(id, x, y, r, corB, corP);
            FORMA f = criaForma('c', c);
            insereNaFila(filaChao, f);
        }

        // --------------------------
        // retângulo: r id x y w h corB corP
        // --------------------------
        else if (strcmp(cmd, "r") == 0) {
            int id; double x, y, w, h;
            if (fscanf(geo, "%d %lf %lf %lf %lf %127s %127s",
                       &id, &x, &y, &w, &h, corB, corP) != 7) {
                fprintf(stderr, "[carregador] linha 'r' malformada.\n");
                break;
            }
            RETANGULO r_ = criaRetangulo(id, x, y, w, h, corB, corP);
            FORMA f = criaForma('r', r_);
            insereNaFila(filaChao, f);
        }

        // --------------------------
        // linha: l id x1 y1 x2 y2 cor
        // --------------------------
        else if (strcmp(cmd, "l") == 0) {
            int id; double x1, y1, x2, y2;
            if (fscanf(geo, "%d %lf %lf %lf %lf %127s",
                       &id, &x1, &y1, &x2, &y2, corL) != 6) {
                fprintf(stderr, "[carregador] linha 'l' malformada.\n");
                break;
            }
            LINHA l = criaLinha(id, x1, y1, x2, y2, corL);
            FORMA f = criaForma('l', l);
            insereNaFila(filaChao, f);
        }

        // --------------------------
        // texto: t id x y corB corP ancora conteudo...
        // OBS: conteudo é o resto da linha; ancora deve ser 1 char ('i','m','f' etc.)
        // --------------------------
        else if (strcmp(cmd, "t") == 0) {
            int id; double x, y;
            if (fscanf(geo, "%d %lf %lf %127s %127s %7s", &id, &x, &y, corB, corP, ancoraStr) != 6) {
                fprintf(stderr, "[carregador] linha 't' malformada.\n");
                break;
            }
            // lê o restante da linha como conteúdo
            char linhaRestante[1024] = {0};
            fgets(linhaRestante, sizeof(linhaRestante), geo);
            strip(linhaRestante);
            strip_quotes(linhaRestante); // remove aspas se veio "assim"

            char ancora = ancoraStr[0]; // pega o primeiro char

            // garante estilo (se alguém chamou destruirCarregador antes por engano)
            if (!gEstilo) gEstilo = criaEstilo("sans", "normal", "12px");

            TEXTO t = criaTexto(id, x, y, corB, corP, ancora, linhaRestante, gEstilo);
            FORMA f = criaForma('t', t);
            insereNaFila(filaChao, f);
        }

        // --------------------------
        // estilo de texto: ts family weight size
        // --------------------------
        else if (strcmp(cmd, "ts") == 0) {
            char fam[128], weight[128], size[128];
            if (fscanf(geo, "%127s %127s %127s", fam, weight, size) != 3) {
                fprintf(stderr, "[carregador] linha 'ts' malformada.\n");
                break;
            }
            defineEstiloTexto(fam, weight, size);
        }

        // --------------------------
        // comando desconhecido: consome resto da linha para evitar loop
        // --------------------------
        else {
            char dump[1024];
            fgets(dump, sizeof(dump), geo);
            fprintf(stderr, "[carregador] comando desconhecido: %s\n", cmd);
        }
    }

    fclose(geo);
}
