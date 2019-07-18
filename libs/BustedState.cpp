/**/

#include "States/BustedState.hpp"
#include "DEFINITIONS.hpp"
#include <iostream>
#include <sstream>

namespace cp
{
    BustedState::BustedState(GameDataRef _data) : data(_data) {}
    BustedState::~BustedState() {}

    void BustedState::init() {
        data->assets.load_texture("GameOverState background", GAME_OVER_BACKGROUND_FILEPATH);
        data->assets.load_texture("MenuButton", MENU_BUTTON_FILEPATH);
        background_sprite.setTexture(data->assets.get_texture("GameOverState background"));
        background_sprite.scale(1, 600.0f / 512.0f);
        background_sprite.setPosition(
            SCREEN_WIDTH  / 2.0f - background_sprite.getGlobalBounds().width  / 2.0f,
            SCREEN_HEIGHT / 2.1f - background_sprite.getGlobalBounds().height / 2.0f
        );
        menu_button_sprite.setTexture(data->assets.get_texture("MenuButton"));
        menu_button_sprite.scale(Scaling_factor / 2.5f);
        menu_button_sprite.setPosition(
            SCREEN_WIDTH  / 2.0f - menu_button_sprite.getGlobalBounds().width  / 2.0f, 
            SCREEN_HEIGHT / 2.1f - menu_button_sprite.getGlobalBounds().height * 2.3f
        );
        menu_button_sprite.setColor(sf::Color(255, 255, 255, 210));
    }

    void BustedState::handle_input(float delta) {
        sf::Event event;
        while (data->window.pollEvent(event)) {
            if (sf::Event::Closed == event.type) {
                data->window.close();
            }
            else if (data->input.is_sprite_clicked(menu_button_sprite, sf::Mouse::Left, data->window)) {
                    data->machine.remove_state(); // back to main menu
            }
        }
    }

    void BustedState::update(float delta) {
    }

    void BustedState::draw(float delta) {
        data->window.clear();
        data->window.draw(background_sprite);
        data->window.draw(menu_button_sprite);
        data->window.display();
    }

}

/**/
