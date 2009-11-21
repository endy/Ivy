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
    public abstract class IvyGame : Microsoft.Xna.Framework.Game,
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

        protected IvyGame()
        {
            // Move this back to initialize
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            m_instance = this;
        }
        
        public static IvyGame Get()
        {
            if (m_instance == null)
            {
                //@TODO debug!
                // uh oh!  shouldn't even happen!
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


            Texture2D roomBackground = IvyGame.Get().Content.Load<Texture2D>("Sprites\\environment");

            Rectangle shaftRoomRect = new Rectangle(21, 94, 256, 705);
            Rectangle openRoomRect = new Rectangle(329, 50, 512, 192);
            Rectangle stairsRoomRect = new Rectangle(334, 333, 512, 448);
            Rectangle labRoomRect = new Rectangle(48, 873, 512, 192);
            Rectangle corridorRoomRect = new Rectangle(704, 690, 512, 192);
            Rectangle encounterRoomRect = new Rectangle(565, 1133, 256, 192);

            Rectangle roomBounds = new Rectangle(0, 0, shaftRoomRect.Width, shaftRoomRect.Height);
            Room mainRoom = new Room(m_world, roomBounds, roomBackground, shaftRoomRect);
            mainRoom.Initialize();

            // Add Room Entities
            Skree skree = null;

            Random r = new Random();
            for (int i = 0; i < 0; i++)
            {
                skree = new Skree(this);
                skree.Initialize();
                skree.Position = new Point(r.Next() % 400, r.Next() % 150);
                mainRoom.AddEntity(skree);
            }

            m_world.SetCurrentRoom(mainRoom);

            // Add Player
            m_playerOne = new Player(this);
            m_playerOne.Initialize();
            m_world.AddPlayer(m_playerOne);

            // Create Camera
            Rectangle screenRect = new Rectangle(0, 0, 800, 600);

            int cameraHeight = 192; // roomBounds.Height;
            int cameraWidth = 256; // (int)(roomBounds.Height * (screenRect.Width / (float)screenRect.Height));
            Rectangle cameraBounds = new Rectangle(0, 0, cameraWidth, cameraHeight);

            m_camera = new Camera2D(roomBounds, cameraBounds, screenRect);
    
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
            consolePos = new Vector2(40, 50);
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
            m_camera.Update(gameTime, m_playerOne);

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
                                   0, FontOrigin, 1.2f, SpriteEffects.None, 0.5f);

            spriteBatch.End();

            m_world.Draw3D();

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
                    Message msg = new Message(MessageType.Jump, this, m_playerOne);
                    MessageDispatcher.Get().SendMessage(msg);
                }
                else if (e.EventType == InputEventType.Released)
                {
                    // end jump
                    Message msg = new Message(MessageType.Fall, this, m_playerOne);
                    MessageDispatcher.Get().SendMessage(msg);
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
