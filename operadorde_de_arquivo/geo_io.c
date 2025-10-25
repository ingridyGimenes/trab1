#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "geo_io.h"
#include "forma.h"      // FORMA, cria_forma, tipo_forma, ...
#include "fila.h"       // add_na_fila
#include "retangulo.h"  // cria_retangulo, getters...
#include "circulo.h"    // cria_circulo
#include "linha.h"      // cria_linha
#include "texto.h"      // cria_texto

typedef struct {
    void* fila_chao; // sua Fila*
} GeoLoader;

static char* dupstr(const char* s){
    if(!s) return NULL;
    size_t n = strlen(s)+1;
    char* d = (char*)malloc(n);
    if(d) memcpy(d,s,n);
    return d;
}

GEO_LOADER geo_loader_create(void* fila_chao){
    GeoLoader* g = (GeoLoader*)malloc(sizeof(GeoLoader));
    if(!g) return NULL;
    g->fila_chao = fila_chao;
    return (GEO_LOADER)g;
}

void geo_loader_destroy(GEO_LOADER gl){
    if(!gl) return;
    free(gl);
}

// Consome até o fim da linha atual (para pular comentários/resto)
static void flush_line(FILE* f){
    int c;
    while((c=fgetc(f))!=EOF && c!='\n');
}

// Lê uma palavra (token sem espaços). Retorna false no EOF.
static bool read_token(FILE* f, char* buf, size_t cap){
    int c;
    // pular espaços
    do {
        c = fgetc(f);
        if(c==EOF) return false;
    } while(isspace(c));

    // ler token
    size_t i=0;
    do{
        if(i+1<cap) buf[i++] = (char)c;
        c = fgetc(f);
    } while(c!=EOF && !isspace(c));
    buf[i] = '\0';

    // se parou num \n, ok. senão, devolve o char para o stream
    if(c!=EOF && c!='\n') ungetc(c, f);
    return true;
}

// Lê resto da linha como string (inclui espaços), trimando \n final
static char* read_remainder_line(FILE* f){
    char* line = NULL;
    size_t cap = 0;
    ssize_t n = getline(&line, &cap, f);
    if(n <= 0){ free(line); return dupstr(""); }
    // remover \n
    if(n>0 && line[n-1]=='\n') line[n-1] = '\0';
    return line;
}

bool geo_loader_load_file(GEO_LOADER gl, const char* filepath){
    if(!gl || !filepath) return false;
    GeoLoader* g = (GeoLoader*)gl;

    FILE* f = fopen(filepath, "r");
    if(!f){
        fprintf(stderr, "geo_io: não consegui abrir %s\n", filepath);
        return false;
    }

    char t[32];
    bool ok = true;

    while(1){
        long pos = ftell(f);
        int c = fgetc(f);
        if(c==EOF) break;

        if(isspace(c)) continue;
        if(c=='#'){ flush_line(f); continue; } // comentário

        ungetc(c, f);

        if(!read_token(f, t, sizeof(t))) break;

        // c: id x y r corb corp
        // r: id x y w h corb corp
        // l: id x1 y1 x2 y2 cor
        // t: id x y ancora(i|m|f) corb corp <texto ate o fim da linha>
        if(strcmp(t,"c")==0){
            int id; double x,y,r; char corb[128], corp[128];
            if(fscanf(f, "%d %lf %lf %lf %127s %127s", &id,&x,&y,&r, corb,corp)!=6){
                fprintf(stderr, "geo_io: linha 'c' inválida\n");
                ok=false; break;
            }
            flush_line(f);
            CIRCLE cobj = cria_circulo(id,x,y,r, corb,corp);
            if(!cobj){ ok=false; break; }
            FORMA F = cria_forma('c', cobj);
            add_na_fila(g->fila_chao, F);

        }else if(strcmp(t,"r")==0){
            int id; double x,y,w,h; char corb[128], corp[128];
            if(fscanf(f, "%d %lf %lf %lf %lf %127s %127s", &id,&x,&y,&w,&h, corb,corp)!=7){
                fprintf(stderr, "geo_io: linha 'r' inválida\n");
                ok=false; break;
            }
            flush_line(f);
            RETANGULO robj = cria_retangulo(id,x,y,w,h, corb,corp);
            if(!robj){ ok=false; break; }
            FORMA F = cria_forma('r', robj);
            add_na_fila(g->fila_chao, F);

        }else if(strcmp(t,"l")==0){
            int id; double x1,y1,x2,y2; char cor[128];
            if(fscanf(f, "%d %lf %lf %lf %lf %127s", &id,&x1,&y1,&x2,&y2, cor)!=6){
                fprintf(stderr, "geo_io: linha 'l' inválida\n");
                ok=false; break;
            }
            flush_line(f);
            LINHA lobj = cria_linha(id,x1,y1,x2,y2, cor);
            if(!lobj){ ok=false; break; }
            FORMA F = cria_forma('l', lobj);
            add_na_fila(g->fila_chao, F);

        }else if(strcmp(t,"t")==0){
            int id; double x,y; char ancora_ch[8]; char corb[128], corp[128];
            if(fscanf(f, "%d %lf %lf %7s %127s %127s", &id,&x,&y, ancora_ch, corb,corp)!=6){
                fprintf(stderr, "geo_io: início de linha 't' inválido\n");
                ok=false; break;
            }
            // resto da linha é o texto (pode ter espaços)
            char* rest = read_remainder_line(f);
            // remover aspas ao redor, se existirem
            char* txt = rest;
            while(*txt==' ' || *txt=='\t') txt++;
            size_t L = strlen(txt);
            if(L>=2 && ((txt[0]=='\"' && txt[L-1]=='\"') || (txt[0]=='\'' && txt[L-1]=='\''))){
                txt[L-1]='\0'; txt++;
            }
            char ancora = ancora_ch[0]; // i|m|f
            TEXTO tobj = cria_texto(id,x,y, corb,corp, ancora, txt);
            free(rest);
            if(!tobj){ ok=false; break; }
            FORMA F = cria_forma('t', tobj);
            add_na_fila(g->fila_chao, F);

        }else{
            // token desconhecido → ignora a linha, mas avisa
            fprintf(stderr, "geo_io: token '%s' desconhecido (linha ignorada)\n", t);
            // avança até o fim da linha para não loopar
            flush_line(f);
        }

        // guarda posição para diagnóstico, se quiser
        (void)pos;
    }

    fclose(f);
    return ok;
}
