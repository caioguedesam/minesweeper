/*

Campo Minado
Caio Guedes - 10/2022

O código do jogo segue uma arquitetura de três níveis, cada uma se comunicando com a camada abaixo dela:
> UI (ui.h, ui.cpp)
    > Renderiza a interface de usuário do jogo e captura entrada do usuário, invocando ações da camada de jogo dependendo da entrada.
    > Funciona com base em um sistema básico de Immediate-mode GUI, suportando renderização de botões, texto e sprites.
> Jogo (game.h, game.cpp)
    > Realiza as ações de jogo que um usuário pode fazer (revelar tile, colocar/tirar bandeira).
    > Altera e verifica o estado do tabuleiro atual, e altera o estado do jogo de acordo.
> Tabuleiro (board.h, board.cpp)
    > Os dados de um tabuleiro de jogo e armazena o grid de tiles.
    > Expõe consultas para verificar o estado do tabuleiro, como por exemplo descobrir quantas bombas tem adjacentes a um tile.

Créditos:
> Ícones feitos por Lorc. Disponíveis em: https://game-icons.net.

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