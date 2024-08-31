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
using Symmetry.Server;
using System.Threading;
using System.Net.Sockets;
using System.Configuration;
using System.Security.Cryptography;
using Symmetry.Console;
using System.Reflection;
using Symmetry.Util;
using Symmetry.UserManagement;
using System.Data;

namespace Symmetry
{
    class Program
    {
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn static async void InitializeMySQL(Database db)
        ///
        /// \brief  Initializes a MySQL connection.  Connects to the database and performs
        ///         CREATE TABLE queries.  Both connection and query calls are asynchronous.
        ///
        /// \param  db  The database.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        static async void InitializeMySQL(Database db)
        {
            await db.ConnectAsync();

            if (db.Connection == null || db.Connection.State == ConnectionState.Closed)
            {
                Log.Print(LogFlags.ERROR, "A fatal MySQL error occurred that stopped it from connecting to the MySQL database.  The server will"
                    + "shutdown now, press any key to continue...");

                System.Console.Read();
                Environment.Exit(-1);
            }

            // Create the users table if it doesn't exist
            db.Query(string.Format("CREATE TABLE IF NOT EXISTS `{0}`.`{1}` ( " +
                    "`uid` INT NOT NULL, " +
                    "`branch_access` INT NULL, "+
                    "PRIMARY KEY (`uid`));", 
                Properties.Settings.Default.DB_SymDatabase, 
                Properties.Settings.Default.DB_SymUserTable)).Close();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn static void Main(string[] args)
        ///
        /// \brief  Main entry-point for this application.
        ///
        /// \param  args    Array of command-line argument strings.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        static void Main(string[] args)
        {
            Log.Print(LogFlags.NOTIME, Environment.NewLine + "Starting Symmetry Login Server at " + DateTime.Now.ToString("T"));

            // First thing to load is the network server
            NetServer server = NetServer.GetInstance();
            Log.Print("NetServer started");

            // And then the command handler (constructor starts threads)
            CommandHandler com = new CommandHandler();
            Log.Print("CommandHandler loaded");

            // Create a MySQL instance and initialize it
            Database db = Database.GetInstance();

            if (!Properties.Settings.Default.IsLocalServer)
            {
                InitializeMySQL(db);
            }
            else
            {
                Log.Print("WARNING: Running local server, no authorization is performed!");
            }

            Log.Print("Finished loading");
        }
    }
}
