using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace Ivy
{
    public class World
    {
        private WorldZone m_currentZone;

        public World()
        {

        }

        public WorldZone GetCurrentZone()
        {
            return m_currentZone;
        }

        public void SetCurrentRoom(WorldZone room)
        {
            if (m_currentZone != null)
            {
                m_currentZone.Unload();
            }

            m_currentZone = room;
            m_currentZone.Load();
        }

        public void Update(GameTime gameTime)
        {
            // if multiple rooms are active in a game, they should all be updated here
            if (m_currentZone != null)
            {
                m_currentZone.Update(gameTime);
            }
        }

        public void Draw(SpriteBatch spriteBatch)
        {
            // if multiple rooms are active, only the current one is drawn
            if (m_currentZone != null)
            {
                m_currentZone.Draw(spriteBatch);
            }
        }

        public void Draw3D()
        {
            if (m_currentZone != null)
            {
                m_currentZone.Draw3D();
            }
        }
    }
}
