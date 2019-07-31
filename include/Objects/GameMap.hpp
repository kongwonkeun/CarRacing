#ifndef GAME_MAP_HPP
#define GAME_MAP_HPP

#include <vector>
#include <cmath>
#include <memory>
#include "SFML/Graphics.hpp"
#include "Objects/Line.hpp"
#include "States/State.hpp"
#include "Objects/PlayerCar.hpp"
#include "Objects/Camera.hpp"
#include "Objects/Car.hpp"
#include "Objects/Bullet.hpp"
#include "DEFINITIONS.hpp"
#include "Game.hpp"

namespace cp
{
    class GameMap
    {
    public:
        GameMap(GameDataRef _data);
        ~GameMap();
        void init();
        void draw_quad(sf::Color c, float x1, float y1, float w1, float x2, float y2, float w2);
        void project(Line &line, float camX, float camY, float camZ, float camD);
        void draw(int count, const Camera& main_camera);
        void drawSprite(const Line& line);
        void bound_entity(cp::Car& car);
        void bound_entity(Camera& camera);
        void bound_entity(Bullet& bot);
        int get_grid_index(float distance);
        int getRoadWidth() const;
        int getSegL() const;
        int getGridCount() const;
        int getScreenWidth() const;
        int getScreenHeight() const;

        std::vector<Line> lines;

    private:
        GameDataRef data;
        sf::Texture t[10];
        sf::Sprite object[10];
        sf::Sprite background_sprite;
        int width = SCREEN_WIDTH;
        int height = SCREEN_HEIGHT;
        int N = 0; // total grid count
        int roadW = ROAD_WIDTH;
        int segL = SEGMENT_LENGTH;
    };
}

#endif // GAME_MAP_HPP
