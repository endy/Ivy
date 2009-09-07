using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ivy
{
    class EntityStateFall : State
    {
        static EntityStateFall m_instance = null;

        private EntityStateFall() :
            base()
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

        public override void Enter(Entity e)
        {
            throw new NotImplementedException();
        }

        public override void Execute(Entity e)
        {
            throw new NotImplementedException();
        }

        public override void Exit(Entity e)
        {
            throw new NotImplementedException();
        }
    }
}
