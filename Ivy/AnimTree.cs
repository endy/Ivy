using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace Ivy
{
    class AnimTree
    {
        Entity m_owner;

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

        Texture2D m_samusMap;
        AnimatedSprite samusTurnAnim;
        AnimatedSprite samusWaitRightAnim;
        AnimatedSprite samusWaitLeftAnim;
        AnimatedSprite samusRunRightAnim;
        AnimatedSprite samusRunLeftAnim;
        AnimatedSprite samusJumpRollRightAnim;
        AnimatedSprite samusJumpRollLeftAnim;
        AnimatedSprite samusJumpAscendRightAnim;
        AnimatedSprite samusJumpAscendLeftAnim;
        AnimatedSprite samusJumpDescendRightAnim;
        AnimatedSprite samusJumpDescendLeftAnim;
        AnimatedSprite samusJumpLandRightAnim;
        AnimatedSprite samusJumpLandLeftAnim;

        public AnimTree(Entity owner)
        {
            m_owner = owner;
        }

        public void Initialize()
        {
            m_samusMap = IvyGame.Get().Content.Load<Texture2D>("Sprites\\samusMap");

            samusTurnAnim = new AnimatedSprite(IvyGame.Get(), m_samusMap, samusTurnRect, 3, 24f);
            samusTurnAnim.Initialize();
            samusTurnAnim.Loop = false;
            samusTurnAnim.Scale = new Vector2(3.0f, 3.0f);

            samusTurnAnim.OnAnimEnd = this.AnimatedSpriteEnd;

            samusWaitRightAnim = new AnimatedSprite(IvyGame.Get(), m_samusMap, samusWaitRightRect, 5, 6f);
            samusWaitRightAnim.Initialize();
            samusWaitRightAnim.Scale = new Vector2(3.0f, 3.0f);

            samusWaitLeftAnim = new AnimatedSprite(IvyGame.Get(), m_samusMap, samusWaitLeftRect, 5, 6f);
            samusWaitLeftAnim.Initialize();
            samusWaitLeftAnim.Scale = new Vector2(3.0f, 3.0f);

            samusRunRightAnim = new AnimatedSprite(IvyGame.Get(), m_samusMap, samusRunRightRect, 10, 18f);
            samusRunRightAnim.Initialize();
            samusRunRightAnim.Scale = new Vector2(3.0f, 3.0f);

            samusRunLeftAnim = new AnimatedSprite(IvyGame.Get(), m_samusMap, samusRunLeftRect, 10, 18f);
            samusRunLeftAnim.Initialize();
            samusRunLeftAnim.Scale = new Vector2(3.0f, 3.0f);

            samusJumpRollRightAnim = new AnimatedSprite(IvyGame.Get(), m_samusMap, samusJumpRollRightRect, 8, 16f);
            samusJumpRollRightAnim.Initialize();
            samusJumpRollRightAnim.Scale = new Vector2(3.0f, 3.0f);

            samusJumpRollLeftAnim = new AnimatedSprite(IvyGame.Get(), m_samusMap, samusJumpRollLeftRect, 8, 16f);
            samusJumpRollLeftAnim.Initialize();
            samusJumpRollLeftAnim.Scale = new Vector2(3.0f, 3.0f);

            samusJumpAscendRightAnim = new AnimatedSprite(IvyGame.Get(), m_samusMap, samusJumpAscendRightRect, 2, 16f);
            samusJumpAscendRightAnim.Initialize();
            samusJumpAscendRightAnim.Loop = false;
            samusJumpAscendRightAnim.Scale = new Vector2(3.0f, 3.0f);

            samusJumpAscendLeftAnim = new AnimatedSprite(IvyGame.Get(), m_samusMap, samusJumpAscendLeftRect, 2, 16f);
            samusJumpAscendLeftAnim.Initialize();
            samusJumpAscendLeftAnim.Loop = false;
            samusJumpAscendLeftAnim.Scale = new Vector2(3.0f, 3.0f);

            samusJumpDescendRightAnim = new AnimatedSprite(IvyGame.Get(), m_samusMap, samusJumpDescendRightRect, 4, 16f);
            samusJumpDescendRightAnim.Initialize();
            samusJumpDescendRightAnim.Loop = false;
            samusJumpDescendRightAnim.Scale = new Vector2(3.0f, 3.0f);

            samusJumpDescendLeftAnim = new AnimatedSprite(IvyGame.Get(), m_samusMap, samusJumpDescendLeftRect, 4, 16f);
            samusJumpDescendLeftAnim.Initialize();
            samusJumpDescendLeftAnim.Loop = false;
            samusJumpDescendLeftAnim.Scale = new Vector2(3.0f, 3.0f);

            samusJumpLandRightAnim = new AnimatedSprite(IvyGame.Get(), m_samusMap, samusJumpLandRightRect, 2, 16f);
            samusJumpLandRightAnim.Initialize();
            samusJumpLandRightAnim.Loop = false;
            samusJumpLandRightAnim.OnAnimEnd += this.AnimatedSpriteEnd;
            samusJumpLandRightAnim.Scale = new Vector2(3.0f, 3.0f);

            samusJumpLandLeftAnim = new AnimatedSprite(IvyGame.Get(), m_samusMap, samusJumpLandLeftRect, 2, 16f);
            samusJumpLandLeftAnim.Initialize();
            samusJumpLandLeftAnim.Loop = false;
            samusJumpLandLeftAnim.OnAnimEnd += this.AnimatedSpriteEnd;
            samusJumpLandLeftAnim.Scale = new Vector2(3.0f, 3.0f);
        }

        public void ReceiveMessage()
        {

        }

        public void Update(GameTime gameTime)
        {

        }

        public void Draw(SpriteBatch spriteBatch)
        {
            samusWaitRightAnim.Draw(spriteBatch, m_owner.Position);
        }

        public bool AnimatedSpriteEnd(AnimatedSprite anim)
        {
            /*
            switch (m_playerState)
            {
                case PlayerState.Wait:
                    // do nothing for now---should go to run?
                    EnterState_Wait(PlayerState.Wait);
                    break;
                case PlayerState.Run:
                    ExitState_Run(PlayerState.Wait);
                    break;
                case PlayerState.JumpDescend:
                    ExitState_JumpDescend(PlayerState.Wait);
                    break;
                default:
                    // error
                    break;
            }*/

            return true;
        }

    }
}
