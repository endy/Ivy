﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ivy
{
    class EntityStateWait : State
    {
        static EntityStateWait m_instance = null;

        private EntityStateWait() :
            base()
        {

        }

        static public EntityStateWait Get()
        {
            if (m_instance == null)
            {
                m_instance = new EntityStateWait();
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
