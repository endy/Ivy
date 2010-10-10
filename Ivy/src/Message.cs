using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;

namespace Ivy
{
    public enum MessageType
    {
        MoveLeft,
        MoveRight,
        Stand,
        Jump,
        Fall,
        Land,
        FireWeapon,

        CollideWithEntity,
        CollideWithRoom,
        ChangeZone,

        // Test Messages
        ActivateSkree,
    }

    public class EntityCollisionData
    {
        public Entity entity;
    }

    public class PortalMessageData
    {
        public Entity entity;
        public WorldZone targetZone;
    }

    public class Message
    {
        public MessageType Type { get; private set; }
        public IMessageReceiver Receiver { get; private set; }
        public IMessageSender Sender { get; private set; }
        public int SendTime { get; private set; }   /// total game time? ticks or milliseconds?

        public Message(MessageType type, IMessageSender sender, IMessageReceiver receiver)
        {
            Type = type;
            Receiver = receiver;
            Sender = sender;
            SendTime = 0;
        }

        public Message(MessageType type, IMessageSender sender, IMessageReceiver receiver, int sendTime)
        {
            Type = type;
            Receiver = receiver;
            Sender = sender;
            SendTime = sendTime;
        }
    }

    public class EntityCollisionMsg : Message
    {
        public Entity EntityHit { get; private set; }

        public EntityCollisionMsg(IMessageSender sender, IMessageReceiver receiver, Entity entityHit)
            : base(MessageType.CollideWithEntity, sender, receiver)
        {
            EntityHit = entityHit;
        }
    }

    public class ChangeZoneMsg : Message
    {
        public WorldZone DestZone { get; private set; }
        public Point DestPosition { get; private set; }
        public Entity Entity { get; private set; }

        public ChangeZoneMsg(IMessageSender sender, IMessageReceiver receiver, Entity entity, WorldZone destZone, Point destPosition)
            : base(MessageType.ChangeZone, sender, receiver)
        {
            Entity = entity;
            DestZone = destZone;
            DestPosition = destPosition;
        }
    }
}
