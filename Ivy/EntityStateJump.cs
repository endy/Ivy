using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ivy
{
    class EntityStateJump : State
    {
        static EntityStateJump m_instance = null;

        private EntityStateJump() :
            base()
        {

        }

        static public EntityStateJump Get()
        {
            if (m_instance == null)
            {
                m_instance = new EntityStateJump();
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
