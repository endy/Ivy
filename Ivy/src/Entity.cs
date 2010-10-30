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
        // @todo Add 'flying'/'floating' property... ? avoids gravity?

        protected WorldZone WorldZone { get; private set; }

        ///@todo Consider private set
        public Point Position { get; set; }
        public Point LastPosition { get; set; }
        public Vector2 Direction { get; set; }

        public bool Moving { get; set; }
        public bool Movable { get; set; }

        protected AnimGraph m_animGraph;
        protected Box m_box;
        protected Vector2 m_speed;
        public Vector2 CurrentSpeed { get; set; }
        protected Rectangle m_StaticCollisionRect;
        protected StateMgr m_entityStateMgr;

        protected Entity m_platform;

        public State CurrentState
        {
            get
            {
                return m_entityStateMgr.CurrentState;
            }
        }

        public Entity(Game game) :
            base(game)
        {
            Movable = false;
        }

        public override void Initialize()
        {
            Position = new Point(0, 0);
            LastPosition = Position;

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

            if (m_platform != null)
            {
                Vector2 p = GetPositionAtTime(gameTime.ElapsedGameTime.Milliseconds);
                
                Rectangle cRect = CollisionRect();
                cRect.X = (int) p.X;
                cRect.Y = (int)(p.Y + WorldZone.GravityConstant.Y);

                if (m_platform.CollisionRect().Intersects(cRect) == false)
                {
                    MessageDispatcher.Get().SendMessage(new Message(MessageType.Fall, this, this));
                }
            }

            m_entityStateMgr.Update();
        }

        public virtual void Draw(SpriteBatch spriteBatch)
        {

        }

        public virtual void Draw3D()
        {
            Rectangle boxRect = CollisionRect();

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

            if (msg.Type == MessageType.CollideWithEntity)
            {
                Rectangle collisionRect = CollisionRect();
                Rectangle footRect =
                    new Rectangle(collisionRect.X, collisionRect.Y + collisionRect.Height - 1, collisionRect.Width, 3);
                Rectangle headRect = new Rectangle(footRect.X, collisionRect.Y, footRect.Width, 3);

                EntityCollisionMsg entMsg = (EntityCollisionMsg)msg;

                if (entMsg.EntityHit.Movable == false)
                {
                    int dx = Position.X;
                    int dy = Position.Y;
                    
                    Rectangle entRect = entMsg.EntityHit.CollisionRect();

                    bool footHit = footRect.Intersects(entRect);
                    bool headHit = headRect.Intersects(entRect);

                    if (footHit != true && headHit != true)
                    {
                        if (Position.X - LastPosition.X > 0)
                        {
                            dx = entRect.X - collisionRect.Width;
                        }
                        else if (Position.X - LastPosition.X < 0)
                        {
                            dx = entRect.X + entRect.Width;
                        }
                    }
                    else if (footHit == true)
                    {
                        int snapY = entMsg.EntityHit.Position.Y - CollisionRect().Height;

                        if (Math.Abs(dy - snapY) < 10)
                        {
                            dy = snapY;
                            MessageDispatcher.Get().SendMessage(new Message(MessageType.Land, this, this));

                            // standing on this platform
                            m_platform = entMsg.EntityHit;
                        }
                        else
                        {
                            if (Position.X - LastPosition.X > 0)
                            {
                                dx = entRect.X - collisionRect.Width;
                            }
                            else if (Position.X - LastPosition.X < 0)
                            {
                                dx = entRect.X + entRect.Width;
                            }
                        }
                    }
                    else if (headHit == true)
                    {
                        int snapY = entMsg.EntityHit.Position.Y + entRect.Height;

                        if (Math.Abs(dy - snapY) < 10)
                        {
                            dy = snapY;
                        }
                        else
                        {
                            if (Position.X - LastPosition.X > 0)
                            {
                                dx = entRect.X - collisionRect.Width;
                            }
                            else if (Position.X - LastPosition.X < 0)
                            {
                                dx = entRect.X + entRect.Width;
                            }
                        }
                    }
                    
                    Position = new Point(dx, dy);
                }
            }

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
                m_platform = null;
            }
            else if (msg.Type == MessageType.Fall)
            {
                Direction = new Vector2(Direction.X, 1f);
                CurrentSpeed = new Vector2(CurrentSpeed.X, m_speed.Y);
                m_platform = null;
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

        public Vector2 GetPositionAtTime(int elapsedTimeMS)
        {
            int dx = (int)(CurrentSpeed.X * Direction.X * elapsedTimeMS);
            int dy = (int)(CurrentSpeed.Y * Direction.Y * elapsedTimeMS);

            // restrict movement to within world zone
            int x = Math.Max(WorldZone.Bounds.Left, Math.Min(WorldZone.Bounds.Right - CollisionRect().Width, Position.X + dx));
            int y = Math.Max(WorldZone.Bounds.Top, Math.Min(WorldZone.Bounds.Bottom - CollisionRect().Height, Position.Y + dy));

            return new Vector2(x, y);
        }

        public void ChangeZone(WorldZone zone, Point position)
        {
            ///@todo need 'ExitZone' method?
            WorldZone = zone;

            LastPosition = Position;
            Position = position;
        }
    }
}
