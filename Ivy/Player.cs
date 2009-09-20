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
    public class Player : Entity
    {
        Weapon m_armCannon;

        AnimTree m_animTree;
       
        // Sound Effects
        SoundEffect m_rollEffect;
        SoundEffectInstance m_rollInstance;
        SoundEffect m_landEffect;

        public Player(IvyGame game)
            : base(game)
        {

        }

        public override void Initialize()
        {
            m_animTree = new AnimTree(this);
            m_animTree.Initialize();
            
            m_armCannon = new Weapon(IvyGame.Get(), this, new Point(10, 10), Direction);
            m_armCannon.Initialize();

            m_landEffect = Game.Content.Load<SoundEffect>("Audio\\samus_land");
            m_rollEffect = Game.Content.Load<SoundEffect>("Audio\\samus_jump_roll");
            m_rollInstance = m_rollEffect.CreateInstance();

            base.Initialize();
        }

        /// <summary>
        /// Allows the game component to update itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        public override void Update(GameTime gameTime)
        {
            // update position, bounded by world

            m_animTree.Update(gameTime);

            m_armCannon.Update(gameTime);

            base.Update(gameTime);
        }

        public void Draw(SpriteBatch spriteBatch)
        {
            Rectangle cameraRect = IvyGame.Get().Camera.CameraRect;

            Point samusScreenPos;
            samusScreenPos.X = (int)(((Position.X - cameraRect.X) / (float)cameraRect.Width) * 800); // screen width = 800
            samusScreenPos.Y = (int)(((Position.Y - cameraRect.Y) / (float)cameraRect.Height) * 600); // screen height = 600

            m_armCannon.Draw(spriteBatch);

            m_animTree.Draw(spriteBatch);
        }

        public override void ReceiveMessage(Message msg)
        {
            switch (msg.Type)
            {
                case MessageType.FireWeapon:
                    FireWeapon();
                    break;
                default:
                    base.ReceiveMessage(msg);
                    break;
            }
        }
       
        private void FireWeapon()
        {
            m_armCannon.Fire();
        }        
    }
}