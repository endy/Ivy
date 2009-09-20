using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;

namespace Ivy
{
    public class World
    {
        private Rectangle m_bounds;

        public World(Rectangle bounds)
        {
            m_bounds = bounds;
        }

        public void Initialize()
        {

        }

        #region Properties

        public Rectangle Bounds
        {
            get { return m_bounds; }
        }

        #endregion
    }
}
