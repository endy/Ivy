using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ivy
{
    abstract class State
    {
        public State()
        {

        }

        public abstract void Enter(Entity e);
        public abstract void Execute(Entity e);
        public abstract void Exit(Entity e);
    }
}
