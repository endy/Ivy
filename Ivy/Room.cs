using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace Ivy
{
    public class Room : IMessageSender
    {
        World m_world;
        List<Entity> m_entities;

        Texture2D m_background;
        Rectangle m_bgRect;

        List<Rectangle> m_platforms;

        public Rectangle Bounds { get; private set; }

        private Box m_box;

        public Room(World world, Rectangle bounds, Texture2D background, Rectangle bgRect)
        {
            m_world = world;
            Bounds = bounds;
            m_entities = new List<Entity>();

            m_background = background;
            m_bgRect = bgRect;

            m_platforms = new List<Rectangle>();


            m_platforms.Add(new Rectangle(2, bounds.Height-1, bounds.Width-4, 1));

            m_platforms.Add(new Rectangle(181 - bgRect.X, 446 - bgRect.Y, 64, 16));
            m_platforms.Add(new Rectangle(53 - bgRect.X, 462 - bgRect.Y, 64, 16));
            m_platforms.Add(new Rectangle(117 - bgRect.X, 558 - bgRect.Y, 64, 16));
            m_platforms.Add(new Rectangle(53 - bgRect.X, 654 - bgRect.Y, 80, 16));

            m_box = new Box(IvyGame.Get());
            m_box.Initialize();
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

            // do collision detection on every entity vs world

            // do collision detection on every entity vs entity
            foreach (Entity e in m_entities)
            {
                foreach (Rectangle r in m_platforms)
                {
                    Rectangle cr = e.CollisionRect();
                    if (r.Intersects(cr) == true)
                    {
                        Message msg = new Message(MessageType.CollideWithRoom, this, e);
                        MessageDispatcher.Get().SendMessage(msg);
                        break;
                    }
                }
            }

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

            foreach (Rectangle r in m_platforms)
            {

                Rectangle newRect = new Rectangle(0, 0, (int)(r.Width/256f * 800f), (int)(r.Height/ 192f * 600f));
                m_box.UpdateRect(newRect);
                m_box.Draw(r.Location);
            }
        }
    }
}
