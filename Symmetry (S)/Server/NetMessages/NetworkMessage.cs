/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Symmetry.Server.NetMessages
{
    class NetworkMessage
    {
        public virtual void Send(BinaryReader r, BinaryWriter w, Client client)
        {

        }
    }
}