#ifndef NETWORKING_HPP
#define NETWORKING_HPP

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include <iostream>
#include <unistd.h>

namespace cp
{
    class NetworkManager
    {
    public:
        NetworkManager();
        //~NetworkManager();
        static void createServer();
        static void createClient();
        static void run(int type);
        static void sendData(sf::Vector3f pos);
        static void send(sf::Vector3f pos);

        static sf::TcpSocket client;
        sf::Socket::Status s_status;
        sf::Socket::Status c_status;
        std::thread n_thread;
    };
}

#endif // NETWORKING_HPP
