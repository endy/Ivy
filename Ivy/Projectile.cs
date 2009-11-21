using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;
using Microsoft.Xna.Framework.Net;
using Microsoft.Xna.Framework.Storage;


namespace Ivy
{
    /// <summary>
    /// This is a game component that implements IUpdateable.
    /// </summary>
    public class Projectile : Microsoft.Xna.Framework.GameComponent
    {
        private Point m_position;
        private Vector2 m_direction;
        private AnimatedSprite m_projectileAnim;
        private AnimatedSprite m_explosionAnim;

        private AnimatedSprite m_playingAnim;

        private int lifespan = 500;
        private int elapsedTime = 0;

        private bool m_alive = true;
        private bool m_dead = false;

        public Projectile(Game game, Weapon weapon, AnimatedSprite projectileAnim, AnimatedSprite explosionAnim)
            : base(game)
        {
            m_position = weapon.Position;
            m_direction = weapon.Direction;
            
            m_projectileAnim = projectileAnim;
            m_projectileAnim.OnAnimEnd += OnAnimEnd;

            m_explosionAnim = explosionAnim;
            m_explosionAnim.OnAnimEnd += OnAnimEnd;

            m_playingAnim = m_projectileAnim;
        }

        /// <summary>
        /// Allows the game component to perform any initialization it needs to before starting
        /// to run.  This is where it can query for any required services and load content.
        /// </summary>
        public override void Initialize()
        {
            m_playingAnim.Play();
            base.Initialize();
        }

        /// <summary>
        /// Allows the game component to update itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        public override void Update(GameTime gameTime)
        {
            elapsedTime += gameTime.ElapsedGameTime.Milliseconds;

            if (m_alive)
            {
                if (elapsedTime > lifespan)
                {
                    m_alive = false;
                    Explode();
                }
                else
                {
                    m_position.X += (int)m_direction.X * 3;
                    m_position.Y += (int)m_direction.Y * 3;
                }
            }

            m_playingAnim.Update(gameTime);

            base.Update(gameTime);
        }

        public void Draw(SpriteBatch spriteBatch)
        {
            Rectangle cameraRect = IvyGame.Get().Camera.CameraRect;

            Point projectilePos = new Point();
            projectilePos.X = (int)(((m_position.X - cameraRect.X) / (float)cameraRect.Width) * 800); // screen width = 800
            projectilePos.Y = (int)(((m_position.Y - cameraRect.Y) / (float)cameraRect.Height) * 600); // screen height = 600


            m_playingAnim.Draw(spriteBatch, projectilePos);
        }

        private void Explode()
        {
            m_projectileAnim.Stop();
            m_playingAnim = m_explosionAnim;
            m_playingAnim.Reset();
            m_playingAnim.Play();
        }

        public bool IsDead
        {
            get { return m_dead; }
        }

        public void OnAnimEnd(AnimatedSprite anim)
        {
            m_dead = true;
        }
    }
}
