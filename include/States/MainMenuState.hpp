#ifndef MAINMENUSTATE_HPP
#define MAINMENUSTATE_HPP

#include "SFML/Graphics.hpp"
#include "State.hpp"
#include "Game.hpp"

namespace cp
{
    class MainMenuState : public State
    {
    public:
        MainMenuState(GameDataRef _data);
        ~MainMenuState();
        void init();
        void handle_input(float delta);
        void update(float delta) {}
        void draw(float delta);

    private:
        GameDataRef data;
        sf::Sprite background_sprite;
        sf::Sprite title_sprite;
        sf::Sprite play_button_sprite;
        sf::Sprite single_play_button_sprite;
        sf::Sprite host_play_button_sprite;
        sf::Sprite join_play_button_sprite;
        bool server = false;
        bool client = false;
    };
}

#endif // MAINMENUSTATE_HPP
