#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "qry_io.h"
#include "fila.h"
#include "pilha.h"
#include "forma.h"      // FORMA, cria_forma, tipo_forma, objeto_forma
#include "geometria.h"  // area_forma, etc. (se precisar)
#include "processador.h"

// -----------------------------------------------------------------------------
// Estrutura interna (opaca no .h)
// -----------------------------------------------------------------------------

typedef struct {
    void* chao;          // Fila* com FORMA
    void* arena;         // Fila* com FORMA
    void* proc;          // Processador*
    void* pilhaL;        // Pilha* (carregador L)
    void* pilhaR;        // Pilha* (carregador R)
    char  selecionada;   // 'L' ou 'R' para shft
    FILE* txt;           // relatório (pode ser NULL)
    int   next_id;       // gerador local de ids para clones
} QryExec;

// helper: próxima id para clonagem (callback para processador)
static int _next_id_cb(void){
    static int base = 10000;
    return base++;
}

// ----------------------------------------------------------------------------
// Utilidades
// ----------------------------------------------------------------------------

static void log_txt(QryExec* q, const char* fmt, ...){
    if(!q || !q->txt) return;
    va_list ap; va_start(ap, fmt);
    vfprintf(q->txt, fmt, ap);
    va_end(ap);
}

static void* pilha_sel(QryExec* q){ return (q->selecionada=='R') ? q->pilhaR : q->pilhaL; }

// carrega de chao->pilha N itens
static void cmd_lc(QryExec* q, char side, int n){
    q->selecionada = (side=='R' || side=='r') ? 'R' : 'L';
    void* P = pilha_sel(q);
    for(int i=0; i<n; i++){
        if (fila_vazia(q->chao)) break;
        FORMA f = (FORMA) remove_da_fila(q->chao);
        add_na_pilha(P, f);
    }
    log_txt(q, "lc %c %d\n", q->selecionada, n);
}

// alterna pilha selecionada
static void cmd_shft(QryExec* q, char side){
    q->selecionada = (side=='R' || side=='r') ? 'R' : 'L';
    log_txt(q, "shft %c\n", q->selecionada);
}

// dispara: topo da pilha selecionada vai para a arena
static void cmd_dsp(QryExec* q){
    void* P = pilha_sel(q);
    FORMA f = (FORMA) retira_da_pilha(P);
    if (f) add_na_fila(q->arena, f);
    log_txt(q, "dsp\n");
}

// rjd k: repete dsp k vezes
static void cmd_rjd(QryExec* q, int k){
    for(int i=0;i<k;i++) cmd_dsp(q);
    log_txt(q, "rjd %d\n", k);
}

// calc: processa arena contra as regras e devolve ao chão
static void cmd_calc(QryExec* q){
    double rodada = processa_formas(q->proc, q->arena, q->chao, _next_id_cb);
    log_txt(q, "calc: +%.2f pontos (total=%.2f, clones=%d, esmagadas=%d)\n",
            rodada, get_pontuacao_total(q->proc), get_total_clones(q->proc), get_total_esmagadas(q->proc));
}

// TXT: adiciona uma linha arbitrária ao relatório
static void cmd_TXT(QryExec* q, const char* linha){
    if(!linha) linha = "";
    log_txt(q, "%s\n", linha);
}

// ignora o resto da linha
static void flush_line(FILE* f){ int c; while((c=fgetc(f))!=EOF && c!='\n'); }

// lê token simples (sem espaços). retorna false no EOF.
static bool read_token(FILE* f, char* buf, size_t cap){
    int c;
    do{ c=fgetc(f); if(c==EOF) return false; } while(isspace(c));
    size_t i=0;
    do{
        if(i+1<cap) buf[i++]=(char)c;
        c=fgetc(f);
    } while(c!=EOF && !isspace(c));
    buf[i]='\0';
    if(c!=EOF && c!='\n') ungetc(c,f);
    return true;
}

