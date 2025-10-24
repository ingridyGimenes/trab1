#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leitorQRY.h"

// Função auxiliar para remover o \n do final de strings
static void trim(char *s) {
    s[strcspn(s, "\n")] = '\0';
}

void processarQry(const char *nomeArquivo, Fila *chao) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir arquivo %s\n", nomeArquivo);
        exit(1);
    }

    char comando[20];

    // Estruturas do jogo
    Disparador *disparadores[50] = {NULL};
    Carregador *carregadores[100] = {NULL};

    int totalDisparos = 0, totalFormasEsmagadas = 0, totalFormasClonadas = 0;
    double pontuacaoTotal = 0.0;

    while (fscanf(arquivo, "%s", comando) != EOF) {

        // -----------------------------------------------------------------
        // pd l x y → posiciona o disparador l em (x, y)
        // -----------------------------------------------------------------
        if (strcmp(comando, "pd") == 0) {
            int id;
            double x, y;
            fscanf(arquivo, "%d %lf %lf", &id, &x, &y);
            disparadores[id] = cria_disparador(id, x, y);
            printf("[pd] Disparador %d posicionado em (%.2f, %.2f)\n", id, x, y);
        }

        // -----------------------------------------------------------------
        // lc c n → carrega o carregador c com n formas do chão
        // -----------------------------------------------------------------
        else if (strcmp(comando, "lc") == 0) {
            int id, n;
            fscanf(arquivo, "%d %d", &id, &n);
            carregadores[id] = cria_carregador(id);

            for (int i = 0; i < n; i++) {
                void *forma = desenfileira(chao);
                if (forma != NULL)
                    push_forma(carregadores[id], forma);
            }

            printf("[lc] Carregador %d carregado com %d formas\n", id, n);
        }

        // -----------------------------------------------------------------
        // atch d cesq cdir → encaixa carregadores no disparador d
        // -----------------------------------------------------------------
        else if (strcmp(comando, "atch") == 0) {
            int idd, cesq, cdir;
            fscanf(arquivo, "%d %d %d", &idd, &cesq, &cdir);
            conecta_carregadores(disparadores[idd], carregadores[cesq], carregadores[cdir]);
            printf("[atch] Disparador %d conectado a carregadores %d e %d\n", idd, cesq, cdir);
        }

        // -----------------------------------------------------------------
        // shft d [e|d] n → pressiona botão esquerdo ou direito n vezes
        // -----------------------------------------------------------------
        else if (strcmp(comando, "shft") == 0) {
            int idd, n;
            char lado;
            fscanf(arquivo, "%d %c %d", &idd, &lado, &n);
            for (int i = 0; i < n; i++) {
                shift(disparadores[idd], lado);
            }
            printf("[shft] Disparador %d shift %c x%d\n", idd, lado, n);
        }

        // -----------------------------------------------------------------
        // dsp d dx dy [v|i] → dispara forma em direção ao deslocamento
        // -----------------------------------------------------------------
        else if (strcmp(comando, "dsp") == 0) {
            int idd;
            double dx, dy;
            char modo[5] = "i"; // padrão i (invisível)
            fscanf(arquivo, "%d %lf %lf", &idd, &dx, &dy);
            fscanf(arquivo, "%s", modo); // pode faltar, então cuidado

            void *forma = disparar(disparadores[idd], dx, dy, modo[0]);
            if (forma != NULL) {
                totalDisparos++;
                enfileira(chao, forma);
            }

            printf("[dsp] Disparador %d disparou forma em (%.2f, %.2f)\n", idd, dx, dy);
        }

        // -----------------------------------------------------------------
        // rjd d [e|d] dx dy ix iy → rajada de disparos
        // -----------------------------------------------------------------
        else if (strcmp(comando, "rjd") == 0) {
            int idd;
            char lado;
            double dx, dy, ix, iy;

            fscanf(arquivo, "%d %c %lf %lf %lf %lf", &idd, &lado, &dx, &dy, &ix, &iy);

            int i = 0;
            while (carregador_tem_formas(disparadores[idd], lado)) {
                shift(disparadores[idd], lado);
                void *forma = disparar(disparadores[idd], dx + i * ix, dy + i * iy, 'i');
                if (forma != NULL) {
                    enfileira(chao, forma);
                    totalDisparos++;
                }
                i++;
            }

            printf("[rjd] Disparador %d rajada %c completa\n", idd, lado);
        }

        // -----------------------------------------------------------------
        // calc → processa colisões entre formas da arena
        // -----------------------------------------------------------------
        else if (strcmp(comando, "calc") == 0) {
            double pontuacaoRound = 0.0;
            int esmagadas = 0, clonadas = 0;

            processar_colisoes(chao, &pontuacaoRound, &esmagadas, &clonadas);

            pontuacaoTotal += pontuacaoRound;
            totalFormasEsmagadas += esmagadas;
            totalFormasClonadas += clonadas;

            printf("[calc] Round processado: +%.2f pontos (%d esmagadas, %d clonadas)\n",
                   pontuacaoRound, esmagadas, clonadas);
        }

        // -----------------------------------------------------------------
        // Comando desconhecido
        // -----------------------------------------------------------------
        else {
            printf("Comando desconhecido: %s\n", comando);
            char lixo[200];
            fgets(lixo, sizeof(lixo), arquivo);
        }
    }

    fclose(arquivo);

    printf("\n=== Estatísticas Finais ===\n");
    printf("Pontuação total: %.2f\n", pontuacaoTotal);
    printf("Total de disparos: %d\n", totalDisparos);
    printf("Formas esmagadas: %d\n", totalFormasEsmagadas);
    printf("Formas clonadas: %d\n", totalFormasClonadas);
}
