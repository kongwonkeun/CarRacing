#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

#include <memory>
#include <stack>
#include "State.hpp"

namespace cp
{
    typedef std::unique_ptr<State> StateRef;

    class StateMachine
    {
    public:
        StateMachine();
        ~StateMachine();
        void add_state(StateRef new_state, bool is_replacing = true);
        void remove_state();
        void process_state_change();
        StateRef &get_active_state();

    private:
        std::stack<StateRef> stack_state;
        StateRef new_state;
        bool is_adding;
        bool is_replacing;
        bool is_removing;
    };
}

#endif // STATEMACHINE_HPP