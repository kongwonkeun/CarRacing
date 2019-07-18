/**
 * @file ClientState.hpp
 * @author Vijay
 * @brief Handles communication between the computer and the server.
 * @version 0.1
 * @date 2019-02-26
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef CLIENT_STATE_HPP
#define CLIENT_STATE_HPP

#include "States/State.hpp"
#include "Game.hpp"
#include "States/GameSimulator.hpp"
#include "Network/Server.hpp"

namespace cp 
{
    class ClientState : public State
    {
    public:
        using ID = int;
        using Server_ptr = std::shared_ptr<Server>;

        ClientState(GameDataRef _data, Server_ptr server, int unique_id);
        ~ClientState();
        virtual void init();
        virtual void handle_input(float delta);
        virtual void update(float delta);
        virtual void draw(float delta);
        virtual void pause() {}
        virtual void resume() {}

    private:
        void collect_inputs();
        void use_collected_inputs();
        void get_network_snap();
        void use_network_snap();

        GameDataRef game_data;
        GameSimulator simulator;
        GameSimulator::input_return_type input_to_send;
        Server_ptr server;
        GameSimulatorSnap snap;
        bool update_required = true;
        ID my_id;
    };
}

#endif // CLIENT_STATE_HPP
