
#include <string.h>
#include "geo_io.h"

#include <stdlib.h>

#include <ctype.h>

/* ------------------------------------------------------------------
   Dependências dos seus TADs
   ------------------------------------------------------------------ */
#include "../tads_gerais/fila.h"
#include "../formas/forma.h"
#include "../formas/circulo.h"
#include "../formas/retangulo.h"
#include "../formas/linha.h"
#include "../formas/texto.h"

/* Wrappers para criar FORMA a partir dos TADs concretos */
static FORMA mkFormaCirculo(int id, double x, double y, double r,
                            const char* cor_b, const char* cor_p) {
    CIRCULO c = criaCirculo(id, x, y, r, cor_b, cor_p);
    return criaForma('c', c);
}
static FORMA mkFormaRetangulo(int id, double x, double y, double w, double h,
                              const char* cor_b, const char* cor_p) {
    RETANGULO r = criaRetangulo(id, x, y, w, h, cor_b, cor_p);
    return criaForma('r', r);
}
static FORMA mkFormaLinha(int id, double x1, double y1, double x2, double y2,
                          const char* cor) {
    LINHA l = criaLinha(id, x1, y1, x2, y2, cor);
    return criaForma('l', l);
}
static FORMA mkFormaTexto(int id, double x, double y,
                          const char* cor_b, const char* cor_p, char ancora,
                          const char* txto,
                          const char* family, const char* weight, const char* size) {
    ESTILO est = criaEstilo(family, weight, size);
    TEXTO t = criaTexto(id, x, y, cor_b, cor_p, ancora, txto, est);
    return criaForma('t', t);
}


static char* next_tok(char **cursor) {
    char *s = *cursor;
    while (*s && isspace((unsigned char)*s)) s++;
    if (!*s) { *cursor = s; return NULL; }
    char *start = s;
    while (*s && !isspace((unsigned char)*s)) s++;
    if (*s) { *s = '\0'; s++; }
    *cursor = s;
    return start;
}

static void rstrip(char *s) {
    size_t n = strlen(s);
    while (n && (s[n-1]=='\n' || s[n-1]=='\r' || isspace((unsigned char)s[n-1]))) s[--n] = '\0';
}

/* Mantém o estilo de texto corrente definido por 'ts' */
typedef struct {
    char family[16];  /* sans|serif|cursive */
    char weight[8];   /* n|b|b+|l */
    char size[16];    /* ex.: "12", "14px" etc. */
} TextStyle;

static void ts_default(TextStyle *ts) {
    strcpy(ts->family, "sans");
    strcpy(ts->weight, "n");
    strcpy(ts->size,   "12");
}

/* Normaliza tokens do ts (opcional) */
static void ts_apply(TextStyle *ts, const char* fam, const char* wei, const char* siz) {
    if (fam && *fam)  strncpy(ts->family, fam,  sizeof(ts->family)-1),  ts->family[sizeof(ts->family)-1] = '\0';
    if (wei && *wei)  strncpy(ts->weight, wei,  sizeof(ts->weight)-1),  ts->weight[sizeof(ts->weight)-1] = '\0';
    if (siz && *siz)  strncpy(ts->size,   siz,  sizeof(ts->size)-1),    ts->size[sizeof(ts->size)-1] = '\0';
}

/* ------------------------------------------------------------------
   Execução do .geo
   ------------------------------------------------------------------ */

