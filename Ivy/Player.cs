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

            float scale = 3f;

            #region Animation Setup
            AnimatedSprite samusTurnLeftAnim = new AnimatedSprite(IvyGame.Get(), samusMap, samusTurnRect, 3, 24f);

            IAnimGraphNode samusTurnLeftToWaitNode = m_animGraph.AddTransitionAnim(samusTurnLeftAnim);
            samusTurnLeftToWaitNode.Anim.Initialize();
            samusTurnLeftToWaitNode.Anim.Loop = false;
            samusTurnLeftToWaitNode.Anim.Scale = new Vector2(scale, scale);
            samusTurnLeftToWaitNode.Anim.Name = "SamusTurnLeft";

            AnimatedSprite samusTurnRightAnim = new AnimatedSprite(IvyGame.Get(), samusMap, samusTurnRect, 3, 24f);

            IAnimGraphNode samusTurnRightToWaitNode = m_animGraph.AddTransitionAnim(samusTurnRightAnim);
            samusTurnRightToWaitNode.Anim.Initialize();
            samusTurnRightToWaitNode.Anim.Loop = false;
            samusTurnRightToWaitNode.Anim.Reverse = true;
            samusTurnRightToWaitNode.Anim.Scale = new Vector2(scale, scale);
            samusTurnRightToWaitNode.Anim.Name = "SamusTurnRight";

            IAnimGraphNode samusTurnLeftToRunNode = m_animGraph.AddTransitionAnim(samusTurnLeftAnim);
            samusTurnLeftToRunNode.Anim.Initialize();
            samusTurnLeftToRunNode.Anim.Loop = false;
            samusTurnLeftToRunNode.Anim.Scale = new Vector2(scale, scale);
            samusTurnLeftToRunNode.Anim.Name = "SamusTurnLeft";

            IAnimGraphNode samusTurnRightToRunNode = m_animGraph.AddTransitionAnim(samusTurnRightAnim);
            samusTurnRightToRunNode.Anim.Initialize();
            samusTurnRightToRunNode.Anim.Loop = false;
            samusTurnRightToRunNode.Anim.Reverse = true;
            samusTurnRightToRunNode.Anim.Scale = new Vector2(scale, scale);
            samusTurnRightToRunNode.Anim.Name = "SamusTurnRight";

            IAnimGraphNode samusWaitLeftNode = m_animGraph.AddAnim(
                new AnimatedSprite(IvyGame.Get(), samusMap, samusWaitLeftRect, 5, 6f));
            samusWaitLeftNode.Anim.Initialize();
            samusWaitLeftNode.Anim.Scale = new Vector2(scale, scale);
            samusWaitLeftNode.Anim.Name = "SamusWaitLeft";

            IAnimGraphNode samusWaitRightNode = m_animGraph.AddAnim( 
                new AnimatedSprite(IvyGame.Get(), samusMap, samusWaitRightRect, 5, 6f));
            samusWaitRightNode.Anim.Initialize();
            samusWaitRightNode.Anim.Scale = new Vector2(scale, scale);
            samusWaitRightNode.Anim.Name = "SamusWaitRight";

            IAnimGraphNode samusRunLeftNode = m_animGraph.AddAnim(
                new AnimatedSprite(IvyGame.Get(), samusMap, samusRunLeftRect, 10, 18f));
            samusRunLeftNode.Anim.Initialize();
            samusRunLeftNode.Anim.Scale = new Vector2(scale, scale);
            samusRunLeftNode.Anim.Name = "SamusRunLeft";

            IAnimGraphNode samusRunRightNode = m_animGraph.AddAnim(
                new AnimatedSprite(IvyGame.Get(), samusMap, samusRunRightRect, 10, 18f));
            samusRunRightNode.Anim.Initialize();
            samusRunRightNode.Anim.Scale = new Vector2(scale, scale);
            samusRunRightNode.Anim.Name = "SamusRunRight";

            // Stand & Run code
            m_animGraph.AddTransition(samusWaitLeftNode, MessageType.MoveLeft, samusRunLeftNode);
            m_animGraph.AddTransition(samusRunLeftNode, MessageType.Stand, samusWaitLeftNode);

            m_animGraph.AddTransition(samusWaitRightNode, MessageType.MoveRight, samusRunRightNode);
            m_animGraph.AddTransition(samusRunRightNode, MessageType.Stand, samusWaitRightNode);

            // Turn Code

            // Turning Left to Right
            m_animGraph.AddTransition(samusWaitLeftNode, MessageType.MoveRight, samusTurnRightToRunNode, samusRunRightNode);
            m_animGraph.AddTransition(samusRunLeftNode, MessageType.MoveRight, samusTurnRightToRunNode, samusRunRightNode);
            m_animGraph.AddTransition(samusTurnRightToRunNode, MessageType.Stand, samusTurnRightToWaitNode, samusWaitRightNode);
            
            // Turning Right to Left            
            m_animGraph.AddTransition(samusWaitRightNode, MessageType.MoveLeft, samusTurnLeftToRunNode, samusRunLeftNode);
            m_animGraph.AddTransition(samusRunRightNode, MessageType.MoveLeft, samusTurnLeftToRunNode, samusRunLeftNode);
            m_animGraph.AddTransition(samusTurnLeftToRunNode, MessageType.Stand, samusTurnLeftToWaitNode, samusWaitLeftNode);
            
            // Change turn direction           
            m_animGraph.AddTransition(samusTurnLeftToRunNode, MessageType.MoveRight, samusTurnRightToRunNode);
            m_animGraph.AddTransition(samusTurnRightToRunNode, MessageType.MoveLeft, samusTurnLeftToRunNode);

            m_animGraph.AddTransition(samusTurnLeftToWaitNode, MessageType.MoveRight, samusTurnRightToRunNode);
            m_animGraph.AddTransition(samusTurnRightToWaitNode, MessageType.MoveLeft, samusTurnLeftToRunNode);


            IAnimGraphNode samusJumpRollLeftNode = m_animGraph.AddAnim(
                new AnimatedSprite(IvyGame.Get(), samusMap, samusJumpRollLeftRect, 8, 16f));
            samusJumpRollLeftNode.Anim.Initialize();
            samusJumpRollLeftNode.Anim.Scale = new Vector2(scale, scale);


            IAnimGraphNode samusJumpRollRightNode = m_animGraph.AddAnim(
                new AnimatedSprite(IvyGame.Get(), samusMap, samusJumpRollRightRect, 8, 16f));
            samusJumpRollRightNode.Anim.Initialize();
            samusJumpRollRightNode.Anim.Scale = new Vector2(scale, scale);

            IAnimGraphNode samusJumpAscendLeftNode = m_animGraph.AddAnim(
                new AnimatedSprite(IvyGame.Get(), samusMap, samusJumpAscendLeftRect, 2, 16f));
            samusJumpAscendLeftNode.Anim.Initialize();
            samusJumpAscendLeftNode.Anim.Loop = false;
            samusJumpAscendLeftNode.Anim.Scale = new Vector2(scale, scale);

            IAnimGraphNode samusJumpAscendRightNode = m_animGraph.AddAnim(
                new AnimatedSprite(IvyGame.Get(), samusMap, samusJumpAscendRightRect, 2, 16f));
            samusJumpAscendRightNode.Anim.Initialize();
            samusJumpAscendRightNode.Anim.Loop = false;
            samusJumpAscendRightNode.Anim.Scale = new Vector2(scale, scale);

            IAnimGraphNode samusJumpDescendLeftNode = m_animGraph.AddAnim(
                new AnimatedSprite(IvyGame.Get(), samusMap, samusJumpDescendLeftRect, 4, 16f));
            samusJumpDescendLeftNode.Anim.Initialize();
            samusJumpDescendLeftNode.Anim.Loop = false;
            samusJumpDescendLeftNode.Anim.Scale = new Vector2(scale, scale);

            IAnimGraphNode samusJumpDescendRightNode = m_animGraph.AddAnim(
                new AnimatedSprite(IvyGame.Get(), samusMap, samusJumpDescendRightRect, 4, 16f));
            samusJumpDescendRightNode.Anim.Initialize();
            samusJumpDescendRightNode.Anim.Loop = false;
            samusJumpDescendRightNode.Anim.Scale = new Vector2(scale, scale);

            IAnimGraphNode samusJumpLandLeftNode = m_animGraph.AddTransitionAnim(
                new AnimatedSprite(IvyGame.Get(), samusMap, samusJumpLandLeftRect, 2, 16f));
            samusJumpLandLeftNode.Anim.Initialize();
            samusJumpLandLeftNode.Anim.Loop = false;
            samusJumpLandLeftNode.Anim.Scale = new Vector2(scale, scale);

            IAnimGraphNode samusJumpLandRightNode = m_animGraph.AddTransitionAnim(
                new AnimatedSprite(IvyGame.Get(), samusMap, samusJumpLandRightRect, 2, 16f));
            samusJumpLandRightNode.Anim.Initialize();
            samusJumpLandRightNode.Anim.Loop = false;
            samusJumpLandRightNode.Anim.Scale = new Vector2(scale, scale);

            // Jump Ascend & Descend
            m_animGraph.AddTransition(samusRunLeftNode, MessageType.Jump, samusJumpRollLeftNode);
            m_animGraph.AddTransition(samusRunRightNode, MessageType.Jump, samusJumpRollRightNode);

            m_animGraph.AddTransition(samusWaitLeftNode, MessageType.Jump, samusJumpAscendLeftNode);
            m_animGraph.AddTransition(samusWaitRightNode, MessageType.Jump, samusJumpAscendRightNode);

            m_animGraph.AddTransition(samusJumpAscendLeftNode, MessageType.Fall, samusJumpDescendLeftNode);
            m_animGraph.AddTransition(samusJumpAscendRightNode, MessageType.Fall, samusJumpDescendRightNode);

            // Jump Turn
            m_animGraph.AddTransition(samusJumpRollLeftNode, MessageType.MoveRight, samusJumpRollRightNode);
            m_animGraph.AddTransition(samusJumpRollRightNode, MessageType.MoveLeft, samusJumpRollLeftNode);
            m_animGraph.AddTransition(samusJumpAscendLeftNode, MessageType.MoveRight, samusJumpAscendRightNode);
            m_animGraph.AddTransition(samusJumpAscendRightNode, MessageType.MoveLeft, samusJumpAscendLeftNode);
            m_animGraph.AddTransition(samusJumpDescendLeftNode, MessageType.MoveRight, samusJumpDescendRightNode);
            m_animGraph.AddTransition(samusJumpDescendRightNode, MessageType.MoveLeft, samusJumpDescendLeftNode);

            // Land
            m_animGraph.AddTransition(samusJumpRollLeftNode, MessageType.Land, samusJumpLandLeftNode, samusRunLeftNode);
            m_animGraph.AddTransition(samusJumpRollRightNode, MessageType.Land, samusJumpLandRightNode, samusRunRightNode);

            m_animGraph.AddTransition(samusJumpDescendLeftNode, MessageType.Land, samusJumpLandLeftNode, samusRunLeftNode);
            m_animGraph.AddTransition(samusJumpDescendRightNode, MessageType.Land, samusJumpLandRightNode, samusRunRightNode);
            
            
            m_animGraph.SetCurrentNode(samusWaitRightNode);
            #endregion

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

        public override void Draw(SpriteBatch spriteBatch)
        {
            m_armCannon.Draw(spriteBatch);

            m_animGraph.Draw(spriteBatch);
        }

        public override void Draw3D()
        {
            m_animGraph.Draw3D();
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