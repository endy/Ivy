using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace Ivy
{
    public class AnimatedSprite : Microsoft.Xna.Framework.GameComponent
    {
        public delegate bool AnimatedSpriteEvent(AnimatedSprite anim);

        public AnimatedSpriteEvent OnAnimEnd;

        // Component Data
        IvyGame m_game;

        // Sprite Properties
        private Texture2D m_texture;
        private Rectangle m_animRect;
        private uint m_frameCount;
        private float m_framesPerSecond;

        private Vector2 m_scale;             // amount to scale the animation

        private uint m_frameWidth;
        private Point m_frameCenter;

        // Animation Properties
        private bool m_playing;             // true if animation is playing
        private bool m_loop;                // loop animation
        private bool m_reverse;             // play frames in reverse order
        private uint m_currentFrame;        // current frame

        // Private Animation Data
        private float m_timePerFrame;       // milliseconds per frame
        private float m_timeElapsed;        // time elapsed in milliseconds

        public AnimatedSprite(IvyGame game, Texture2D texture, Rectangle animRect, uint frameCount,
                              float framesPerSecond) 
                              : 
                              base(game)
        {
            m_game = game;
            
            m_texture = texture;
            m_animRect = animRect;
            m_frameCount = frameCount;
            m_framesPerSecond = framesPerSecond;

            m_playing = false;
            m_loop = true;
            m_reverse = false;

            m_scale = new Vector2(1.0f, 1.0f);
        }

        public override void Initialize()
        {
            if (m_frameCount != 0)
            {
                m_frameWidth = (uint)m_animRect.Width / m_frameCount;
                m_frameCenter = new Point((int)m_frameWidth / 2, m_animRect.Height / 2);
            }

            m_timePerFrame = ((1.0f / m_framesPerSecond) * 1000.0f);
        }

        #region Properties
        public Vector2 Scale
        {
            get { return m_scale; }
            set { m_scale = value; }
        }
        public Point Center
        {
            set { m_frameCenter = value; }
            get { return m_frameCenter; }
        }
        public bool IsPlaying
        {
            get { return m_playing; }
        }
        public bool Loop
        {
            get { return m_loop; }
            set { m_loop = value; }
        }
        public bool Reverse
        {
            get { return m_reverse; }
            set
            {
                if (m_reverse != value)
                {
                    m_reverse = value;
                }
                else
                {
                    m_reverse = value;
                }
                Reset();
            }
        }
        public uint CurrentFrame
        {
            get { return m_currentFrame; }
        }
        #endregion

        public void SetFrame(uint frameNumber)
        {
            if (frameNumber < m_frameCount)
            {
                m_currentFrame = frameNumber;
            }
        }

        public void Play()
        {
            m_playing = true;
        }
        public void Stop()
        {
            m_playing = false;
        }
        public void Reset()
        {
            m_currentFrame = (Reverse) ? (m_frameCount - 1) : 0;
            m_timeElapsed = 0;
        }
        public void Replay()
        {
            Reset();
            Play();
        }

        private bool IsLastFrame()
        {
            bool lastFrame = false;

            if (((Reverse == true) && (m_currentFrame == 0)) ||
                ((Reverse != true) && ((m_currentFrame + 1) == m_frameCount)))
            {
                lastFrame = true;
            }

            return lastFrame;
        }

        private void NextFrame()
        {
            if (Reverse == true)
            {
                m_currentFrame--;
                if (m_currentFrame < 0)
                {
                    m_currentFrame = m_frameCount - 1;
                }
            }
            else
            {
                m_currentFrame++;
                if (m_currentFrame >= m_frameCount)
                {
                    m_currentFrame = 0;
                }
            }
        }

        public override void Update(GameTime gameTime)
        {
            if (IsPlaying)
            {
                m_timeElapsed += gameTime.ElapsedGameTime.Milliseconds;

                if (m_timeElapsed > m_timePerFrame)
                {
                    if (IsLastFrame() == false)
                    {
                        NextFrame();                       
                        m_timeElapsed = 0;
                    }
                    else 
                    {
                        if (Loop == true)
                        {
                            NextFrame();
                            m_timeElapsed = 0;
                        }
                        else
                        {
                            Stop();
                            if (OnAnimEnd != null)
                            {
                                OnAnimEnd(this);
                            }
                        }
                    }
                }
            }
        }

        public void Draw(SpriteBatch spriteBatch, Point pos)
        {
            Rectangle srcRect = new Rectangle((int)(m_animRect.X + (m_frameWidth * m_currentFrame)),
                                              m_animRect.Y,
                                              (int)m_frameWidth,
                                              m_animRect.Height);

            Rectangle dstRect = new Rectangle(Center.X + pos.X, Center.Y + pos.Y,
                                              (int)(srcRect.Width * Scale.X),
                                              (int)(srcRect.Height * Scale.Y));

            spriteBatch.Draw(m_texture, dstRect, srcRect, Color.White);
        }
    }
}
