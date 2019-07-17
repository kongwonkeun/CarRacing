/**
 * @file MainMenuState.cpp
 * @author Vijay
 * @brief State that represents the MainMenu in the game.
 * @version 0.1
 * @date 2019-03-01
 *
 * @copyright Copyright (c) 2019
 *
 */
#include "States/MainMenuState.hpp"
#include "DEFINITIONS.hpp"
#include "NetworkManager.hpp"
#include <iostream>
#include <thread>
#include <sstream>
#include "Network/ServerRoom.hpp"
#include "Network/ClientRoom.hpp"

namespace cp
{
    MainMenuState::MainMenuState(GameDataRef _data):data(_data) {}
    MainMenuState::~MainMenuState() {}

    void MainMenuState::init() {
        data->assets.load_texture("MainMenuStateBackground", MAIN_MENU_BACKGROUND_FILEPATH);
        data->assets.load_texture("PlayButton3", RESUME_BUTTON_FILEPATH);
        data->assets.load_texture("PlayButton2_host", HOST_PLAY_BUTTON_FILEPATH);
        data->assets.load_texture("PlayButton2_join", JOIN_PLAY_BUTTON_FILEPATH);
        data->assets.load_texture("PlayButton1", SINGLE_PLAY_BUTTON_FILEPATH);
        background_sprite.setTexture(data->assets.get_texture("MainMenuStateBackground"));
        background_sprite.scale(1, 600.0f / 512.0f);
        background_sprite.setPosition(
            SCREEN_WIDTH  / 2.0f - background_sprite.getGlobalBounds().width  / 2.0f,
            SCREEN_HEIGHT / 2.1f - background_sprite.getGlobalBounds().height / 2.0f
        );
        single_play_button_sprite.setTexture(data->assets.get_texture("PlayButton1"));
        single_play_button_sprite.scale(Scaling_factor / 2.5f);
        single_play_button_sprite.setPosition(
            SCREEN_WIDTH  / 2.0f - single_play_button_sprite.getGlobalBounds().width  / 2.0f,
            SCREEN_HEIGHT / 2.0f - single_play_button_sprite.getGlobalBounds().height / 2.0f
        );
        single_play_button_sprite.setColor(sf::Color(255, 255, 255, 220));
        host_play_button_sprite.setTexture(data->assets.get_texture("PlayButton2_host"));
        host_play_button_sprite.scale(Scaling_factor / 2.5f);
        host_play_button_sprite.setPosition(
            SCREEN_WIDTH  / 2.0f - host_play_button_sprite.getGlobalBounds().width  / 2.0f,
            SCREEN_HEIGHT / 2.0f + host_play_button_sprite.getGlobalBounds().height / 1.2f
        );
        host_play_button_sprite.setColor(sf::Color(255, 255, 255, 220));
        join_play_button_sprite.setTexture(data->assets.get_texture("PlayButton2_join"));
        join_play_button_sprite.scale(Scaling_factor / 2.5f);
        join_play_button_sprite.setPosition(
            SCREEN_WIDTH  / 2.0f - join_play_button_sprite.getGlobalBounds().width  / 2.0f,
            SCREEN_HEIGHT / 2.0f + join_play_button_sprite.getGlobalBounds().height * 2.0f
        );
        join_play_button_sprite.setColor(sf::Color(255, 255, 255, 220));
    }

    void MainMenuState::handle_input(float delta) {
        sf::Event event;
        while (data->window.pollEvent(event)) {
            if (sf::Event::Closed == event.type) {
                data->window.close();
            }
            else if (data->input.is_sprite_clicked(single_play_button_sprite, sf::Mouse::Left, data->window)) { // single play
                data->machine.add_state(StateRef(new ServerState(data, std::set<std::shared_ptr<Client>>())), false);
            }
            else if (data->input.is_sprite_clicked(host_play_button_sprite, sf::Mouse::Left, data->window)) { // host play
                data->machine.add_state(StateRef(new ServerRoom(data)), false);
            }
            else if (data->input.is_sprite_clicked(join_play_button_sprite, sf::Mouse::Left, data->window)) { // join play
                data->machine.add_state(StateRef(new ClientRoom(data)), false);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                data->machine.add_state(StateRef(new ServerState(data, std::set<std::shared_ptr<Client>>())), false); // single play (key s)
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { // exit the game app (key escape)
                data->window.close();
            }
        }
    }

    void MainMenuState::draw(float delta) {
        data->window.clear();
        data->window.draw(background_sprite);
        data->window.draw(title_sprite);
        data->window.draw(single_play_button_sprite);
        data->window.draw(join_play_button_sprite);
        data->window.draw(host_play_button_sprite);
        data->window.display();
    }

}
