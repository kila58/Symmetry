using Symmetry_GUI.Interop;
using Symmetry_GUI.Interop.Routines;
using Symmetry_GUI.IO;
using Symmetry_GUI.Util;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Symmetry_GUI.Core
{
    class RoutineHandler
    {
        private CredentialHandler Credentials = null;
        private NETConnection NConnection = null;
        private IPCConnection IConnection = null;
        private FileContainer Container = null;

        public bool Running = false;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void RunRoutines()
        ///
        /// \brief  Executes a the LoginRoutine and whatever comes after that, according
        ///         BaseRoutine.GetNextRoutine.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public void RunRoutines()
        {
            this.Running = true;

            // Make sure we have good credentials
            if (this.Credentials == null)
            {
                this.Credentials = new CredentialHandler();
            }
            else if (this.Credentials.CredentialsInvalid)
            {
                this.Credentials.RequestNewCredentials(false);
            }

            // Make sure we have a good file container (can't contain old files!!)
            if (this.Container == null)
            {
                this.Container = new FileContainer();
            }
            else if (this.Container.GetFileCount() > 0)
            {
                this.Container.Dispose();
                this.Container = new FileContainer();
            }

            // Make sure we have a good network connection
            try
            {
                if (this.NConnection != null)
                    this.NConnection.Dispose();

                this.NConnection = new NETConnection();
                this.NConnection.SetContainer(this.Container);
            }
            catch // SEHException
            {
                SymIO.Write(Level.Bad, "NConnection failed: {0}", SymIO.FormatLastError());
            }

            // Make sure we have a good IPC connection 
            try
            {
                // Make sure we have a good network connection
                if (this.IConnection != null)
                    this.IConnection.Dispose();

                this.IConnection = new IPCConnection();
                this.IConnection.SetContainer(this.Container);
            }
            catch // SEHException
            {
                SymIO.Write(Level.Bad, "IConnection failed: {0}", SymIO.FormatLastError());
            }

            RoutineBase routine = new CheckUpdatedRoutine();
#if DEBUG
            if (CommandLine.HasArg("-skipupdate"))
                routine = new LoginRoutine();
#endif
            do
            {
                // The fact that I have to initialize each routine with
                // certain values kinda defeats the purpose of having a
                // GetNextRoutine function, but I didn't think about that
                // when I was implementing it, so it's like this for now
                if (routine is LoginRoutine)
                {
                    LoginRoutine log = (LoginRoutine)routine;

                    log.Credentials = this.Credentials;
                    log.Connection = this.NConnection;
                }
                else if (routine is DownloadRoutine)
                {
                    DownloadRoutine dl = (DownloadRoutine)routine;

                    dl.Connection = this.NConnection;
                    dl.Container = this.Container;
                }
                else if (routine is UpdateRoutine)
                {
                    UpdateRoutine up = (UpdateRoutine)routine;

                    up.Container = this.Container;
                    up.Credentials = this.Credentials;
                }
                else if (routine is MapRoutine)
                {
                    MapRoutine map = (MapRoutine)routine;

                    map.Container = this.Container;
                }
                else if (routine is BinaryInitializeRoutine)
                {
                    BinaryInitializeRoutine bin = (BinaryInitializeRoutine)routine;

                    bin.Connection = this.IConnection;
                    bin.Container = this.Container;
                    bin.Credentials = this.Credentials;
                }
                else if (routine is PingLoopRoutine)
                {
                    PingLoopRoutine ping = (PingLoopRoutine)routine;

                    ping.Connection = this.NConnection;
                }
                else if (routine is IdleRoutine)
                {
                    IdleRoutine idle = (IdleRoutine)routine;

                    idle.NetConnection = this.NConnection;
                    idle.IpcConnection = this.IConnection;
                    idle.Credentials = this.Credentials;
                }

                routine.Run();

                // Explicit exiting does not prompt anything
                if (routine.RoutineState == RtnState.Exit)
                    break;

                // This routine's state has to be RtnState.Finished to continue
                if (routine.RoutineState != RtnState.Finished)
                {
                    if (this.PromptRetry("Routine '" + routine.Name + "' failed, retry?"))
                    {
                        this.RunRoutines();
                    }
                    break;
                }

            } while ((routine = routine.GetNextRoutine()) != null);

            this.Cleanup();

            this.Running = false;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void Cleanup()
        ///
        /// \brief  Cleans up connections and containers used.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public void Cleanup()
        {
            // Cleanup
            if (this.NConnection != null)
                this.NConnection.Dispose();

            if (this.IConnection != null)
                this.IConnection.Dispose();

            if (this.Container != null)
                this.Container.Dispose();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public bool PromptRetry(string retryReason)
        ///
        /// \brief  Prompt retry.
        ///
        /// \param  retryReason The retry reason.
        ///
        /// \return true if it succeeds, false if it fails.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public bool PromptRetry(string retryReason)
        {
            if (Symmetry.Window == null)
                return false;

            DialogResult result = DialogResult.No;

            Symmetry.Window.Invoke(new Action(() =>
                {
                    result = MessageBox.Show(Symmetry.Window, retryReason, "Routine Error", MessageBoxButtons.YesNo);
                }));

            return result == DialogResult.Yes;
        }
    }
}
