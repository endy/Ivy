using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace Ivy
{
    public class Entity : Microsoft.Xna.Framework.GameComponent, IMessageReceiver, IMessageSender
    {
        // @todo Add bCollidable property...
        protected WorldZone WorldZone { get; private set; }

        public Point Position { get; set; }
        public Vector2 Direction { get; set; }

        public bool Moving { get; set; }

        protected AnimGraph m_animGraph;
        protected Box m_box;

        protected Vector2 m_speed;

        public Vector2 CurrentSpeed { get; set; }

        protected Rectangle m_StaticCollisionRect;

        // TODO: query this in the jump state?
        //int jumpTime = 800;
        //int jumpElapsedTime = 0;

        protected StateMgr m_entityStateMgr;

        public Entity(Game game) :
            base(game)
        {

        }

        public override void Initialize()
        {
            Position = new Point(0, 0);
            Direction = new Vector2(1f, 0f);

            Moving = false;

            m_speed = new Vector2(0.2f, 0.3f);
            CurrentSpeed = Vector2.Zero;

            m_entityStateMgr = new StateMgr(this);
            m_entityStateMgr.Initialize();

            m_box = new Box(Game);
            m_box.Initialize();

            // use static collision rect for now
            if (m_animGraph != null)
            {
                m_StaticCollisionRect = m_animGraph.GetCurrentNode().Anim.GetFrameBounds();
            }
            else
            {
                m_StaticCollisionRect = new Rectangle(0, 0, 1, 1);
            }
        }

        public override void Update(GameTime gameTime)
        {
            base.Update(gameTime);

            m_entityStateMgr.Update();

            if (Moving)
            {
                UpdatePosition(gameTime);
            }
        }

        public virtual void Draw(SpriteBatch spriteBatch)
        {

        }

        public virtual void Draw3D()
        {
            Rectangle boxRect = CollisionRect();

            //@todo remove this hack...
            if (m_animGraph != null)
            {
                boxRect.Width = (int)(boxRect.Width / 256f * 800);
                boxRect.Height = (int)(boxRect.Height / 192f * 600);
            }

            m_box.UpdateRect(boxRect);

            m_box.Draw(Position); 
        }

        public Rectangle CollisionRect()
        {
            Rectangle rect = m_StaticCollisionRect;

            return new Rectangle(Position.X, Position.Y, rect.Width, rect.Height);
        }

        public virtual void ReceiveMessage(Message msg)
        {
            m_entityStateMgr.HandleMessage(msg);

            if (msg.Type == MessageType.MoveLeft)
            {
                Direction = new Vector2(-1f, Direction.Y);
                CurrentSpeed = new Vector2(m_speed.X, CurrentSpeed.Y);
            }
            else if (msg.Type == MessageType.MoveRight)
            {
                Direction = new Vector2(1f, Direction.Y);
                CurrentSpeed = new Vector2(m_speed.X, CurrentSpeed.Y);
            }
            else if (msg.Type == MessageType.Stand)
            {
                CurrentSpeed = new Vector2(0f, CurrentSpeed.Y);
            }
            else if (msg.Type == MessageType.Jump)
            {
                Direction = new Vector2(Direction.X, -1f);
                CurrentSpeed = new Vector2(CurrentSpeed.X, m_speed.Y);
            }
            else if (msg.Type == MessageType.Fall)
            {
                Direction = new Vector2(Direction.X, 1f);
                CurrentSpeed = new Vector2(CurrentSpeed.X, m_speed.Y);
            }
            else if (msg.Type == MessageType.Land)
            {
                CurrentSpeed = new Vector2(CurrentSpeed.X, 0f);
            }

            // if moving message...update direction?
            // or should that be done on state change??
            // and do we query input mgr or do we do it some other way

        }
        public void ChangeState(State nextState)
        {
            m_entityStateMgr.ChangeState(nextState);

            // TODO: update entity properties based on new state
            //       or should this be in a seperate entity method that the state calls?
            //       does it matter?  -- maybe it does -- so the state has more 'control' over the entity
        }

        private void UpdatePosition(GameTime gameTime)
        {
            int dx = (int)(CurrentSpeed.X * Direction.X * gameTime.ElapsedGameTime.Milliseconds);
            int dy = (int)(CurrentSpeed.Y * Direction.Y * gameTime.ElapsedGameTime.Milliseconds);

            Position = new Point(Position.X + dx, Position.Y + dy);
        }

        public void ChangeZone(WorldZone zone, Point position)
        {
            ///@todo need 'ExitZone' method?
            WorldZone = zone;

            Position = position;
        }
    }
}
