#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <map>

namespace cp
{
    class InputManager
    {
    public:
        using ID = int;
        using input_type = std::vector<bool>;
        using register_input_type = std::pair<ID, input_type>;

        InputManager();
        ~InputManager();
        void register_input(register_input_type input_pair);
        bool is_sprite_clicked(sf::Sprite sprite, sf::Mouse::Button button, sf::RenderWindow& window);
        sf::Vector2i get_mouse_position(sf::RenderWindow& window);
        input_type get_mask(ID id);

    private:
        std::map<ID, input_type> mp;
    };
}

#endif // INPUTMANAGER_HPP
