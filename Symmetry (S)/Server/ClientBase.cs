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
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Security;
using System.Net.Sockets;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Symmetry.Server
{
    partial class Client
    {
        public TcpClient   InternalTcpClient;
        private NetServer   Net;

        private Thread      NetMsgThread;

        public RSACryptoServiceProvider RSA;
        public RijndaelManaged          AES;

        public string   Username = null;
        public string   Password = null;

        public string   IPAddress = null;

        public DateTime Subscription;
        public Timer SubscriptionTimer;
        public Timer PingReplyTimer;

        public bool     Authenticated = false;
        public bool     WaitingForSubEnd = false;
        public bool     Discarding = false;
        public bool     Discarded = false;

        public CancellationTokenSource CancelToken = new CancellationTokenSource();

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public Client(Socket sock)
        ///
        /// \brief  Default constructor.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public Client(TcpClient client)
        {
            this.InternalTcpClient  = client;
            this.Net                = NetServer.GetInstance();

            this.IPAddress          = client.Client.RemoteEndPoint.ToString();

            // Create cryptography instances - AES is used for large network messages
            // and RSA for smaller client -> server messages
            this.AES = new RijndaelManaged();
            this.RSA = new RSACryptoServiceProvider(1024);

            this.AES.KeySize = 128;

            // Generate Key/IV for AES (it doesn't do it itself in the constructor)
            this.AES.GenerateKey();
            this.AES.GenerateIV();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void StartAuth()
        ///
        /// \brief  Starts the client's authentication process.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public void StartAuth()
        {
            // Perform authorization right away, if it fails discard the client
            // if it succeeds, keep the connection open and listen for net
            // messages.
            if (!this.PerformAuthorization())
            {
                this.Discard();
            }
            else
            {
                this.NetMsgThread = this.ReadNetMessages(this.InternalTcpClient.GetStream());
                this.NetMsgThread.Start();

                this.Authenticated = true;
                Log.Print(LogFlags.VERBOSE, "{0} was authenticated", this.Username);
            } 
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void Discard()
        ///
        /// \brief  Discards this client, closing it's streams and removing it from the NetServers'
        ///         ClientList.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public void Discard()
        {
            // This client is currently in the process of discarding itself
            this.Discarding = true;

            this.Net.ClientList.Remove(this);

            if (this.InternalTcpClient != null)
                this.InternalTcpClient.Close();

            if (this.NetMsgThread != null)
                this.NetMsgThread.Abort();

            // Stop waiting for the subscription to end, if we are
            if (this.SubscriptionTimer != null)
            {
                this.SubscriptionTimer.Change(Timeout.Infinite, Timeout.Infinite);
            }

            // Stop waiting to reply to a ping, if we are
            if (this.PingReplyTimer != null)
            {
                this.PingReplyTimer.Change(Timeout.Infinite, Timeout.Infinite);
            }

            // This client is now discarded
            this.Discarded = true;

            // The client is discarded and logged out
            if (this.Authenticated)
            {
                Log.Print("{0} disconected", this.Username);
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void PerformSubscriptionCheck(DateTime time)
        ///
        /// \brief  Runs a chunk of code at DateTime.  This is used to end subscriptions when a user logs in.
        ///         
        ///         For example, a user could log in with 10 minutes on their subscription and keep running
        ///         symmetry indefinitely until they log in again.  We do this here to make sure that doesn't
        ///         happen.
        ///
        /// \param  time    The time when the user's subscription ends.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public void PerformSubscriptionCheck(DateTime time)
        {
            TimeSpan length = TimeSpan.FromSeconds((time - DateTime.Now).TotalSeconds);

            // We can't set timeouts for longer than 30 days
            // and there's no way someone will stay logged in this long anyway.
            if (length.TotalDays > 30.0D)
                return;

            this.SubscriptionTimer = new Timer(o =>
                {
                    if (!this.Discarding && !this.Discarded)
                    {
                        Log.Print("DISPOSED {0}", length.Seconds);
                        this.SendNetworkMessage(new NetMsgSubEnd());
                        this.Discard();
                    }
                }, null, length, length);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public bool PerformAuthorization()
        ///
        /// \brief  Attempts to authorize the client.
        ///
        /// \return true if it succeeds, false if it fails.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public bool PerformAuthorization()
        {
            try
            {
                byte[]  username, password;

                BinaryReader r = new BinaryReader(this.InternalTcpClient.GetStream());
                BinaryWriter w = new BinaryWriter(this.InternalTcpClient.GetStream());

                RSAParameters param = this.RSA.ExportParameters(false);

                // Write modulus and exponent (public key)
                w.Write(param.Exponent.Length);
                w.Write(param.Exponent);

                w.Write(param.Modulus.Length);
                w.Write(param.Modulus);

                // Read and decrypt the username:password combonation
                username = this.RSA.Decrypt(r.ReadBytes(r.ReadInt32()), true);
                password = this.RSA.Decrypt(r.ReadBytes(r.ReadInt32()), true);

                // Read branch, used for versioning/version access
                int branch = r.ReadInt32();

                string sUsername = Encoding.ASCII.GetString(username);
                string sPassword = Encoding.ASCII.GetString(password);

                // Check if another user by this name is already logged in
                if (NetServer.GetInstance().ClientList.
                    Where(client => client.Username == sUsername).Count() > 0)
                {
                    w.Write(false);
                    w.Write(string.Format("'{0}' is already logged in", sUsername));
                    return false;
                }

                this.Username = sUsername;
                this.Password = sPassword;

                if (Authorization.DoAuth(sUsername, sPassword))
                {
                    // Check if they have access to the branch
                    if (!Authorization.CanAccessBranch(sUsername, branch))
                    {
                        w.Write(false);
                        w.Write("You cannot access this branch");
                        return false;
                    }

                    this.Subscription = Authorization.GetSubscriptionEndTime(sUsername);

                    // Check if they've an active subscription
                    if (DateTime.Now > this.Subscription)
                    {
                        w.Write(false);
                        w.Write(string.Format("Your subscription has ended.  Visit {0} to get another subscription", 
                            Properties.Settings.Default.SubscriptionURL));

                        return false;
                    }
                    
                    // Start a timer to disconnect the client when their subscription runs out
                    this.PerformSubscriptionCheck(this.Subscription);

                    // They're allowed to log in
                    w.Write(true);

                    // Send them version
                    if (FileManager.BranchIdToString(branch) == "beta")
                    {
                        string revision = null;

                        try
                        {
                            revision = File.ReadAllText(Properties.Settings.Default.SvnVersionFile);
                            revision = revision.TrimEnd('\n', ' ', '\t');
                        }
                        catch { };

                        if (revision != null)
                        {
                            w.Write("(beta, R" + revision + ")");
                        }
                        else
                            w.Write("(beta)");
                    }

                    // Convert DateTime to UnixTime and send that to them
                    w.Write((ulong)(this.Subscription - new DateTime(1970, 1, 1).ToLocalTime()).TotalSeconds);

                    return true;
                }
                else
                {
                    w.Write(false);
                    
                    // When a login fails, the client expects a reason
                    if (!Authorization.IsCorrectPassword(sUsername, sPassword))
                    {
                        w.Write("Bad password or username");
                    }
                    else
                    {
                        // Only other cause of failure is when they're banned
                        string reason = "Unknown ban reason";

                        if (Authorization.IsBanned(sUsername, ref reason))
                        {
                            w.Write(reason);
                        }
                        else
                        {
                            // ...????
                            w.Write("Unknown login error");
                        }
                    }

                    // Discard the unauthorized client (returning false is done below)
                    this.Discard();
                }
            }
            catch (CryptographicException e)
            {
                Log.Print(LogFlags.ERROR | LogFlags.VERBOSE, "CryptographicException in authentication process for {0}: {1}",
                        this.IPAddress, e.Message);

                this.Discard();
            }
            catch (Exception e)
            {
                Log.Print(LogFlags.ERROR | LogFlags.VERBOSE, "Unknown exception in authentication process for {0}: {1}",
                        this.IPAddress, e.Message);

                this.Discard();
            }

            return false;
        }
    }
}
