#include "qry_io.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "../tads_gerais/fila.h"
#include "../tads_gerais/pilha.h"

#include "../formas/forma.h"
#include "../tads_trabalho/processador.h"
#include "../tads_trabalho/geometria.h"
#include "../tads_trabalho/disparador.h"  /* usa o seu TAD DISPARADOR e APIs pd/atch/shft/dsp/rjd/lc */



#define MAX_REG 8192  /* ajuste se precisar */

typedef struct {
    int usado;
    int id;
    DISPARADOR d;
} SlotDisp;

typedef struct {
    int usado;
    int id;
    PILHA p;
} SlotCarr;

static SlotDisp g_disps[MAX_REG];
static SlotCarr g_carrs[MAX_REG];

static inline int idx(int id) {
    if (id < 0) id = -id;
    return id % MAX_REG;
}

static void registry_reset(void) {
    memset(g_disps, 0, sizeof(g_disps));
    memset(g_carrs, 0, sizeof(g_carrs));
}

static DISPARADOR reg_get_or_make_disp(int id) {
    int k = idx(id);
    if (g_disps[k].usado && g_disps[k].id == id) return g_disps[k].d;

    /* cria via seu TAD */
    DISPARADOR d = criaDisparador(id);
    g_disps[k].usado = 1;
    g_disps[k].id    = id;
    g_disps[k].d     = d;
    return d;
}

static DISPARADOR reg_get_disp(int id) {
    int k = idx(id);
    if (g_disps[k].usado && g_disps[k].id == id) return g_disps[k].d;
    return NULL;
}

static PILHA reg_get_or_make_carr(int id) {
    int k = idx(id);
    if (g_carrs[k].usado && g_carrs[k].id == id) return g_carrs[k].p;

    PILHA p = cria_pilha(); /* seu TAD de PILHA */
    g_carrs[k].usado = 1;
    g_carrs[k].id    = id;
    g_carrs[k].p     = p;
    return p;
}

static PILHA reg_get_carr(int id) {
    int k = idx(id);
    if (g_carrs[k].usado && g_carrs[k].id == id) return g_carrs[k].p;
    return NULL;
}

/* util: próximo token num buffer mutável (quebra por espaço) */
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

/* -----------------------------------------------------------
   Execução dos comandos .qry delegando aos seus TADs
   ----------------------------------------------------------- */

