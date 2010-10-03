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
        private static IvyGame m_instance = null;

        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        public World World { get; private set; }
        public Camera2D Camera { get; private set; }

        public Entity CameraTarget { get; set; }

        // refactor into Console class
        public string ConsoleStr { get; set; }
        private SpriteFont consoleFont;
        private Vector2 consolePos;

        private string m_fpsStr;
        

        protected IvyGame()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            if (m_instance != null)
            {
                // @todo error! exception?
            }

            m_instance = this;
        }
        
        public static IvyGame Get()
        {
            if (m_instance == null)
            {
                //@TODO exception!
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

            // Create World
            World = new World();

            Camera = new Camera2D();
    
            ConsoleStr = "\n";
            m_fpsStr = "\n";

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
        
            ///@todo move to console class
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
            Content.Unload();
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
            World.Update(gameTime);

            // Update Camera based on Player Position
            Camera.Update(gameTime, CameraTarget);

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
            m_fpsStr = "FPS: " + fps + "\n";

            GraphicsDevice.Clear(Color.Black);
    
            spriteBatch.Begin(SpriteBlendMode.AlphaBlend, SpriteSortMode.Immediate, SaveStateMode.None, Matrix.Identity);
            graphics.GraphicsDevice.SamplerStates[0].MagFilter = TextureFilter.Point;
            graphics.GraphicsDevice.SamplerStates[0].MinFilter = TextureFilter.Point;
            graphics.GraphicsDevice.SamplerStates[0].MipFilter = TextureFilter.Point;

            World.Draw(spriteBatch);

            // Draw Console           
            // Find the center of the string
            Vector2 FontCenter = consoleFont.MeasureString(ConsoleStr) / 2;

            Vector2 drawConsolePos = new Vector2(consolePos.X + FontCenter.X, consolePos.Y);
                        
            // Draw the string
            spriteBatch.DrawString(consoleFont, ConsoleStr, drawConsolePos, Color.LimeGreen,
                                   0, FontCenter, 1.2f, SpriteEffects.None, 0.5f);

            Vector2 FpsDims = consoleFont.MeasureString(m_fpsStr);
            Vector2 drawFpsPos = new Vector2((Camera.ScreenRect.Right - (FpsDims.X * 2)), FpsDims.Y);
            spriteBatch.DrawString(consoleFont, m_fpsStr, drawFpsPos, Color.LimeGreen, 0, FpsDims / 2, 2.0f, SpriteEffects.None, 0.5f);

            spriteBatch.End();

            World.Draw3D();

            base.Draw(gameTime);
        }


    }
}
