using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace Ivy
{
    class AnimatedSprite
    {
        private Texture2D m_spriteMap;

        private Rectangle m_animRect;

        private int m_frameCount = 0;
        private float framesPerSecond = 16.0f;

        private float timePerFrameMS;
        private float totalElapsedMS;

        private int currFrame = 0;

        private int m_frameWidth;
        private Point m_center;

        public AnimatedSprite(Texture2D spriteMap, int frameCount, Rectangle animRect)
        {
            m_spriteMap = spriteMap;
            m_frameCount = frameCount;
            m_animRect = animRect;

            timePerFrameMS = ((1.0f / framesPerSecond) * 1000.0f);

            m_frameWidth = m_animRect.Width / m_frameCount;
            m_center = new Point(m_frameWidth / 2, m_animRect.Height / 2);

        }

        public Point Center
        {
            get { return Point.Zero; }
        }

        public void Update(float elapsedTimeMS)
        {
            totalElapsedMS += elapsedTimeMS;

            if (totalElapsedMS > timePerFrameMS)
            {
                currFrame = ((currFrame + 1) % m_frameCount);


                totalElapsedMS = 0;
            }
        }

        

        public void Draw(SpriteBatch spriteBatch, Point pos)
        {
            Rectangle srcRect = new Rectangle(m_animRect.X + (m_frameWidth * currFrame),
                                              m_animRect.Y,
                                              m_frameWidth,
                                              m_animRect.Height);
            Rectangle dstRect = new Rectangle((int)pos.X, (int)pos.Y, (int)(srcRect.Width * 2.3f), (int)(srcRect.Height *2.3f));
            spriteBatch.Draw(m_spriteMap, dstRect, srcRect, Color.White);

        }
    }
}
