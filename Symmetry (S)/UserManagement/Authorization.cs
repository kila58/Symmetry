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
using Symmetry.Util.Crypto;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Symmetry.UserManagement
{
    // **
    // Probably should have done something in here 
    // so the same code isn't repeated in like 4 places.
    // Oh well, we can redo this later.
    // **
    
    class Authorization
    {

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public static bool DoAuth(string username, string password)
        ///
        /// \brief  Authenticates a username and password combonation.  This function will return false 
        ///         if:
        ///         
        ///         * The user is banned.
        ///         * The user does not exist.
        ///         * The password is incorrect.
        ///         
        ///
        /// \param  username    The username.
        /// \param  password    The password.
        ///
        /// \return See above.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static bool DoAuth(string username, string password)
        {
            if (Properties.Settings.Default.IsLocalServer)
                return true;

            return !IsBanned(username) && IsCorrectPassword(username, password);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public static bool IsBanned(string username, out string reason)
        ///
        /// \brief  Check if a user is banned, the ban reason if one existed will be set by reference
        ///         (second argument).
        ///
        /// \param  username        The username of the user.
        /// \param [out]    reason  The ban reason, if one exists.
        ///
        /// \return true if banned, false if not.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static bool IsBanned(string username, ref string reason)
        {
            if (Properties.Settings.Default.IsLocalServer)
                return false;

            string query = string.Format("SELECT `lifted`, `reason` FROM `{0}`.`{1}banned` WHERE `uid`={2}",
                Properties.Settings.Default.DB_ForumDatabase,
                Properties.Settings.Default.DB_ForumPrefix,
                Authorization.UsernameToUID(username));

            try
            {
                using (MySqlDataReader reader = Database.GetInstance().Query(query))
                {
                    if (reader.Read())
                    {
                        long lifted = reader.GetInt64("lifted");
                        reason      = reader.GetString("reason");

                        if (FromUnixTime(lifted) > DateTime.Now)
                            return true;
                    }
                    return false;
                }
            }
            catch (MySqlException e)
            {
                Log.Print(LogFlags.ERROR, "MySQL error: {0}", e.ToString());
            }
            catch (Exception e)
            {
                Log.Print(LogFlags.ERROR, "Authorization error in IsBanned: {0}", e.ToString());
            }

            return false;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public static bool IsBanned(string username)
        ///
        /// \brief  Same as above, without the need for a second argument.
        ///
        /// \param  username    The username of the user.
        ///
        /// \return true if banned, false if not.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static bool IsBanned(string username)
        {
            if (Properties.Settings.Default.IsLocalServer)
                return false;

            string discard = "";
            return IsBanned(username, ref discard);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public static bool CanAccessBranch(string username, int branch)
        ///
        /// \brief  Checks if a user can acess a branch.
        ///
        /// \param  username    The username of the user.
        /// \param  branch      The UID of the branch.
        ///
        /// \return true if the user can access branch, false if they cannot not.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static bool CanAccessBranch(string username, int branch)
        {
            if (Properties.Settings.Default.IsLocalServer)
                return true;

            string query = string.Format("SELECT `branch_access` FROM `{0}`.`{1}` WHERE `uid`={2}",
                MySqlHelper.EscapeString(Properties.Settings.Default.DB_SymDatabase),
                MySqlHelper.EscapeString(Properties.Settings.Default.DB_SymUserTable),
                Authorization.UsernameToUID(username));

            try
            {
                using (MySqlDataReader reader = Database.GetInstance().Query(query))
                {
                    if (reader.Read())
                    {
                        return reader.GetInt32("branch_access") >= branch;
                    }
                }

                return false;
            }
            catch (MySqlException e)
            {
                Log.Print(LogFlags.ERROR, "MySQL error: {0}", e.ToString());
            }
            catch (Exception e)
            {
                Log.Print(LogFlags.ERROR, "Authorization error in CanAccessBranch: {0}", e.ToString());
            }

            return false;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public static bool IsCorrectPassword(string username, string password)
        ///
        /// \brief  Checks if password is valid for a user.
        ///
        /// \param  username    The username of the user.
        /// \param  password    The MD5 hash of the password (not including salt).
        ///
        /// \return true if the password is correct, false if it isn't.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static bool IsCorrectPassword(string username, string password)
        {
            if (Properties.Settings.Default.IsLocalServer)
                return true;

            string query = string.Format("SELECT `password`, `salt` FROM `{0}`.`{1}users` WHERE `username`='{2}'",
                MySqlHelper.EscapeString(Properties.Settings.Default.DB_ForumDatabase),
                MySqlHelper.EscapeString(Properties.Settings.Default.DB_ForumPrefix),
                MySqlHelper.EscapeString(username));

            try
            {
                using (MySqlDataReader reader = Database.GetInstance().Query(query))
                {
                    if (reader.Read())
                    {
                        string dbpassword = reader.GetString("password");
                        string salt       = reader.GetString("salt");

                        // This is how MyBB handles passwords.
                        // password (the field inside mybb_users) = md5(md5(salt) + md5(actual password))
                        return dbpassword == MD5.Compute(MD5.Compute(salt) + password);
                    }
                }

                return false;
            }
            catch (MySqlException e)
            {
                Log.Print(LogFlags.ERROR, "MySQL error: {0}", e.ToString());
            }
            catch (Exception e)
            {
                Log.Print(LogFlags.ERROR, "Authorization error in IsCorrectPassword: {0}", e.ToString());
            }

            return false;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn private static DateTime FromUnixTime(long unixTime)
        ///
        /// \brief  Another private member that has nothing to do with Authorization.  This is 
        ///         her because it's only needed to convert Unix time used by MyBB in bans.
        ///         
        ///         This function converts a long integer (unix time, seconds since Jan 1st, 1970) 
        ///         into a DateTime object.
        ///
        /// \param  unixTime    Seconds since Epoch.
        ///
        /// \return .
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        private static DateTime FromUnixTime(long unixTime)
        {
            var epoch = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc);
            return epoch.AddSeconds(unixTime);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn private static int UsernameToUID(string username)
        ///
        /// \brief  Converts a username to a UID, as defined in mybb.mybb_users table.
        ///         If the username was not found in the mybb.mybb_users table, -1 is returned.
        ///         
        ///         In MySQL the UID is defined as an unsigned int, so unless this function fails
        ///         the return value will be > 0.
        ///
        /// \param  username    The username.
        ///
        /// \return The UID of the user.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        private static int UsernameToUID(string username)
        {
            if (Properties.Settings.Default.IsLocalServer)
                return 1;

            string query = string.Format("SELECT `uid` FROM `{0}`.`{1}users` WHERE `username`='{2}'",
                Properties.Settings.Default.DB_ForumDatabase,
                Properties.Settings.Default.DB_ForumPrefix,
                MySqlHelper.EscapeString(username));

            try
            {
                using (MySqlDataReader reader = Database.GetInstance().Query(query))
                {
                    if (reader.Read())
                    {
                        return reader.GetInt32("uid");
                    }
                }

                return -1;
            }
            catch (MySqlException e)
            {
                Log.Print(LogFlags.ERROR, "MySQL error: {0}", e.ToString());
            }
            catch (Exception e)
            {
                Log.Print(LogFlags.ERROR, "Authorization error in UsernameToUID: {0}", e.ToString());
            }

            return -1;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public static DateTime GetSubscriptionEndTime(string username)
        ///
        /// \brief  Gets the time that the user's subscription ends.  This will return DateTime.MinValue
        ///         to signify a failed evaluation.  For example, if the user does not have a
        ///         subscription this will return DateTime.MinValue.  Or if a MySQL error was thrown,
        ///         this will return DateTime.MinValue.
        ///
        /// \param  username    The username.
        ///
        /// \return The DateTime that the user's subscription ends.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static DateTime GetSubscriptionEndTime(string username)
        {
            // Local server, subscription will be forever
            if (Properties.Settings.Default.IsLocalServer)
                return DateTime.MaxValue;

            string query = string.Format("SELECT `subscription` FROM `{0}`.`{1}` WHERE `uid`='{2}'",
                Properties.Settings.Default.DB_SymDatabase,
                Properties.Settings.Default.DB_SymUserTable,
                Authorization.UsernameToUID(username));

            try
            {
                using (MySqlDataReader reader = Database.GetInstance().Query(query))
                {
                    if (reader.Read())
                    {
                        object obj = reader.GetValue(0);

                        if (obj == null || obj == DBNull.Value)
                            return DateTime.MinValue;

                        return (DateTime)obj;
                    }
                }
            }
            catch (MySqlException e)
            {
                Log.Print(LogFlags.ERROR, "MySQL error: {0}", e.ToString());
            }
            catch (Exception e)
            {
                Log.Print(LogFlags.ERROR, "Authorization error in GetSubscriptionEndTime: {0}", e.ToString());
            }

            // Error
            return DateTime.MinValue;
        }
    }
}
