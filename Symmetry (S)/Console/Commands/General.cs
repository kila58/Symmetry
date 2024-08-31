/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

using Symmetry.Server;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Symmetry.Console.Commands
{
    class General
    {
        static public void status(string name, string argString, string[] args)
        {
            NetServer net = NetServer.GetInstance();

            // Only really here so the message below this makes more sense
            if (net.ClientList.Count == 0)
            {
                Log.Print(LogFlags.NOLOG, "There are no connected clients.");
            }

            Log.Print(LogFlags.NOLOG, "{0} connected clients:", net.ClientList.Count);

            foreach (Client client in net.ClientList)
            {
                if (!client.Authenticated)
                {
                    Log.Print("{0}: Not authed.", client.IPAddress);
                }
                else
                {
                    Log.Print("{0}: USER={1}", client.IPAddress, client.Username);
                }
            }
        }

        static public void exit(string name, string argString, string[] args)
        {
            Environment.Exit(0);
        }

        static public void crash(string name, string argString, string[] args)
        {
            throw new Exception("Controlled crash");
        }
    }
}
