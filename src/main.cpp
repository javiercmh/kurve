#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <vector>

#include "display.hpp"
#include "game.hpp"
#include "gl_calls.hpp"
#include "user.hpp"


int main(void) {
    // Create our user data struct:
    user_data_t user_data =
        {
            .window_width = 800,
            .window_height = 600,
            .game_state = GAME_MENU
        };

    Display display(&user_data);
    GLFWwindow* window = display.get_window();

    Game game(window);

    game.loop();

    game.terminate();

    return 0;
}
