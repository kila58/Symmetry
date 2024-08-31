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
using Symmetry.Server.NetMessages;
using Symmetry.Util;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Symmetry.Console.Commands
{
    class Svn
    {
        static public void svn_files(string name, string argString, string[] args)
        {
            if (args.Length == 0)
            {
                Log.Print(LogFlags.NOLOG, "Specify a branch name");
            }
            else if (FileManager.BranchNameToId(args[0]) == -1)
            {
                Log.Print(LogFlags.NOLOG, "Branch does not exist");
            }
            else
            {
                List<SvnFile> files = FileManager.GetFilesInBranch((short)FileManager.BranchNameToId(args[0]));

                Log.Print(LogFlags.NOLOG, "Listing {0} files in branch {1} ({2})", files.Count, args[0], Properties.Settings.Default.SvnPath);
                foreach (SvnFile file in files)
                {
                    int pathlen = Properties.Settings.Default.SvnPath.Length;
                    string path = file.Path.Substring(pathlen, file.Path.Length - pathlen);

                    Log.Print(LogFlags.NOLOG, "{0}: M={1} E={2} L={3} I={4}", path, file.IsMapped, file.IsEncrypted, file.IsLuaAutorun, file.IsInjectorFile);
                }
            }
        }

        static public void svn_update(string name, string argString, string[] args)
        {
            NetServer net = NetServer.GetInstance();

            // Only really here so the message below this makes more sense
            if (net.ClientList.Count == 0)
            {
                Log.Print(LogFlags.NOLOG, "Not enough clients to perform update.");
            }

            Log.Print("Sending update packet to {0} client(s)", net.ClientList.Count);

            foreach (Client client in net.ClientList)
            {
                client.SendNetworkMessage(new NetMsgUpdate());
            }
        }
    }
}
