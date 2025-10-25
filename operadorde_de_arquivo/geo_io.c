#include "geo_io.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "../formas/circulo.h"
#include "../formas/retangulo.h"
#include "../formas/linha.h"
#include "../formas/texto.h"
#include "../formas/forma.h"
#include "../tads_gerais/fila.h"

/* --------- util --------- */

static void rstrip(char *s) {
    if (!s) return;
    size_t n = strlen(s);
    while (n && (s[n-1]=='\n' || s[n-1]=='\r' || isspace((unsigned char)s[n-1]))) {
        s[--n] = '\0';
    }
}

static char *lskip(const char *s) {
    while (*s && isspace((unsigned char)*s)) s++;
    return (char*)s;
}

/* lê a próxima palavra (sem aspas) em src; 
 * devolve ponteiro no próprio buffer e avança *cursor */
static char* next_token(char **cursor) {
    char *s = *cursor;
    s = lskip(s);
    if (!*s) { *cursor = s; return NULL; }
    char *start = s;
    while (*s && !isspace((unsigned char)*s)) s++;
    if (*s) { *s = '\0'; s++; }
    *cursor = s;
    return start;
}

/* Lê o resto da linha como texto (preserva espaços).
 * Se vier entre aspas, remove aspas externas. */
static char* read_rest_as_text(char *s) {
    s = lskip(s);
    if (!*s) return strdup("");
    if (*s == '\"') {
        s++;
        char *end = strrchr(s, '\"');
        if (end) *end = '\0';
        return strdup(s);
    }
    return strdup(s);
}

/* --------- estado de estilo de texto (ts) --------- */

typedef struct {
    char family[32];  /* "sans", "serif", "cursive" */
    char weight[8];   /* "n", "b", "b+", "l" */
    char size[16];    /* livre, ex: "12" ou "12px" */
} EstiloTexto;

static void estilo_texto_default(EstiloTexto *e) {
    strcpy(e->family, "sans");
    strcpy(e->weight, "n");
    strcpy(e->size,   "12");
}

/* --------- criação de formas e enfileiramento --------- */

static int enfileira_forma(FILA fila_chao, FORMA f) {
    if (!f) return 0;
    add_na_fila(fila_chao, f);
    return 1;
}

/* --------- parser .geo --------- */

