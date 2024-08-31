using Symmetry_GUI.Core;
using Symmetry_GUI.IO;
using Symmetry_GUI.Util;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Symmetry_GUI.Interop.Routines
{
    enum UpdateStatus
    {
        Failed,
        Required,
        UpToDate,
    }

    class UpdateRoutine : RoutineBase
    {        
        public FileContainer Container = null;
        public CredentialHandler Credentials = null;

        // Native functions
        [DllImport("symlibui.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern byte Rtn_Update(IntPtr pFileContainer, string sLibPath, string sExePath,
            string sUsername, string sPassword, int iBranch);

        [DllImport("symlibui.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern UpdateStatus Rtn_CheckUpdate(IntPtr pFileContainer, string sLibPath, string sExePath);

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public UpdateRoutine()
        ///
        /// \brief  Default constructor.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public UpdateRoutine()
        {
            this.Name = "update";
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public override RoutineBase GetNextRoutine()
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public override RoutineBase GetNextRoutine()
        {
            return new MapRoutine();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public override RtnState Run()
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public override RtnState Run()
        {
            // Check for valid paramters
            if (this.Container == null || this.Container.GetFileCount() == 0 || this.Credentials == null)
            {
                this.RoutineState = RtnState.Failed;
                return base.Run();
            }

            this.RoutineState = RtnState.Active;

            try
            {
                string lib = Symmetry.GetCPPLibName();
                string exe = Application.ExecutablePath;

                SymIO.Write(Level.Normal, "Checking for changes in injector files ..");

                UpdateStatus stat = UpdateRoutine.Rtn_CheckUpdate(this.Container.GetNative(), lib, exe);

                if (stat == UpdateStatus.Failed)
                {
                    this.RoutineState = RtnState.Failed;
                }
                else if (stat == UpdateStatus.Required)
                {
                    // Print information into the console regarding the update
                    SymIO.Write(Level.Warning, "Required update found! You must update to continue running symmetry");
                    SymIO.Write(Level.Warning, "Click 'yes' to install update, clicking 'no' will exit symmetry");

                    // Notify the user of the required update
                    DialogResult result = DialogResult.No;

                    // Needs to be done on the UI thread
                    Symmetry.Window.Invoke(new Action(() =>
                    {
                        result = MessageBox.Show(Symmetry.Window, "Install required update?", "Update", MessageBoxButtons.YesNo);
                    }));
                   
                    if (result == DialogResult.Yes)
                    {
                        if (UpdateRoutine.Rtn_Update(this.Container.GetNative(), lib, exe,
                            this.Credentials.Username, this.Credentials.Password, Branching.GetBranch()) == 1)
                        {
                            this.RoutineState = RtnState.Exit;
                        }
                        else
                        {
                            this.RoutineState = RtnState.Failed;
                            SymIO.Write(Level.Bad, "Failed to install update!");
                        }
                    }
                    else
                    {
                        this.RoutineState = RtnState.Exit;
                    }
                }
                else
                {
                    SymIO.Write(Level.Good, "Injector files are up to date!");
                    this.RoutineState = RtnState.Finished;
                }
            }
            catch // SEHException
            {
                this.RoutineState = RtnState.Failed;
                SymIO.Write(Level.Bad, "UpdateErr: {0}", SymIO.FormatLastError());
            }

            return base.Run();
        }
    }
}
