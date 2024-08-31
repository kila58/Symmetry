using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Symmetry.Server.NetMessages
{
    class NetMsgSubEnd : NetworkMessage
    {
        public override void Send(BinaryReader r, BinaryWriter w, Client client)
        {
            // Simple message, send the network ID for subscription end events
            w.Write((short)NetMsg.SubEnd);
        }
    }
}
