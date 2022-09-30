#include "ui.h"

//// CLI MAIN LOOP
//int main()
//{
//	Game game;
//
//	while (true)
//	{
//		ui_clear();
//		ui_render_game(&game);
//		ui_poll_input(&game);
//		game.check_end_condition();
//	}
//
//	return 0;
//}

// GUI MAIN LOOP
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