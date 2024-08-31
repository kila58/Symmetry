/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

using Symmetry.Console;
using Symmetry.Server.NetMessages;
using Symmetry.UserManagement;
using Symmetry.Util;
using Symmetry.Util.Crypto;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Symmetry.Server
{
    // Network message ID's
    enum NetMsg
    {
        Ping,		// i -> s: keeps an active login connection alive
        ReqAes,		// i -> s: requests generated aes key & iv from the server
        ReqFiles,	// i -> s: requests files from the server
        Update,		// s -> i: notifaction of svn update
        SendFile,	// s -> i: sends a single file from the server to the injector 
        SubEnd,		// s -> i: tells the injector that our subscription ended
    };

    partial class Client
    {
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn private void ProcessNetMessage(BinaryReader r, BinaryWriter w)
        ///
        /// \brief  Process a single network message from r (assumed to be wrapping a network
        ///         socket).
        ///
        /// \param  r   The input stream.
        /// \param  w   The output stream.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        private void ProcessNetMessage(BinaryReader r, BinaryWriter w)
        {
            try
            {
                switch ((NetMsg)r.ReadInt16())
                {
                    case NetMsg.Ping:
                        {
                            // Reply in x seconds
                            this.PingReplyTimer = new Timer(o =>
                            {
                                if (!this.Discarding && !this.Discarded)
                                {
                                    this.SendNetworkMessage(new NetMsgPing());
                                }
                            }, null, TimeSpan.FromMilliseconds(Properties.Settings.Default.PingReplyDelay), TimeSpan.Zero);

                            break;
                        }
                    case NetMsg.ReqAes:
                        {
                            // Send AES keys to the client
                            this.SendNetworkMessage(new NetMsgReqAES());

                            break;
                        }
                    case NetMsg.ReqFiles:
                        {
                            int branch = r.ReadInt32();

                            if (Authorization.CanAccessBranch(this.Username, branch))
                            {
                                w.Write(true);
                            }
                            else
                            {
                                w.Write(false);
                                break;
                            }

                            // Write the number of files to the client
                            w.Write(FileManager.GetFilesInBranch(branch).Count());

                            // Send off a packet for each file in this branch
                            this.SendNetworkMessage(new NetMsgReqFiles(branch));

                            break;
                        }
                }
            }
            catch (Exception e)
            {
                Log.Print(LogFlags.VERBOSE | LogFlags.ERROR, 
                    "Client '{0}' ({1}) disconnected because of a network error reading network messages: {2}", this.Username, this.IPAddress, e.Message);

                this.Discard();
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn protected Thread ReadNetMessages(Stream str)
        ///
        /// \brief  Returns a thread that when started will read network messages from the input stream.
        ///
        /// \param  str The input/ouput stream.
        ///
        /// \return The net message thread.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        protected Thread ReadNetMessages(Stream str)
        {
            BinaryReader r = new BinaryReader(str);
            BinaryWriter w = new BinaryWriter(str);

            Thread t = new Thread(() =>
            {
                while (true)
                {
                    this.ProcessNetMessage(r, w);
                }
            });
            
            return t;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void SendNetworkMessage(NetworkMessage message)
        ///
        /// \brief  Sends a network message.  Useful when sending messages from outside of the 
        ///         Client class (streams are protected).
        ///
        /// \param  message The message.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public void SendNetworkMessage(NetworkMessage message)
        {
            try
            {
                BinaryReader r = new BinaryReader(this.InternalTcpClient.GetStream());
                BinaryWriter w = new BinaryWriter(this.InternalTcpClient.GetStream());

                message.Send(r, w, this);
            }
            catch (Exception e)
            {
                Log.Print(LogFlags.VERBOSE | LogFlags.ERROR,
                    "Client '{0}' ({1}) disconnected because of a network error sending network message ({2}): {3}", this.Username, 
                    this.IPAddress, message.ToString(), e.Message);

                this.Discard();
            }
        }
    }
}
