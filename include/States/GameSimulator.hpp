/**
 * @file GameSimulator.hpp
 * @author Vijay
 * @brief A game simulator just like Game class but it get's its clock sync
 * and resource manager from object owner.
 * @version 0.1
 * @date 2019-02-26
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef GAME_SIMULATOR_HPP
#define GAME_SIMULATOR_HPP

#include "Game.hpp"
#include <SFML/Graphics.hpp>
#include "GameOverState.hpp"
#include "DEFINITIONS.hpp"
#include "Objects/PlayerCar.hpp"
#include "Objects/Line.hpp"
#include "Physics/Collision.hpp"
#include "Objects/Camera.hpp"
#include "Objects/GameMap.hpp"
#include <memory>
#include <fstream>
#include <set>
#include <SFML/Network.hpp>
#include "PercentageBar.hpp"
#include "Objects/Bullet.hpp"
#include "ObjectPool.hpp"
#include "States/MainMenuState.hpp"
#include "BustedState.hpp"
#include <future>

#define lli long long int

namespace cp
{
    class entity_info
    {
    public:
        friend class GameSimulator;
        using ID = long long int;

        entity_info() {}
        entity_info(cp::PlayerCar& car) {
            x = car.getPosition().x;
            y = car.getPosition().y;
            z = car.getPosition().z;
            speed_x = car.getSpeed().x;
            speed_y = car.getSpeed().y;
            speed_z = car.getSpeed().z;
            id = car.id;
        }
        /*
        friend std::ofstream& operator << (std::ofstream& fout, const entity_info& entity_i) {
            fout << "Position:" << entity_i.x << " " << entity_i.y << " " << entity_i.z << std::endl;
            fout << "Speed   :" << entity_i.speed_x << " " << entity_i.speed_y << " " << entity_i.speed_z << std::endl;
            return fout;
        }
        */
        friend sf::Packet& operator << (sf::Packet& fout, const entity_info& entity_i) {
            fout << entity_i.x << entity_i.y << entity_i.z;
            return fout;
        }

        friend sf::Packet& operator >> (sf::Packet& fin, entity_info& entity_i) {
            fin >> entity_i.x >> entity_i.y >> entity_i.z;
            return fin;
        }

    private:
        float x = 0, y = 0, z = 0;
        float speed_x = 0, speed_y = 0, speed_z = 0;
        ID id;
    };

    enum class SnapFlag
    {
        NETWORK_SNAP,
        OFFLINE_SNAP
    };

    class GameSimulatorSnap
    {
    public:
        friend class GameSimulator;
        using ID = long long int;

        GameSimulatorSnap() {}
        GameSimulatorSnap(int a, int b, int c, int d, std::map<ID, PlayerCar>& players_map) {
            ext_players_count = a;
            bot_players_count = b;
            MAX_EXT_ALLOWED = c;
            main_player_id = d;
            data.clear();
            for (auto& player : players_map) {
                data.insert(std::pair<ID, entity_info>(player.first, entity_info(player.second)));
            }
        }

        friend sf::Packet& operator << (sf::Packet& fout, const GameSimulatorSnap& snap) {
            sf::Uint64 size = snap.data.size();
            fout << snap.ext_players_count << snap.bot_players_count << size;
            for (auto& player_i : snap.data) {
                fout << player_i.first << player_i.second;
            }
            return fout;
        }

        friend sf::Packet& operator >> (sf::Packet& fin, GameSimulatorSnap& snap) {
            sf::Uint64 size;
            GameSimulatorSnap::ID first;
            entity_info second;
            snap.data.clear();
            fin >> snap.ext_players_count >> snap.bot_players_count >> size;
            for (int i = 0; i < size; i++) {
                fin >> first >> second;
                snap.data.insert(std::pair<GameSimulatorSnap::ID, entity_info>(first, second));
            }
            return fin;
        }

    private:
        ID main_player_id = 0;
        std::map<ID, entity_info> data;
        int ext_players_count = 0;
        int bot_players_count = 0;
        int MAX_EXT_ALLOWED = 0;
    };

    class GameSimulator
    {
    public:
        using ID = long long int;
        using input_type = Car::input_type;
        using input_return_type = std::pair<ID, input_type>;
        using CarRef = std::shared_ptr<PlayerCar>;

        GameSimulator(GameDataRef res_store);
        ~GameSimulator();
        void init();
        void handle_input(float delta);
        void update(float delta);
        void draw(float delta);

        GameSimulatorSnap get_current_snap(SnapFlag flag);
        void use_snap(const GameSimulatorSnap& snap, bool is_forced = true);
        PlayerCar generate_bot(const entity_info& info, ID id);
        float distance(entity_info& a, entity_info& b) { return ((a.x - b.x) * (a.x - b.x) + (a.z - b.z) * (a.z - b.z)); }

        void output(entity_info& a, entity_info& b, std::vector<bool>& input) {
            /*
            switch (rand() % 6) {
            case 0: input.push_back(0); input.push_back(0); input.push_back(0); input.push_back(0); break;
            case 1: input.push_back(1); input.push_back(0); input.push_back(0); input.push_back(0); break;
            case 2: input.push_back(0); input.push_back(0); input.push_back(1); input.push_back(0); break;
            case 3: input.push_back(1); input.push_back(0); input.push_back(1); input.push_back(0); break;
            case 4: input.push_back(0); input.push_back(0); input.push_back(0); input.push_back(1); break;
            case 5: input.push_back(1); input.push_back(0); input.push_back(0); input.push_back(1); break;
            default:
                break;
            }
            */
            if (rand() % 4 >= 2) {
                input.push_back(0);
                input.push_back(0);
                input.push_back(0);
                input.push_back(0);
            }
            else {
                float diff = (a.z - b.z) / 200;
                if (std::abs(diff) < 800) {
                    input.push_back(a.z < b.z);
                    input.push_back(0);
                }
                else {
                    input.push_back(a.z > b.z);
                    input.push_back(0);
                }
                input.push_back(a.x > b.x);
                input.push_back(a.x < b.x);
            }
        }

        void AI_bot_output() {
            GameSimulatorSnap snap = get_current_snap(SnapFlag::NETWORK_SNAP);
            std::map<ID, entity_info> cars[2];
            for (auto& player_i : snap.data) {
                if (player_i.first > 0)
                    cars[0].insert(std::pair<lli, entity_info>(player_i.first, player_i.second));
                else
                    cars[1].insert(std::pair<lli, entity_info>(player_i.first, player_i.second));
            }
            std::vector<bool> input_for_bots;
            for (auto& bot : cars[1]) {
                for (auto& player : cars[0]) {
                    output(bot.second, player.second, input_for_bots);
                    resource_store->input.register_input(std::pair<lli, std::vector<bool>>(bot.first, input_for_bots));
                    //---- kong ----
                    break;
                    //
                }
            }
        }

        static void update_bullets(Bullet *itr, GameMap *mapp, float *delta) {
            mapp->bound_entity(*itr);
            itr->update_car(*delta, mapp->lines, static_cast<float>(mapp->getSegL()));
        }

        bool add_external_player(ID id) {
            if (ext_players_count >= MAX_EXT_ALLOWED) return false;
            if (players_map.find(id) != players_map.end()) return false;
            players_map.insert(std::pair<ID, PlayerCar>(id, PlayerCar(resource_store, 5, id)));
            ext_players_count++;
            return true;
        }
        /*
        void remove_ext_player(ID id) {
            if (players_map.find(id) != players_map.end()) {
                players_map.erase(id);
                ext_players_count--;
            }
        }
        */
        bool add_bot_players() {
            ID id = -1 * bot_players_count - 1;
            players_map.insert(std::pair<ID, PlayerCar>(id, PlayerCar(resource_store, 8, id)));
            bot_players_count++;
            return true;
        }

        bool update_main_player(ID id) {
            if (players_map.find(id) != players_map.end()) {
                main_player_id = id;
                players_map.at(main_player_id).e_position.z = 1000;
                return true;
            }
            return false;
        }

        bool is_main_player_available() {
            if (players_map.find(main_player_id) != players_map.end()) return true;
            else return false;
        }

        input_return_type get_input() {
            input_return_type input;
            input.first = main_player_id;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    input.second.push_back(1);
            else input.second.push_back(0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  input.second.push_back(1);
            else input.second.push_back(0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  input.second.push_back(1);
            else input.second.push_back(0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) input.second.push_back(1);
            else input.second.push_back(0);
            return input;
        }

        void focus_on(ID id) {
            if (players_map.find(id) != players_map.end()) {
                main_camera.e_position.x = players_map.at(id).getPosition().x * 1024;
                main_camera.e_position.z = players_map.at(id).getPosition().z - 2000;
            }
        }

        std::future<void> async_update[200];

    private:
        void drawSprite(Line &line);

        void init_car_res() {
            for (int i = 0; i < TOTAL_CARS; i++) {
                resource_store->assets.load_texture("CarImage" + std::to_string(i), CAR_IMAGE_FILEPATH(i));
            }
            for (int i = 1; i <= 12; i++) {
                resource_store->assets.load_texture("f" + std::to_string(i),FIRE_IMAGE_FILEPATH(i));
            }
        }

        void update_controllable(float delta) {
            for (auto& player : players_map) {
                input_type mask = get_mask(player.first);
                player.second.handle_input(mask, delta);
            }
        }

        input_type get_mask(ID id) {
            return resource_store->input.get_mask(id);
        }

        void bot_inp_reg() {
            input_type inp;
            inp.push_back(1);
            inp.push_back(0);
            inp.push_back(0);
            inp.push_back(0);
            for (auto &player_i : players_map) {
                if (player_i.first < 0)
                    resource_store->input.register_input(input_return_type(player_i.first, inp));
            }
        }

        std::vector<Car*> get_entity_list();

        void draw_all_players() {
            for (auto& player_i : players_map) {
                draw_player(player_i.second);
            }
        }

        void draw_player(PlayerCar& player) {
            int index = map.get_grid_index(player.getPosition().z);
            Line &temp_line = map.lines[index];
            player.drawSprite(map.lines[index]);
        }

        const PlayerCar& getPlayer(ID id) {
            return players_map.at(id);
        }

        ID main_player_id = -1;
        PercentageBar bar;
        Camera main_camera;
        GameMap map;
        GameDataRef resource_store;
        Collision collision;
        ObjectPool<Bullet> pool;
        Bullet* bullet = nullptr;
        std::set<Bullet*> bullet_set[2];
        int ext_players_count = 0;
        int bot_players_count = 0;
        int MAX_EXT_ALLOWED = 4;
        std::map<ID, PlayerCar> players_map;
        sf::Font font;
        sf::Text text[5];
        long long int score = 0;
        int current = 0;
    };
}

#endif // GAME_SIMULATOR_HPP
