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
    public class Ceres : Ivy.IvyGame, IMessageSender
    {
        ///@todo add player management
        protected Player m_playerOne;

        /// <summary>
        /// WorldZone bounds/coordinates within the large image file
        /// TODO: Migrate this to a data file
        /// </summary>
        Rectangle shaftZoneRect = new Rectangle(21, 94, 256, 705);
        Rectangle openZoneRect = new Rectangle(329, 50, 512, 192);
        Rectangle stairsZoneRect = new Rectangle(334, 333, 256, 448);
        Rectangle labZoneRect = new Rectangle(48, 873, 512, 192);
        Rectangle corridorZoneRect = new Rectangle(704, 690, 512, 192);
        Rectangle encounterZoneRect = new Rectangle(565, 1133, 256, 192);

        public Ceres()
        {

        }

        protected override void Initialize()
        {
            base.Initialize();

            Texture2D zoneBackground = IvyGame.Get().Content.Load<Texture2D>("Sprites\\environment");

            #region  Create Rooms
            Rectangle shaftBounds = new Rectangle(0, 0, shaftZoneRect.Width, shaftZoneRect.Height);
            WorldZone shaftRoom = new WorldZone(shaftBounds, zoneBackground, shaftZoneRect);
            shaftRoom.Initialize();

            Rectangle openRoomBounds = new Rectangle(0, 0, openZoneRect.Width, openZoneRect.Height);
            WorldZone openRoom = new WorldZone(openRoomBounds, zoneBackground, openZoneRect);
            openRoom.Initialize();

            Rectangle stairsZoneBounds = new Rectangle(0, 0, stairsZoneRect.Width, stairsZoneRect.Height);
            WorldZone stairsZone = new WorldZone(stairsZoneBounds, zoneBackground, stairsZoneRect);
            stairsZone.Initialize();

            Rectangle labZoneBounds = new Rectangle(0, 0, labZoneRect.Width, labZoneRect.Height);
            WorldZone labZone = new WorldZone(labZoneBounds, zoneBackground, labZoneRect);
            labZone.Initialize();

            Rectangle corridorZoneBounds = new Rectangle(0, 0, corridorZoneRect.Width, corridorZoneRect.Height);
            WorldZone corridorZone = new WorldZone(corridorZoneBounds, zoneBackground, corridorZoneRect);
            corridorZone.Initialize();

            Rectangle encounterZoneBounds = new Rectangle(0, 0, encounterZoneRect.Width, encounterZoneRect.Height);
            WorldZone encounterZone = new WorldZone(encounterZoneBounds, zoneBackground, encounterZoneRect);
            encounterZone.Initialize();

            #endregion

            #region Create Entities

            ChangeZoneMsg addEntityMsg = null;

            // Add player

            // TODO: Find out if components need to be individually initialized here,
            //       XNA may provide a way of doing this.
            m_playerOne = new Player(this);
            m_playerOne.Initialize();
            addEntityMsg = new ChangeZoneMsg(this, shaftRoom, m_playerOne, shaftRoom, new Point(0, 0));
            MessageDispatcher.Get().SendMessage(addEntityMsg);

            #region Shaft Platforms

            // Shaft Platforms
            Platform p = new Platform();
            p.Initialize();
            p.SetSize(64, 16);
            addEntityMsg = new ChangeZoneMsg(this, shaftRoom, p, shaftRoom, new Point(181 - shaftZoneRect.X, 446 - shaftZoneRect.Y));
            MessageDispatcher.Get().SendMessage(addEntityMsg);

            p = new Platform();
            p.Initialize();
            p.SetSize(64, 16);
            addEntityMsg = new ChangeZoneMsg(this, shaftRoom, p, shaftRoom, new Point(53 - shaftZoneRect.X, 462 - shaftZoneRect.Y));
            MessageDispatcher.Get().SendMessage(addEntityMsg);

            p = new Platform();
            p.Initialize();
            p.SetSize(64, 16);
            addEntityMsg = new ChangeZoneMsg(this, shaftRoom, p, shaftRoom, new Point(117 - shaftZoneRect.X, 558 - shaftZoneRect.Y));
            MessageDispatcher.Get().SendMessage(addEntityMsg);

            p = new Platform();
            p.Initialize();
            p.SetSize(80, 16);
            addEntityMsg = new ChangeZoneMsg(this, shaftRoom, p, shaftRoom, new Point(53 - shaftZoneRect.X, 654 - shaftZoneRect.Y));
            MessageDispatcher.Get().SendMessage(addEntityMsg);

#endregion

            #region Portal Entities

            // Shaft Entities
            ZonePortal portalShaftToOpen = new ZonePortal(openRoom, new Point(10, 85), new Rectangle(0, 0, 4, 32));
            portalShaftToOpen.Initialize();
            addEntityMsg = new ChangeZoneMsg(this, shaftRoom, portalShaftToOpen, shaftRoom, new Point(225, 595));
            MessageDispatcher.Get().SendMessage(addEntityMsg);

            // Open Zone Entities
            ZonePortal portalOpenToShaft = new ZonePortal(shaftRoom, new Point(175, 595), new Rectangle(0, 0, 4, 32));
            portalOpenToShaft.Initialize();
            addEntityMsg = new ChangeZoneMsg(this, openRoom, portalOpenToShaft, openRoom, new Point(5, 85));
            MessageDispatcher.Get().SendMessage(addEntityMsg);

            ZonePortal portalOpenToStairs = new ZonePortal(stairsZone, new Point(15, 80), new Rectangle(0, 0, 4, 32));
            portalOpenToStairs.Initialize();
            addEntityMsg = new ChangeZoneMsg(this, openRoom, portalOpenToStairs, openRoom, new Point(500, 85));
            MessageDispatcher.Get().SendMessage(addEntityMsg);

            // Stairs Zone Entities
            ZonePortal portalStairsToOpen = new ZonePortal(openRoom, new Point(460, 85), new Rectangle(0, 0, 4, 32));
            portalStairsToOpen.Initialize();
            addEntityMsg = new ChangeZoneMsg(this, stairsZone, portalStairsToOpen, stairsZone, new Point(10, 80));
            MessageDispatcher.Get().SendMessage(addEntityMsg);
            
            ZonePortal portalStairsToLab = new ZonePortal(labZone, new Point(10, 75), new Rectangle(0, 0, 4, 32));
            portalStairsToLab.Initialize();
            addEntityMsg = new ChangeZoneMsg(this, stairsZone, portalStairsToLab, stairsZone, new Point(240, 340));
            MessageDispatcher.Get().SendMessage(addEntityMsg);


            // Lab Entities
            ZonePortal portalLabToStairs = new ZonePortal(stairsZone, new Point(200, 340), new Rectangle(0, 0, 4, 32));
            portalLabToStairs.Initialize();
            addEntityMsg = new ChangeZoneMsg(this, labZone, portalLabToStairs, labZone, new Point(2, 75));
            MessageDispatcher.Get().SendMessage(addEntityMsg);

            #endregion

            #endregion

            SetCurrentZone(shaftRoom);

            SetCameraTarget(m_playerOne);

            // Create Camera
            Rectangle screenRect = new Rectangle(0, 0, 800, 600);

            int cameraHeight = 192;
            int cameraWidth = 256;
            Rectangle cameraBounds = new Rectangle(0, 0, cameraWidth, cameraHeight);
            Camera.Initialize(shaftBounds, cameraBounds, screenRect);


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
