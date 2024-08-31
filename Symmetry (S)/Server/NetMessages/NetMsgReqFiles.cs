/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

using Symmetry.Util;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Symmetry.Server.NetMessages
{
    class NetMsgReqFiles : NetworkMessage
    {
        private int Branch;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public NetMsgReqFiles()
        ///
        /// \brief  Default constructor.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public NetMsgReqFiles(int branch)
        {
            this.Branch = branch;
        }

        public override void Send(BinaryReader r, BinaryWriter w, Client client)
        {
            List<SvnFile> files = FileManager.GetFilesInBranch(this.Branch);

            // No timing out whilst downloading..
            client.InternalTcpClient.SendTimeout    = 0;
            client.InternalTcpClient.ReceiveTimeout = 0;

            // Send each file as a network message
            foreach (SvnFile file in files)
            {
                client.SendNetworkMessage(new NetMsgSndFile(file));
            }

            // NOW, after the user has download files, we can use real timeouts
            client.InternalTcpClient.SendTimeout = Convert.ToInt32(Properties.Settings.Default.SendTimeout);
            client.InternalTcpClient.ReceiveTimeout = Convert.ToInt32(Properties.Settings.Default.RecvTimeout);
        }
    }
}
