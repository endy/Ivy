using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace Ivy
{
    public interface IAnimGraphNode
    {
        AnimatedSprite Anim { get; }
    }

    public class AnimGraph : IMessageReceiver
    {
        #region AnimGraphNode class

        private class AnimGraphNode : IAnimGraphNode
        {
            public AnimatedSprite Anim { get; private set; }

            protected AnimGraph m_graph;

            private Dictionary<MessageType, AnimGraphNode> m_adjacentNodes;

            public AnimGraphNode(AnimGraph graph, AnimatedSprite anim)
            {
                m_graph = graph;
                Anim = anim;

                m_adjacentNodes = new Dictionary<MessageType, AnimGraphNode>();
            }

            public virtual bool HandleMessage(Message msg)
            {
                bool handledMessage = false;
                if (m_adjacentNodes.ContainsKey(msg.Type) == true)
                {
                    m_graph.SetCurrentNode(m_adjacentNodes[msg.Type]);
                    handledMessage = true;

                    //IvyGame.Get().ConsoleStr += " Handled Message: " + msg.Type + " in " + Anim.Name + "\n";
                }
                else
                {
                    //IvyGame.Get().ConsoleStr += " Unhandled Message: " + msg.Type + " in " + Anim.Name + "\n";
                }

                return handledMessage;
            }

            public virtual bool AddTransition(MessageType msgType, AnimGraphNode endNode)
            {
                bool added = false;
                if (m_adjacentNodes.ContainsKey(msgType) == false)
                {
                    m_adjacentNodes.Add(msgType, endNode);
                    added = true;
                }

                return added;
            }

            public virtual bool AddTransition(AnimGraphNode previousNode, AnimGraphNode nextNode)
            {
                return false;
            }

            public virtual bool RemoveTransition(MessageType msgType)
            {
                bool removed = false;
                if (m_adjacentNodes.ContainsKey(msgType) == true)
                {
                    m_adjacentNodes.Remove(msgType);
                    removed = true;
                }

                return removed;
            }
        }

        private class AnimGraphTransitionNode : AnimGraphNode
        {
            private AnimGraphNode m_previousNode;
            private AnimGraphNode m_nextNode;

            public AnimGraphTransitionNode(AnimGraph graph, AnimatedSprite anim)
                : base(graph, anim)
            {
                Anim.OnAnimEnd += AnimatedSpriteEnd;
            }
            
            public override bool AddTransition(AnimGraphNode previousNode, AnimGraphNode nextNode)
            {
                m_previousNode = previousNode;
                m_nextNode = nextNode;

                return true;
            }

            public bool AnimatedSpriteEnd(AnimatedSprite anim)
            {
                if (m_graph.GetCurrentNode() == this)
                {
                    m_graph.SetCurrentNode(m_nextNode);
                }

                return true;
            } 
        }

        #endregion

        Entity m_owner;
        AnimGraphNode m_currentNode;

        List<AnimGraphNode> m_nodeList;

        public AnimGraph(Entity owner)
        {
            m_owner = owner;
        }

        public void Initialize()
        {
            m_nodeList = new List<AnimGraphNode>();
        }

        public void ReceiveMessage(Message msg)
        {
            if (m_currentNode != null)
            {
                m_currentNode.HandleMessage(msg);
            }
        }

        public void Update(GameTime gameTime)
        {
            if (m_currentNode != null)
            {
                m_currentNode.Anim.Update(gameTime);
            }
        }

        public void Draw(SpriteBatch spriteBatch)
        {
            if (m_currentNode != null)
            {
                m_currentNode.Anim.Draw(spriteBatch, m_owner.Position);
            }
        }

        public IAnimGraphNode GetCurrentNode()
        {
            return m_currentNode;
        }

        public void SetCurrentNode(IAnimGraphNode iNode)
        {
            AnimGraphNode node = GetNode(iNode);

            // set node to current node
            if (node != null)
            {
                if (m_currentNode != null)
                {
                    //IvyGame.Get().ConsoleStr += "Change Node: " + m_currentNode.Anim.Name + " -> " + node.Anim.Name + "\n";
                    m_currentNode.Anim.Stop();

                    // a hack for syncing animations
                    // TODO: expose some way of turning this sync 'off'
                    if (m_currentNode.Anim != node.Anim)
                    {
                        node.Anim.Reset();
                    }
                }                

                m_currentNode = node;
                m_currentNode.Anim.Play();
            }
            else
            {
                // TODO: error
            }
        }

        public IAnimGraphNode AddAnim(AnimatedSprite anim)
        {
            AnimGraphNode node = new AnimGraphNode(this, anim);

            m_nodeList.Add(node);

            return node;
        }

        public IAnimGraphNode AddTransitionAnim(AnimatedSprite anim)
        {
            AnimGraphNode node = new AnimGraphTransitionNode(this, anim);

            m_nodeList.Add(node);

            return node;
        }

        public bool AddTransition(IAnimGraphNode iStartNode, MessageType msgType, IAnimGraphNode iEndNode)
        {
            AnimGraphNode startNode = GetNode(iStartNode);
            AnimGraphNode endNode = GetNode(iEndNode);

            bool added = false;

            if ((startNode != null) && (endNode != null))
            {
                startNode.AddTransition(msgType, endNode);
                added = true;
            }
            else
            {
                // TODO: error!
            }

            return added;
        }

        public bool AddTransition(IAnimGraphNode iStartNode, MessageType msgType, IAnimGraphNode iTransNode, IAnimGraphNode iEndNode)
        {
            AnimGraphNode startNode = GetNode(iStartNode);
            AnimGraphNode transNode = GetNode(iTransNode);
            AnimGraphNode endNode = GetNode(iEndNode);

            bool added = false;

            if ((startNode != null) && (endNode != null) && (transNode != null))
            {
                if ((startNode.AddTransition(msgType, transNode) == true) &&
                    (transNode.AddTransition(startNode, endNode) == true))
                {
                    added = true;
                }
            }

            return added;
        }

        // TODO: why would i remove a node?  is there an anim graph editor?
        public void RemoveNode(IAnimGraphNode node)
        {
            // find node and remove it
            // find all references to node in other nodes, and remove it
        }

        // TODO: why would i remove a transition?  is there an anim graph editor?
        public void RemoveTransition(IAnimGraphNode start, MessageType msg, IAnimGraphNode end)
        {
            
        }

        public bool Contains(IAnimGraphNode node)
        {
            bool containsNode = false;
            foreach (AnimGraphNode n in m_nodeList)
            {
                if (n == node)
                {
                    containsNode = true;
                }
            }

            return containsNode;
        }

        private AnimGraphNode GetNode(IAnimGraphNode node)
        {
            foreach (AnimGraphNode n in m_nodeList)
            {
                if (n == node)
                {
                    return n;
                }
            }

            return null;
        }
    }
}
