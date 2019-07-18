#ifndef SERVER_STATE_HPP
#define SERVER_STATE_HPP

#include "States/State.hpp"
#include "Game.hpp"
#include "States/GameSimulator.hpp"
#include "Network/Client.hpp"
#include <map>
#include <vector>
#include <fstream>
#include <set>

namespace cp
{
    class ServerState : public State
    {
    public:
        using TcpClient_ptr = std::shared_ptr<Client>;

        ServerState(GameDataRef _data, std::set<TcpClient_ptr> clients);
        ~ServerState();
        virtual void init();
        virtual void handle_input(float delta);
        virtual void update(float delta);
        virtual void draw(float delta);
        virtual void pause() {}
        virtual void resume() {}

    private:
        void collect_network_inputs();
        void use_collected_inputs();
        void generate_outputs();
        void use_generated_outputs();

        GameDataRef game_data;
        GameSimulator simulator;
        GameSimulatorSnap temp_snap;
        std::vector<Client::key_input_type> inputs;
        std::set<TcpClient_ptr> clients;
        bool update_required = true;
    };
}

#endif // SERVER_STATE_HPP
