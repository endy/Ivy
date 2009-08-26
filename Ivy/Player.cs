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
        AnimatedSprite samusJumpRollRight;
        AnimatedSprite samusJumpRollLeft;
        Point samusPos;
        Vector2 samusDir;
        bool jump;

        // Prototype Vars
        AnimatedSprite testAnim;
        bool playTestAnim;


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

            Rectangle samusRunRightRect = new Rectangle(0, 66, 460, 45);
            Rectangle samusRunLeftRect = new Rectangle(0, 111, 460, 45);

            Rectangle samusJumpRightRect = new Rectangle(0, 0, 280, 32);
            Rectangle samusJumpLeftRect = new Rectangle(0, 33, 280, 32);

            samusRunRight = new AnimatedSprite(m_game, samusMap, samusRunRightRect, 10, 10f);
            samusRunRight.Initialize();
            samusRunRight.Scale = new Vector2(3.0f, 3.0f);
            samusRunRight.Play();

            samusRunLeft = new AnimatedSprite(m_game, samusMap, samusRunLeftRect, 10, 10f);
            samusRunLeft.Initialize();
            samusRunLeft.Scale = new Vector2(3.0f, 3.0f);
            samusRunLeft.Play();

            samusJumpRollRight = new AnimatedSprite(m_game, samusMap, samusJumpRightRect, 8, 8f);
            samusJumpRollRight.Initialize();

            samusJumpRollLeft = new AnimatedSprite(m_game, samusMap, samusJumpLeftRect, 8, 8f);
            samusJumpRollLeft.Initialize();

            Rectangle samusTurn = new Rectangle(0, 156, 78, 47);
            Rectangle samusWaitRight = new Rectangle(0, 203, 140, 45);
            Rectangle samusWaitLeft = new Rectangle(0, 248, 140, 45);
            Rectangle samusJumpRight = new Rectangle(0, 293, 224, 47);
            Rectangle samusJumpLeft = new Rectangle(0, 340, 224, 47);

            AnimatedSprite turnAnim = new AnimatedSprite(m_game, samusMap, samusTurn, 3, 3f);
            turnAnim.Initialize();
            turnAnim.Scale = new Vector2(3.0f, 3.0f);
            turnAnim.Reverse = true;
            turnAnim.Play();

            AnimatedSprite samusJumpLeftAnim = new AnimatedSprite(m_game, samusMap, samusJumpLeft, 8, 8f);
            samusJumpLeftAnim.Initialize();
            samusJumpLeftAnim.Scale = new Vector2(3.0f, 3.0f);

            testAnim = turnAnim;  

            playTestAnim = false;

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
                samusDir.X = 2f;
            }
            else if (gamePadState.ThumbSticks.Left.X < -deadZone)
            {
                samusDir.X = -2f;
            }
            if (gamePadState.ThumbSticks.Left.Y < -deadZone)
            {
                samusDir.Y = 2f;
            }
            else if (gamePadState.ThumbSticks.Left.Y > deadZone)
            {
                samusDir.Y = -2f;
            }

            jump = false;
            if (gamePadState.Buttons.A == ButtonState.Pressed)
            {
                jump = true;
            }

            if (gamePadState.Buttons.B == ButtonState.Pressed)
            {
                playTestAnim = true;
            }
            else
            {
                playTestAnim = false;
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
            if (playTestAnim)
            {
                testAnim.Update(gameTime);
            }

            if (jump)
            {
                // Samus Jump
                samusJumpRollLeft.Update(gameTime);
                samusJumpRollRight.Update(gameTime);
            }
            else if ((samusDir.X * samusDir.X) > 0.001)
            {
                samusRunLeft.Update(gameTime);
                samusRunRight.Update(gameTime);
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
                    samusJumpRollRight.Draw(spriteBatch, samusScreenPos);
                }
                else
                {
                    samusJumpRollLeft.Draw(spriteBatch, samusScreenPos);
                }
            }
            else if (!playTestAnim)
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
            else
            {
                testAnim.Draw(spriteBatch, samusScreenPos);
            }

            
        }

        public Point PlayerPos
        {
            get { return samusPos; }
        }
    }
}