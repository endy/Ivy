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

        private bool FindIntersectTime(Rectangle aRect, Rectangle bRect, 
                                Vector2 aVec, Vector2 bVec,
                                ref float tFirst, ref float tLast)
        {
            tFirst = 0.0f;
            tLast = 1.0f;

            Vector2 v = bVec - aVec;

            // X
            if (v.X < 0.0f)
            {
                if (bRect.Right < aRect.Left) { return false; }     // Moving apart

                if (aRect.Right < bRect.Left) { tFirst = Math.Max((aRect.Right - bRect.Left) / v.X, tFirst); }
                if (bRect.Right > aRect.Left) { tLast = Math.Min((aRect.Left - bRect.Right) / v.X, tLast); }
            }
            if (v.X > 0.0f)
            {
                if (bRect.Left > aRect.Right) { return false; }     // Moving apart

                if (bRect.Right < aRect.Left) { tFirst = Math.Max((aRect.Left - bRect.Right) / v.X, tFirst); }
                if (aRect.Right > bRect.Left) { tLast = Math.Min((aRect.Right - bRect.Left) / v.X, tLast); }
            }

            if (tFirst > tLast)
            {
                return false;
            }

            // Y
            if (v.Y < 0.0f)
            {
                if (bRect.Bottom < aRect.Top) { return false; }     // Moving apart

                if (aRect.Bottom < bRect.Top) { tFirst = Math.Max((aRect.Bottom - bRect.Top) / v.Y, tFirst); }
                if (bRect.Bottom > aRect.Top) { tLast = Math.Min((aRect.Top - bRect.Bottom) / v.Y, tLast); }
            }
            if (v.Y > 0.0f)
            {
                if (bRect.Top > aRect.Bottom) { return false; }     // Moving apart

                if (bRect.Bottom < aRect.Top) { tFirst = Math.Max((aRect.Top - bRect.Bottom) / v.Y, tFirst); }
                if (aRect.Bottom > bRect.Top) { tLast = Math.Min((aRect.Bottom - bRect.Top) / v.Y, tLast); }
            }

            if (tFirst > tLast)
            {
                return false;
            }

            return true;
        }

        public void Update(GameTime gameTime)
        {
            // update entity positions
            foreach (Entity e in m_entities)
            {
                int elapsedTimeMS = gameTime.ElapsedGameTime.Milliseconds;
                e.Position = e.GetPositionAtTime(elapsedTimeMS);
            }

            // Update entity positions

            List<Entity> entityListA = new List<Entity>(m_entities);
            List<Entity> entityListB = new List<Entity>(m_entities);

            foreach (Entity a in entityListA)
            {
                if (a.Moving == false)
                {
                    continue;
                }

                entityListB.Remove(a);

                foreach (Entity b in entityListB)
                {
                    Rectangle aRect = a.CollisionRect();
                    Rectangle bRect = b.CollisionRect();

                    if (aRect.Intersects(bRect))
                    {
                        MessageDispatcher.Get().SendMessage(new EntityCollisionMsg(this, a, b));
                        MessageDispatcher.Get().SendMessage(new EntityCollisionMsg(this, b, a));
                    }
                }
            }

            // Update misc zone resources (background positions, etc)

            // Update entity states
            foreach (Entity e in m_entities)
            {
                e.Update(gameTime);
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
