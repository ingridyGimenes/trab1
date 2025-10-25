#include "qry_io.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../tads_gerais/fila.h"
#include "../tads_gerais/pilha.h"

#include "../formas/forma.h"
#include "../tads_trabalho/processador.h"
#include "../tads_trabalho/geometria.h"

/* ===========================================================
   Estruturas internas (não aparecem em headers)
   =========================================================== */

typedef struct {
    int id;
    double x, y;     /* posição do disparador (pd) */
    PILHA  esq;      /* carregador esquerdo (pilha) */
    PILHA  dir;      /* carregador direito (pilha) */
    FORMA  slot;     /* forma em posição de disparo (apenas 1) */
} DisparadorRT;

typedef struct {
    int id;
    PILHA pilha;     /* carregador é uma PILHA no runtime */
} CarregadorRT;

/* Tabelas simples por ID (pode trocar por hash se quiser) */
#define MAX_REG 2048

static DisparadorRT* g_disps[MAX_REG];
static CarregadorRT* g_carr[MAX_REG];

static inline int idx(int id) {
    if (id < 0) id = -id;
    return id % MAX_REG;
}

static void rt_reset(void) {
    memset(g_disps, 0, sizeof(g_disps));
    memset(g_carr, 0, sizeof(g_carr));
}

static DisparadorRT* get_or_make_disp(int id) {
    int k = idx(id);
    if (g_disps[k] && g_disps[k]->id == id) return g_disps[k];
    DisparadorRT* d = (DisparadorRT*)calloc(1, sizeof(*d));
    d->id = id;
    d->x = d->y = 0.0;
    d->esq = NULL;
    d->dir = NULL;
    d->slot = NULL;
    g_disps[k] = d;
    return d;
}

static DisparadorRT* get_disp(int id) {
    int k = idx(id);
    if (g_disps[k] && g_disps[k]->id == id) return g_disps[k];
    return NULL;
}

static CarregadorRT* get_or_make_carr(int id) {
    int k = idx(id);
    if (g_carr[k] && g_carr[k]->id == id) return g_carr[k];
    CarregadorRT* c = (CarregadorRT*)calloc(1, sizeof(*c));
    c->id = id;
    c->pilha = cria_pilha(); /* seu TAD retorna handle */
    g_carr[k] = c;
    return c;
}

static CarregadorRT* get_carr(int id) {
    int k = idx(id);
    if (g_carr[k] && g_carr[k]->id == id) return g_carr[k];
    return NULL;
}

/* util: próximo token em linha mutável */
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

/* ===========================================================
   Regras do enunciado para shft / dsp / rjd
   =========================================================== */

/* devolve forma do slot (se houver) para topo da pilha oposta */
static void slot_to_opposite(DisparadorRT* d, char side, FILE* txt) {
    if (!d || !d->slot) return;
    PILHA op = (side == 'e') ? d->dir : d->esq;
    if (op) {
        add_na_pilha(op, d->slot);
        if (txt) fprintf(txt, "shft: forma do slot foi para o topo do carregador '%c'\n",
                         (side=='e')?'d':'e');
        d->slot = NULL;
    }
}

/* pressiona o botão side ('e' ou 'd') n vezes */
static void op_shft(DisparadorRT* d, char side, int n, FILE* txt) {
    if (!d) return;
    if (side != 'e' && side != 'd') side = 'e';
    if (n < 0) n = 0;

    for (int k = 0; k < n; ++k) {
        /* 1) se há forma no slot, devolve ao topo da pilha oposta */
        if (d->slot) slot_to_opposite(d, side, txt);

        /* 2) tira do topo da pilha escolhida e põe no slot */
        PILHA alvo = (side == 'e') ? d->esq : d->dir;
        if (!alvo || pilha_vazia(alvo)) {
            if (txt) fprintf(txt, "shft: carregador '%c' vazio\n", side);
            continue;
        }
        d->slot = retira_da_pilha(alvo); /* retorna FORMA */
        if (txt) {
            fprintf(txt, "shft: forma '%c' colocada no slot\n", tipoForma(d->slot));
        }
    }
}

/* dsp: coloca forma do slot na ARENA (FILA).
 * Por falta de API para mover âncora, apenas registramos posição alvo no TXT. */
