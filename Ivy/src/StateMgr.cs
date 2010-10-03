using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ivy
{
    public class StateMgr
    {
        Entity m_owner;     /// Entity that owns this state machine
                             
        State m_currentState;
        State m_previousState;

        public StateMgr(Entity owner)
        {
            m_owner = owner;
        }

        public void Initialize()
        {
            m_currentState = EntityStateStand.Get();
        }

        public void ChangeState(State nextState)
        {
            if (m_currentState != null)
            {
                m_currentState.Exit(m_owner);
                m_previousState = m_currentState;
                m_currentState = nextState;
                m_currentState.Enter(m_owner);
            }
        }

        public void Update()
        {
            m_currentState.Execute(m_owner);
        }

        public void HandleMessage(Message msg)
        {
            m_currentState.HandleMessage(m_owner, msg);
        }
    }
}
