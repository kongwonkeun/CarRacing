#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <set>
#include "States/GameOverState.hpp"
#include "BustedState.hpp"
#include "DEFINITIONS.hpp"
#include "Objects/Bot.hpp"
#include "Objects/PlayerCar.hpp"
#include "States/State.hpp"
#include "Game.hpp"
#include "Objects/Line.hpp"
#include "Physics/Collision.hpp"
#include "Objects/Camera.hpp"
#include "Objects/GameMap.hpp"
#include "States/PauseState.hpp"
#include "PercentageBar.hpp"
#include "Objects/Bullet.hpp"
#include "ObjectPool.hpp"

namespace cp
{
    class GameState : public State
    {
    public:
        typedef std::shared_ptr<PlayerCar> CarRef;

        GameState(GameDataRef _data);
        ~GameState();
        void init();
        void handle_input(float delta);
        void draw(float delta);
        void update(float delta);
        void drawSprite(Line& line);
        //static void network_handler(GameDataRef data, std::shared_ptr<PlayerCar> car, std::shared_ptr<Bot> bot);

    private:
        Camera main_camera;
        GameMap map;
        GameDataRef data;
        CarRef car;
        Collision collision;
        PercentageBar bar;
        ObjectPool<Bullet> pool;
        Bullet* bullet = nullptr;
        std::set<Bullet*> bullet_set[2];
        std::shared_ptr<Bot> bot[TOTAL_BOTS];
        std::thread network_handle;
        sf::Sprite  s;
        sf::Font font;
        sf::Text text[5];
        //sf::Clock clock;
        int current = 0;
        long long int score = 0;
        //float fuel = 100;
        //float current_time = 0;
        //float new_time = 0;
    };
}

#endif // GAMESTATE_HPP
