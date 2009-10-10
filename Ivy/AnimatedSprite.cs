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

        // Animation Properties
        public Vector2 Scale { get; set; }              // amount to scale the animation

        public bool Playing { get; private set; }       // true if animation is playing
        public bool Loop { get; set; }                  // loop animation
        public uint CurrentFrame { get; private set; }  // current frame

        private Rectangle m_frameRect;                  // dimensions of a single frame
        private bool m_reverse;                         // play frames in reverse order

        // Private Animation Data
        private uint m_frameCount;
        private float m_framesPerSecond;
        private float m_timePerFrame;       // milliseconds per frame
        private float m_timeElapsed;        // time elapsed in milliseconds

        public string Name { get; set; }    // Anim name   // TODO: remove

        public AnimatedSprite(
            IvyGame game, 
            Texture2D texture, 
            Rectangle animRect, 
            uint frameCount,
            float framesPerSecond) 
            : 
            base(game)
        {
            m_game = game;
            
            m_texture = texture;
            m_animRect = animRect;
            m_frameCount = frameCount;
            m_framesPerSecond = framesPerSecond;

            Playing = false;
            Loop = true;
            m_reverse = false;

            Scale = new Vector2(1.0f, 1.0f);

            if (m_frameCount != 0)
            {
                m_frameRect = new Rectangle(0, 0, m_animRect.Width / (int)m_frameCount, m_animRect.Height);
            }
            else
            {
                m_frameRect = Rectangle.Empty;
            }

            m_timePerFrame = ((1.0f / m_framesPerSecond) * 1000.0f);

            Name = "Animation";
        }

        public override void Initialize()
        {
            // TODO: refactor code into initialize method
        }
     
        public override void Update(GameTime gameTime)
        {
            if (Playing)
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
            Rectangle srcRect = new Rectangle((m_animRect.X + (m_frameRect.Width * (int)CurrentFrame)),
                                              m_animRect.Y,
                                              m_frameRect.Width,
                                              m_animRect.Height);

            Rectangle dstRect = new Rectangle(pos.X, 
                                              pos.Y,
                                              (int)(srcRect.Width / 256f * 800f * Scale.X),
                                              (int)(srcRect.Height / 192f * 600f * Scale.Y));

            spriteBatch.Draw(m_texture, dstRect, srcRect, Color.White);
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

        public Rectangle GetFrameBounds()
        {
            return new Rectangle(0,
                                 0, 
                                (int)(m_frameRect.Width * Scale.X),
                                (int)(m_frameRect.Height * Scale.Y));
        }

        public void SetFrame(uint frameNumber)
        {
            if (frameNumber < m_frameCount)
            {
                CurrentFrame = frameNumber;
            }
        }

        public void Play()
        {
            Playing = true;
        }
        public void Stop()
        {
            Playing = false;
        }
        public void Reset()
        {
            CurrentFrame = (Reverse) ? (m_frameCount - 1) : 0;
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

            if (((Reverse == true) && (CurrentFrame == 0)) ||
                ((Reverse != true) && ((CurrentFrame + 1) == m_frameCount)))
            {
                lastFrame = true;
            }

            return lastFrame;
        }

        private void NextFrame()
        {
            if (Reverse == true)
            {
                CurrentFrame--;
                if (CurrentFrame < 0)
                {
                    CurrentFrame = m_frameCount - 1;
                }
            }
            else
            {
                CurrentFrame++;
                if (CurrentFrame >= m_frameCount)
                {
                    CurrentFrame = 0;
                }
            }
        }
    }
}
