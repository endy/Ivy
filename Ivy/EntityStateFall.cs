﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ivy
{
    class EntityStateFall : State
    {
        private static EntityStateFall m_instance = null;

        private EntityStateFall()
        {

        }

        static public EntityStateFall Get()
        {
            if (m_instance == null)
            {
                m_instance = new EntityStateFall();
            }
            return m_instance;
        }

        public override void Enter(Entity entity)
        {
            entity.Moving = true;
        }

        public override void HandleMessage(Entity entity, Message msg)
        {
            switch (msg.Type)
            {
                case MessageType.Land:
                    entity.ChangeState(EntityStateStand.Get());
                    break;
                default:
                    // TODO: error!
                    break;
            }
        }
    }
}
