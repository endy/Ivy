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
    /// This is the main type for your game
    /// </summary>
    public class IvyGame : Microsoft.Xna.Framework.Game,
                           IMessageSender
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        private SpriteFont consoleFont;
        private Vector2 consolePos;

        private World m_world;
        private Camera2D m_camera;

        private string m_consoleString;

        private static IvyGame m_instance = null;

        private Player m_playerOne;

        private IvyGame()
        {
            // Move this back to initialize
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
        }
        
        public static IvyGame Get()
        {
            if (m_instance == null)
            {
                m_instance = new IvyGame();
            }

            return m_instance;
        }       

        /// <summary>
        /// Allows the game to perform any initialization it needs to before starting to run.
        /// This is where it can query for any required services and load any non-graphic
        /// related content.  Calling base.Initialize will enumerate through any components
        /// and initialize them as well.
        /// </summary>
        protected override void Initialize()
        {
            // TODO: Find out if components need to be individually initialized here,
            //       XNA may provide a way of doing this.

            // Build World & Rooms
            m_world = new World();

            Rectangle roomBounds = new Rectangle(0, 0, 512, 192);
            Room mainRoom = new Room(m_world, roomBounds);
            mainRoom.Initialize();

            // Add Room Entities
            Skree skree = new Skree(this);
            skree.Initialize();
            skree.Position = new Point(100, 100);
            mainRoom.AddEntity(skree);

            skree = new Skree(this);
            skree.Initialize();
            skree.Position = new Point(300, 100);
            mainRoom.AddEntity(skree);

            m_world.SetCurrentRoom(mainRoom);

            // Add Player
            m_playerOne = new Player(this);
            m_playerOne.Initialize();
            m_world.AddPlayer(m_playerOne);

            // Create Camera
            int width = 192 * (800 / 600);
            Rectangle screenRect = new Rectangle(0, 0, width, 192);

            m_camera = new Camera2D(m_world.GetCurrentRoom().Bounds, screenRect);
    
            // Movement
            InputMgr.Get().RegisterGamePadButton(Buttons.LeftThumbstickLeft, OnGamePadDirectionEvent);
            InputMgr.Get().RegisterGamePadButton(Buttons.LeftThumbstickRight, OnGamePadDirectionEvent);
            InputMgr.Get().RegisterGamePadButton(Buttons.LeftThumbstickUp, OnGamePadDirectionEvent);
            InputMgr.Get().RegisterGamePadButton(Buttons.LeftThumbstickDown, OnGamePadDirectionEvent);

            InputMgr.Get().RegisterGamePadButton(Buttons.DPadLeft, OnGamePadDirectionEvent);
            InputMgr.Get().RegisterGamePadButton(Buttons.DPadRight, OnGamePadDirectionEvent);
            InputMgr.Get().RegisterGamePadButton(Buttons.DPadUp, OnGamePadDirectionEvent);
            InputMgr.Get().RegisterGamePadButton(Buttons.DPadDown, OnGamePadDirectionEvent);

            InputMgr.Get().RegisterKey(Keys.Up, OnKeyboardDirectionEvent);
            InputMgr.Get().RegisterKey(Keys.Down, OnKeyboardDirectionEvent);
            InputMgr.Get().RegisterKey(Keys.Left, OnKeyboardDirectionEvent);
            InputMgr.Get().RegisterKey(Keys.Right, OnKeyboardDirectionEvent);

            // Actions
            InputMgr.Get().RegisterGamePadButton(Buttons.A, OnGamePadButtonEvent); // Run/Alt
            InputMgr.Get().RegisterGamePadButton(Buttons.B, OnGamePadButtonEvent); // Jump
            InputMgr.Get().RegisterGamePadButton(Buttons.Y, OnGamePadButtonEvent); // Fire Weapon

            // Keyboard Actions
            InputMgr.Get().RegisterKey(Keys.F, OnKeyboardEvent);                    // Fire
            InputMgr.Get().RegisterKey(Keys.Space, OnKeyboardEvent);                // Jump

            // Debug Actions
            InputMgr.Get().RegisterGamePadButton(Buttons.X, DebugOnGamePadButtonEvent);

            ConsoleStr = "\n";

            // Initialize components
            base.Initialize();
        }

        /// <summary>
        /// LoadContent will be called once per game and is the place to load
        /// all of your content.
        /// </summary>
        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);
        
            consoleFont = Content.Load<SpriteFont>("Fonts\\Console");
            consolePos = new Vector2(0, 100);
        }

        /// <summary>
        /// UnloadContent will be called once per game and is the place to unload
        /// all content.
        /// </summary>
        protected override void UnloadContent()
        {
            // TODO: Unload any non ContentManager content here
        }

        /// <summary>
        /// Allows the game to run logic such as updating the world,
        /// checking for collisions, gathering input, and playing audio.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Update(GameTime gameTime)
        {
            //ConsoleStr = "\n\n"; 

            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            // Dispatch queued messages
            MessageDispatcher.Get().Update(gameTime);

            // Input
            InputMgr.Get().Update();

            // Update World
            m_world.Update(gameTime);

            // Update Camera based on Player Position
            m_camera.Update(m_playerOne.Position);

            // Update Sound FX?

            base.Update(gameTime);
        }

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            float fps = (1 / (float)gameTime.ElapsedGameTime.Milliseconds) * 1000;
            //ConsoleStr += "FPS: " + fps + "\n";

            GraphicsDevice.Clear(Color.Black);
    
            spriteBatch.Begin(SpriteBlendMode.AlphaBlend, SpriteSortMode.Immediate, SaveStateMode.None, Matrix.Identity);
            graphics.GraphicsDevice.SamplerStates[0].MagFilter = TextureFilter.Point;
            graphics.GraphicsDevice.SamplerStates[0].MinFilter = TextureFilter.Point;
            graphics.GraphicsDevice.SamplerStates[0].MipFilter = TextureFilter.Point;

            m_world.Draw(spriteBatch);

            // Draw Console           
            // Find the center of the string
            Vector2 FontOrigin = consoleFont.MeasureString(ConsoleStr) / 2;

            Vector2 drawConsolePos = new Vector2(consolePos.X + FontOrigin.X, consolePos.Y);
                        
            // Draw the string
            spriteBatch.DrawString(consoleFont, ConsoleStr, drawConsolePos, Color.LimeGreen,
                                   0, FontOrigin, 1.0f, SpriteEffects.None, 0.5f);

            spriteBatch.End();           

            base.Draw(gameTime);
        }

        #region Input Handlers

        bool OnGamePadDirectionEvent(GamePadButtonEvent e)
        {
            // TODO: design some way to map events to messages to avoid switch code like this
            if ((e.Button == Buttons.LeftThumbstickLeft) ||
                (e.Button == Buttons.DPadLeft))
            {
                if (e.EventType == InputEventType.Pressed)
                {
                    Message msg = new Message(MessageType.MoveLeft, this, m_playerOne);
                    MessageDispatcher.Get().SendMessage(msg);
                }
                else if ((e.EventType == InputEventType.Released) &&
                         (InputMgr.Get().GamePadState.IsButtonUp(Buttons.LeftThumbstickRight)) &&
                         (InputMgr.Get().GamePadState.IsButtonUp(Buttons.DPadRight)))
                {
                    // TODO: need a better way of handling mutually exclusive button inputs

                    Message msg = new Message(MessageType.Stand, this, m_playerOne);
                    MessageDispatcher.Get().SendMessage(msg);
                }
            }
            else if ((e.Button == Buttons.LeftThumbstickRight) ||
                     (e.Button == Buttons.DPadRight))
            {
                if (e.EventType == InputEventType.Pressed)
                {
                    Message msg = new Message(MessageType.MoveRight, this, m_playerOne);
                    MessageDispatcher.Get().SendMessage(msg);
                }
                else if ((e.EventType == InputEventType.Released) &&
                         (InputMgr.Get().GamePadState.IsButtonUp(Buttons.LeftThumbstickLeft)) &&
                         (InputMgr.Get().GamePadState.IsButtonUp(Buttons.DPadLeft)))
                {
                    // TODO: need a better way of handling mutually exclusive button inputs

                    Message msg = new Message(MessageType.Stand, this, m_playerOne);
                    MessageDispatcher.Get().SendMessage(msg);
                }
            }
            
            return true;
        }

        bool OnKeyboardDirectionEvent(KeyboardEvent e)
        {
            // TODO: design some way to map events to messages to avoid switch code like this

            // TODO:  add some intelligence here to make sure that left and right keys are mutually exclusive
            if (e.Key == Keys.Left)
            {
                if (e.EventType == InputEventType.Pressed)
                {
                    Message msg = new Message(MessageType.MoveLeft, this, m_playerOne);
                    MessageDispatcher.Get().SendMessage(msg);
                }
                else if (e.EventType == InputEventType.Released)
                {
                    Message msg = new Message(MessageType.Stand, this, m_playerOne);
                    MessageDispatcher.Get().SendMessage(msg);
                }
            }
            else if (e.Key == Keys.Right)
            {
                if (e.EventType == InputEventType.Pressed)
                {
                    Message msg = new Message(MessageType.MoveRight, this, m_playerOne);
                    MessageDispatcher.Get().SendMessage(msg);
                }
                else if (e.EventType == InputEventType.Released)
                {
                    Message msg = new Message(MessageType.Stand, this, m_playerOne);
                    MessageDispatcher.Get().SendMessage(msg);
                }
            }

            return true;
        }

        bool OnGamePadButtonEvent(GamePadButtonEvent e)
        {
            if (e.Button == Buttons.B)
            {
                if (e.EventType == InputEventType.Pressed)
                {
                    // start jump
                    Message msg = new Message(MessageType.Jump, this, m_playerOne);
                    MessageDispatcher.Get().SendMessage(msg);
                }
                else if (e.EventType == InputEventType.Released)
                {
                    // end jump!
                    Message msg = new Message(MessageType.Fall, this, m_playerOne);
                    MessageDispatcher.Get().SendMessage(msg);
                }
            }

            if ((e.Button == Buttons.Y) && (e.EventType == InputEventType.Pressed))
            {
                // Fire
                Message msg = new Message(MessageType.FireWeapon, this, m_playerOne);
                MessageDispatcher.Get().SendMessage(msg);
            }

            return true;
        }

        bool DebugOnGamePadButtonEvent(GamePadButtonEvent e)
        {
            return true;
        }

        bool OnKeyboardEvent(KeyboardEvent e)
        {
            // Jump!
            if (e.Key == Keys.Space)
            {
                if (e.EventType == InputEventType.Pressed)
                {
                    // start jump
                }
                else if (e.EventType == InputEventType.Released)
                {
                    // end jump
                }
            }

            if ((e.Key == Keys.F) && (e.EventType == InputEventType.Pressed))
            {
                // Fire
                Message msg = new Message(MessageType.FireWeapon, this, m_playerOne);
                MessageDispatcher.Get().SendMessage(msg);
            }

            return true;
        }

        #endregion

        public Camera2D Camera
        {
            get { return m_camera; }
        }

        public string ConsoleStr
        {
            get { return m_consoleString; }
            set { m_consoleString = value; }
        }

        public World World
        {
            get { return m_world; }
        }
    }
}
