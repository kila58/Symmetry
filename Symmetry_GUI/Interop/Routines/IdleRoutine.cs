using Symmetry_GUI.Core;
using Symmetry_GUI.IO;
using Symmetry_GUI.Util;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Symmetry_GUI.Interop.Routines
{
    class IdleRoutine : RoutineBase
    {
        // These need to be set before IdleRoutine.Run is called
        public NETConnection NetConnection = null;
        public IPCConnection IpcConnection = null;
        public CredentialHandler Credentials = null;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public IdleRoutine()
        ///
        /// \brief  Default constructor.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public IdleRoutine()
        {
            this.Name = "idle";
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public override RoutineBase GetNextRoutine()
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public override RoutineBase GetNextRoutine()
        {
            return null;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public override RtnState Run()
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public override RtnState Run()
        {
            if (this.IpcConnection == null || this.NetConnection == null)
            {
                this.RoutineState = RtnState.Failed;
                return base.Run();
            }

            // CommandQueue can now process IPC messages
            CommandQueue.GetInstance().CommandConnection = this.IpcConnection;
            CommandQueue.GetInstance().CanProcess = true;

            BackgroundTask[] tasks = new BackgroundTask[3];

            // Receive IPC messages
            tasks[0] = new BackgroundTask(new Action(() =>
                {
                    while (true)
                        this.IpcConnection.RecvMsg();
                }), "IPC");

            // Receive network messages
            tasks[1] = new BackgroundTask(new Action(() =>
                {
                    while (true)
                    {
                        try
                        {
                            this.NetConnection.RecvMsg();
                        }
                        catch // SEHExpcetion (connection error)
                        {
                            SymIO.Write(Level.Bad, "Connection failure! ({0})", SymIO.FormatLastError());

                            int i = 1;

                            // Retry 10 times, bail if an error with credentials occurs
                            for (i = 1; i <= 10; i++)
                            {
                                try 
                                {
                                    if (!this.NetConnection.Reconnect(this.Credentials))
                                    {
                                        // Cannot recover from bad credentials
                                        SymIO.SetLastError(-3);
                                        throw new Exception();
                                    }
                                    else 
                                    {
                                        SymIO.Write(Level.Good, "Reconnection successful!");
                                        break;
                                    }
                                }
                                catch // SEHException (another connection error)
                                {
                                    SymIO.Write(Level.Bad, "Reconnection {0}/10 failed! ({1})", i, SymIO.FormatLastError());
                                }
                            }

                            if (i >= 10)
                            {
                                // Exceeded max retries
                                SymIO.SetLastError(-4);
                                throw new Exception();
                            }
                        }
                    }
                }), "NET");

            // Wait for game to exit
            tasks[2] = new BackgroundTask(new Action(() =>
                {
                    Process.GetProcessesByName("hl2").First().WaitForExit();
                    SymIO.SetLastError(-2);
                }), "Game");

            // Wait for any of the above tasks to exit and set 'task'
            // to the task that was exited
            BackgroundTask task = tasks[WaitHandle.WaitAny(tasks)];

            // Ugly error switch
            switch (task.Error)
            {
                case 109:   // ERROR_BROKEN_PIPE
                    this.RoutineState = RtnState.Finished; break;
                case -1:    // IPC_EXIT/NET_EXIT
#if DEBUG 
                    MessageBox.Show(string.Format("Received {0}_EXIT", task.Name), "Symmetry Debug Error"); 
#endif
                    this.RoutineState = RtnState.Finished; break;

                case -2:     // bad process
#if DEBUG 
                    MessageBox.Show("Process ended", "Symmetry Debug Error"); 
#endif
                    this.RoutineState = RtnState.Finished; break;
                case -3:    // bad reconnection credentials

                    SymIO.Write(Level.Bad, "Reconnection failed because credentials were invalid");
                    this.RoutineState = RtnState.Failed; break;

                case -4:    // exceeded max reconnection attempts

                    SymIO.Write(Level.Bad, "Exceeded max reconnection attempts");
                    this.RoutineState = RtnState.Failed; break;

                default:    // Other errors
                    SymIO.Write(Level.Bad, "IdleErr: {0}_{1}", task.Name, SymIO.FormatError(task.Error));
                    this.RoutineState = RtnState.Failed; break;
            }

            return base.Run();
        }
    }
}
