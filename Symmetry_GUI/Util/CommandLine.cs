using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Symmetry_GUI.Util
{
    class CommandLine
    {
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public static bool HasArg(string arg)
        ///
        /// \brief  Checks the commandline for any occurence of 'arg'. 'arg' must be space-seperated. 
        ///         
        ///         For example, -test would occur in "-test -a -b -c", but not in "-testabc".
        ///
        /// \param  arg The argument.
        ///
        /// \return true if argument occurs in the commandline, false if not.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static bool HasArg(string arg)
        {
            string[] args = Environment.GetCommandLineArgs();

            // Look for argument on the commandline
            for (int i = 0; i < args.Length; i++)
            {
                if (args[i] == arg)
                    return true;
            }

            return false;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public static bool HasArgVal(string arg)
        ///
        /// \brief  Like above function, checks for an argument on the command-line, however this will
        ///         only return true if the argument has a value directly after it.  For example:
        ///         
        ///         "-test a", would return true for -test, but "-test" would not as there's nothing
        ///         after -test.
        ///
        /// \param  arg The argument.
        ///
        /// \return See above.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static bool HasArgVal(string arg)
        {
            string[] args = Environment.GetCommandLineArgs();

            // Look for argument on the commandline and make sure
            // an argument comes after this one
            for (int i = 0; i < args.Length; i++)
            {
                if (args[i] == arg && args.Length >= (i + 1))
                    return true;
            }

            return false;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public static string GetArg(string arg)
        ///
        /// \brief  Gets an argument directly after another.  For example, if 'arg' is "-test" and the
        ///         commandline is: "-test abc", this will return "abc".  This function will return
        ///         null if there's nothing after "-test" or if "-test" isn't in the commandline.
        ///
        /// \param  arg The argument.
        ///
        /// \return See above.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static string GetArg(string arg)
        {
            string[] args = Environment.GetCommandLineArgs();

            // Look for argument on the commandline and make sure
            // an argument comes after this one
            for (int i = 0; i < args.Length; i++)
            {
                if (args[i] == arg && args.Length >= (i + 1))
                    return args[i + 1];
            }

            return null;
        }
    }
}
