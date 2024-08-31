using Symmetry_GUI.Core;
using Symmetry_GUI.IO;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Symmetry_GUI.Interop.Routines
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \class  BinaryInitializeRoutine
    ///
    /// \brief  Initializes the binary that's just been injected so it's fully ready for use, 
    ///         initialization sends files and variables that the binary needs from the injector
    ///         to it.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    class BinaryInitializeRoutine : RoutineBase
    {
        // These need to be set before BinaryInitializeRoutine.Run is called
        public FileContainer        Container       = null;
        public IPCConnection        Connection      = null;
        public CredentialHandler    Credentials     = null;

        // Something that's needed by the binary initialization routine
        private IntPtr ModuleBase = IntPtr.Zero;

        // Native functions
        [DllImport("symlibui.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern byte Rtn_Init(IntPtr pIPCConnection, IntPtr pFileContainer, string sUsername,
            ulong tExpiryDate, string sVersion, IntPtr pBaseAddr);

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public BinaryInitializeRoutine()
        ///
        /// \brief  Default constructor.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public BinaryInitializeRoutine(IntPtr module)
        {
            this.Name = "initialize";
            this.ModuleBase = module;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public override RoutineBase GetNextRoutine()
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public override RoutineBase GetNextRoutine()
        {
            return new PingLoopRoutine();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public override RtnState Run()
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public override RtnState Run()
        {
            if (this.Connection == null || this.Container == null || this.Credentials == null ||
                this.ModuleBase == IntPtr.Zero)
            {
                this.RoutineState = RtnState.Failed;
                return base.Run();
            }

            try
            {
                SymIO.Write(Level.Normal, "Initializing binary ..");
                DateTime start = DateTime.Now;

                if (BinaryInitializeRoutine.Rtn_Init(this.Connection.GetNative(), this.Container.GetNative(),
                    this.Credentials.Username, Symmetry.ExpiryDate, Symmetry.Version, this.ModuleBase) != 1)
                {
                    this.RoutineState = RtnState.Failed;
                    SymIO.Write(Level.Bad, "BinInitErr: initialization failure");
                }
                else
                {
                    this.RoutineState = RtnState.Finished;
                    SymIO.Write(Level.Good, "Initialization complete! ({0}ms)", Math.Round((DateTime.Now - start).TotalMilliseconds, 2));
                }
            }
            catch // SEHException
            {
                this.RoutineState = RtnState.Failed;
                SymIO.Write(Level.Bad, "BinInitErr: {0}", SymIO.FormatLastError());
            }

            return base.Run();
        }
    }
}
