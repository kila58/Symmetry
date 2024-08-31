/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

using Symmetry.UserManagement;
using Symmetry.Util.Crypto;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Symmetry.Console.Commands
{
    class Auth
    {
        static public void auth_test(string name, string argString, string[] args)
        {
            if (args.Length < 2)
            {
                Log.Print(LogFlags.NOLOG, "Not enough args.  Usage: auth_test <username> <password> [branch]");
            }
            else 
            {
                int branch = 0;

                // Attempt to parse the third optional argument
                try 
                {
                    if (args.Length > 2)
                        branch = Convert.ToInt32(args[2]);
                } catch (Exception){};

                string username = args[0];
                string password = MD5.Compute(args[1]);

                // Call some functions from Authorization on
                // username, password and branch
                if (Authorization.DoAuth(username, password))
                {
                    if (!Authorization.CanAccessBranch(username, branch))
                    {
                        Log.Print(LogFlags.NOLOG, "{0} cannot access branch {1}", username, branch);
                    }
                    else 
                    {
                        Log.Print(LogFlags.NOLOG, "User authorized successfully!");
                    }
                }
                else 
                {
                    string reason = "Unknown reason.";
                    if (Authorization.IsBanned(username, ref reason))
                    {
                        Log.Print(LogFlags.NOLOG, "{0} is banned: {1}", username, reason);
                    }
                    else 
                    {
                        Log.Print(LogFlags.NOLOG, "Incorrect password for user {0}", username);
                    }
                }
            }
        }
    }
}
