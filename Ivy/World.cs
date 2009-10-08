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
        private Room m_currentRoom;

        private List<Player> m_players;

        public World()
        {           
            m_players = new List<Player>();
        }

        public Room GetCurrentRoom()
        {
            return m_currentRoom;
        }

        public void SetCurrentRoom(Room room)
        {
            if (m_currentRoom != null)
            {
                foreach (Player p in m_players)
                {
                    m_currentRoom.RemoveEntity(p);
                }

                m_currentRoom.Unload();
            }

            m_currentRoom = room;
            m_currentRoom.Load();

            foreach (Player p in m_players)
            {
                m_currentRoom.AddEntity(p);
            }
        }

        public void AddPlayer(Player player)
        {
            if (m_players.Contains(player) == false)
            {
                m_players.Add(player);

                if (m_currentRoom != null)
                {
                    m_currentRoom.AddEntity(player);
                }
            }
        }

        public void RemovePlayer(Player player)
        {
            if (m_players.Contains(player) == true)
            {
                m_players.Remove(player);

                if (m_currentRoom != null)
                {
                    m_currentRoom.RemoveEntity(player);
                }
            }
        }

        public void Update(GameTime gameTime)
        {
            // if multiple rooms are active in a game, they should all be updated here
            if (m_currentRoom != null)
            {
                m_currentRoom.Update(gameTime);
            }
        }

        public void Draw(SpriteBatch spriteBatch)
        {
            // if multiple rooms are active, only the current one is drawn
            if (m_currentRoom != null)
            {
                m_currentRoom.Draw(spriteBatch);
            }
        }

        public void Draw3D()
        {
            if (m_currentRoom != null)
            {
                m_currentRoom.Draw3D();
            }
        }
    }
}
