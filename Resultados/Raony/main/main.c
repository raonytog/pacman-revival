#include "tFantasma.h"
#include "tPacman.h"
#include "tArquivos.h"

#define DIRETORIO_MAX_SIZE 1001

#define COMIDA '*'
#define PORTAL '@'
#define PAREDE '#'
#define PACMAN '>'
#define VAZIO ' '

bool OcorreuCruzamento(tPacman * pacman, tPosicao * antiga, 
                       tFantasma * left, tFantasma *  right, tFantasma * down, tFantasma * up) {
        // posicoes atuais iguais
    if (SaoIguaisPosicao(ObtemPosicaoPacman(pacman), ObtemPosicaoAtualFantasma(left)) ||
        SaoIguaisPosicao(ObtemPosicaoPacman(pacman), ObtemPosicaoAtualFantasma(right)) ||
        SaoIguaisPosicao(ObtemPosicaoPacman(pacman), ObtemPosicaoAtualFantasma(down)) ||
        SaoIguaisPosicao(ObtemPosicaoPacman(pacman), ObtemPosicaoAtualFantasma(up)) ||

        // se a atualPacman = antigaFantasma && antigaFantasma = atualPacman
        (SaoIguaisPosicao(ObtemPosicaoPacman(pacman), ObtemPosicaoAntigaFantasma(left)) &&
        SaoIguaisPosicao(antiga, ObtemPosicaoAtualFantasma(left))) ||

        (SaoIguaisPosicao(ObtemPosicaoPacman(pacman), ObtemPosicaoAntigaFantasma(right)) &&
        SaoIguaisPosicao(antiga, ObtemPosicaoAtualFantasma(right))) ||

        (SaoIguaisPosicao(ObtemPosicaoPacman(pacman), ObtemPosicaoAntigaFantasma(down)) &&
        SaoIguaisPosicao(antiga, ObtemPosicaoAtualFantasma(down))) ||

        (SaoIguaisPosicao(ObtemPosicaoPacman(pacman), ObtemPosicaoAntigaFantasma(up)) &&
        SaoIguaisPosicao(antiga, ObtemPosicaoAtualFantasma(up)))
        ) return true;
    return false;
}

COMANDO RetornaComando (char mov) {
    switch (mov) {
        case 'a':
            return MOV_ESQUERDA;

        case 'w':
            return MOV_CIMA;

        case 's':
            return MOV_BAIXO;

        case 'd':
            return MOV_DIREITA;
    }
}

void AtualizaPacmanMapa(tMapa * mapa, tPacman * pacman, tPosicao * antiga, COMANDO comando) {
    MovePacman(pacman, mapa, comando);
    if (ObtemItemMapa(mapa, antiga) == VAZIO || ObtemItemMapa(mapa, antiga) == PACMAN) AtualizaItemMapa(mapa, antiga, VAZIO);
    // AtualizaItemMapa(mapa, ObtemPosicaoPacman(pacman), PACMAN);
    if (ObtemItemMapa(mapa, ObtemPosicaoPacman(pacman)) == VAZIO) AtualizaItemMapa(mapa, ObtemPosicaoPacman(pacman), PACMAN);
}

void ImprimeMapa(tMapa * mapa) {
    for (int i = 0; i < ObtemNumeroLinhasMapa(mapa); i++) {
        for (int j = 0; j < ObtemNumeroColunasMapa(mapa); j++) {
            printf("%c", mapa->grid[i][j]);
        }
        printf("\n");
    }
}

int main (int agrc, char * argv[]) {
    if (agrc <= 1) {
        printf("ERRO: O diretorio de arquivos de configuracao nao foi informado\n");
        return 1;
    }

    char diretorio[DIRETORIO_MAX_SIZE];
    sprintf(diretorio, "%s", argv[1]);

// int main () {
//     char diretorio[DIRETORIO_MAX_SIZE] = "Casos/02";

    tMapa * mapa = CriaMapa(diretorio);
    tPacman * pacman = CriaPacman(ObtemPosicaoItemMapa(mapa, PACMAN));

    tFantasma * left = CriaFantasma(mapa, LEFT_GHOST);
    tFantasma * right = CriaFantasma(mapa, RIGHT_GHOST);
    tFantasma * down = CriaFantasma(mapa, DOWN_GHOST);
    tFantasma * up = CriaFantasma(mapa, UP_GHOST);

    GeraArquivoInicializacao(mapa, pacman);
    CriaTrilhaPacman(pacman, ObtemNumeroLinhasMapa(mapa), ObtemNumeroColunasMapa(mapa));

    char mov = '\0';
    while ( EstaVivoPacman(pacman) && 
            ObtemNumeroAtualMovimentosPacman(pacman) < ObtemNumeroMaximoMovimentosMapa(mapa) &&
            ObtemPontuacaoAtualPacman(pacman) < ObtemQuantidadeFrutasIniciaisMapa(mapa)) {

        tPosicao * clone = ClonaPosicao(ObtemPosicaoPacman(pacman));

        scanf("%c%*c", &mov);
        printf("Estado do jogo apos o movimento '%c':\n", mov);

        AtualizaPacmanMapa(mapa, pacman, clone, RetornaComando(mov));
        MoveFantasmas(down, up, left, right, mapa);

        ImprimeMapa(mapa);
        printf("Pontuacao: %d\n\n", ObtemPontuacaoAtualPacman(pacman));

        if (OcorreuCruzamento(pacman, clone, left, right, down, up)) {
            InsereNovoMovimentoSignificativoPacman(pacman, RetornaComando(mov), "fim de jogo por encostar em um fantasma");
            MataPacman(pacman);
        }

        DesalocaPosicao(clone);
    }

    if (EstaVivoPacman(pacman) && ObtemPontuacaoAtualPacman(pacman) == ObtemQuantidadeFrutasIniciaisMapa(mapa)) printf("Voce venceu!\n");
    else printf("Game over!\n");
    printf("Pontuacao final: %d\n", ObtemPontuacaoAtualPacman(pacman));

    GeraArquivos(pacman);

    DesalocaMapa(mapa);
    
    DesalocaPacman(pacman);

    DesalocaFantasma(left);
    DesalocaFantasma(right);
    DesalocaFantasma(down);
    DesalocaFantasma(up);

    return 0;
}