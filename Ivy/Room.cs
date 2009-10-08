using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace Ivy
{
    public class Room
    {
        World m_world;
        List<Entity> m_entities;

        Texture2D m_background;
        Rectangle m_bgRect;

        public Rectangle Bounds { get; private set; }

        public Room(World world, Rectangle bounds, Texture2D background, Rectangle bgRect)
        {
            m_world = world;
            Bounds = bounds;
            m_entities = new List<Entity>();

            m_background = background;
            m_bgRect = bgRect;
        }

        public void Initialize()
        {
        }

        public void Load()
        {

        }

        public void Unload()
        {

        }

        public void AddEntity(Entity entity)
        {
            if (m_entities.Contains(entity) == false)
            {
                m_entities.Add(entity);
            }
        }

        public void RemoveEntity(Entity entity)
        {
            if (m_entities.Contains(entity) == true)
            {
                m_entities.Remove(entity);
            }
        }

        public void Update(GameTime gameTime)
        {
            foreach (Entity e in m_entities)
            {
                e.Update(gameTime);
            }

            // update background based on player position (distance moved this timestep, curr position)
        }

        public void Draw(SpriteBatch spriteBatch)
        {
            // Draw Background
            Rectangle cameraRect = IvyGame.Get().Camera.CameraRect;     // camera rect in world space

            Rectangle srcRect = new Rectangle(m_bgRect.Left + cameraRect.X, m_bgRect.Top + cameraRect.Y, cameraRect.Width, cameraRect.Height);
            Rectangle dstRect = new Rectangle(0, 0, 800, 600);
            spriteBatch.Draw(m_background, dstRect, srcRect, Color.White);

            foreach (Entity e in m_entities)
            {
                e.Draw(spriteBatch);
            }
        }

        public void Draw3D()
        {
            foreach (Entity e in m_entities)
            {
                e.Draw3D();
            }
        }
    }
}
