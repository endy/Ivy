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
        #region Transition Data
        private int m_staticWidth;
        private int m_staticHeight;
        private int m_transWidth;
        private int m_transHeight;
        #endregion

        private Rectangle m_worldBounds;
        private Rectangle m_cameraBounds;

        private Rectangle m_cameraRect;  // in world coordinates

        public Camera2D(Rectangle worldBounds, Rectangle cameraBounds)
        {
            m_worldBounds = worldBounds;
            
            m_cameraBounds = cameraBounds;

            m_cameraRect = Rectangle.Empty;
            m_cameraRect.Width = cameraBounds.Width;
            m_cameraRect.Height = cameraBounds.Height;

            #region Transition Data
            m_staticWidth = (int)(m_worldBounds.Width * 0.05f);
            m_staticHeight = (int)(m_worldBounds.Height * 0.01f);
            m_transWidth = (int)(m_worldBounds.Width * 0.10f);
            m_transHeight = (int)(m_worldBounds.Height * 0.0f);
            #endregion
        }

        public Rectangle CameraRect
        {
            get
            {
                return m_cameraRect;
            }
        }

        #region Transition Helpers
        private bool InCenterX(Point focusPos)
        {
            bool inCenter = false;

            if ((focusPos.X > (m_staticWidth + m_transWidth)) &&
                (focusPos.X < (m_worldBounds.Width - (m_staticWidth + m_transWidth))))
            {
                inCenter = true;
            }

            return inCenter;
        }

        private bool InCenterY(Point focusPos)
        {
            bool inCenter = false;

            if ((focusPos.Y > (m_staticHeight + m_transHeight)) &&
                (focusPos.Y < (m_worldBounds.Height - (m_staticHeight + m_transHeight))))
            {
                inCenter = true;
            }

            return inCenter;
        }

        private bool InStaticX(Point focusPos)
        {
            bool inStatic = false;

            if ((focusPos.X < m_staticWidth) ||
                (focusPos.X > (m_worldBounds.Width - m_staticWidth)))
            {
                inStatic = true;
            }

            return inStatic;
        }

        private bool InStaticY(Point focusPos)
        {
            bool inStatic = false;

            if ((focusPos.Y < m_staticHeight) ||
                (focusPos.Y > (m_worldBounds.Height - m_staticHeight)))
            {
                inStatic = true;
            }

            return inStatic;
        }
        #endregion

        public void Update(Point focusPos)
        {
            ///@todo Determine focus point in camera? or in game logic?
            ///      because focus point changes based on character direction!
            ///      Probably makes sense to have the camera be a little smarter 
            ///      (i.e. also knows character direction)

            ///@todo Clamp Focus To World Bounds?            

            // center always
            m_cameraRect.X = focusPos.X - (m_cameraBounds.Width / 2);
            m_cameraRect.Y = focusPos.Y - (m_cameraBounds.Height / 2);

            #region Prototype Transition Code
            /*
            
            int worldCenterX = m_worldBounds.Width / 2;
            int worldCenterY = m_worldBounds.Height / 2;
             
            if (m_staticWidth * 2 >= m_worldBounds.Width)
            {
                // camera is always static, no need to do anything
            }
            else if ((m_staticWidth * 2 + m_transWidth * 2) >= m_worldBounds.Width)
            {
                // transition from static to static
                if (InStaticX(focusPos))
                {

                }
                else
                {
                    // in transition
                }
            }
            else
            {
                // center exists
                if (InCenterX(focusPos))
                {
                    int halfCameraWidth = m_cameraBounds.Width / 2;
                    m_cameraRect.X = focusPos.X - halfCameraWidth;

                    bInCenterX = true;
                    bInStaticX = false;
                }
                else if (InStaticX(focusPos))
                {
                    bInStaticX = true;
                    bInCenterX = false;

                    
                }
                else
                {
                    bInCenterX = false;
                    bInStaticX = false;

                    if (focusPos.X < worldCenterX)
                    {
                        // focus is left of world center

                        float leftWorldEdge = m_worldBounds.X;
                        float leftStaticEdge = leftWorldEdge + m_staticWidth;
                        float leftTransEdge = leftStaticEdge + m_transWidth;

                        float percentPassedTrans = (focusPos.X - leftStaticEdge) / (leftTransEdge - leftStaticEdge);

                        float leftCameraEdgeAtTrans = (leftTransEdge - (m_cameraRect.Width / 2.0f));
                        m_cameraRect.X = (int)(leftCameraEdgeAtTrans * percentPassedTrans);

                    }
                    else
                    {
                        float rightWorldEdge = m_worldBounds.X + m_worldBounds.Width;
                        float rightStaticEdge = rightWorldEdge - m_staticWidth;
                        float rightTransEdge = rightStaticEdge - m_transWidth;

                        float percentPassedTrans = (focusPos.X - rightTransEdge) / (rightStaticEdge - rightTransEdge);

                        float rightCameraEdgeAtTrans = (rightTransEdge + m_cameraRect.Width / 2.0f);
                        float transDelta = (rightWorldEdge - rightCameraEdgeAtTrans) * percentPassedTrans;

                        m_cameraRect.X = (int)(rightCameraEdgeAtTrans + transDelta) - m_cameraRect.Width;

                    }

                }  
            }
            */

            /*
            if (m_staticHeight * 2 >= m_worldBounds.Height)
            {
                // camera is always static
            }
            else if ((m_staticHeight * 2 + m_transHeight * 2) >= m_worldBounds.Height)
            {
                // transition from static to static
            }
            else
            {
                // center exists
            }
            */
            #endregion

            if (m_cameraRect.X < 0)
            {
                m_cameraRect.X = 0;
            }
            if ((m_cameraRect.X + m_cameraRect.Width) > (m_worldBounds.X + m_worldBounds.Width))
            {
                m_cameraRect.X = (m_worldBounds.X + m_worldBounds.Width) - m_cameraRect.Width;
            }
            if (m_cameraRect.Y < 0)
            {
                m_cameraRect.Y = 0;
            }
            if ((m_cameraRect.Y + m_cameraRect.Height) > (m_worldBounds.Y + m_worldBounds.Height))
            {
                m_cameraRect.Y = (m_worldBounds.Y + m_worldBounds.Height) - m_cameraRect.Height;
            }
        }

        public void Draw(SpriteBatch spriteBatch)
        {
            // draw camera info here (call to console?)
        }
       
    }
}
