using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;

namespace Ivy
{
    public class Platform : Entity
    {
        private Box m_box;

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
    }
}
