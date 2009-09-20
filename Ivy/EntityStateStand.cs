using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ivy
{
    class EntityStateStand : State
    {
        private static EntityStateStand m_instance = null;

        private EntityStateStand()
        {

        }

        public static EntityStateStand Get()
        {
            if (m_instance == null)
            {
                m_instance = new EntityStateStand();
            }
            return m_instance;
        }

        public override void Enter(Entity entity)
        {
            entity.Moving = false;

            /*
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
             */
        }

        public override void Execute(Entity entity)
        {
           // IvyGame.Get().ConsoleStr += "STATE=WAIT\n";

            /*
             

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
             */
        }

        public override void Exit(Entity entity)
        {
            /*
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
            */
        }

        public override void HandleMessage(Entity entity, Message msg)
        {
            switch (msg.Type)
            {
                case MessageType.MoveLeft:
                case MessageType.MoveRight:
                    entity.ChangeState(EntityStateMove.Get());
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
