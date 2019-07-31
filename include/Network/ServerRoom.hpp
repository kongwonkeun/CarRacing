#ifndef SERVER_ROOM_HPP
#define SERVER_ROOM_HPP

#include <iostream>
#include <cstring>
#include <set>
#include <fstream>
#include <memory>
#include "States/State.hpp"
#include "States/ServerState.hpp"
#include "Network/Client.hpp"
#include "Game.hpp"

namespace cp
{
    class ServerRoom : public State
    {
    public:
        using TcpClient_ptr = std::shared_ptr<Client>;

        ServerRoom(GameDataRef _data) : game_data(_data) {}
        ~ServerRoom() {}

        void init() {
            clock.restart();
            int n_try = 0;
            while (listener.listen(PORT) != sf::Socket::Done && n_try < 10) {
                std::cout << "port listen error" << std::endl;
                n_try++;
            }
            if (n_try >= 10) {
                std::cout << "port listen timeover error" << std::endl;
                PORT_OPEN_SUCCESS = false;
                handle_port_problem();
                return;
            }
            selector.add(listener);
        }

        virtual void handle_input(float delta) {
            if (!update_required) return;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                JUST_WAIT = 2;
                notify_clients();
                game_data->machine.remove_state();
                update_required = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                JUST_WAIT = 0;
                notify_clients();
                game_data->machine.add_state(StateRef(new ServerState(game_data, clients_res)), true);
                for (auto it : clients_res) it->get_socket().setBlocking(false);
                update_required = false;
            }
        }

        virtual void update(float delta) {
            if (!update_required) return;
            if (join) {
                JUST_WAIT = 0;
                notify_clients();
                game_data->machine.add_state(StateRef(new ServerState(game_data, clients_res)), true);
                update_required = false;
            }
            else {
                check_incoming_connections();
            }
        }

        virtual void draw(float delta) {}
        virtual void pause() {}
        virtual void resume() {}

    private:
        void check_incoming_connections() {
            if (selector.wait(sf::milliseconds(1))) {
                if (selector.isReady(listener)) {
                    int unique_id = ID_JOIN_PLAYER;
                    TcpClient_ptr client = TcpClient_ptr(new Client(unique_id));
                    std::cout << "listen a join request from remote player" << std::endl;

                    if (listener.accept(client->get_socket()) == sf::Socket::Done) {
                        std::cout << "remote player joined" << std::endl;
                        clients_res.insert(client);
                        sf::Packet packet; 
                        packet << unique_id;
                        client->get_socket().send(packet);
                        join = true;
                    }
                }
            }
        }

        void handle_port_problem() {
            if (PORT_OPEN_SUCCESS == false) {
                game_data->machine.remove_state();
                update_required = false;
            }
        }

        void handle_dead_clients() {
            for (auto it = clients_res.begin(); it != clients_res.end(); ) {
                if ((*it)->get_socket().Disconnected) {
                    unassigned_id.insert(static_cast<int>((*it)->get_identity()));
                    it = clients_res.erase(it);
                } else {
                    ++it;
                }
            }
        }

        void notify_clients() {
            sf::Packet response;
            response << JUST_WAIT;
            for (auto client_ptr : clients_res) {
                client_ptr->send_packet(response);
            }
        }

        GameDataRef game_data;
        std::set<TcpClient_ptr> clients_res;
        sf::TcpListener listener;
        sf::SocketSelector selector;
        sf::Sprite background_sprite;
        std::set<int> unassigned_id = { 2, 3, 4, 5 };
        bool PORT_OPEN_SUCCESS = true;
        int JUST_WAIT = 1;
        sf::Clock clock;
        bool time_over = false;
        bool update_required = true;
        bool join = false;
    };
}

#endif // SERVER_ROOM_HPP