int geo_executar(FILE *geo, FILA fila_chao, FILE *txt_out) {
    if (!geo || !fila_chao) return 0;

    TextStyle cur = {0};
    ts_default(&cur);

    int instrucoes = 0;
    char buf[2048];
    long lineno = 0;

    while (fgets(buf, sizeof(buf), geo)) {
        lineno++;
        rstrip(buf);

        /* pular vazias e comentários (linha iniciando com #) */
        char *p = buf;
        while (*p && isspace((unsigned char)*p)) p++;
        if (!*p || *p=='#') continue;

        char *curp = p;
        char *cmd = next_tok(&curp);
        if (!cmd) continue;

        /* ---------------- ts fFamily fWeight fSize ---------------- */
        if (strcmp(cmd, "ts") == 0) {
            char *fam = next_tok(&curp);
            char *wei = next_tok(&curp);
            char *siz = next_tok(&curp);

            if (!fam || !wei || !siz) {
                if (txt_out) fprintf(txt_out, "[.geo:%ld] Erro em 'ts': esperado fFamily fWeight fSize\n", lineno);
                continue;
            }
            ts_apply(&cur, fam, wei, siz);
            if (txt_out) fprintf(txt_out, "ts: family=%s weight=%s size=%s\n", cur.family, cur.weight, cur.size);
            instrucoes++;
            continue;
        }

        /* ---------------- c i x y r corb corp ---------------- */
        if (strcmp(cmd, "c") == 0) {
            int id; double x,y,r;
            char corb[128], corp[128];
            int rscan = sscanf(curp, "%d %lf %lf %lf %127s %127s", &id, &x, &y, &r, corb, corp);
            if (rscan != 6) {
                if (txt_out) fprintf(txt_out, "[.geo:%ld] Erro em 'c': i x y r corb corp\n", lineno);
                continue;
            }
            FORMA F = mkFormaCirculo(id, x, y, r, corb, corp);
            if (F) {
                insereNaFila(fila_chao, F);
                if (txt_out) fprintf(txt_out, "c: id=%d (%.2f,%.2f) r=%.2f %s/%s\n", id, x, y, r, corb, corp);
            }
            instrucoes++;
            continue;
        }

        /* ---------------- r i x y w h corb corp ---------------- */
        if (strcmp(cmd, "r") == 0) {
            int id; double x,y,w,h;
            char corb[128], corp[128];
            int rscan = sscanf(curp, "%d %lf %lf %lf %lf %127s %127s", &id, &x, &y, &w, &h, corb, corp);
            if (rscan != 7) {
                if (txt_out) fprintf(txt_out, "[.geo:%ld] Erro em 'r': i x y w h corb corp\n", lineno);
                continue;
            }
            FORMA F = mkFormaRetangulo(id, x, y, w, h, corb, corp);
            if (F) {
                insereNaFila(fila_chao, F);
                if (txt_out) fprintf(txt_out, "r: id=%d (%.2f,%.2f) w=%.2f h=%.2f %s/%s\n", id, x, y, w, h, corb, corp);
            }
            instrucoes++;
            continue;
        }

        /* ---------------- l i x1 y1 x2 y2 cor ---------------- */
        if (strcmp(cmd, "l") == 0) {
            int id; double x1,y1,x2,y2;
            char cor[128];
            int rscan = sscanf(curp, "%d %lf %lf %lf %lf %127s", &id, &x1, &y1, &x2, &y2, cor);
            if (rscan != 6) {
                if (txt_out) fprintf(txt_out, "[.geo:%ld] Erro em 'l': i x1 y1 x2 y2 cor\n", lineno);
                continue;
            }
            FORMA F = mkFormaLinha(id, x1, y1, x2, y2, cor);
            if (F) {
                insereNaFila(fila_chao, F);
                if (txt_out) fprintf(txt_out, "l: id=%d (%.2f,%.2f)-(%.2f,%.2f) cor=%s\n", id, x1,y1,x2,y2,cor);
            }
            instrucoes++;
            continue;
        }

        /* ---------------- t i x y corb corp a txto ----------------
           OBS: txto vai ATÉ O FIM DA LINHA (pode ter espaços) */
        if (strcmp(cmd, "t") == 0) {
            int id; double x,y;
            char corb[128], corp[128];
            char ancora = 0;

            /* Pegar tokens i, x, y, corb, corp, a via next_tok, e o resto da linha é o texto */
            char *tok_id = next_tok(&curp);
            char *tok_x  = next_tok(&curp);
            char *tok_y  = next_tok(&curp);
            char *tok_cb = next_tok(&curp);
            char *tok_cp = next_tok(&curp);
            char *tok_a  = next_tok(&curp);

            if (!tok_id || !tok_x || !tok_y || !tok_cb || !tok_cp || !tok_a) {
                if (txt_out) fprintf(txt_out, "[.geo:%ld] Erro em 't': i x y corb corp a txto\n", lineno);
                continue;
            }

            id     = atoi(tok_id);
            x      = atof(tok_x);
            y      = atof(tok_y);
            strncpy(corb, tok_cb, sizeof(corb)-1); corb[sizeof(corb)-1] = '\0';
            strncpy(corp, tok_cp, sizeof(corp)-1); corp[sizeof(corp)-1] = '\0';
            ancora = tok_a[0] ? tok_a[0] : 'i';

            /* O restante da linha é o texto, possivelmente com espaços */
            while (*curp && isspace((unsigned char)*curp)) curp++;
            char *texto = curp; /* pode ser vazio, mas é permitido */

            /* Se quiser remover aspas externas, faça aqui (opcional) */

            FORMA F = mkFormaTexto(id, x, y, corb, corp, ancora, texto,
                                  cur.family, cur.weight, cur.size);
            if (F) {
                insereNaFila(fila_chao, F);
                if (txt_out) {
                    fprintf(txt_out, "t: id=%d (%.2f,%.2f) a=%c %s/%s txt=\"%s\" [style %s,%s,%s]\n",
                            id, x, y, ancora, corb, corp, texto, cur.family, cur.weight, cur.size);
                }
            }
            instrucoes++;
            continue;
        }

        /* comando desconhecido */
        if (txt_out) fprintf(txt_out, "[.geo:%ld] Comando desconhecido '%s' — ignorado.\n", lineno, cmd);
    }

    if (txt_out) fprintf(txt_out, "Resumo(.geo): instrucoes=%d\n", instrucoes);
    return instrucoes;
}