// lê string até fim da linha (sem \n)
static char* read_remainder_line(FILE* f){
    char* line=NULL; size_t cap=0; ssize_t n=getline(&line,&cap,f);
    if(n<=0){ free(line); return NULL; }
    if(n>0 && line[n-1]=='\n') line[n-1]='\0';
    return line;
}

// ----------------------------------------------------------------------------
// API pública
// ----------------------------------------------------------------------------

QRY_EXECUTOR qry_exec_create(void* chao, void* arena, void* processador, const char* path_txt){
    QryExec* q = (QryExec*) malloc(sizeof(QryExec));
    if(!q) return NULL;
    q->chao = chao;
    q->arena = arena;
    q->proc = processador;
    q->pilhaL = cria_pilha();
    q->pilhaR = cria_pilha();
    q->selecionada = 'L';
    q->txt = NULL;
    q->next_id = 10000;

    if(path_txt){
        q->txt = fopen(path_txt, "w");
        if(!q->txt){ free(q); return NULL; }
    }
    return (QRY_EXECUTOR) q;
}

bool qry_exec_run_file(QRY_EXECUTOR qe, const char* filepath){
    if(!qe || !filepath) return false;
    QryExec* q = (QryExec*) qe;

    FILE* f = fopen(filepath, "r");
    if(!f){ fprintf(stderr, ".qry: não consegui abrir %s\n", filepath); return false; }

    char tok[32];
    bool ok = true;

    while(1){
        long pos = ftell(f);
        int c = fgetc(f);
        if(c==EOF) break;
        if(isspace(c)) continue;
        if(c=='#'){ flush_line(f); continue; }
        ungetc(c,f);

        if(!read_token(f, tok, sizeof(tok))) break;

        if(strcmp(tok,"pd")==0){
            // Placeholder: parâmetros do disparador, ignorados nesta implementação
            // pulamos até fim de linha
            flush_line(f);
            log_txt(q, "pd (ignorado nesta impl)\n");
        } else if(strcmp(tok,"lc")==0){
            char side; int n;
            if(fscanf(f, " %c %d", &side, &n)!=2){ fprintf(stderr, ".qry: lc inválido\n"); ok=false; break; }
            flush_line(f);
            if(n<0) n=0; cmd_lc(q, side, n);
        } else if(strcmp(tok,"atch")==0){
            // Sem hardware de disparador real; tratamos como no-op
            flush_line(f);
            log_txt(q, "atch\n");
        } else if(strcmp(tok,"shft")==0){
            char side; if(fscanf(f, " %c", &side)!=1){ fprintf(stderr, ".qry: shft inválido\n"); ok=false; break; }
            flush_line(f);
            cmd_shft(q, side);
        } else if(strcmp(tok,"dsp")==0){
            flush_line(f);
            cmd_dsp(q);
        } else if(strcmp(tok,"rjd")==0){
            int k; if(fscanf(f, " %d", &k)!=1){ fprintf(stderr, ".qry: rjd inválido\n"); ok=false; break; }
            flush_line(f);
            if(k<0) k=0; cmd_rjd(q, k);
        } else if(strcmp(tok,"calc")==0){
            flush_line(f);
            cmd_calc(q);
        } else if(strcmp(tok,"TXT")==0){
            char* line = read_remainder_line(f);
            cmd_TXT(q, line);
            free(line);
        } else {
            fprintf(stderr, ".qry: token '%s' desconhecido (linha ignorada)\n", tok);
            flush_line(f);
        }

        (void)pos;
    }

    fclose(f);
    return ok;
}

void qry_exec_destroy(QRY_EXECUTOR qe){
    if(!qe) return;
    QryExec* q = (QryExec*) qe;
    // esvazia e destrói pilhas (formas voltam ao chão para não perder)
    while(!pilha_vazia(q->pilhaL)) add_na_fila(q->chao, retira_da_pilha(q->pilhaL));
    while(!pilha_vazia(q->pilhaR)) add_na_fila(q->chao, retira_da_pilha(q->pilhaR));

    destruir_pilha(q->pilhaL, NULL);
    destruir_pilha(q->pilhaR, NULL);

    if(q->txt){ fclose(q->txt); }
    free(q);
}
