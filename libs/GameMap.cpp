/**/

#include "Objects/GameMap.hpp"

namespace cp
{
    GameMap::GameMap(GameDataRef _data) : data(_data) {}
    GameMap::~GameMap() {}

    void GameMap::init() {
        for (int i = 1; i <= 9; i++) {
            t[i].loadFromFile("../res/" + std::to_string(i) + ".png");
            t[i].setSmooth(true);
            object[i].setTexture(t[i]);
        }
        data->assets.load_texture("GameStateBackground", GAME_STATE_BACKGROUND_FILEPATH);
        data->assets.get_texture("GameStateBackground").setRepeated(true);
        background_sprite.setTexture(data->assets.get_texture("GameStateBackground"));
        background_sprite.setTextureRect(sf::IntRect(0, 0, 6000, 411));
        background_sprite.setPosition(-2000, 0);
        
        for (int i = 0; i < 1600; i++) {
            Line line;
            line.z = static_cast<float>(i * segL);
            if (i > 300 && i < 700) line.curve =  0.5f;
            if (i > 1100)           line.curve = -0.7f;
            if (i % 17 == 0)            { line.spriteX =  1.0f; line.sprite = object[3]; }
            if (i < 300 && i % 20 == 0) { line.spriteX = -2.5f; line.sprite = object[5]; }
            if (i > 300 && i % 20 == 0) { line.spriteX = -1.2f; line.sprite = object[4]; }
            if (i > 800 && i % 20 == 0) { line.spriteX = -1.2f; line.sprite = object[1]; }
            lines.push_back(line);
        }
        N = static_cast<int>(lines.size());
    }

    void GameMap::draw_quad(sf::Color c, float x1, float y1, float w1, float x2, float y2, float w2) {
        sf::ConvexShape shape(4);
        shape.setFillColor(c);
        shape.setPoint(0, sf::Vector2f(x1 - w1, y1));
        shape.setPoint(1, sf::Vector2f(x2 - w2, y2));
        shape.setPoint(2, sf::Vector2f(x2 + w2, y2));
        shape.setPoint(3, sf::Vector2f(x1 + w1, y1));
        data->window.draw(shape);
    }

    void GameMap::project(Line& line, float camX, float camY, float camZ, float camD) {
        line.scale = camD / (line.z - camZ);
        line.no_curve_X = (1 - line.scale * (camX))          * width  / 2;
        line.X =          (1 + line.scale * (line.x - camX)) * width  / 2;
        line.no_curve_Y = (1 + line.scale * (camY))          * height / 2;
        line.Y =          (1 - line.scale * (line.y - camY)) * height / 2;
        line.W = line.scale * roadW * width / 2;
    }

    void GameMap::drawSprite(const Line& line) {
        sf::Sprite s = line.sprite;
        int w = s.getTextureRect().width;
        int h = s.getTextureRect().height;
        float destX = line.X + line.scale * line.spriteX * width / 2;
        float destY = line.Y + 4;
        float destW = w * line.W / 266;
        float destH = h * line.W / 266;
        destX += destW * line.spriteX; // offsetX
        destY += destH * (-1); // offsetY
        float clipH = destY + destH - line.clip;
        if (clipH < 0) clipH = 0;
        if (clipH >= destH) return;
        s.setTextureRect(sf::IntRect(0, 0, w, static_cast<int>(h - h * clipH / destH)));
        s.setScale(destW / w, destH / h);
        s.setPosition(destX, destY);
        data->window.draw(s);
    }

    void GameMap::draw(const int count, const Camera& camera) {
        data->window.draw(background_sprite);
        Camera main_camera = camera;
        int startPos = get_grid_index(main_camera.getPosition().z);
        main_camera.e_position.y += lines[startPos].y;
        int temp_z = startPos * segL;

        int maxy = height;
        float x = 0, dx = 0;
        sf::Vector3f l_snap;
        for (int n = startPos; n < startPos + count; n++) {
            Line& l = lines[n % N];
            l_snap = sf::Vector3f(l.x, l.y, l.z);
            main_camera.e_position.z = static_cast<float>(temp_z); // for rendering bug
            l.x  = 0;
            l.z += (n >= N ? N * segL : 0);
            l.x += x;

            project(
                l,
                main_camera.getPosition().x,
                main_camera.getPosition().y,
                main_camera.getPosition().z,
                main_camera.getCamD()
            );

            l.z = l_snap.z;
            x += dx;
            dx += l.curve;

            l.clip = static_cast<float>(maxy);
            if (l.Y >= maxy) continue;
            maxy = static_cast<int>(l.Y);

            sf::Color grass   = (n / 3) % 2 ? sf::Color(16, 200, 16)   : sf::Color(0, 154, 0);
            sf::Color marking = (n / 3) % 2 ? sf::Color::White         : sf::Color(105, 105, 105);
            sf::Color rumble  = (n / 3) % 2 ? sf::Color(255, 255, 255) : sf::Color(0, 0, 0);
            sf::Color road    = (n / 3) % 2 ? sf::Color(107, 107, 107) : sf::Color(105, 105, 105);

            Line p = lines[(n - 1) % N];

            draw_quad(grass,   0.0f, p.Y, static_cast<float>(width), 0.0f, l.Y, static_cast<float>(width));
            draw_quad(rumble,  p.X,  p.Y, p.W * 1.1f,  l.X, l.Y, l.W * 1.1f);
            draw_quad(road,    p.X,  p.Y, p.W,         l.X, l.Y, l.W);
            draw_quad(marking, p.X,  p.Y, p.W * 0.35f, l.X, l.Y, l.W * 0.35f);
            draw_quad(road,    p.X,  p.Y, p.W * 0.3f,  l.X, l.Y, l.W * 0.3f);
        }
    }

    int GameMap::get_grid_index(const float distance) {
        return static_cast<int>(distance / segL);
    }
    
    void GameMap::bound_entity(cp::Car& car) {
        while (car.e_position.z >= N * segL) { car.e_position.z -= N * segL; }
        while (car.e_position.z <  0) { car.e_position.z += N * segL; }
    }
    
    void GameMap::bound_entity(Camera& camera) {
        while (camera.e_position.z >= N * segL) { camera.e_position.z -= N * segL; }
        while (camera.e_position.z <  0) { camera.e_position.z += N * segL; }
    }
    
    void GameMap::bound_entity(Bullet& bullet) {
        while (bullet.e_position.z >= N * segL) { bullet.e_position.z -= N * segL; }
        while (bullet.e_position.z <  0) { bullet.e_position.z += N * segL; }
    }

    int GameMap::getRoadWidth() const { return roadW; }
    int GameMap::getSegL() const { return segL; }
    int GameMap::getGridCount() const { return N; }
    int GameMap::GameMap::getScreenWidth() const { return width; }
    int GameMap::getScreenHeight() const { return height; }

}

/**/