static void op_dsp(PROCESSADOR proc, DisparadorRT* d,
                   double dx, double dy, int flag_v, FILA arena, FILE* txt) {
    if (!d) return;
    if (!d->slot) {
        if (txt) fprintf(txt, "dsp: slot vazio; nada a disparar\n");
        return;
    }

    /* Aqui seria o ponto ideal para mover a forma:
       -> geometriaPosicionaForma(d->slot, d->x + dx, d->y + dy);  (NÃO EXISTE)
       Como não há API, registramos no TXT e deixamos a forma seguir à arena. */
    if (txt) {
        fprintf(txt, "dsp: forma '%c' -> alvo (%.2f, %.2f)\n",
                tipoForma(d->slot), d->x + dx, d->y + dy);
        if (flag_v) fprintf(txt, "dsp[v]: (anotar dimensões de disparo no SVG)\n");
    }

    add_na_fila(arena, d->slot);
    d->slot = NULL;

    if (proc) registrarDisparo(proc, 1);
}

/* rjd: repete shft+dsp com deslocamentos incrementais enquanto houver formas no carregador side */
static void op_rjd(PROCESSADOR proc, DisparadorRT* d, char side,
                   double dx, double dy, double ix, double iy,
                   FILA arena, FILE* txt) {
    if (!d) return;
    if (side != 'e' && side != 'd') side = 'e';
    PILHA alvo = (side == 'e') ? d->esq : d->dir;
    if (!alvo) {
        if (txt) fprintf(txt, "rjd: disparador %d sem carregador '%c'\n", d->id, side);
        return;
    }

    int i = 0;
    while (!pilha_vazia(alvo)) {
        op_shft(d, side, 1, txt);
        op_dsp(proc, d, dx + i*ix, dy + i*iy, 0, arena, txt);
        i++;
    }
    if (txt) fprintf(txt, "rjd: %d disparos executados\n", i);
}

/* ===========================================================
   Parser do .qry
   =========================================================== */

