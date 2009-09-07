using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ivy
{
    class StateMachine
    {
        Entity m_owner;     /// Entity that owns this state machine
                             
        State m_currentState;

        public StateMachine(Entity owner)
        {
            m_owner = owner;
        }

        public void ChangeState(State nextState)
        {
            m_currentState.Exit(m_owner);
            m_currentState = nextState;
            m_currentState.Enter(m_owner);
        }
    }
}
