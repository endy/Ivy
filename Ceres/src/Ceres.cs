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

using Ivy;

namespace Ceres
{
    /// <summary>
    /// This is the main type for your game
    /// </summary>
    public class Ceres : Ivy.IvyGame
    {
        ///@todo add player management
        protected Player m_playerOne;

        /// <summary>
        /// WorldZone bounds/coordinates within the large image file
        /// TODO: Migrate this to a data file
        /// </summary>
        Rectangle shaftRoomRect = new Rectangle(21, 94, 256, 705);
        Rectangle openRoomRect = new Rectangle(329, 50, 512, 192);
        Rectangle stairsRoomRect = new Rectangle(334, 333, 512, 448);
        Rectangle labRoomRect = new Rectangle(48, 873, 512, 192);
        Rectangle corridorRoomRect = new Rectangle(704, 690, 512, 192);
        Rectangle encounterRoomRect = new Rectangle(565, 1133, 256, 192);

        public Ceres()
        {

        }

        protected override void Initialize()
        {
            base.Initialize();

            // TODO: Find out if components need to be individually initialized here,
            //       XNA may provide a way of doing this.
            m_playerOne = new Player(this);
            m_playerOne.Initialize();

            CameraTarget = m_playerOne;

            // Create Camera
            Rectangle screenRect = new Rectangle(0, 0, 800, 600);

            int cameraHeight = 192; // roomBounds.Height;
            int cameraWidth = 256; // (int)(roomBounds.Height * (screenRect.Width / (float)screenRect.Height));
            Rectangle cameraBounds = new Rectangle(0, 0, cameraWidth, cameraHeight);

            Rectangle roomBounds = new Rectangle(0, 0, shaftRoomRect.Width, shaftRoomRect.Height);

            Camera.Initialize(roomBounds, cameraBounds, screenRect);

            Texture2D roomBackground = IvyGame.Get().Content.Load<Texture2D>("Sprites\\environment");

            WorldZone mainRoom = new WorldZone(World, roomBounds, roomBackground, shaftRoomRect);
            mainRoom.Initialize();

            // Add WorldZone Entities
            Skree skree = null;

            Random r = new Random();
            for (int i = 0; i < 0; i++)
            {
                skree = new Skree(this);
                skree.Initialize();
                skree.Position = new Point(r.Next() % 400, r.Next() % 150);
                mainRoom.AddEntity(skree);
            }

            World.SetCurrentRoom(mainRoom);

            mainRoom.AddEntity(m_playerOne);

            #region Register Input Handlers
            // Movement
            InputMgr.Get().RegisterGamePadButton(Buttons.LeftThumbstickLeft, OnGamePadDirectionEvent);
            InputMgr.Get().RegisterGamePadButton(Buttons.LeftThumbstickRight, OnGamePadDirectionEvent);
            InputMgr.Get().RegisterGamePadButton(Buttons.LeftThumbstickUp, OnGamePadDirectionEvent);
            InputMgr.Get().RegisterGamePadButton(Buttons.LeftThumbstickDown, OnGamePadDirectionEvent);

            InputMgr.Get().RegisterGamePadButton(Buttons.DPadLeft, OnGamePadDirectionEvent);
            InputMgr.Get().RegisterGamePadButton(Buttons.DPadRight, OnGamePadDirectionEvent);
            InputMgr.Get().RegisterGamePadButton(Buttons.DPadUp, OnGamePadDirectionEvent);
            InputMgr.Get().RegisterGamePadButton(Buttons.DPadDown, OnGamePadDirectionEvent);

            InputMgr.Get().RegisterKey(Keys.Up, OnKeyboardDirectionEvent);
            InputMgr.Get().RegisterKey(Keys.Down, OnKeyboardDirectionEvent);
            InputMgr.Get().RegisterKey(Keys.Left, OnKeyboardDirectionEvent);
            InputMgr.Get().RegisterKey(Keys.Right, OnKeyboardDirectionEvent);

            // Actions
            InputMgr.Get().RegisterGamePadButton(Buttons.A, OnGamePadButtonEvent); // Run/Alt
            InputMgr.Get().RegisterGamePadButton(Buttons.B, OnGamePadButtonEvent); // Jump
            InputMgr.Get().RegisterGamePadButton(Buttons.Y, OnGamePadButtonEvent); // Fire Weapon

            // Keyboard Actions
            InputMgr.Get().RegisterKey(Keys.F, OnKeyboardEvent);                    // Fire
            InputMgr.Get().RegisterKey(Keys.Space, OnKeyboardEvent);                // Jump

            // Debug Actions
            InputMgr.Get().RegisterGamePadButton(Buttons.X, DebugOnGamePadButtonEvent);
            #endregion
        }

        protected override void LoadContent()
        {
            base.LoadContent();
        }

        #region Input Handlers

