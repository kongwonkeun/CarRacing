#ifndef BUSTED_STATE_HPP
#define BUSTED_STATE_HPP

#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "Game.hpp"

namespace cp
{
    class BustedState : public State
    {
    public:
        BustedState(GameDataRef _data);
        ~BustedState();
        void init();
        void handle_input(float delta);
        void update(float delta);
        void draw(float delta);

    private:
        GameDataRef data;
        sf::Sprite menu_button_sprite;
        sf::Sprite background_sprite;
        sf::Font font;
        sf::Text text;
    };
}

#endif // BUSTED_STATE_HPP
