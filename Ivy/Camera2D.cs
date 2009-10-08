using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace Ivy
{
    public class Camera2D
    {
        private Rectangle m_roomBounds;
        private Rectangle m_cameraBounds;   // in room coordinates
        private Rectangle m_screenRect;     // in screen coordinates

        // calculated camera rect
        private Rectangle m_cameraRect;     // in room coordinates

        private Vector2 m_speed;

        public Camera2D(Rectangle roomBounds, Rectangle cameraBounds, Rectangle screenRect)
        {
            m_roomBounds = roomBounds;            
            m_cameraBounds = cameraBounds;
            m_screenRect = screenRect;

            m_cameraRect = Rectangle.Empty;
            m_cameraRect.Width = cameraBounds.Width;
            m_cameraRect.Height = cameraBounds.Height;

            m_speed = new Vector2(.3f, .3f);
        }

        public Rectangle CameraRect
        {
            get
            {
                return m_cameraRect;
            }
        }

        public Vector2 GetRoomPointForScreenPoint(Vector2 screenPoint)
        {
            Vector2 roomPoint = Vector2.Zero;

            if ((m_screenRect.Left <= screenPoint.X) &&
                (screenPoint.X < m_screenRect.Right) &&
                (m_screenRect.Top <= screenPoint.Y) &&
                (screenPoint.Y < m_screenRect.Bottom))
            {
                roomPoint.X = m_cameraRect.X + ((screenPoint.X / (float)m_screenRect.Width) * m_cameraRect.Width);
                roomPoint.Y = m_cameraRect.Y + ((screenPoint.Y / (float)m_screenRect.Height) * m_cameraRect.Height);
            }
            else
            {
                // TODO: probably an error
            }

            return roomPoint;
        }

        public Vector2 GetScreenPointForRoomPoint(Vector2 roomPoint)
        {
            Vector2 screenPoint = Vector2.Zero;

            if ((m_roomBounds.Left <= roomPoint.X) &&
                (roomPoint.X < m_roomBounds.Right) &&
                (m_roomBounds.Top <= roomPoint.Y) &&
                (roomPoint.Y < m_roomBounds.Bottom))
            {
                screenPoint.X = ((roomPoint.X - m_cameraRect.X) / (float)m_cameraRect.Width) * m_screenRect.Width;
                screenPoint.Y = ((roomPoint.Y - m_cameraRect.Y) / (float)m_cameraRect.Height) * m_screenRect.Height;
            }
            else
            {
                // TODO: probably an error
            }

            return screenPoint;
        }

        public void Update(GameTime gameTime, Entity target)
        {
            
            Vector2 actualCameraCenter = new Vector2(m_cameraRect.Center.X, m_cameraRect.Center.Y);

            Vector2 idealCameraCenter = new Vector2(target.Position.X + (1 / 5f * ((float)m_cameraBounds.Width * target.Direction.X)),
                                        target.Position.Y + (1 / 10f * m_cameraBounds.Height * target.Direction.Y));

            IvyGame.Get().ConsoleStr += "Ideal Center: " + idealCameraCenter + "\n";
            IvyGame.Get().ConsoleStr += "Actual Center: " + actualCameraCenter + "\n";

            if (target.Moving)
            {
                Vector2 maxTravelDist = new Vector2((m_speed.X * gameTime.ElapsedGameTime.Milliseconds),
                                                    (m_speed.Y * gameTime.ElapsedGameTime.Milliseconds));

                Vector2 newCameraCenter = idealCameraCenter;
                
                if (Math.Abs(idealCameraCenter.X - actualCameraCenter.X) > maxTravelDist.X)
                {
                    // transition!
                    if ((idealCameraCenter.X - actualCameraCenter.X) < 0)
                    {
                        newCameraCenter.X = actualCameraCenter.X - maxTravelDist.X;
                    }
                    else if ((idealCameraCenter.X - actualCameraCenter.X) > 0)
                    {
                        newCameraCenter.X = actualCameraCenter.X + maxTravelDist.X;
                    }
                }

                if (Math.Abs(idealCameraCenter.Y - actualCameraCenter.Y) > maxTravelDist.Y)
                {
                    // transition!
                    
                    //newCameraCenter.Y = actualCameraCenter.Y + (maxTravelDist.Y * target.Direction.Y);
                }
                
                
                IvyGame.Get().ConsoleStr += "new camera center: " + newCameraCenter + "\n";

                m_cameraRect.X = (int)(newCameraCenter.X - (m_cameraBounds.Width / 2f));
                m_cameraRect.Y = (int)(newCameraCenter.Y - (m_cameraBounds.Height / 2f));
                
                //*
                if (m_cameraRect.X < 0)
                {
                    m_cameraRect.X = 0;
                }
                if ((m_cameraRect.X + m_cameraRect.Width) > (m_roomBounds.X + m_roomBounds.Width))
                {
                    m_cameraRect.X = (m_roomBounds.X + m_roomBounds.Width) - m_cameraRect.Width;
                }
                if (m_cameraRect.Y < 0)
                {
                    m_cameraRect.Y = 0;
                }
                if ((m_cameraRect.Y + m_cameraRect.Height) > (m_roomBounds.Y + m_roomBounds.Height))
                {
                    m_cameraRect.Y = (m_roomBounds.Y + m_roomBounds.Height) - m_cameraRect.Height;
                }
                //*/
            }

            IvyGame.Get().ConsoleStr += "Room Bounds: " + m_roomBounds + "\n";
            IvyGame.Get().ConsoleStr += "Camera Rect: " + m_cameraRect + "\n";
        }       
    }
}