        bool OnGamePadDirectionEvent(GamePadButtonEvent e)
        {
            // TODO: design some way to map events to messages to avoid switch code like this
            if ((e.Button == Buttons.LeftThumbstickLeft) ||
                (e.Button == Buttons.DPadLeft))
            {
                if (e.EventType == InputEventType.Pressed)
                {
                    Message msg = new Message(MessageType.MoveLeft, this, m_playerOne);
                    MessageDispatcher.Get().SendMessage(msg);
                }
                else if ((e.EventType == InputEventType.Released) &&
                         (InputMgr.Get().GamePadState.IsButtonUp(Buttons.LeftThumbstickRight)) &&
                         (InputMgr.Get().GamePadState.IsButtonUp(Buttons.DPadRight)))
                {
                    // TODO: need a better way of handling mutually exclusive button inputs

                    Message msg = new Message(MessageType.Stand, this, m_playerOne);
                    MessageDispatcher.Get().SendMessage(msg);
                }
            }
            else if ((e.Button == Buttons.LeftThumbstickRight) ||
                     (e.Button == Buttons.DPadRight))
            {
                if (e.EventType == InputEventType.Pressed)
                {
                    Message msg = new Message(MessageType.MoveRight, this, m_playerOne);
                    MessageDispatcher.Get().SendMessage(msg);
                }
                else if ((e.EventType == InputEventType.Released) &&
                         (InputMgr.Get().GamePadState.IsButtonUp(Buttons.LeftThumbstickLeft)) &&
                         (InputMgr.Get().GamePadState.IsButtonUp(Buttons.DPadLeft)))
                {
                    // TODO: need a better way of handling mutually exclusive button inputs

                    Message msg = new Message(MessageType.Stand, this, m_playerOne);
                    MessageDispatcher.Get().SendMessage(msg);
                }
            }

            return true;
        }

        bool OnKeyboardDirectionEvent(KeyboardEvent e)
        {
            // TODO: design some way to map events to messages to avoid switch code like this

            // TODO:  add some intelligence here to make sure that left and right keys are mutually exclusive
            if (e.Key == Keys.Left)
            {
                if (e.EventType == InputEventType.Pressed)
                {
                    Message msg = new Message(MessageType.MoveLeft, this, m_playerOne);
                    MessageDispatcher.Get().SendMessage(msg);
                }
                else if (e.EventType == InputEventType.Released)
                {
                    Message msg = new Message(MessageType.Stand, this, m_playerOne);
                    MessageDispatcher.Get().SendMessage(msg);
                }
            }
            else if (e.Key == Keys.Right)
            {
                if (e.EventType == InputEventType.Pressed)
                {
                    Message msg = new Message(MessageType.MoveRight, this, m_playerOne);
                    MessageDispatcher.Get().SendMessage(msg);
                }
                else if (e.EventType == InputEventType.Released)
                {
                    Message msg = new Message(MessageType.Stand, this, m_playerOne);
                    MessageDispatcher.Get().SendMessage(msg);
                }
            }

            return true;
        }

        bool OnGamePadButtonEvent(GamePadButtonEvent e)
        {
            if (e.Button == Buttons.B)
            {
                if (e.EventType == InputEventType.Pressed)
                {
                    // start jump
                    Message msg = new Message(MessageType.Jump, this, m_playerOne);
                    MessageDispatcher.Get().SendMessage(msg);
                }
                else if (e.EventType == InputEventType.Released)
                {
                    // end jump!
                    Message msg = new Message(MessageType.Fall, this, m_playerOne);
                    MessageDispatcher.Get().SendMessage(msg);
                }
            }

            if ((e.Button == Buttons.Y) && (e.EventType == InputEventType.Pressed))
            {
                // Fire
                Message msg = new Message(MessageType.FireWeapon, this, m_playerOne);
                MessageDispatcher.Get().SendMessage(msg);
            }

            return true;
        }

        bool DebugOnGamePadButtonEvent(GamePadButtonEvent e)
        {
            return true;
        }

        bool OnKeyboardEvent(KeyboardEvent e)
        {
            // Jump!
            if (e.Key == Keys.Space)
            {
                if (e.EventType == InputEventType.Pressed)
                {
                    // start jump
                    Message msg = new Message(MessageType.Jump, this, m_playerOne);
                    MessageDispatcher.Get().SendMessage(msg);
                }
                else if (e.EventType == InputEventType.Released)
                {
                    // end jump
                    Message msg = new Message(MessageType.Fall, this, m_playerOne);
                    MessageDispatcher.Get().SendMessage(msg);
                }
            }

            if ((e.Key == Keys.F) && (e.EventType == InputEventType.Pressed))
            {
                // Fire
                Message msg = new Message(MessageType.FireWeapon, this, m_playerOne);
                MessageDispatcher.Get().SendMessage(msg);
            }

            return true;
        }
        #endregion
    }
}
