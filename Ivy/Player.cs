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
    public class Player : Microsoft.Xna.Framework.GameComponent
    {
        IvyGame m_game;

        Rectangle m_worldBounds;

        Texture2D samusMap;
        AnimatedSprite samusRunRight;
        AnimatedSprite samusRunLeft;
        AnimatedSprite samusJumpRight;
        AnimatedSprite samusJumpLeft;
        Point samusPos;
        Vector2 samusDir;
        bool jump;


        public Player(IvyGame game, Rectangle worldBounds)
            : base(game)
        {
            // TODO: Construct any child components here
            m_game = game;
            m_worldBounds = worldBounds;
        }

        /// <summary>
        /// Allows the game component to perform any initialization it needs to before starting
        /// to run.  This is where it can query for any required services and load content.
        /// </summary>
        public override void Initialize()
        {
            // TODO: Add your initialization code here

            samusPos = new Point(m_worldBounds.X + (m_worldBounds.Width / 2),
                                 m_worldBounds.Y + (m_worldBounds.Height / 2));
            samusDir = Vector2.Zero;
            jump = false;

            samusMap = Game.Content.Load<Texture2D>("Sprites\\samusMap");

            Rectangle samusRunRightRect = new Rectangle(0, 70, 460, 44);
            Rectangle samusRunLeftRect = new Rectangle(0, 114, 460, 44);

            Rectangle samusJumpRightRect = new Rectangle(0, 0, 280, 35);
            Rectangle samusJumpLeftRect = new Rectangle(0, 35, 280, 35);

            samusRunRight = new AnimatedSprite(samusMap, 10, samusRunRightRect);
            samusRunLeft = new AnimatedSprite(samusMap, 10, samusRunLeftRect);
            samusJumpRight = new AnimatedSprite(samusMap, 8, samusJumpRightRect);
            samusJumpLeft = new AnimatedSprite(samusMap, 8, samusJumpLeftRect);


            base.Initialize();
        }

        /// <summary>
        /// Allows the game component to update itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        public override void Update(GameTime gameTime)
        {
            // TODO: Add your update code here
            // Get Player Input
            samusDir = Vector2.Zero;

            GamePadState gamePadState = GamePad.GetState(PlayerIndex.One);

            float deadZone = 0.1f;

            if (gamePadState.ThumbSticks.Left.X > deadZone)
            {
                samusDir.X = 1.0f;
            }
            else if (gamePadState.ThumbSticks.Left.X < -deadZone)
            {
                samusDir.X = -1.0f;
            }
            if (gamePadState.ThumbSticks.Left.Y < -deadZone)
            {
                samusDir.Y = 1.0f;
            }
            else if (gamePadState.ThumbSticks.Left.Y > deadZone)
            {
                samusDir.Y = -1.0f;
            }

            jump = false;
            if (gamePadState.Buttons.A == ButtonState.Pressed)
            {
                jump = true;
            }

#if !XBOX
            /*
            KeyboardState keyboardState = Keyboard.GetState();
            if (keyboardState.IsKeyDown(Keys.Left))
            {
                samusDir.X -= 1;
            }
            */
#endif
            // Update player character position & clamp movement to world bounds
            Point newPos = samusPos;
            newPos.X += (int)samusDir.X;
            newPos.Y += (int)samusDir.Y;
            if (newPos.X < m_worldBounds.X)
            {
                newPos.X = m_worldBounds.X;
            }
            if ((m_worldBounds.X + m_worldBounds.Width) < newPos.X)
            {
                newPos.X = m_worldBounds.X + m_worldBounds.Width;
            }
            if (newPos.Y < m_worldBounds.Y)
            {
                newPos.Y = m_worldBounds.Y;
            }
            if ((m_worldBounds.Y + m_worldBounds.Height) < newPos.Y)
            {
                newPos.Y = m_worldBounds.Y + m_worldBounds.Height;
            }

            samusPos = newPos;

            // Update animations
            float elapsedTimeMS = gameTime.ElapsedGameTime.Milliseconds;

            if (jump)
            {
                // Samus Jump
                samusJumpLeft.Update(elapsedTimeMS);
                samusJumpRight.Update(elapsedTimeMS);
            }
            else if ((samusDir.X * samusDir.X) > 0.001)
            {
                samusRunLeft.Update(elapsedTimeMS);
                samusRunRight.Update(elapsedTimeMS);
            }          


            base.Update(gameTime);
        }

        public void Draw(SpriteBatch spriteBatch)
        {
            bool right = (samusDir.X > 0) ? true : false;

            Rectangle cameraRect = m_game.Camera.CameraRect;

            Point samusScreenPos;
            samusScreenPos.X = (int)(((samusPos.X - cameraRect.X) / (float)cameraRect.Width) * 800); // screen width = 800
            samusScreenPos.Y = (int)(((samusPos.Y - cameraRect.Y) / (float)cameraRect.Height) * 600); // screen height = 600

            if (jump)
            {
                if (right)
                {
                    samusJumpRight.Draw(spriteBatch, samusScreenPos);
                }
                else
                {
                    samusJumpLeft.Draw(spriteBatch, samusScreenPos);
                }
            }
            else
            {
                if (right)
                {
                    samusRunRight.Draw(spriteBatch, samusScreenPos);
                }
                else
                {
                    samusRunLeft.Draw(spriteBatch, samusScreenPos);
                }
            }
        }

        public Point PlayerPos
        {
            get { return samusPos; }
        }
    }
}