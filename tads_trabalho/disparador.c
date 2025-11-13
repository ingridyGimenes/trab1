
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "disparador.h"
#include "../tads_gerais/pilha.h"
#include "../tads_gerais/fila.h"
#include "../formas/forma.h"
#include "../formas/circulo.h"
#include "../formas/retangulo.h"
#include "../formas/linha.h"
#include "../formas/texto.h"

struct disparador {
    int id;
    double x, y;
    PILHA esq, dir;
    FORMA slot; // posição de disparo
};

static void transladaForma(FORMA f, double dx, double dy) {
    if (!f) return;
    char t = tipoForma(f);
    void* o = objetoForma(f);
    if (t=='c'){ setXCirculo(o, getXCirculo(o)+dx); setYCirculo(o, getYCirculo(o)+dy); }
    else if (t=='r'){ setXRetangulo(o, getXRetangulo(o)+dx); setYRetangulo(o, getYRetangulo(o)+dy); }
    else if (t=='l'){
        setX1Linha(o, getX1Linha(o)+dx); setY1Linha(o, getY1Linha(o)+dy);
        setX2Linha(o, getX2Linha(o)+dx); setY2Linha(o, getY2Linha(o)+dy);
    } else if (t=='t'){ setXTexto(o, getXTexto(o)+dx); setYTexto(o, getYTexto(o)+dy); }
}

static void rotacionaParaSlot(DISPARADOR d, PILHA de, PILHA paraOutro) {
    if (!d || !de || !paraOutro) return;
    FORMA nova = (FORMA)pop(de);
    if (!nova) return;
    if (d->slot) push(paraOutro, d->slot);
    d->slot = nova;
}

DISPARADOR criaDisparador(int id) {
    DISPARADOR d = malloc(sizeof(*d));
    if (!d) { perror("criaDisparador"); exit(1); }
    d->id=id; d->x=0; d->y=0; d->esq=NULL; d->dir=NULL; d->slot=NULL;
    return d;
}
void destruirDisparador(DISPARADOR d) {
    if (!d) return;
    d->esq=d->dir=NULL; d->slot=NULL; free(d);
}

void setPosicaoDisparador(DISPARADOR d, double x, double y){ if(d){ d->x=x; d->y=y; } }
void getPosicaoDisparador(DISPARADOR d, double* x, double* y){ if(d){ if(x)*x=d->x; if(y)*y=d->y; } }

void atachaCarregadores(DISPARADOR d, PILHA esq, PILHA dir){ if(d){ d->esq=esq; d->dir=dir; } }

// já existente no seu arquivo; deixe a sua versão se preferir
void shft(DISPARADOR d, char lado, int n) {
    if (!d || n<=0) return;
    for (int i=0;i<n;i++){
        if (lado=='e'){
            if (!d->esq || pilhaVazia(d->esq)) break;
            PILHA outro = d->dir ? d->dir : d->esq;
            rotacionaParaSlot(d, d->esq, outro);
        } else {
            if (!d->dir || pilhaVazia(d->dir)) break;
            PILHA outro = d->esq ? d->esq : d->dir;
            rotacionaParaSlot(d, d->dir, outro);
        }
    }
}

// já existente no seu arquivo; deixe a sua versão se preferir
void dsp(DISPARADOR d, double dx, double dy, bool visualizar, FILA arena) {
    (void)visualizar;
    if (!d || !arena || !d->slot) return;
    transladaForma(d->slot, dx, dy);
    insereNaFila(arena, d->slot);
    d->slot = NULL;
}

void rjd(DISPARADOR d, char lado, double dx, double dy, double ix, double iy, FILA arena) {
    if (!d || !arena) return;
    int i=0;
    for(;;){
        if (lado=='e'){ if (!d->esq || pilhaVazia(d->esq)) break; }
        else          { if (!d->dir || pilhaVazia(d->dir)) break; }
        shft(d, lado, 1);
        dsp(d, dx + i*ix, dy + i*iy, false, arena);
        i++;
    }
}

void lc(FILA chao, PILHA carregador, int n) {
    if (!chao || !carregador || n<=0) return;
    for (int i=0;i<n;i++){
        if (filaVazia(chao)) break;
        FORMA f = (FORMA)removeDaFila(chao); // FIFO
        push(carregador, f);                 // LIFO
    }
}
