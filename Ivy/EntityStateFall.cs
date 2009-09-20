using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ivy
{
    class EntityStateFall : State
    {
        private static EntityStateFall m_instance = null;

        private EntityStateFall()
        {

        }

        static public EntityStateFall Get()
        {
            if (m_instance == null)
            {
                m_instance = new EntityStateFall();
            }
            return m_instance;
        }

        public override void Enter(Entity entity)
        {
            entity.Moving = true;

            /*
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
             */
        }

        public override void Execute(Entity entity)
        {
            /*
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
            */
        }

        public override void Exit(Entity entity)
        {
            /*
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
            */
        }

        public override void HandleMessage(Entity entity, Message msg)
        {
            switch (msg.Type)
            {
                case MessageType.Land:
                    entity.ChangeState(EntityStateStand.Get());
                    break;
                default:
                    ///todo error!
                    break;
            }
        }
    }
}
