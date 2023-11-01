#ifndef TFANTASMA_H_
#define TFANTASMA_H_

#include "tPosicao.h"
#include "tJogo.h"
#include "tMapa.h"

#define MAX_FANTASMAS 4

#define FANT_B 'B'  //fantasma horizontal   - inicio: esquerda
#define FANT_C 'C'  //fantasma horizontal   - inicio: direita
#define FANT_I 'I'  //fantasma vertical     - inicio: baixo
#define FANT_P 'P'  //fantasma vertical     - inicio: cima

typedef struct tFantasma {
    int presenteMapa;
    int dx;
    int dy;
    tPosicao * posicao;
    tPosicao * posicaoAntiga;
} tFantasma;

tPosicao * ObtemPosicaoFantasma (tFantasma * fantasma);

tFantasma * CriaFantasma(tMapa * mapa, char simoblogia_fantasma);

tFantasma* RetornaFantasmaB(tJogo * jogo);

tFantasma* RetornaFantasmaC(tJogo * jogo);

tFantasma* RetornaFantasmaI(tJogo * jogo);

tFantasma* RetornaFantasmaP(tJogo * jogo);

void MoveFantasmas (tJogo * fantasma, tMapa * mapa)

int EstaPresenteFantasma (tFantasma * fantasma);

#endif