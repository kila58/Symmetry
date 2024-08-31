using Symmetry.Server;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Symmetry.Console.Commands
{
    class Crypto
    {
        static public void aes_keys(string name, string argString, string[] args)
        {
            if (args.Length < 1)
            {
                Log.Print("Usage: aes_keys <username>");
            }
            else
            {
                string username = args[0];

                NetServer net = NetServer.GetInstance();

                // Only really here so the message below this makes more sense
                if (net.ClientList.Count == 0)
                {
                    Log.Print(LogFlags.NOLOG, "There are no connected clients.");
                }

                foreach (Client client in net.ClientList)
                {
                    if (client.Authenticated && client.Username == username)
                    {
                        byte[] key = client.AES.Key, iv = client.AES.IV;
                        string skey = "", siv = "";

                        // Parse key
                        foreach (byte b in key)
                            skey += b.ToString("x2") + " ";

                        // Parse IV
                        foreach (byte b in iv)
                            siv += b.ToString("x2") + " ";

                        // Print key and Iv
                        Log.Print("Key({0}) = {1}", skey.Length, skey);
                        Log.Print("IV ({0}) = {1}", siv.Length, siv);

                        return;
                    }
                }

                Log.Print("User not found");
            }
        }
    }
}
