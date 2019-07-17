/**
 *
 * @file Server.hpp
 * @author Vijay
 * @brief Server class that handles the data sending over the network
 * @version 0.1
 * @date 2019-02-26
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef SERVER_HPP
#define SERVER_HPP

#include "States/GameSimulator.hpp"

namespace cp
{
    class Server
    {
    public:
        using ID = long long int;
        using key_input_type = std::pair<ID, std::vector<bool>>;

        Server(ID identity) : id(identity) {}
        ~Server() {}

        friend Server& operator << (Server& server, const key_input_type& labelled_input) {
            sf::Packet packet;
            sf::Uint64 size = labelled_input.second.size();
            packet << labelled_input.first << size;
            for (int i = 0; i < size; i++) {
                packet << labelled_input.second[i];
            }
            server.send_packet(packet);
            return server;
        }

        friend Server& operator >> (Server& server, GameSimulatorSnap& snap) {
            sf::Packet packet;
            server.recieve_packet(packet);
            if (server.getLastStatus() == sf::Socket::Done) {
                packet >> snap;
            }
            return server;
        }

        void connect_to(const std::string& ip, int port) {
            while ((last_status = socket.connect(ip, port, sf::seconds(5))) == sf::Socket::Partial) {
                std::cout << "try to connect: " << ip << ":" << port << std::endl;
            }
        }

        void send_packet(sf::Packet& packet) {
            while ((last_status = socket.send(packet)) == sf::Socket::Partial) {
                std::cout << "partially sent" << std::endl;
            }
        }

        void recieve_packet(sf::Packet& packet) {
            while ((last_status = socket.receive(packet)) == sf::Socket::Partial) {
                std::cout << "paritally recieved" << std::endl;
            }
        }

        ID get_identity() const { return id; }
        sf::TcpSocket& get_socket() { return socket; }
        sf::Socket::Status getLastStatus() const { return last_status; }

    private:
        long long int id = 0;
        sf::TcpSocket socket;
        sf::Socket::Status last_status = sf::Socket::Status::NotReady;
    };
}

#endif // SERVER_HPP
