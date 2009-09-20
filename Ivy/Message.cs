using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

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
}
