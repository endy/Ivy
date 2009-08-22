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
    public class IvyGame : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        private Rectangle worldBounds;

        private SpriteFont consoleFont;
        private Vector2 consolePos;

        Texture2D background;

        private Camera2D m_camera;
        private Player m_player;
        

        GamePadState previousGamePadState = GamePad.GetState(PlayerIndex.One);
        KeyboardState previousKeyboardState = Keyboard.GetState();

        // prototype vars
        Box box;

        public IvyGame()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
            

            worldBounds = new Rectangle(0, 0, 512, 192);

            int width = 192 * (800 / 600);
            Rectangle screenRect = new Rectangle(0, 0, width, 192);

            m_camera = new Camera2D(worldBounds, screenRect);

            m_player = new Player(this, worldBounds);

            box = new Box(this, new Rectangle(0, 0, 200, 200));
            
        }

        /// <summary>
        /// Allows the game to perform any initialization it needs to before starting to run.
        /// This is where it can query for any required services and load any non-graphic
        /// related content.  Calling base.Initialize will enumerate through any components
        /// and initialize them as well.
        /// </summary>
        protected override void Initialize()
        {
            // TODO: Add your initialization logic here

            m_player.Initialize();
            box.Initialize();

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

            // TODO: use this.Content to load your game content here

            background = Content.Load<Texture2D>("Sprites\\environment");
        
            consoleFont = Content.Load<SpriteFont>("Fonts\\Console");
            consolePos = new Vector2(0, 30);


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
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            // TODO: Add your update logic here

            m_player.Update(gameTime);

            // focus pos shouldn't be character position
            // it should also be calculated by camera, most like
            Point focusPos = new Point(m_player.PlayerPos.X, m_player.PlayerPos.Y);
            m_camera.Update(focusPos);

            box.Update(gameTime);

            base.Update(gameTime);
        }

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.Red);
            
            // TODO: Add your drawing code here

            spriteBatch.Begin();

            // Draw Background
            Rectangle cameraRect = m_camera.CameraRect;     // camera rect in world space

            //int width = 192 * (800 / 600);

            // bg: 512x192, tl=48,873
            Rectangle srcRect = new Rectangle(48+cameraRect.X, 873+cameraRect.Y, cameraRect.Width, cameraRect.Height);
            Rectangle dstRect = new Rectangle(0, 0, 800, 600);
            spriteBatch.Draw(background, dstRect, srcRect, Color.White);

            m_player.Draw(spriteBatch);

            // Draw Console

            float fps = (1 / (float)gameTime.ElapsedGameTime.Milliseconds) * 1000;

            string output = "FPS: " + fps + "\n";
            output += "Camera Rect (X:" + cameraRect.X + " Y:" + cameraRect.Y + ")\n";
            output += "\n";


            // Find the center of the string
            Vector2 FontOrigin = consoleFont.MeasureString(output) / 2;

            Vector2 drawConsolePos = new Vector2(consolePos.X + FontOrigin.X, consolePos.Y);
                        
            // Draw the string
            spriteBatch.DrawString(consoleFont, output, drawConsolePos, Color.LimeGreen,
                0, FontOrigin, 1.0f, SpriteEffects.None, 0.5f);


            spriteBatch.End();

            box.Draw(gameTime);
            

            base.Draw(gameTime);
        }

        public Camera2D Camera
        {
            get { return m_camera; }
        }
    }
}
