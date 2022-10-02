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