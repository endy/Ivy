using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ivy
{
    class EntityStateMove : State
    {
        private static EntityStateMove m_instance = null;

        private EntityStateMove()
        {

        }

        public static EntityStateMove Get()
        {
            if (m_instance == null)
            {
                m_instance = new EntityStateMove();
            }
            return m_instance;
        }

        public override void Enter(Entity entity)
        {
            entity.Moving = true;

            /*
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
            */
        }

        public override void Execute(Entity entity)
        {
            /*
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
            */
        }

        public override void Exit(Entity entity)
        {
            
        }

        public override void HandleMessage(Entity entity, Message msg)
        {
            switch (msg.Type)
            {
                case MessageType.Stand:
                    entity.ChangeState(EntityStateStand.Get());
                    break;
                case MessageType.Jump:
                    entity.ChangeState(EntityStateJump.Get());
                    break;
                case MessageType.Fall:
                    entity.ChangeState(EntityStateFall.Get());
                    break;
                default:
                    ///todo error!
                    break;
            }
        }
    }
}
