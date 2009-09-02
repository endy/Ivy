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
        Vector2 m_playerSpeed;
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

        Weapon m_armCannon;

        SoundEffect m_rollEffect;
        SoundEffectInstance m_rollInstance;

        SoundEffect m_landEffect;

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

        ///todo move this somewhere...
        GameTime m_gameTime;

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
            m_playerSpeed = Vector2.One;
            facingRight = true;

            jumpTime = 800;
            jumpElapsedTime = 0;

            m_samusMap = Game.Content.Load<Texture2D>("Sprites\\samusMap");

            samusTurnAnim = new AnimatedSprite(m_game, m_samusMap, samusTurnRect, 3, 24f);
            samusTurnAnim.Initialize();
            samusTurnAnim.Loop = false;
            samusTurnAnim.Scale = new Vector2(3.0f, 3.0f);

            samusTurnAnim.OnAnimEnd = this.AnimatedSpriteEnd;

            samusWaitRightAnim = new AnimatedSprite(IvyGame.Get(), m_samusMap, samusWaitRightRect, 5, 6f);
            samusWaitRightAnim.Initialize();
            samusWaitRightAnim.Scale = new Vector2(3.0f, 3.0f);

            samusWaitLeftAnim = new AnimatedSprite(m_game, m_samusMap, samusWaitLeftRect, 5, 6f);
            samusWaitLeftAnim.Initialize();
            samusWaitLeftAnim.Scale = new Vector2(3.0f, 3.0f);

            samusRunRightAnim = new AnimatedSprite(m_game, m_samusMap, samusRunRightRect, 10, 18f);
            samusRunRightAnim.Initialize();
            samusRunRightAnim.Scale = new Vector2(3.0f, 3.0f);

            samusRunLeftAnim = new AnimatedSprite(m_game, m_samusMap, samusRunLeftRect, 10, 18f);
            samusRunLeftAnim.Initialize();
            samusRunLeftAnim.Scale = new Vector2(3.0f, 3.0f);

            samusJumpRollRightAnim = new AnimatedSprite(m_game, m_samusMap, samusJumpRollRightRect, 8, 16f);
            samusJumpRollRightAnim.Initialize();
            samusJumpRollRightAnim.Scale = new Vector2(3.0f, 3.0f);

            samusJumpRollLeftAnim = new AnimatedSprite(m_game, m_samusMap, samusJumpRollLeftRect, 8, 16f);
            samusJumpRollLeftAnim.Initialize();
            samusJumpRollLeftAnim.Scale = new Vector2(3.0f, 3.0f);

            samusJumpAscendRightAnim = new AnimatedSprite(m_game, m_samusMap, samusJumpAscendRightRect, 2, 16f);
            samusJumpAscendRightAnim.Initialize();
            samusJumpAscendRightAnim.Loop = false;
            samusJumpAscendRightAnim.Scale = new Vector2(3.0f, 3.0f);

            samusJumpAscendLeftAnim = new AnimatedSprite(m_game, m_samusMap, samusJumpAscendLeftRect, 2, 16f);
            samusJumpAscendLeftAnim.Initialize();
            samusJumpAscendLeftAnim.Loop = false;
            samusJumpAscendLeftAnim.Scale = new Vector2(3.0f, 3.0f);

            samusJumpDescendRightAnim = new AnimatedSprite(m_game, m_samusMap, samusJumpDescendRightRect, 4, 16f);
            samusJumpDescendRightAnim.Initialize();
            samusJumpDescendRightAnim.Loop = false;
            samusJumpDescendRightAnim.Scale = new Vector2(3.0f, 3.0f);
            
            samusJumpDescendLeftAnim = new AnimatedSprite(m_game, m_samusMap, samusJumpDescendLeftRect, 4, 16f);
            samusJumpDescendLeftAnim.Initialize();
            samusJumpDescendLeftAnim.Loop = false;
            samusJumpDescendLeftAnim.Scale = new Vector2(3.0f, 3.0f);

            samusJumpLandRightAnim = new AnimatedSprite(m_game, m_samusMap, samusJumpLandRightRect, 2, 16f);
            samusJumpLandRightAnim.Initialize();
            samusJumpLandRightAnim.Loop = false;
            samusJumpLandRightAnim.OnAnimEnd += this.AnimatedSpriteEnd;
            samusJumpLandRightAnim.Scale = new Vector2(3.0f, 3.0f);

            samusJumpLandLeftAnim = new AnimatedSprite(m_game, m_samusMap, samusJumpLandLeftRect, 2, 16f);
            samusJumpLandLeftAnim.Initialize();
            samusJumpLandLeftAnim.Loop = false;
            samusJumpLandLeftAnim.OnAnimEnd += this.AnimatedSpriteEnd;
            samusJumpLandLeftAnim.Scale = new Vector2(3.0f, 3.0f);

            m_playerState = PlayerState.Wait;

            m_armCannon = new Weapon(IvyGame.Get(), this, new Point(10, 10), m_playerDir);
            m_armCannon.Initialize();

            m_landEffect = Game.Content.Load<SoundEffect>("Audio\\samus_land");
            m_rollEffect = Game.Content.Load<SoundEffect>("Audio\\samus_jump_roll");
            m_rollInstance = m_rollEffect.CreateInstance();
 
            ///todo should we enter Wait with Wait state?
            m_playerAnim = samusWaitRightAnim;
            EnterState_Wait(m_playerState);

            base.Initialize();
        }

        private bool PlayerMoving()
        {
            bool moving = false;

            float deadZone = 0.1f;
            if ((m_currentGamePadState.ThumbSticks.Left.X > deadZone)||
                (m_currentGamePadState.ThumbSticks.Left.X < -deadZone))
            {
                moving = true;
            }

            return moving;
        }

        /// <summary>
        /// Allows the game component to update itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        public override void Update(GameTime gameTime)
        {
            m_gameTime = gameTime;
            m_currentGamePadState = GamePad.GetState(PlayerIndex.One);

            // update position, bounded by world

            float deadZone = 0.1f;
            if (m_currentGamePadState.ThumbSticks.Left.X > deadZone)
            {
                m_playerDir.X = 1.0f;
            }
            else if (m_currentGamePadState.ThumbSticks.Left.X < -deadZone)
            {
                m_playerDir.X = -1.0f;
            }

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

            m_armCannon.Update(gameTime);

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

            m_armCannon.Draw(spriteBatch);

            if (m_playerAnim != null)
            {
                m_playerAnim.Draw(spriteBatch, samusScreenPos);
            }   
        }

        public Point Position
        {
            get { return m_playerPos; }
        }

        public Vector2 Direction
        {
            get { return m_playerDir; }
        }

        private bool ChangedDir()
        {
            bool changedDir = false;
            if (((facingRight == true) && (m_playerDir.X < 0)) ||
                ((facingRight == false) && (m_playerDir.X > 0)))
            {
                changedDir = true;
            }

            m_game.ConsoleStr += "\nFACING RIGHT = " + facingRight + "\nPLAYER DIR = " + m_playerDir.X + "\n";
            
            return changedDir;
        }

        private void UpdatePosition()
        {
            if (PlayerMoving())
            {
                m_playerPos.X += (int)Math.Round((m_playerDir.X * m_playerSpeed.X));
            }
            m_playerPos.Y -= (int)Math.Round((m_playerDir.Y * m_playerSpeed.Y));
        }

        private void ChangeAnim(AnimatedSprite newAnim)
        {
            m_playerAnim.Stop();
            m_playerAnim = newAnim;
            m_playerAnim.Reset();
            m_playerAnim.Play();
        }

        private void FireWeapon()
        {
            m_armCannon.Fire();
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

            if (m_currentGamePadState.Buttons.A == ButtonState.Pressed)
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

            if ((m_currentGamePadState.Buttons.B == ButtonState.Pressed) &&
                (m_lastGamePadState.Buttons.B == ButtonState.Released))
            {
                FireWeapon();
            }

            //hack
            if (samusTurnAnim == m_playerAnim)
            {
                UpdatePosition();
            }

            if ((!samusTurnAnim.IsPlaying) && (PlayerMoving() == true))
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
            m_playerSpeed.X = 1.5f;
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
        private void ExecuteState_Run()
        {
            m_game.ConsoleStr += "STATE=RUN\n";

            if (m_currentGamePadState.Buttons.A == ButtonState.Pressed)
            {
                ExitState_Run(PlayerState.JumpAscend);
            }
            else if (PlayerMoving() == false)
            {
                ExitState_Run(PlayerState.Wait);
            }
            else
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

                UpdatePosition();

                if ((m_currentGamePadState.Buttons.B == ButtonState.Pressed) &&
                    (m_lastGamePadState.Buttons.B == ButtonState.Released))
                {
                    FireWeapon();
                }
            }
        }

        private void ExitState_Run(PlayerState nextState)
        {
            m_playerSpeed.X = 1.4f;
            switch (nextState)
            {
                case PlayerState.Wait:
                    EnterState_Wait(PlayerState.Run); 
                    break;
                case PlayerState.JumpAscend:
                    EnterState_JumpAscend(PlayerState.Run);
                    break;
                default:
                    break;
            };
        }

        private bool JumpRoll()
        {
            if ((m_playerAnim == samusJumpRollLeftAnim) ||
                (m_playerAnim == samusJumpRollRightAnim))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        private void EnterState_JumpAscend(PlayerState prevState)
        {
            m_playerDir.Y = 1.0f;

            if (prevState == PlayerState.Wait)
            {
                if (facingRight == true)
                {
                    ChangeAnim(samusJumpAscendRightAnim);
                }
                else
                {
                    ChangeAnim(samusJumpAscendLeftAnim);
                }
            }
            else if (prevState == PlayerState.Run)
            {
                if (facingRight == true)
                {
                    ChangeAnim(samusJumpRollRightAnim);
                }
                else
                {
                    ChangeAnim(samusJumpRollLeftAnim);
                }
                m_rollInstance.Play();
            }

            m_playerState = PlayerState.JumpAscend;
            ExecuteState_JumpAscend();
        }
        private void ExecuteState_JumpAscend()
        {
            m_game.ConsoleStr += "STATE=JUMP_ASCEND\n";
            jumpElapsedTime += m_gameTime.ElapsedGameTime.Milliseconds;

            // if not holding A button, or time ran out
            if ((m_currentGamePadState.Buttons.A == ButtonState.Released) ||
                (jumpElapsedTime >= jumpTime))
            {
                jumpElapsedTime = 0;
                ExitState_JumpAscend(PlayerState.JumpDescend);
            }
            else
            {
                if (JumpRoll() == true)
                {
                    if (ChangedDir() == true)
                    {
                        if (facingRight == true)
                        {
                            ChangeAnim(samusJumpRollRightAnim);
                        }
                        else
                        {
                            ChangeAnim(samusJumpRollLeftAnim);
                        }
                        facingRight = !facingRight;
                    }
                }
                else if (ChangedDir())
                {
                    if (facingRight == true)
                    {
                        ChangeAnim(samusJumpAscendRightAnim);
                        samusJumpAscendRightAnim.SetFrame((uint)samusJumpAscendLeftAnim.CurrentFrame);
                    }
                    else
                    {
                        ChangeAnim(samusJumpAscendLeftAnim);
                        samusJumpAscendLeftAnim.SetFrame((uint)samusJumpAscendRightAnim.CurrentFrame);
                    }
                    facingRight = !facingRight;
                }

                UpdatePosition();

            }
        }
        private void ExitState_JumpAscend(PlayerState nextState)
        {
            EnterState_JumpDescend(PlayerState.JumpAscend);
        }

        private void EnterState_JumpDescend(PlayerState prevState)
        {
            m_playerDir.Y = -1.0f;

            if (!JumpRoll())
            {
                if (facingRight == true)
                {
                    ChangeAnim(samusJumpDescendRightAnim);
                }
                else
                {
                    ChangeAnim(samusJumpDescendLeftAnim);
                }
            }
            else
            {

            }

            m_playerState = PlayerState.JumpDescend;
            ExecuteState_JumpDescend();
        }
        private void ExecuteState_JumpDescend()
        {
            m_game.ConsoleStr += "STATE=JUMP_DESCEND\n";

            // if hit floor? check position, go to wait/erm transition next anim
            if (m_playerPos.Y >= (m_worldBounds.Y + (m_worldBounds.Height / 2)))
            {
                if (m_playerDir.Y != 0)
                {
                    m_playerDir.Y = 0;
                    if (facingRight == true)
                    {
                        ChangeAnim(samusJumpLandRightAnim);
                    }
                    else
                    {
                        ChangeAnim(samusJumpLandLeftAnim);
                    }

                    m_rollInstance.Stop();
                    m_landEffect.Play();
                }

                UpdatePosition();
            }
            else
            {
                if (JumpRoll() == true)
                {
                    if (ChangedDir() == true)
                    {
                        if (facingRight == true)
                        {
                            ChangeAnim(samusJumpRollRightAnim);
                        }
                        else
                        {
                            ChangeAnim(samusJumpRollLeftAnim);
                        }
                        facingRight = !facingRight;
                    }
                }
                else if (ChangedDir())
                {
                    if (facingRight == true)
                    {
                        ChangeAnim(samusJumpDescendRightAnim);
                        samusJumpDescendRightAnim.SetFrame(samusJumpDescendLeftAnim.CurrentFrame);
                    }
                    else
                    {
                        ChangeAnim(samusJumpDescendLeftAnim);
                        samusJumpDescendLeftAnim.SetFrame(samusJumpDescendRightAnim.CurrentFrame);
                    }
                    facingRight = !facingRight;
                }

                UpdatePosition();
            }
        }
        private void ExitState_JumpDescend(PlayerState nextState)
        {
            m_playerDir.Y = 0;

            switch (nextState)
            {
                case PlayerState.Wait:
                    EnterState_Wait(PlayerState.JumpDescend);
                    break;
                default:
                    // error
                    break;
            }
        }

        #region Incomplete States
        private void EnterState_Fall(PlayerState prevState)
        {
            m_playerState = PlayerState.Fall;
            ExecuteState_Fall();
        }
        private void ExecuteState_Fall()
        {
            m_game.ConsoleStr += "STATE=JUMP_DESCEND\n";

            UpdatePosition();
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
            }

            return true;
        }

    }
}