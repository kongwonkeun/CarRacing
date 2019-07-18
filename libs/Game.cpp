/**/

#include "Game.hpp"
#include "DEFINITIONS.hpp"
#include "States/SplashState.hpp"

namespace cp
{
    Game::Game() {
        data->window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "K-Roller", sf::Style::Fullscreen);
        data->machine.add_state(StateRef(new SplashState(data)));
        data->window.setFramerateLimit(80);
        run();
    }

    Game::~Game() {
    }
    
    void Game::run() {
        float new_time, frame_time, interpolation;
        float current_time = clock.getElapsedTime().asSeconds();
        float accumulator = 0.0f;

        while (data->window.isOpen()) {
            data->machine.process_state_change();
            new_time = clock.getElapsedTime().asSeconds();
            frame_time = new_time - current_time;

            if (frame_time > 0.25f) {
                frame_time = 0.25f;
            }
            current_time = new_time;
            accumulator += frame_time;

            while (accumulator >= delta) {
                data->machine.get_active_state()->handle_input(delta);
                data->machine.get_active_state()->update(delta);
                accumulator -= delta;
            }
            interpolation = accumulator / delta;
            data->machine.get_active_state()->draw(interpolation);
        }
    }

}

/**/
