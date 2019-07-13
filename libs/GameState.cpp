#include <iostream>
#include <cmath>
#include <sstream>
#include "States/GameOverState.hpp"
#include "States/GameState.hpp"
#include "DEFINITIONS.hpp"

namespace cp
{
    GameState::GameState(GameDataRef _data) : data(_data), map(_data), bar(_data), pool(200) {}
    GameState::~GameState() {}

    void GameState::init() {
        map.init();
        for (int i = 0; i < TOTAL_CARS; i++) {
            data->assets.load_texture("CarImage" + std::to_string(i), CAR_IMAGE_FILEPATH(i));
        }
        for (int i = 3; i <= 5; i++) {
            data->assets.load_texture("PCarImage"+std::to_string(i),P_CAR_IMAGE_FILEPATH(i));
        }
        for(int i = 1; i <= 12; i++) {
            data->assets.load_texture("f"+std::to_string(i),FIRE_IMAGE_FILEPATH(i));
        }
        data->assets.load_texture("Bullet", "../res/bullet.png");
        car = std::shared_ptr<PlayerCar>(new PlayerCar(data,5));
        car->e_position.x = 1.1f;
        for (int i = 0; i < TOTAL_BOTS; i++) {
            bot[i] = std::shared_ptr<PlayerCar>(new PlayerCar(data, 5));
            //bot[i]->e_position.x = (i & 1) ? 0.0 : -1.1;
            //bot[i]->e_position.z = (i / 4) * 4000;
            //bot[i]->e_speed.z = 0;
        }
        data->assets.load_texture("GameOverState background", GAME_OVER_BACKGROUND_FILEPATH);
        font = data->assets.get_font("sfafont");

        for (int i = 0; i < 5; i++) {
            text[i].setFont(font);
            text[i].setCharacterSize(36);
            text[i].setStyle(sf::Text::Bold);
            text[i].setOutlineColor(sf::Color::White);
            text[i].setOutlineThickness(2);
            text[i].setFillColor(sf::Color::Red);
        }
        text[0].setString("Score:");
        text[0].setPosition(SCREEN_WIDTH / 100.0f,     SCREEN_HEIGHT / 100.0f);
        text[1].setPosition(SCREEN_WIDTH / 100.0f,     SCREEN_HEIGHT / 130.0f);
        text[2].setString("Health:");
        text[2].setPosition(SCREEN_WIDTH / 2.0f - 150, SCREEN_HEIGHT / 100.0f);

        bar.init(
            sf::Vector2f(300, 20),
            sf::Vector2f(SCREEN_WIDTH / 2.0f - 150, 70.0f),
            sf::Color::White,
            sf::Color::Black
        );
    }

    void GameState::handle_input(float delta) {
        sf::Event event;
        while (data->window.pollEvent(event)) {
            if (sf::Event::Closed == event.type) {
                data->window.close();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                data->machine.remove_state();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                car->health = 0;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                data->machine.add_state(StateRef(new PauseState(data)), false);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
                bullet = pool.getObject(data, 5);
                bullet->init(car->e_position);
                bullet_set[current].insert(bullet);
            }
        }
        // TODO : Create a driver/bot_mind class
        std::vector<bool> mask;
        for (int i = 0; i < TOTAL_BOTS; i++) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                mask.push_back(1);
            else
                mask.push_back(0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                mask.push_back(1);
            else
                mask.push_back(0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                mask.push_back(1);
            else
                mask.push_back(0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                mask.push_back(1);
            else
                mask.push_back(0);
            for (int i = 0; i < TOTAL_BOTS; i++) {
                bot[i]->handle_input(mask, delta);
            }
        }
        car->handle_input(mask, delta);
        main_camera.e_position.x = bot[0]->e_position.x * 1024;
        main_camera.e_position.z = bot[0]->e_position.z - 2000;

        map.bound_entity(main_camera);
        for (int i = 0; i < TOTAL_BOTS; i++) {
            map.bound_entity(*bot[i]);
        }
        for (auto itr : bullet_set[current]) {
            map.bound_entity(*itr);
        }
        map.bound_entity(*car);
    }

}
