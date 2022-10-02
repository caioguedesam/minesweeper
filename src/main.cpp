/*

Campo Minado
Caio Guedes - 10/2022

O c�digo do jogo segue uma arquitetura de tr�s n�veis, cada uma se comunicando com a camada abaixo dela:
> UI (ui.h, ui.cpp)
    > Renderiza a interface de usu�rio do jogo e captura entrada do usu�rio, invocando a��es da camada de jogo dependendo da entrada.
    > Funciona com base em um sistema b�sico de Immediate-mode GUI, suportando renderiza��o de bot�es, texto e sprites.
> Jogo (game.h, game.cpp)
    > Realiza as a��es de jogo que um usu�rio pode fazer (revelar tile, colocar/tirar bandeira).
    > Altera e verifica o estado do tabuleiro atual, e altera o estado do jogo de acordo.
> Tabuleiro (board.h, board.cpp)
    > Os dados de um tabuleiro de jogo e armazena o grid de tiles.
    > Exp�e consultas para verificar o estado do tabuleiro, como por exemplo descobrir quantas bombas tem adjacentes a um tile.

Cr�ditos:
> �cones feitos por Lorc. Dispon�veis em: https://game-icons.net.

*/

#include "ui.h"

int main()
{
    Game game;

    ui_init();
    while (game.is_running)
    {
        ui_poll_input(&game);
        ui_clear();
        ui_render_game(&game);
        ui_present();
        game.check_end_condition();
    }

    return 0;
}