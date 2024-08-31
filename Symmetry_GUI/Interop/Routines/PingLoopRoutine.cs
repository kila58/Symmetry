using Symmetry_GUI.IO;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Symmetry_GUI.Interop.Routines
{
    class PingLoopRoutine : RoutineBase
    {
        public NETConnection Connection = null;

        // Native functions
        [DllImport("symlibui.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr Rtn_PingLoop(IntPtr pFileContainer);

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public BinaryInitializeRoutine()
        ///
        /// \brief  Default constructor.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public PingLoopRoutine()
        {
            this.Name = "ping-loop";
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public override RoutineBase GetNextRoutine()
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public override RoutineBase GetNextRoutine()
        {
            return new IdleRoutine();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public override RtnState Run()
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public override RtnState Run()
        {
            // Check for valid paramters
            if (this.Connection == null)
            {
                this.RoutineState = RtnState.Failed;
                return base.Run();
            }

            this.RoutineState = RtnState.Active;

            try
            {
                // Silent routine
                PingLoopRoutine.Rtn_PingLoop(this.Connection.GetNative());

                this.RoutineState = RtnState.Finished;
            }
            catch // SEHException
            {
                this.RoutineState = RtnState.Failed;
                SymIO.Write(Level.Bad, "PingLoopErr: {0}", SymIO.FormatLastError());
            }

            return base.Run();
        }
    }
}