int geo_ler(FILE *geo, FILA fila_chao) {
    if (!geo || !fila_chao) return 0;

    char line[1024];
    int criadas = 0;
    EstiloTexto estilo;
    estilo_texto_default(&estilo);

    long lineno = 0;

    while (fgets(line, sizeof(line), geo)) {
        lineno++;
        rstrip(line);
        char *p = lskip(line);

        /* ignora vazias e comentários (# ...) */
        if (!*p || *p == '#') continue;

        /* pega comando (primeiro token) */
        char *cursor = p;
        char *cmd = next_token(&cursor);
        if (!cmd) continue;

        /* ----- círculo: c i x y r corb corp ----- */
        if (strcmp(cmd, "c") == 0) {
            int i;
            double x,y,r;
            char corb[64], corp[64];
            int ok = sscanf(cursor, "%d %lf %lf %lf %63s %63s", &i, &x, &y, &r, corb, corp);
            if (ok != 6) {
                fprintf(stderr, "[.geo:%ld] Erro em 'c': esperado i x y r corb corp\n", lineno);
                continue;
            }
            CIRCULO c = criaCirculo(i, x, y, r, corb, corp);
            FORMA f = criaForma('c', c);
            criadas += enfileira_forma(fila_chao, f);
            continue;
        }

        /* ----- retângulo: r i x y w h corb corp ----- */
        if (strcmp(cmd, "r") == 0) {
            int i;
            double x,y,w,h;
            char corb[64], corp[64];
            int ok = sscanf(cursor, "%d %lf %lf %lf %lf %63s %63s", &i, &x, &y, &w, &h, corb, corp);
            if (ok != 8) {
                fprintf(stderr, "[.geo:%ld] Erro em 'r': esperado i x y w h corb corp\n", lineno);
                continue;
            }
            RETANGULO r = criaRetangulo(i, x, y, w, h, corb, corp);
            FORMA f = criaForma('r', r);
            criadas += enfileira_forma(fila_chao, f);
            continue;
        }

        /* ----- linha: l i x1 y1 x2 y2 cor ----- */
        if (strcmp(cmd, "l") == 0) {
            int i;
            double x1,y1,x2,y2;
            char cor[64];
            int ok = sscanf(cursor, "%d %lf %lf %lf %lf %63s", &i, &x1, &y1, &x2, &y2, cor);
            if (ok != 6) {
                fprintf(stderr, "[.geo:%ld] Erro em 'l': esperado i x1 y1 x2 y2 cor\n", lineno);
                continue;
            }
            LINHA l = criaLinha(i, x1, y1, x2, y2, cor);
            FORMA f = criaForma('l', l);
            criadas += enfileira_forma(fila_chao, f);
            continue;
        }

        /* ----- texto: t i x y corb corp a texto... ----- */
        if (strcmp(cmd, "t") == 0) {
            /* precisamos pegar os 6 primeiros campos e depois o resto da linha */
            int i;
            double x, y;
            char corb[64], corp[64], ancora_char[8];

            /* tokenize manualmente para sobrar o resto como texto */
            char *c1 = cursor;
            char *tok_i   = next_token(&c1);
            char *tok_x   = next_token(&c1);
            char *tok_y   = next_token(&c1);
            char *tok_corb= next_token(&c1);
            char *tok_corp= next_token(&c1);
            char *tok_a   = next_token(&c1);

            if (!tok_i || !tok_x || !tok_y || !tok_corb || !tok_corp || !tok_a) {
                fprintf(stderr, "[.geo:%ld] Erro em 't': esperado i x y corb corp a texto...\n", lineno);
                continue;
            }

            i = atoi(tok_i);
            x = strtod(tok_x, NULL);
            y = strtod(tok_y, NULL);
            strncpy(corb, tok_corb, sizeof(corb)); corb[sizeof(corb)-1] = '\0';
            strncpy(corp, tok_corp, sizeof(corp)); corp[sizeof(corp)-1] = '\0';
            strncpy(ancora_char, tok_a, sizeof(ancora_char)); ancora_char[sizeof(ancora_char)-1] = '\0';

            char ancora = ancora_char[0];
            if (ancora != 'i' && ancora != 'm' && ancora != 'f') {
                fprintf(stderr, "[.geo:%ld] Aviso em 't': âncora inválida '%c' (use i|m|f). Usando 'i'.\n", lineno, ancora);
                ancora = 'i';
            }

            char *texto = read_rest_as_text(c1);

            /* Dois possíveis formatos de API do seu TAD TEXTO:
             * 1) Sem estilo: TEXTO criaTexto(int id, double x, double y, char a, const char* corb, const char* corp, const char* txt);
             * 2) Com estilo:  TEXTO criaTextoEstilizado(int id, double x, double y, char a, const char* corb, const char* corp,
             *                                            const char* family, const char* weight, const char* size, const char* txt);
             *
             * Se você usa o formato (1), defina -DTEXTO_SEM_ESTILO no Makefile.
             */
            TEXTO tobj = NULL;

#ifdef TEXTO_SEM_ESTILO
            tobj = criaTexto(i, x, y, ancora, corb, corp, texto);
#else
            /* caso exista a versão estilizada; se não existir, basta criar um thin wrapper no seu TAD texto */
            tobj = criaTextoEstilizado(i, x, y, ancora, corb, corp,
                                       estilo.family, estilo.weight, estilo.size,
                                       texto);
#endif
            free(texto);

            if (!tobj) {
                fprintf(stderr, "[.geo:%ld] Erro ao criar TEXTO (id=%d)\n", lineno, i);
                continue;
            }

            FORMA f = criaForma('t', tobj);
            criadas += enfileira_forma(fila_chao, f);
            continue;
        }

        /* ----- estilo de textos: ts fFamily fWeight fSize ----- */
        if (strcmp(cmd, "ts") == 0) {
            char fam[32], w[8], sz[16];
            int ok = sscanf(cursor, "%31s %7s %15s", fam, w, sz);
            if (ok != 3) {
                fprintf(stderr, "[.geo:%ld] Erro em 'ts': esperado fFamily fWeight fSize\n", lineno);
                continue;
            }
            strncpy(estilo.family, fam, sizeof(estilo.family));
            estilo.family[sizeof(estilo.family)-1] = '\0';
            strncpy(estilo.weight, w, sizeof(estilo.weight));
            estilo.weight[sizeof(estilo.weight)-1] = '\0';
            strncpy(estilo.size, sz, sizeof(estilo.size));
            estilo.size[sizeof(estilo.size)-1] = '\0';
            continue;
        }

        /* comando desconhecido: ignore com aviso */
        fprintf(stderr, "[.geo:%ld] Comando desconhecido '%s' — ignorando.\n", lineno, cmd);
    }

    return criadas;
}
