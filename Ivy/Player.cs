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

        AnimGraph m_animGraph;
       
        // Sound Effects
        SoundEffect m_rollEffect;
        SoundEffectInstance m_rollInstance;
        SoundEffect m_landEffect;

        public Player(IvyGame game) : base(game)
        {

        }

        public override void Initialize()
        {
            m_animGraph = new AnimGraph(this);
            m_animGraph.Initialize();
            
            m_armCannon = new Weapon(IvyGame.Get(), this, new Point(10, 10), Direction);
            m_armCannon.Initialize();

            m_landEffect = Game.Content.Load<SoundEffect>("Audio\\samus_land");
            m_rollEffect = Game.Content.Load<SoundEffect>("Audio\\samus_jump_roll");
            m_rollInstance = m_rollEffect.CreateInstance();

            #region Anim Rects
            Rectangle samusTurnRect = new Rectangle(0, 156, 78, 47);
            Rectangle samusWaitRightRect = new Rectangle(0, 203, 140, 45);
            Rectangle samusWaitLeftRect = new Rectangle(0, 248, 140, 45);
            Rectangle samusRunRightRect = new Rectangle(0, 66, 460, 45);
            Rectangle samusRunLeftRect = new Rectangle(0, 111, 460, 45);
            Rectangle samusJumpRollRightRect = new Rectangle(0, 0, 280, 32);
            Rectangle samusJumpRollLeftRect = new Rectangle(0, 33, 280, 32);
            Rectangle samusJumpAscendRightRect = new Rectangle(0, 293, 56, 47);
            Rectangle samusJumpAscendLeftRect = new Rectangle(0, 340, 56, 47);
            Rectangle samusJumpDescendRightRect = new Rectangle(56, 293, 112, 47);
            Rectangle samusJumpDescendLeftRect = new Rectangle(56, 340, 112, 47);
            Rectangle samusJumpLandRightRect = new Rectangle(168, 293, 56, 47);
            Rectangle samusJumpLandLeftRect = new Rectangle(168, 340, 56, 47);
            #endregion

            Texture2D samusMap = Game.Content.Load<Texture2D>("Sprites\\samusMap");

            IAnimGraphNode samusTurnLeftNode = m_animGraph.AddTransitionAnim(
                new AnimatedSprite(IvyGame.Get(), samusMap, samusTurnRect, 3, 24f));
            samusTurnLeftNode.Anim.Initialize();
            samusTurnLeftNode.Anim.Loop = false;
            samusTurnLeftNode.Anim.Scale = new Vector2(3.0f, 3.0f);
            samusTurnLeftNode.Anim.Name = "SamusTurnLeft";

            IAnimGraphNode samusTurnRightNode = m_animGraph.AddTransitionAnim(
                new AnimatedSprite(IvyGame.Get(), samusMap, samusTurnRect, 3, 24f));
            samusTurnRightNode.Anim.Initialize();
            samusTurnRightNode.Anim.Loop = false;
            samusTurnRightNode.Anim.Reverse = true;
            samusTurnRightNode.Anim.Scale = new Vector2(3.0f, 3.0f);
            samusTurnRightNode.Anim.Name = "SamusTurnRight";

            IAnimGraphNode samusWaitLeftNode = m_animGraph.AddAnim(
                new AnimatedSprite(IvyGame.Get(), samusMap, samusWaitLeftRect, 5, 6f));
            samusWaitLeftNode.Anim.Initialize();
            samusWaitLeftNode.Anim.Scale = new Vector2(3f, 3f);
            samusWaitLeftNode.Anim.Name = "SamusWaitLeft";

            IAnimGraphNode samusWaitRightNode = m_animGraph.AddAnim( 
                new AnimatedSprite(IvyGame.Get(), samusMap, samusWaitRightRect, 5, 6f));
            samusWaitRightNode.Anim.Initialize();
            samusWaitRightNode.Anim.Scale = new Vector2(3.0f, 3.0f);
            samusWaitRightNode.Anim.Name = "SamusWaitRight";

            m_animGraph.AddTransition(samusWaitRightNode, MessageType.MoveLeft, samusTurnLeftNode, samusWaitLeftNode);
            m_animGraph.AddTransition(samusWaitLeftNode, MessageType.MoveRight, samusTurnRightNode, samusWaitRightNode);

            IAnimGraphNode samusRunLeftNode = m_animGraph.AddAnim(
                new AnimatedSprite(IvyGame.Get(), samusMap, samusRunLeftRect, 10, 18f));
            samusRunLeftNode.Anim.Initialize();
            samusRunLeftNode.Anim.Scale = new Vector2(3f, 3f);
            samusRunLeftNode.Anim.Name = "SamusRunLeft";

            m_animGraph.AddTransition(samusWaitLeftNode, MessageType.MoveLeft, samusRunLeftNode);
            m_animGraph.AddTransition(samusRunLeftNode, MessageType.Stand, samusWaitLeftNode);

            IAnimGraphNode samusRunRightNode = m_animGraph.AddAnim(
                new AnimatedSprite(IvyGame.Get(), samusMap, samusRunRightRect, 10, 18f));
            samusRunRightNode.Anim.Initialize();
            samusRunRightNode.Anim.Scale = new Vector2(3f, 3f);
            samusRunRightNode.Anim.Name = "SamusRunRight";

            m_animGraph.AddTransition(samusWaitRightNode, MessageType.MoveRight, samusRunRightNode);
            m_animGraph.AddTransition(samusRunRightNode, MessageType.Stand, samusWaitRightNode);

            m_animGraph.AddTransition(samusRunLeftNode, MessageType.MoveRight, samusTurnRightNode, samusRunRightNode);
            m_animGraph.AddTransition(samusRunRightNode, MessageType.MoveLeft, samusTurnLeftNode, samusRunLeftNode);

            IAnimGraphNode samusJumpRollLeftNode = m_animGraph.AddAnim(
                new AnimatedSprite(IvyGame.Get(), samusMap, samusJumpRollLeftRect, 8, 16f));
            samusJumpRollLeftNode.Anim.Initialize();
            samusJumpRollLeftNode.Anim.Scale = new Vector2(3f, 3f);

            m_animGraph.AddTransition(samusRunLeftNode, MessageType.Jump, samusJumpRollLeftNode);

            IAnimGraphNode samusJumpRollRightNode = m_animGraph.AddAnim(
                new AnimatedSprite(IvyGame.Get(), samusMap, samusJumpRollRightRect, 8, 16f));
            samusJumpRollRightNode.Anim.Initialize();
            samusJumpRollRightNode.Anim.Scale = new Vector2(3f, 3f);

            m_animGraph.AddTransition(samusRunRightNode, MessageType.Jump, samusJumpRollRightNode);

            m_animGraph.AddTransition(samusJumpRollLeftNode, MessageType.MoveRight, samusJumpRollRightNode);
            m_animGraph.AddTransition(samusJumpRollRightNode, MessageType.MoveLeft, samusJumpRollLeftNode);

            IAnimGraphNode samusJumpAscendLeftNode = m_animGraph.AddAnim(
                new AnimatedSprite(IvyGame.Get(), samusMap, samusJumpAscendLeftRect, 2, 16f));
            samusJumpAscendLeftNode.Anim.Initialize();
            samusJumpAscendLeftNode.Anim.Loop = false;
            samusJumpAscendLeftNode.Anim.Scale = new Vector2(3.0f, 3.0f);

            m_animGraph.AddTransition(samusWaitLeftNode, MessageType.Jump, samusJumpAscendLeftNode);

            IAnimGraphNode samusJumpAscendRightNode = m_animGraph.AddAnim(
                new AnimatedSprite(IvyGame.Get(), samusMap, samusJumpAscendRightRect, 2, 16f));
            samusJumpAscendRightNode.Anim.Initialize();
            samusJumpAscendRightNode.Anim.Loop = false;
            samusJumpAscendRightNode.Anim.Scale = new Vector2(3.0f, 3.0f);

            m_animGraph.AddTransition(samusWaitRightNode, MessageType.Jump, samusJumpAscendRightNode);

            IAnimGraphNode samusJumpDescendLeftNode = m_animGraph.AddAnim(
                new AnimatedSprite(IvyGame.Get(), samusMap, samusJumpDescendLeftRect, 4, 16f));
            samusJumpDescendLeftNode.Anim.Initialize();
            samusJumpDescendLeftNode.Anim.Loop = false;
            samusJumpDescendLeftNode.Anim.Scale = new Vector2(3.0f, 3.0f);

            m_animGraph.AddTransition(samusJumpAscendLeftNode, MessageType.Fall, samusJumpDescendLeftNode);

            IAnimGraphNode samusJumpDescendRightNode = m_animGraph.AddAnim(
                new AnimatedSprite(IvyGame.Get(), samusMap, samusJumpDescendRightRect, 4, 16f));
            samusJumpDescendRightNode.Anim.Initialize();
            samusJumpDescendRightNode.Anim.Loop = false;
            samusJumpDescendRightNode.Anim.Scale = new Vector2(3.0f, 3.0f);

            m_animGraph.AddTransition(samusJumpAscendRightNode, MessageType.Fall, samusJumpDescendRightNode);

            IAnimGraphNode samusJumpLandLeftNode = m_animGraph.AddTransitionAnim(
                new AnimatedSprite(IvyGame.Get(), samusMap, samusJumpLandLeftRect, 2, 16f));
            samusJumpLandLeftNode.Anim.Initialize();
            samusJumpLandLeftNode.Anim.Loop = false;
            samusJumpLandLeftNode.Anim.Scale = new Vector2(3.0f, 3.0f);

            IAnimGraphNode samusJumpLandRightNode = m_animGraph.AddTransitionAnim(
                new AnimatedSprite(IvyGame.Get(), samusMap, samusJumpLandRightRect, 2, 16f));
            samusJumpLandRightNode.Anim.Initialize();
            samusJumpLandRightNode.Anim.Loop = false;
            samusJumpLandRightNode.Anim.Scale = new Vector2(3.0f, 3.0f);

            m_animGraph.AddTransition(samusJumpRollLeftNode, MessageType.Land, samusJumpLandLeftNode);
            m_animGraph.AddTransition(samusJumpRollRightNode, MessageType.Land, samusJumpLandRightNode);

            m_animGraph.AddTransition(samusJumpDescendLeftNode, MessageType.Land, samusJumpLandLeftNode, samusWaitLeftNode);
            m_animGraph.AddTransition(samusJumpDescendRightNode, MessageType.Land, samusJumpLandRightNode, samusWaitRightNode);

            m_animGraph.SetCurrentNode(samusWaitRightNode);            

            base.Initialize();
        }

        /// <summary>
        /// Allows the game component to update itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        public override void Update(GameTime gameTime)
        {
            base.Update(gameTime);

            m_animGraph.Update(gameTime);

            m_armCannon.Update(gameTime);

        }

        public void Draw(SpriteBatch spriteBatch)
        {
            Rectangle cameraRect = IvyGame.Get().Camera.CameraRect;

            Point samusScreenPos;
            samusScreenPos.X = (int)(((Position.X - cameraRect.X) / (float)cameraRect.Width) * 800); // screen width = 800
            samusScreenPos.Y = (int)(((Position.Y - cameraRect.Y) / (float)cameraRect.Height) * 600); // screen height = 600

            m_armCannon.Draw(spriteBatch);

            m_animGraph.Draw(spriteBatch);
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

            m_animGraph.ReceiveMessage(msg);
        }
       
        private void FireWeapon()
        {
            m_armCannon.Fire();
        }        
    }
}