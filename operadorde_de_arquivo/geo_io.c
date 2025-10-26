#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "geo_io.h"

// ======== INCLUDES dos módulos de formas ========
#include "../formas/forma.h"
#include "../formas/retangulo.h"
#include "../formas/circulo.h"
#include "../formas/linha.h"
#include "../formas/texto.h"   // <- usa criaTexto(...) + criaEstilo(...)

// ======== INCLUDES de TADs gerais ========
#include "../tads_gerais/fila.h"

// ============================================================================
//                            ADAPTE AQUI (MACROS)
// ============================================================================
// 1) ENFILEIRAR NA FILA "CHÃO"
// Troque se o nome da sua função de fila for diferente:
#ifndef ENFILEIRAR
#define ENFILEIRAR(FILA_CHAO, FORMA_OBJ) filaEnfileira((FILA_CHAO), (FORMA_OBJ))
#endif

// 2) FACTORIES DE FORMAS
// Se suas factories já retornam FORMA, mantenha. Se retornam tipos específicos,
// troque por wrappers (ex.: formaDeRetangulo(criaRetangulo(...))).

// CÍRCULO
#define FORMA_CIRCULO(id, x, y, r, corB, corP) criaCirculo((id), (x), (y), (r), (corB), (corP))

// RETÂNGULO
#define FORMA_RETANGULO(id, x, y, w, h, corB, corP) criaRetangulo((id), (x), (y), (w), (h), (corB), (corP))

// LINHA
#define FORMA_LINHA(id, x1, y1, x2, y2, cor) criaLinha((id), (x1), (y1), (x2), (y2), (cor))

// TEXTO — ADAPTADO À SUA API:
// Assinatura real: criaTexto(int id, double x, double y, const char* corB, const char* corP,
//                             char ancora, const char* conteudo, ESTILO estilo)
//
// Vamos criar um ESTILO na hora via criaEstilo(fFamily, fWeight, fSizeString).
// (Seu ESTILO usa strings; portanto guardaremos size como string, ex.: "12")
#define FORMA_TEXTO__USANDO_ESTILO(id, x, y, corB, corP, ancora, conteudo, fFam, fWgt, fSzStr) \
    criaTexto((id), (x), (y), (corB), (corP), (ancora), (conteudo), criaEstilo((fFam),(fWgt),(fSzStr)))

// ============================================================================
//                     IMPLEMENTAÇÃO DO PARSER DE .GEO
// ============================================================================

typedef struct {
    char fontFamily[16];  // "sans", "serif", "cursive"
    char fontWeight[4];   // "n", "b", "b+", "l"
    char fontSize[16];    // guardado como STRING, ex.: "12"
} TextoEstilo;

static void estilo_texto_default(TextoEstilo* st) {
    strcpy(st->fontFamily, "sans");
    strcpy(st->fontWeight, "n");
    strcpy(st->fontSize,  "12");
}

static char* rtrim(char* s) {
    size_t n = strlen(s);
    while (n && isspace((unsigned char)s[n-1])) { s[--n] = '\0'; }
    return s;
}
static char* ltrim(char* s) {
    while (*s && isspace((unsigned char)*s)) ++s;
    return s;
}

