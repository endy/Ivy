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

        Point m_playerPos;
        Vector2 m_playerDir;
        bool facingRight;

        int jumpTime;
        int jumpElapsedTime;


        Rectangle samusTurnRect = new Rectangle(0, 156, 78, 47);
        Rectangle samusWaitRightRect = new Rectangle(0, 203, 140, 45);
        Rectangle samusWaitLeftRect = new Rectangle(0, 248, 140, 45);
        Rectangle samusRunRightRect = new Rectangle(0, 66, 460, 45);
        Rectangle samusRunLeftRect = new Rectangle(0, 111, 460, 45);
        Rectangle samusJumpRollRightRect = new Rectangle(0, 0, 280, 32);
        Rectangle samusJumpRollLeftRect = new Rectangle(0, 33, 280, 32);
        Rectangle samusJumpRightRect = new Rectangle(0, 293, 224, 47);
        Rectangle samusJumpLeftRect = new Rectangle(0, 340, 224, 47);

        Texture2D m_samusMap;
        AnimatedSprite samusTurnAnim;
        AnimatedSprite samusWaitRightAnim;
        AnimatedSprite samusWaitLeftAnim;
        AnimatedSprite samusRunRightAnim;
        AnimatedSprite samusRunLeftAnim;
        AnimatedSprite samusJumpRollRightAnim;
        AnimatedSprite samusJumpRollLeftAnim;
        AnimatedSprite samusJumpRightAnim;
        AnimatedSprite samusJumpLeftAnim;

        // Player State Data
        enum PlayerState
        {
            Wait = 0,
            Run,
            JumpAscend,
            JumpDescend,
            Fall
        };

        PlayerState m_playerState;
        AnimatedSprite m_playerAnim;

        GamePadState m_lastGamePadState;
        GamePadState m_currentGamePadState;

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

            m_playerPos = new Point(m_worldBounds.X + (m_worldBounds.Width / 2),
                                 m_worldBounds.Y + (m_worldBounds.Height / 2));
            m_playerDir = Vector2.Zero;
            facingRight = true;

            jumpTime = 2000;
            jumpElapsedTime = 0;

            m_samusMap = Game.Content.Load<Texture2D>("Sprites\\samusMap");

            samusTurnAnim = new AnimatedSprite(m_game, m_samusMap, samusTurnRect, 3, 24f);
            samusTurnAnim.Initialize();
            samusTurnAnim.Loop = false;
            samusTurnAnim.Scale = new Vector2(3.0f, 3.0f);

            samusTurnAnim.OnAnimEnd = this.AnimatedSpriteEnd;

            samusWaitRightAnim = new AnimatedSprite(m_game, m_samusMap, samusWaitRightRect, 5, 6f);
            samusWaitRightAnim.Initialize();
            samusWaitRightAnim.Scale = new Vector2(3.0f, 3.0f);

            samusWaitLeftAnim = new AnimatedSprite(m_game, m_samusMap, samusWaitLeftRect, 5, 6f);
            samusWaitLeftAnim.Initialize();
            samusWaitLeftAnim.Scale = new Vector2(3.0f, 3.0f);

            samusRunRightAnim = new AnimatedSprite(m_game, m_samusMap, samusRunRightRect, 10, 10f);
            samusRunRightAnim.Initialize();
            samusRunRightAnim.Scale = new Vector2(3.0f, 3.0f);

            samusRunLeftAnim = new AnimatedSprite(m_game, m_samusMap, samusRunLeftRect, 10, 10f);
            samusRunLeftAnim.Initialize();
            samusRunLeftAnim.Scale = new Vector2(3.0f, 3.0f);

            samusJumpRollRightAnim = new AnimatedSprite(m_game, m_samusMap, samusJumpRollRightRect, 8, 8f);
            samusJumpRollRightAnim.Initialize();
            samusJumpRollRightAnim.Scale = new Vector2(3.0f, 3.0f);

            samusJumpRollLeftAnim = new AnimatedSprite(m_game, m_samusMap, samusJumpRollLeftRect, 8, 8f);
            samusJumpRollLeftAnim.Initialize();
            samusJumpRollLeftAnim.Scale = new Vector2(3.0f, 3.0f);

            samusJumpRightAnim = new AnimatedSprite(m_game, m_samusMap, samusJumpRightRect, 8, 8f);
            samusJumpRightAnim.Initialize();
            samusJumpRightAnim.Scale = new Vector2(3.0f, 3.0f);

            samusJumpLeftAnim = new AnimatedSprite(m_game, m_samusMap, samusJumpLeftRect, 8, 8f);
            samusJumpLeftAnim.Initialize();
            samusJumpLeftAnim.Scale = new Vector2(3.0f, 3.0f);

            m_playerState = PlayerState.Wait;

            ///todo should we enter Wait with Wait state?
            m_playerAnim = samusWaitRightAnim;
            EnterState_Wait(m_playerState);

            base.Initialize();
        }

        /// <summary>
        /// Allows the game component to update itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        public override void Update(GameTime gameTime)
        {
            m_currentGamePadState = GamePad.GetState(PlayerIndex.One);

            float deadZone = 0.01f;
            if (m_currentGamePadState.ThumbSticks.Left.X > deadZone)
            {
                m_playerDir.X = 1.0f;
            }
            else if (m_currentGamePadState.ThumbSticks.Left.X < -deadZone)
            {
                m_playerDir.X = -1.0f;
            }
            else
            {
                m_playerDir.X = 0;
            }

            if (m_currentGamePadState.ThumbSticks.Left.Y > deadZone)
            {
                m_playerDir.Y = -1.0f;
            }
            else if (m_currentGamePadState.ThumbSticks.Left.Y < -deadZone)
            {
                m_playerDir.Y = 1.0f;
            }
            else
            {
                m_playerDir.Y = 0;
            }

            // update position, bounded by world

            switch (m_playerState)
            {
                case PlayerState.Wait:
                    ExecuteState_Wait();
                    break;
                case PlayerState.Run:
                    ExecuteState_Run();
                    break;
                case PlayerState.JumpAscend:
                    ExecuteState_JumpAscend();
                    break;
                case PlayerState.JumpDescend:
                    ExecuteState_JumpDescend();
                    break;
                case PlayerState.Fall:
                    ExecuteState_Fall();
                    break;
                default:
                    //error!
                    break;
            };


            m_playerAnim.Update(gameTime);

            m_lastGamePadState = m_currentGamePadState;

            base.Update(gameTime);
        }

        public void Draw(SpriteBatch spriteBatch)
        {
            bool right = (m_playerDir.X > 0) ? true : false;

            Rectangle cameraRect = m_game.Camera.CameraRect;

            Point samusScreenPos;
            samusScreenPos.X = (int)(((m_playerPos.X - cameraRect.X) / (float)cameraRect.Width) * 800); // screen width = 800
            samusScreenPos.Y = (int)(((m_playerPos.Y - cameraRect.Y) / (float)cameraRect.Height) * 600); // screen height = 600

            if (m_playerAnim != null)
            {
                m_playerAnim.Draw(spriteBatch, samusScreenPos);
            }   
        }

        public Point PlayerPos
        {
            get { return m_playerPos; }
        }

        private bool ChangedDir()
        {
            bool changedDir = false;
            if ((facingRight == true && (m_playerDir.X < 0)) ||
               (facingRight == false && (m_playerDir.X > 0)))
            {
                changedDir = true;
            }

            return changedDir;
        }

        private void UpdatePosition()
        {
            m_playerPos.X += (int)m_playerDir.X;
            m_playerPos.Y -= (int)m_playerDir.Y;
        }

        private void ChangeAnim(AnimatedSprite newAnim)
        {
            m_playerAnim.Stop();
            m_playerAnim.Reset();
            m_playerAnim = newAnim;
            m_playerAnim.Reset();
            m_playerAnim.Play();
        }

        private void EnterState_Wait(PlayerState prevState)
        {
            if (facingRight == true)
            {
                ChangeAnim(samusWaitRightAnim);
            }
            else
            {
                ChangeAnim(samusWaitLeftAnim);
            }

            m_playerState = PlayerState.Wait;
            ExecuteState_Wait();
        }

        private void ExecuteState_Wait()
        {
            m_game.ConsoleStr += "STATE=WAIT\n";

            if ((m_currentGamePadState.Buttons.A == ButtonState.Pressed) ||
                (jumpElapsedTime >= jumpTime))
            {
                ExitState_Wait(PlayerState.JumpAscend);
            }
            else if (ChangedDir())
            {
                if (facingRight == true)
                {
                    samusTurnAnim.Reverse = false;
                    ChangeAnim(samusTurnAnim);
                }
                else
                {
                    samusTurnAnim.Reverse = true;
                    ChangeAnim(samusTurnAnim);
                }
                facingRight = !facingRight;

            }

            if ((!samusTurnAnim.IsPlaying) && (m_playerDir.X != 0))
            {
                ExitState_Wait(PlayerState.Run);
            }
        }
        private void ExitState_Wait(PlayerState nextState)
        {
            switch (nextState)
            {
                case PlayerState.Run:
                    EnterState_Run(PlayerState.Wait);
                    break;
                case PlayerState.JumpAscend:
                    EnterState_JumpAscend(PlayerState.Wait);
                    break;
                default:
                    // error
                    break;
            }
        }

        private void EnterState_Run(PlayerState prevState)
        {
            if (ChangedDir())
            {
                if (facingRight == true)
                {
                    samusTurnAnim.Reverse = false;
                    ChangeAnim(samusTurnAnim);
                }
                else
                {
                    samusTurnAnim.Reverse = true;
                    ChangeAnim(samusTurnAnim);
                }
                facingRight = !facingRight;
            }
            else
            {
                if (facingRight)
                {
                    ChangeAnim(samusRunRightAnim);
                }
                else
                {
                    ChangeAnim(samusRunLeftAnim);
                }

                m_playerState = PlayerState.Run;
                ExecuteState_Run();
            }
        }
        private void ExecuteState_Run()
        {
            m_game.ConsoleStr += "STATE=RUN\n";
            
            if ((m_currentGamePadState.Buttons.A == ButtonState.Pressed) ||
                (jumpElapsedTime >= jumpTime))
            {
                ExitState_Run(PlayerState.JumpAscend);
            }
            else if (m_playerDir.X == 0)
            {
                ExitState_Run(PlayerState.Wait);
            }

            UpdatePosition();
        }

        private void ExitState_Run(PlayerState nextState)
        {
            switch (nextState)
            {
                case PlayerState.Wait:
                    EnterState_Wait(PlayerState.Run); 
                    break;
                default:
                    break;
            };
        }

        #region Incomplete States
        private void EnterState_JumpAscend(PlayerState prevState)
        {
            m_game.ConsoleStr += "STATE=JUMP_ASCEND\n";

            m_playerDir.Y = 1.0f;

            m_playerState = PlayerState.JumpAscend;
            ExecuteState_JumpAscend();
        }
        private void ExecuteState_JumpAscend()
        {
            // if not holding A button, or time ran out
            if ((m_currentGamePadState.Buttons.A == ButtonState.Released) ||
                (jumpElapsedTime >= jumpTime))
            {
                ExitState_JumpAscend(PlayerState.JumpDescend);
            }

            if (facingRight == true)
            {
                ChangeAnim(samusJumpRightAnim);
            }
            else
                
            {
                ChangeAnim(samusJumpLeftAnim);
            }

            UpdatePosition();
        }
        private void ExitState_JumpAscend(PlayerState nextState)
        {
            EnterState_JumpDescend(PlayerState.JumpAscend);
        }

        private void EnterState_JumpDescend(PlayerState prevState)
        {
            m_game.ConsoleStr += "STATE=JUMP_DESCEND\n";

            m_playerDir.Y = -1.0f;

            m_playerState = PlayerState.JumpDescend;
            ExecuteState_JumpDescend();
        }
        private void ExecuteState_JumpDescend()
        {
            // if hit floor? check position, go to wait/erm transition next anim
        }
        private void ExitState_JumpDescend(PlayerState nextState)
        {
        }

        private void EnterState_Fall(PlayerState prevState)
        {
            m_playerState = PlayerState.Fall;
            ExecuteState_Fall();
        }
        private void ExecuteState_Fall()
        {
        }
        private void ExitState_Fall(PlayerState nextState)
        {
        }
        #endregion

        private bool AnimatedSpriteEnd(AnimatedSprite anim)
        {
            switch (m_playerState)
            {
                case PlayerState.Wait:
                    // do nothing for now---should go to run?
                    break;
                case PlayerState.Run:
                    ExitState_Run(PlayerState.Wait);
                    break;
                default:
                    // error
                    break;
            }

            return true;
        }

    }
}