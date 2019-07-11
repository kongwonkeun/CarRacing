#include "DEFINITIONS.hpp"
#include <time.h>
#include "Game.hpp"
#include <iostream>
//#include <SFML/Graphics.hpp>

int main() 
{
    srand(static_cast<unsigned int>(time(NULL)));
    std::cout << "----AAAA----" << std::endl;
    cp::Game(SCREEN_WIDTH, SCREEN_HEIGHT, "SFML POPL2 Chor Police Project");
    std::cout << "----BBBB----" << std::endl;
    std::cout << "----HI----" << std::endl;
    /*
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        window.draw(shape);
        window.display();
    }
    */
    return EXIT_SUCCESS;
}