static bool parse_linha_geo(const char* orig, long lineno, FILA chao, TextoEstilo* estilo) {
    char buf[8192];
    strncpy(buf, orig, sizeof(buf)-1);
    buf[sizeof(buf)-1] = '\0';

    const char* p = orig;

    // ignora vazias/comentários
    while (*p && isspace((unsigned char)*p)) ++p;
    if (*p == '\0' || *p == '#') return true;

    char cmd[8] = {0};
    int consumed = 0;
    if (sscanf(p, " %7s %n", cmd, &consumed) != 1) {
        fprintf(stderr, "[.geo:%ld] Erro: comando ausente.\n", lineno);
        return false;
    }
    p += consumed;

    if (strcmp(cmd, "c") == 0) {
        int id; double x, y, r; char corb[64], corp[64];
        if (sscanf(p, " %d %lf %lf %lf %63s %63s", &id, &x, &y, &r, corb, corp) != 6) {
            fprintf(stderr, "[.geo:%ld] Erro em 'c': esperado: c i x y r corb corp\n", lineno);
            return false;
        }
        FORMA f = FORMA_CIRCULO(id, x, y, r, corb, corp);
        ENFILEIRAR(chao, f);
        return true;
    }
    else if (strcmp(cmd, "r") == 0) {
        int id; double x, y, w, h; char corb[64], corp[64];
        if (sscanf(p, " %d %lf %lf %lf %lf %63s %63s", &id, &x, &y, &w, &h, corb, corp) != 7) {
            fprintf(stderr, "[.geo:%ld] Erro em 'r': esperado: r i x y w h corb corp\n", lineno);
            return false;
        }
        FORMA f = FORMA_RETANGULO(id, x, y, w, h, corb, corp);
        ENFILEIRAR(chao, f);
        return true;
    }
    else if (strcmp(cmd, "l") == 0) {
        int id; double x1, y1, x2, y2; char cor[64];
        if (sscanf(p, " %d %lf %lf %lf %lf %63s", &id, &x1, &y1, &x2, &y2, cor) != 6) {
            fprintf(stderr, "[.geo:%ld] Erro em 'l': esperado: l i x1 y1 x2 y2 cor\n", lineno);
            return false;
        }
        FORMA f = FORMA_LINHA(id, x1, y1, x2, y2, cor);
        ENFILEIRAR(chao, f);
        return true;
    }
    else if (strcmp(cmd, "t") == 0) {
        // t i x y corb corp a txto  (txto vai até o fim da linha)
        int id; double x, y; char corb[64], corp[64]; char ancora;
        if (sscanf(p, " %d %lf %lf %63s %63s %c", &id, &x, &y, corb, corp, &ancora) != 6) {
            fprintf(stderr, "[.geo:%ld] Erro em 't': esperado: t i x y corb corp a txto\n", lineno);
            return false;
        }

        // localizar início do 'txto' na linha inteira:
        char* line = buf;
        {   // pular "t"
            char tmp[8]; int off = 0;
            if (sscanf(line, " %7s %n", tmp, &off) == 1) line += off;
        }
        for (int k = 0; k < 6; ++k) {
            while (*line && isspace((unsigned char)*line)) ++line;
            while (*line && !isspace((unsigned char)*line)) ++line;
        }
        line = ltrim(rtrim(line));  // texto até o fim
        if (!line) line = "";

        // cria ESTILO string-based (sua API)
        // estilo->fontFamily, ->fontWeight, ->fontSize (ex.: "12")
        FORMA f = FORMA_TEXTO__USANDO_ESTILO(
            id, x, y, corb, corp, ancora, line,
            estilo->fontFamily, estilo->fontWeight, estilo->fontSize
        );
        ENFILEIRAR(chao, f);
        return true;
    }
    else if (strcmp(cmd, "ts") == 0) {
        // ts fFamily fWeight fSize   (no seu ESTILO, tudo são strings)
        char ff[16], fw[4], fs[16];
        if (sscanf(p, " %15s %3s %15s", ff, fw, fs) != 3) {
            // muitos .geo usam inteiro para size; aceitaremos int também:
            int fsi;
            if (sscanf(p, " %15s %3s %d", ff, fw, &fsi) == 3) {
                snprintf(fs, sizeof(fs), "%d", fsi);
            } else {
                fprintf(stderr, "[.geo:%ld] Erro em 'ts': esperado: ts fFamily fWeight fSize\n", lineno);
                return false;
            }
        }
        // normalizações simples
        if (strcmp(ff, "sans") && strcmp(ff, "serif") && strcmp(ff, "cursive")) {
            fprintf(stderr, "[.geo:%ld] Aviso: fontFamily '%s' desconhecida. Usando 'sans'.\n", lineno, ff);
            strcpy(ff, "sans");
        }
        if (strcmp(fw, "n") && strcmp(fw, "b") && strcmp(fw, "b+") && strcmp(fw, "l")) {
            fprintf(stderr, "[.geo:%ld] Aviso: fontWeight '%s' desconhecido. Usando 'n'.\n", lineno, fw);
            strcpy(fw, "n");
        }
        // atualiza estado
        strncpy(estilo->fontFamily, ff, sizeof(estilo->fontFamily)-1);
        estilo->fontFamily[sizeof(estilo->fontFamily)-1] = '\0';
        strncpy(estilo->fontWeight, fw, sizeof(estilo->fontWeight)-1);
        estilo->fontWeight[sizeof(estilo->fontWeight)-1] = '\0';
        strncpy(estilo->fontSize, fs, sizeof(estilo->fontSize)-1);
        estilo->fontSize[sizeof(estilo->fontSize)-1] = '\0';
        return true;
    }
    else {
        fprintf(stderr, "[.geo:%ld] Erro: comando '%s' desconhecido.\n", lineno, cmd);
        return false;
    }
}

bool geo_ler_stream(FILE* fin, FILA chao) {
    if (!fin || !chao) return false;

    TextoEstilo estilo;
    estilo_texto_default(&estilo);

    char line[8192];
    long lineno = 0;
    while (fgets(line, sizeof(line), fin)) {
        ++lineno;
        if (!parse_linha_geo(line, lineno, chao, &estilo)) {
            return false;
        }
    }
    return true;
}

bool geo_ler_arquivo(const char* caminho_geo, FILA chao) {
    if (!caminho_geo || !chao) return false;
    FILE* f = fopen(caminho_geo, "r");
    if (!f) {
        perror("geo_ler_arquivo: fopen");
        return false;
    }
    bool ok = geo_ler_stream(f, chao);
    fclose(f);
    return ok;
}
