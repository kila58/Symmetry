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
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace Symmetry.Server
{
    class NetServer
    {
        private static NetServer Instance;

        public List<Client>     ClientList = new List<Client>();

        private TcpListener     Listener;
       
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public static NetServer GetInstance()
        ///
        /// \brief  Gets or creates an instance of NetServer.
        ///
        /// \return See above.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static NetServer GetInstance()
        {
            if (Instance == null)
            {
                Instance = new NetServer();
            }

            return Instance;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public NetServer()
        ///
        /// \brief  Default constructor.  Starts listening for clients
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public NetServer()
        {
            int port = Convert.ToInt32(Properties.Settings.Default.Port);

            this.Listener = new TcpListener(IPAddress.Any, port);
            this.Listener.Start();

            // Debug
            //Console.WriteLine("NetServer listening on " + port);

            new Thread(() => this.Listen()).Start();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void Listen()
        ///
        /// \brief  Listens for a client connection, once one's established it will listen for another
        ///         on a new thread.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public void Listen()
        {
            TcpClient tcpclient = this.Listener.AcceptTcpClient();

            // Big timeout cus downloads
            tcpclient.SendTimeout       = 1000 * 24;
            tcpclient.ReceiveTimeout    = 1000 * 24;

            // Enable keep alive on the socket (SO_KEEPALIVE)
            tcpclient.Client.SetSocketOption(
                SocketOptionLevel.Socket,
                SocketOptionName.KeepAlive, true);

            Client c = new Client(tcpclient);

            // Add the client to the clientlist before starting the client's
            // authentication process
            this.ClientList.Add(c);

            // .. start the client's authenticiation processs
            c.StartAuth();           
            
            // And listen for a new connection on a new thread
            new Thread(() => this.Listen()).Start();
        }
    }
}
