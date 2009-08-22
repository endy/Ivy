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
    public class Box : Microsoft.Xna.Framework.GameComponent
                       //Microsoft.Xna.Framework.IDrawable
    {
        IvyGame ivyGame;

        Matrix worldMatrix;
        Matrix viewMatrix;
        Matrix projectionMatrix;

        BasicEffect basicEffect;
        VertexDeclaration vertexDeclaration;

        VertexPositionColor[] vertexArray;
        short[] lineIndicies;

        Rectangle m_rect;

        public Box(IvyGame game, Rectangle rect)
            : base(game)
        {
            ivyGame = game;
            m_rect = rect;
        }

        /// <summary>
        /// Allows the game component to perform any initialization it needs to before starting
        /// to run.  This is where it can query for any required services and load content.
        /// </summary>
        public override void Initialize()
        {
            // TODO: Add your initialization code here

            InitializeVertexes();
            InitializeTransform();
            InitializeEffect();

            base.Initialize();
        }

        private void InitializeVertexes()
        {
            vertexArray = new VertexPositionColor[4];

            vertexArray[0].Position = Vector3.Zero;
            vertexArray[0].Color = Color.LawnGreen;
            vertexArray[1].Position = new Vector3(m_rect.Width, 0, 0);
            vertexArray[1].Color = Color.LawnGreen;
            vertexArray[2].Position = new Vector3(m_rect.Width, m_rect.Height, 0);
            vertexArray[2].Color = Color.LawnGreen;
            vertexArray[3].Position = new Vector3(0, m_rect.Height, 0);
            vertexArray[3].Color = Color.LawnGreen;


            lineIndicies = new short[5] { 0, 1, 2, 3, 0 };
        }

        private void InitializeTransform()
        {
            viewMatrix = Matrix.CreateLookAt(
                new Vector3(0.0f, 0.0f, 1.0f),
                Vector3.Zero,
                Vector3.Up
                );

            projectionMatrix = Matrix.CreateOrthographicOffCenter(
                0,
                (float)Game.GraphicsDevice.Viewport.Width,
                (float)Game.GraphicsDevice.Viewport.Height,
                0,
                1.0f, 1000.0f);
        }

        private void InitializeEffect()
        {

            vertexDeclaration = new VertexDeclaration(Game.GraphicsDevice,
                                                      VertexPositionColor.VertexElements);

            basicEffect = new BasicEffect(Game.GraphicsDevice, null);
            basicEffect.VertexColorEnabled = true;

            worldMatrix = Matrix.CreateTranslation(0, 0, 0);

            basicEffect.World      = worldMatrix;
            basicEffect.View       = viewMatrix;
            basicEffect.Projection = projectionMatrix;
        }

        /// <summary>
        /// Allows the game component to update itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        public override void Update(GameTime gameTime)
        {
            // TODO: Add your update code here


            Rectangle cameraRect = ivyGame.Camera.CameraRect;


            base.Update(gameTime);
        }

        public void Draw(GameTime gameTime)
        {          
            basicEffect.Begin();

            foreach (EffectPass pass in basicEffect.CurrentTechnique.Passes)
            {
                pass.Begin();

                Game.GraphicsDevice.DrawUserIndexedPrimitives<VertexPositionColor>(
                    PrimitiveType.LineStrip,
                    vertexArray,
                    0, 
                    4,
                    lineIndicies,  // index of the first vertex to draw
                    0, 
                    4);   // number of primitives
                
                pass.End();
            }
            basicEffect.End();
        }
        
    }
}