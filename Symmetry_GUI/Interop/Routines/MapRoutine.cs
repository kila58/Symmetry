using Symmetry_GUI.IO;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Symmetry_GUI.Interop.Routines
{
    class MapRoutine : RoutineBase
    {
        // These need to be set before LoginRoutine.Run is called
        public FileContainer Container = null;

        // Something that's needed by the binary initialization routine
        private IntPtr ModuleBase = IntPtr.Zero;

        // Native functions
        [DllImport("symlibui.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern byte Rtn_CheckMap();

        [DllImport("symlibui.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr Rtn_Map(IntPtr pFileContainer);

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public MapRoutine()
        ///
        /// \brief  Default constructor.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public MapRoutine()
        {
            this.Name = "injection";
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public override RoutineBase GetNextRoutine()
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public override RoutineBase GetNextRoutine()
        {
            return new BinaryInitializeRoutine(this.ModuleBase);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public override RtnState Run()
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public override RtnState Run()
        {
            if (this.Container == null || this.Container.GetFileCount() == 0)
            {
                this.RoutineState = RtnState.Failed;
                return base.Run();
            }

            try
            {
                if (MapRoutine.Rtn_CheckMap() != 1)
                {
                    this.RoutineState = RtnState.Failed;
                    SymIO.Write(Level.Bad, "InjectErr: target process is not ready (did you forget to launch Garry's Mod?)");
                }
                else
                {
                    SymIO.Write(Level.Normal, "Injecting files ..");
                    DateTime start = DateTime.Now;

                    if ((this.ModuleBase = MapRoutine.Rtn_Map(this.Container.GetNative()))
                        == (IntPtr)(-1))
                    {
                        this.RoutineState = RtnState.Failed;
                        SymIO.Write(Level.Bad, "InjectErr: injection failed ({0})", SymIO.FormatLastError());
                    }
                    else
                    {
                        this.RoutineState = RtnState.Finished;
                        SymIO.Write(Level.Good, "Injected successfully! ({0}ms)", Math.Round((DateTime.Now - start).TotalMilliseconds, 2));
                    }
                }
            }
            catch // SEHException
            {
                this.RoutineState = RtnState.Failed;
                SymIO.Write(Level.Bad, "InjectErr: {0}", SymIO.FormatLastError());
            }

            return base.Run();
        }
    }
}
