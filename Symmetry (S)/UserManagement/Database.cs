/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

using MySql.Data.MySqlClient;
using Symmetry.Console;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Symmetry.UserManagement
{
    class Database
    {
        private static Database Instance;

        public MySqlConnection Connection;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void GetInstance()
        ///
        /// \brief  Returns or creates an instance of Database.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static Database GetInstance()
        {
            if (Instance == null)
            {
                Instance = new Database();
            }
            return Instance;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public async Task ConnectAsync()
        ///
        /// \brief  Connects to the database asynchronously.  There's no need for a synchronous version
        ///         of this, so it doesn't exist.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public async Task ConnectAsync()
        {
            try 
            {
                string addr = Properties.Settings.Default.DB_Address;

                string username = Properties.Settings.Default.DB_Username;
                string password = Properties.Settings.Default.DB_Password;

                string db = Properties.Settings.Default.DB_Database;
                string dbf = db != "" ? ("Database=" + Properties.Settings.Default.DB_Database + ";") : "";

                this.Connection = new MySqlConnection(string.Format("Server={0}; Uid={1}; Pwd={2}; Convert Zero Datetime=True;",
                    addr, username, password, dbf));

                await this.Connection.OpenAsync();
            
                Log.Print("Connected to MySQL database {0}@{1}", username, addr);
            }
            catch (Exception e)
            {
                Log.Print(LogFlags.ERROR, "MySQL connection error: {0}", e.ToString());
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public static void ValidateConnection()
        ///
        /// \brief  Validates *the* connection to the database, only returns true if queries
        ///         can be run now.  In the case that the state that's not necessarily "invalid"
        ///         but queries still can't be run (such as ConnectionState.Connecting) the function
        ///         will return false, but no error message will be printed.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public bool ValidateConnection()
        {
            switch (this.Connection.State)
            {
                case ConnectionState.Broken:
                case ConnectionState.Closed:
                    return false;
                case ConnectionState.Connecting:
                    return false;
                default:
                    return true;
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public bool MakeConnectionValid()
        ///
        /// \brief  Attemptedly makes the connection valid.  If this returns false, the connection 
        ///         could not be valid and queries should not be made.
        ///
        /// \return See above.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public bool MakeConnectionValid()
        {
            if (this.ValidateConnection())
            {
                // Connection is valid and we can return now
                return true;
            }
            else
            {
                try 
                {
                    string addr = Properties.Settings.Default.DB_Address;

                    string username = Properties.Settings.Default.DB_Username;
                    string password = Properties.Settings.Default.DB_Password;

                    string db = Properties.Settings.Default.DB_Database;
                    string dbf = db != "" ? ("Database=" + Properties.Settings.Default.DB_Database + ";") : "";

                    this.Connection = new MySqlConnection(string.Format("Server={0}; Uid={1}; Pwd={2}; Convert Zero Datetime=True;",
                        addr, username, password, dbf));

                    this.Connection.Open();
            
                    Log.Print("Reconnected to MySQL database {0}@{1} (probably connection timeout?)", username, addr);

                    // Reconnected 
                    return true;
                }
                catch (Exception e)
                {
                    Log.Print(LogFlags.ERROR, "MySQL connection error: {0}", e.ToString());

                    // Could not reconnect
                    return false;
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public MySqlDataReader Query(string query)
        ///
        /// \brief  Queries the MySQL server SYNCHRONOUSLY.  This function CAN halt.
        ///         Asynchronous functions don't exist because only one IDataReader can exist for each
        ///         connection.
        ///
        /// \param  query   The MySQL query.
        ///
        /// \return The data returned by the query. 
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public MySqlDataReader Query(string query)
        {
            if (!this.MakeConnectionValid())
            {
                Log.Print(LogFlags.VERBOSE | LogFlags.ERROR, "ABANDONED QUERY: {0}", query);
                return null;
            }

            MySqlCommand cmd = Connection.CreateCommand();
            cmd.CommandText = query;

            return cmd.ExecuteReader();
        }

    }
}
