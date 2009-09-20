using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ivy
{
    abstract public class State
    {
        protected State()
        {

        }

        public abstract void Enter(Entity entity);
        public abstract void Execute(Entity entity);
        public abstract void Exit(Entity entity);

        public abstract void HandleMessage(Entity entity, Message msg);
    }
}