int qry_executar(FILE *qry, FILA fila_chao, FILE *txt_out) {
    if (!qry || !fila_chao) return 0;

    registry_reset();

    /* Arena temporária (formas disparadas aguardando o calc) */
    FILA arena = cria_fila();

    /* PROCESSADOR centraliza pontuação/estatísticas do round */
    PROCESSADOR proc = criaProcessador();

    int instrucoes = 0;

    char buf[1024];
    long lineno = 0;

    while (fgets(buf, sizeof(buf), qry)) {
        lineno++;

        /* strip \r\n */
        size_t n = strlen(buf);
        while (n && (buf[n-1]=='\n' || buf[n-1]=='\r')) buf[--n] = '\0';

        /* skip vazias / comentários iniciando com # */
        char *p = buf;
        while (*p && isspace((unsigned char)*p)) p++;
        if (!*p || *p == '#') continue;

        char *cur = p;
        char *cmd = next_tok(&cur);
        if (!cmd) continue;

        /* ------------------ pd d x y ------------------ */
        if (strcmp(cmd, "pd") == 0) {
            int d_id; double x,y;
            if (sscanf(cur, "%d %lf %lf", &d_id, &x, &y) != 3) {
                if (txt_out) fprintf(txt_out, "[.qry:%ld] Erro em 'pd': esperado d x y\n", lineno);
                continue;
            }
            DISPARADOR d = reg_get_or_make_disp(d_id);
            setPosicaoDisparador(d, x, y);            /* TAD DISPARADOR */
            if (txt_out) fprintf(txt_out, "pd: disparador %d -> (%.2f, %.2f)\n", d_id, x, y);
            /* Se quiser contar instrução em PROCESSADOR, você pode:
               registrarDisparo(proc, 0);  // ou ter API específica para 'comando executado' */
            instrucoes++;
            continue;
        }

        /* --------------- lc c n ----------------
           Tira n formas do chão (FILA) e empilha no carregador (PILHA). */
        if (strcmp(cmd, "lc") == 0) {
            int c_id, nload;
            if (sscanf(cur, "%d %d", &c_id, &nload) != 2) {
                if (txt_out) fprintf(txt_out, "[.qry:%ld] Erro em 'lc': esperado c n\n", lineno);
                continue;
            }
            if (nload < 0) nload = 0;

            PILHA car = reg_get_or_make_carr(c_id);
            /* usa sua função declarada no seu header do disparador: */
            lc(fila_chao, car, nload);

            if (txt_out) fprintf(txt_out, "lc: carregador %d recebeu até %d forma(s)\n", c_id, nload);
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
            DISPARADOR d = reg_get_or_make_disp(d_id);
            PILHA pe = reg_get_or_make_carr(c_esq);
            PILHA pd = reg_get_or_make_carr(c_dir);

            atachaCarregadores(d, pe, pd);            /* TAD DISPARADOR */
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
            DISPARADOR d = reg_get_disp(d_id);
            if (!d) {
                if (txt_out) fprintf(txt_out, "shft: disparador %d inexistente\n", d_id);
            } else {
                shft(d, side, npress);                /* TAD DISPARADOR */
                if (txt_out) fprintf(txt_out, "shft: d=%d lado=%c n=%d\n", d_id, side, npress);
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
            bool visualizar = (r == 4 && (flagbuf[0]=='v' || flagbuf[0]=='V'));

            DISPARADOR d = reg_get_disp(d_id);
            if (!d) {
                if (txt_out) fprintf(txt_out, "dsp: disparador %d inexistente\n", d_id);
            } else {
                dsp(d, dx, dy, visualizar, arena);    /* TAD DISPARADOR (posiciona e enfileira na arena) */
                /* Se o seu PROCESSADOR contar disparos aqui, avise-o: */
                registrarDisparo(proc, 1);
                if (txt_out) {
                    fprintf(txt_out, "dsp: d=%d -> (dx=%.2f, dy=%.2f)%s\n",
                            d_id, dx, dy, visualizar ? " [v]" : "");
                }
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
            DISPARADOR d = reg_get_disp(d_id);
            if (!d) {
                if (txt_out) fprintf(txt_out, "rjd: disparador %d inexistente\n", d_id);
            } else {
                /* O seu TAD já executa a sequência shft+dsp até esgotar, enfileirando na arena */
                rjd(d, side, dx, dy, ix, iy, arena);
                /* Não sei se seu rjd já contabiliza “disparos”; para garantir, podemos estimar
                   pelo que entrou na arena antes/depois — mas vamos apenas registrar "evento" aqui: */
                /* registrarDisparo(proc, ???);  -> deixe o TAD ou o calc consolidarem */
                if (txt_out) fprintf(txt_out, "rjd: d=%d lado=%c base(%.2f,%.2f) inc(%.2f,%.2f)\n",
                                     d_id, side, dx, dy, ix, iy);
            }
            instrucoes++;
            continue;
        }

        /* ------------------ calc ------------------ */
        if (strcmp(cmd, "calc") == 0) {
            if (txt_out) {
                fprintf(txt_out, "calc: processando %d forma(s) na arena...\n", tamanho_fila(arena));
            }
            /* Processa colisões/clonagem/pontuação e desloca de volta ao chão */
            double esmagada_round = processarArena(proc, arena, fila_chao);

            if (txt_out) {
                fprintf(txt_out, "calc: area esmagada no round = %.2f\n", esmagada_round);
                fprintf(txt_out,
                        "calc: total pontos = %.2f | disparos=%d | clones=%d | esmagadas=%d\n",
                        getPontuacaoTotal(proc), getTotalDisparos(proc),
                        getTotalClones(proc), getTotalEsmagadas(proc));
            }
            instrucoes++;
            continue;
        }

        /* comando desconhecido */
        if (txt_out) fprintf(txt_out, "[.qry:%ld] Comando desconhecido '%s' — ignorado.\n", lineno, cmd);
    }

    /* Resumo final exigido no PDF */
    if (txt_out) {
        fprintf(txt_out,
                "Resumo: instrucoes=%d, disparos=%d, clones=%d, esmagadas=%d, pontuacao=%.2f\n",
                instrucoes, getTotalDisparos(proc), getTotalClones(proc),
                getTotalEsmagadas(proc), getPontuacaoTotal(proc));
    }

    destruirProcessador(proc);
    /* arena é temporária; se sua FILA requer liberar, faça conforme seu TAD */
    return instrucoes;
}
