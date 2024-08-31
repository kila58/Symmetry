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
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Symmetry.Console
{
    public enum LogFlags : int
    {
        VERBOSE     = 1 << 0,
        ERROR       = 1 << 1,
        NOLOG       = 1 << 2,
        NOTIME      = 1 << 3,
    }

    class Log
    {
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn private string GetTimeString()
        ///
        /// \brief  Returns the current time as a string formatted as [HH:MM:SS], a trailing whitespace
        ///         character is also returned.
        ///
        /// \return The time string.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        private static string GetTimeString()
        {
            return "[" + DateTime.Now.ToString("T") + "] ";
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn private string GetDateString()
        ///
        /// \brief  Returns the current date as a string formatted as yyyy_MM_dd.
        ///
        /// \return The date string.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        private static string GetDateString()
        {
            return DateTime.Now.ToString("yyyy_MM_dd");
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void Log(string str, params object[] format)
        ///
        /// \brief  Logs a string to file, the log files are kept in LogDirectory and are named 
        ///         after the date.  The file names are what GetDateString returns.
        ///
        /// \param  str     The string to print.
        /// \param  format  Formatting arguments.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static void LogToFile(string str, params object[] format)
        {
            string dir = Properties.Settings.Default.LogDirectory;
            string name = dir + "/" + GetDateString() + ".log";

            // Ensure the directory exists
            if (!Directory.Exists(dir))
                Directory.CreateDirectory(dir);

            // Open streams
            FileStream f = File.Open(name, FileMode.Append);
            BinaryWriter w = new BinaryWriter(f);

            // Format and write the string 
            w.Write(string.Format(str, format));

            // Clean up streams that we created
            f.Close();
            w.Close();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn void Print(string str, params object[] format)
        ///
        /// \brief  Prints to console, very simple implementation.  Text printed is also logged to file.
        ///
        /// \param  str     The string to print.
        /// \param  format  Formatting arguments.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static void Print(string str, params object[] format)
        {
            // Write the text
            System.Console.WriteLine(GetTimeString() + string.Format(str, format));

            // Write to file as well
            LogToFile(GetTimeString() + string.Format(str, format) + Environment.NewLine);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public static void Print(LogFlags flags, string str, params object[] format)
        ///
        /// \brief  Prints to console (at a bare minimum), flags for logging, marking output
        ///         as verbose and other stuff (see LogFlags) are also available as the first paramter
        ///
        /// \param  flags   The flags.
        /// \param  str     The string to print.
        /// \param  format  Formatting arguments.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static void Print(LogFlags flags, string str, params object[] format)
        {
            string data = string.Empty;

            if ((flags & LogFlags.VERBOSE) == LogFlags.VERBOSE)
            {
                // If the message is verbose, only print if the settings comply
                if (!Properties.Settings.Default.Verbose)
                    return;
            }

            // Print the time if it's enabled (or rather: not disabled)
            if ((flags & LogFlags.NOTIME) != LogFlags.NOTIME)
            {
                data += GetTimeString();
            }

            // Append the actual formatted data to the string
            data += string.Format(str, format);

            // Write the string to file if it's enabled (or rather: not disabled)
            if ((flags & LogFlags.NOLOG) != LogFlags.NOLOG)
            {
                LogToFile(data);
            }

            // Set the colour of the foreground to red if it's an error message
            if ((flags & LogFlags.ERROR) == LogFlags.ERROR)
            {
                System.Console.ForegroundColor = ConsoleColor.Red;
            }
            
            // And finally write the strin to the console
            System.Console.WriteLine(data);

            System.Console.ResetColor();
        }
    }
}
