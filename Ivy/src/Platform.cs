using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;

namespace Ivy
{
    public class Platform : Entity
    {
        public Platform()
            : base(IvyGame.Get())
        {

        }

        public override void Initialize()
        {
            base.Initialize();

            m_box = new Box(IvyGame.Get());
            m_box.Initialize();
        }

        public void SetSize(int width, int height)
        {
            m_StaticCollisionRect = new Rectangle(0, 0, width, height);
        }

        public override void Draw3D()
        {
            Rectangle r = m_StaticCollisionRect;
            Rectangle newRect = new Rectangle(0, 0, (int)(r.Width/256f * 800f), (int)(r.Height/ 192f * 600f));
            m_box.UpdateRect(newRect);
            m_box.Draw(Position);
        }

        public override void ReceiveMessage(Message msg)
        {
            if (msg.Type == MessageType.CollideWithEntity)
            {
                EntityCollisionMsg entMsg = (EntityCollisionMsg)msg;

                Entity e = entMsg.EntityHit;

                int snapDeltaY = 10;

                // Push entity in opposite direction of motion
                Point snap = new Point(0, 0);

                int snapDownDy = Math.Abs(e.CollisionRect().Top - CollisionRect().Bottom);
                if (snapDownDy < snapDeltaY)
                {
                    // snap down
                    snap.Y = snapDownDy;
                }

                int snapUpDy = Math.Abs(e.CollisionRect().Bottom - CollisionRect().Top);
                if (snapUpDy < snapDeltaY)
                {
                    // snap up
                    snap.Y = -snapUpDy;
                }

                int snapDeltaX = 5;

                int snapRightDx = Math.Abs(e.CollisionRect().Left - CollisionRect().Right);
                if (snapRightDx < snapDeltaX)
                {
                    // snap right
                    snap.X = snapRightDx;
                }

                
                int snapLeftDx = Math.Abs(e.CollisionRect().Right - CollisionRect().Left);
                if (snapLeftDx < snapDeltaX)
                {
                    // snap left
                    snap.X = -snapLeftDx;
                }

                e.Position = new Point(e.Position.X + snap.X, e.Position.Y + snap.Y);

                Message newMsg = new Message(MessageType.CollideWithEnv, this, e);
                MessageDispatcher.Get().SendMessage(newMsg);
            }
            else
            {
                base.ReceiveMessage(msg);
            }
        }
    }
}