int qry_executar(FILE *qry, FILA fila_chao, FILE *txt_out) {
    if (!qry || !fila_chao) return 0;

    rt_reset();

    /* arena dos disparos: onde as formas ficam até o calc */
    FILA arena = cria_fila();

    /* PROCESSADOR: centraliza pontuação, clones, esmagadas, etc. */
    PROCESSADOR proc = criaProcessador();

    int instrucoes = 0;

    char buf[512];
    long lineno = 0;

    while (fgets(buf, sizeof(buf), qry)) {
        lineno++;
        /* strip \r\n */
        size_t n = strlen(buf);
        while (n && (buf[n-1]=='\n' || buf[n-1]=='\r')) buf[--n] = '\0';

        /* pular vazias/comentários */
        char *p = buf;
        while (*p && isspace((unsigned char)*p)) p++;
        if (!*p || *p == '#') continue;

        char *cur = p;
        char *cmd = next_tok(&cur);
        if (!cmd) continue;

        /* -------- pd d x y -------- */
        if (strcmp(cmd, "pd") == 0) {
            int d_id; double x,y;
            if (sscanf(cur, "%d %lf %lf", &d_id, &x, &y) != 3) {
                if (txt_out) fprintf(txt_out, "[.qry:%ld] Erro em 'pd': esperado d x y\n", lineno);
                continue;
            }
            DisparadorRT* d = get_or_make_disp(d_id);
            d->x = x; d->y = y;
            if (txt_out) fprintf(txt_out, "pd: disparador %d -> (%.2f, %.2f)\n", d_id, x, y);
            instrucoes++;
            continue;
        }

        /* -------- lc c n -------- (tira do chão e empilha no carregador c) */
        if (strcmp(cmd, "lc") == 0) {
            int c_id, nload;
            if (sscanf(cur, "%d %d", &c_id, &nload) != 2) {
                if (txt_out) fprintf(txt_out, "[.qry:%ld] Erro em 'lc': esperado c n\n", lineno);
                continue;
            }
            if (nload < 0) nload = 0;

            CarregadorRT* c = get_or_make_carr(c_id);

            /* Removemos n da FILA (FIFO) e empilhamos (LIFO). */
            int ok = 0;
            for (int i = 0; i < nload; ++i) {
                if (fila_vazia(fila_chao)) break;
                FORMA f = remove_da_fila(fila_chao);
                add_na_pilha(c->pilha, f);
                ok++;
                /* TXT: pode imprimir info da forma f aqui se quiser detalhar */
            }
            if (txt_out) fprintf(txt_out, "lc: carregador %d recebeu %d forma(s)\n", c_id, ok);
            instrucoes++;
            continue;
        }

        /* -------- atch d cesq cdir -------- */
        if (strcmp(cmd, "atch") == 0) {
            int d_id, c_esq, c_dir;
            if (sscanf(cur, "%d %d %d", &d_id, &c_esq, &c_dir) != 3) {
                if (txt_out) fprintf(txt_out, "[.qry:%ld] Erro em 'atch': esperado d cesq cdir\n", lineno);
                continue;
            }
            DisparadorRT* d = get_or_make_disp(d_id);
            CarregadorRT* ce = get_or_make_carr(c_esq);
            CarregadorRT* cd = get_or_make_carr(c_dir);
            d->esq = ce ? ce->pilha : NULL;
            d->dir = cd ? cd->pilha : NULL;
            if (txt_out) fprintf(txt_out, "atch: d=%d <= (e:%d, d:%d)\n", d_id, c_esq, c_dir);
            instrucoes++;
            continue;
        }

        /* -------- shft d [e|d] n -------- */
        if (strcmp(cmd, "shft") == 0) {
            int d_id, npress; char side=0;
            if (sscanf(cur, "%d %c %d", &d_id, &side, &npress) != 3) {
                if (txt_out) fprintf(txt_out, "[.qry:%ld] Erro em 'shft': esperado d [e|d] n\n", lineno);
                continue;
            }
            DisparadorRT* d = get_disp(d_id);
            if (!d) {
                if (txt_out) fprintf(txt_out, "shft: disparador %d inexistente\n", d_id);
            } else {
                op_shft(d, side, npress, txt_out);
            }
            instrucoes++;
            continue;
        }

        /* -------- dsp d dx dy [v|i] -------- */
        if (strcmp(cmd, "dsp") == 0) {
            int d_id; double dx, dy; char flagbuf[8]={0};
            int r = sscanf(cur, "%d %lf %lf %7s", &d_id, &dx, &dy, flagbuf);
            if (r < 3) {
                if (txt_out) fprintf(txt_out, "[.qry:%ld] Erro em 'dsp': esperado d dx dy [v|i]\n", lineno);
                continue;
            }
            int flag_v = (r == 4 && (flagbuf[0]=='v' || flagbuf[0]=='V')) ? 1 : 0;

            DisparadorRT* d = get_disp(d_id);
            if (!d) {
                if (txt_out) fprintf(txt_out, "dsp: disparador %d inexistente\n", d_id);
            } else {
                op_dsp(proc, d, dx, dy, flag_v, arena, txt_out);
            }
            instrucoes++;
            continue;
        }

        /* -------- rjd d [e|d] dx dy ix iy -------- */
        if (strcmp(cmd, "rjd") == 0) {
            int d_id; char side=0; double dx,dy,ix,iy;
            if (sscanf(cur, "%d %c %lf %lf %lf %lf", &d_id, &side, &dx, &dy, &ix, &iy) != 6) {
                if (txt_out) fprintf(txt_out, "[.qry:%ld] Erro em 'rjd': d [e|d] dx dy ix iy\n", lineno);
                continue;
            }
            DisparadorRT* d = get_disp(d_id);
            if (!d) {
                if (txt_out) fprintf(txt_out, "rjd: disparador %d inexistente\n", d_id);
            } else {
                op_rjd(proc, d, side, dx, dy, ix, iy, arena, txt_out);
            }
            instrucoes++;
            continue;
        }

        /* -------- calc -------- */
        if (strcmp(cmd, "calc") == 0) {
            if (txt_out) {
                fprintf(txt_out, "calc: processando %d forma(s) na arena...\n", tamanho_fila(arena));
            }
            double esmagada_round = processarArena(proc, arena, fila_chao);
            if (txt_out) {
                fprintf(txt_out, "calc: area esmagada no round = %.2f\n", esmagada_round);
                fprintf(txt_out, "calc: total pontos = %.2f | disparos=%d | clones=%d | esmagadas=%d\n",
                        getPontuacaoTotal(proc), getTotalDisparos(proc),
                        getTotalClones(proc), getTotalEsmagadas(proc));
            }
            instrucoes++;
            continue;
        }

        /* comando desconhecido */
        if (txt_out) fprintf(txt_out, "[.qry:%ld] Comando desconhecido '%s' — ignorado.\n", lineno, cmd);
    }

    /* resumo final (exigido pelo enunciado) */
    if (txt_out) {
        fprintf(txt_out, "Resumo: instrucoes=%d, disparos=%d, clones=%d, esmagadas=%d, pontuacao=%.2f\n",
                instrucoes, getTotalDisparos(proc), getTotalClones(proc),
                getTotalEsmagadas(proc), getPontuacaoTotal(proc));
    }

    destruirProcessador(proc);
    /* se sua FILA precisar liberar, faça aqui; normalmente o chão retém as formas. */
    return instrucoes;
}
