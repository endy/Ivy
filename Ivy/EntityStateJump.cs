using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ivy
{
    class EntityStateJump : State
    {
        private static EntityStateJump m_instance = null;

        private EntityStateJump()
        {

        }

        /*
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
         */

        static public EntityStateJump Get()
        {
            if (m_instance == null)
            {
                m_instance = new EntityStateJump();
            }
            return m_instance;
        }

        public override void Enter(Entity entity)
        {
            /*
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
            */
        }

        public override void Execute(Entity entity)
        {
            /*
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
            */
        }

        public override void Exit(Entity entity)
        {
            //EnterState_JumpDescend(PlayerState.JumpAscend);
        }

        public override void HandleMessage(Entity entity, Message msg)
        {

        }
    }
}
