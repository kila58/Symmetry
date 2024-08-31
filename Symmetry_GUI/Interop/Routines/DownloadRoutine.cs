using Symmetry_GUI.IO;
using Symmetry_GUI.Util;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Symmetry_GUI.Interop.Routines
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \class  DownloadRoutine
    ///
    /// \brief  Takes a connection to the master server and a FileContainer.  Requests all of the 
    ///         files in the branch that the C++ side was built in.  Routine will finish running after
    ///         all of the files have been downloaded.  NETConnection.Recv is called until each file
    ///         is received.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    class DownloadRoutine : RoutineBase
    {
        // These need to be set before DownloadRoutine.Run is called
        public NETConnection Connection = null;
        public FileContainer Container = null;

        // Native functions
        [DllImport("symlibui.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern byte Rtn_Download(IntPtr pNetConnection, IntPtr pNetFileContainer, int branch, ref int iFileCount);

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public DownloadRoutine()
        ///
        /// \brief  Default constructor.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public DownloadRoutine()
        {
            this.Name = "download";
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public override RoutineBase GetNextRoutine()
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public override RoutineBase GetNextRoutine()
        {
#if DEBUG
            if (CommandLine.HasArg("-skipupdate"))
                return new MapRoutine();
#endif
            return new UpdateRoutine();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public override RtnState Run()
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public override RtnState Run()
        {
            // Check for valid paramters
            if (this.Connection == null || this.Container == null)
            {
                this.RoutineState = RtnState.Failed;
                return base.Run();
            }

            this.RoutineState = RtnState.Active;
            int count = 0; 

            try
            {
                SymIO.Write(Level.Normal, "Downloading resources .. ");
                DateTime start = DateTime.Now;

                // Attempt to download files (could halt for a significant amount of time!)
                if (DownloadRoutine.Rtn_Download(this.Connection.GetNative(), this.Container.GetNative(),
                    Branching.GetBranch(), ref count) != 0 &&
                    count == this.Container.GetFileCount())
                {
                    // All files downloaded and Rtn_Download gave us the go-ahead
                    this.RoutineState = RtnState.Finished;

                    SymIO.Write(Level.Good, "{0} resources downloaded! ({1}ms)", count, Math.Round((DateTime.Now - start).TotalMilliseconds, 2));
                }
                else
                {
                    this.RoutineState = RtnState.Failed;
                    SymIO.Write(Level.Bad, "DlErr: resource download failed ({0}/{1}) {2}", count, this.Container.GetFileCount(),
                        SymIO.FormatLastError());
                }
            }
            catch // SEHException
            {
                this.RoutineState = RtnState.Failed;
                SymIO.Write(Level.Bad, "DlErr: resource download failed {0}", SymIO.FormatLastError());
            }

            return base.Run();
        }
    }
}
