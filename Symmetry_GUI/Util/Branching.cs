using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Symmetry_GUI.Util
{
    class Branching
    {
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public static int GetBranch()
        ///
        /// \brief  Gets the desired branch from the command line, default return value is '0', for
        ///         public branches.
        ///
        /// \return The branch.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static int GetBranch()
        {
            // Beta
            if (CommandLine.HasArg("-beta"))
                return 1;

            // Other??
            try
            {
                if (CommandLine.HasArg("-branch"))
                    return Int32.Parse(CommandLine.GetArg("-branch"));
            }
            catch (FormatException) { };

            return 0;
        }
    }
}
