using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace Ivy
{
    public class WorldZone : IMessageSender, IMessageReceiver
    {
        List<Entity> m_entities;

        Texture2D m_background;
        Rectangle m_bgRect;

        List<Rectangle> m_platforms;

        public Rectangle Bounds { get; private set; }

        public WorldZone(Rectangle bounds, Texture2D background, Rectangle bgRect)
        {
            Bounds = bounds;
            m_entities = new List<Entity>();

            m_background = background;
            m_bgRect = bgRect;

            // Add floor -- eventually will be removed
            Platform p = new Platform();
            p.Initialize();
            p.SetSize(bounds.Width, 1);
            m_entities.Add(p);
            p.ChangeZone(this, new Point(0, bounds.Height - 1));
        }

        public void Initialize()
        {
        }

        public void Update(GameTime gameTime)
        {
            foreach (Entity e in m_entities)
            {
                e.Update(gameTime);
            }

            // do collision detection on every entity vs world

            // do collision detection on every entity vs entity

            List<Entity> entityListA = new List<Entity>(m_entities);
            List<Entity> entityListB = new List<Entity>(m_entities);

            foreach (Entity a in entityListA)
            {
                entityListB.Remove(a);

                Point aPos = a.GetPositionAtTime(gameTime);

                foreach (Entity b in entityListB)
                {

                    Rectangle aRect = a.CollisionRect();
                    Rectangle bRect = b.CollisionRect();

                    //Console.WriteLine(aRect);
                    //Console.WriteLine(bRect);
                    if (aRect.Intersects(bRect))
                    {
                        MessageDispatcher.Get().SendMessage(new EntityCollisionMsg(this, a, b));
                        MessageDispatcher.Get().SendMessage(new EntityCollisionMsg(this, b, a));
                    }
                }

                a.Position = aPos;
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

        public virtual void ReceiveMessage(Message msg)
        {
            if (msg.Type == MessageType.ChangeZone)
            {
                ChangeZoneMsg czMsg = (ChangeZoneMsg)msg;
                Entity entity = czMsg.Entity;
                if (czMsg.DestZone == this)
                {
                    if (m_entities.Contains(entity) == false)
                    {
                        m_entities.Add(entity);
                        entity.ChangeZone(this, czMsg.DestPosition);
                    }
                }
                else
                {
                    if (m_entities.Contains(entity) == true)
                    {
                        m_entities.Remove(entity);
                    }
                }
            }
        }
    }
}
