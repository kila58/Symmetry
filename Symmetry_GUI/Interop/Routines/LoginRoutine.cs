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
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \class  LoginRoutine
    ///
    /// \brief  Takes a connection to the master server, userand and password an authenticates it
    ///         with login information. Authentication must be performed before anything else is done,
    ///         authentication must be done within x seconds since a connection with the master
    ///         server is established, the connection will timeout if the user wasn't authenticated
    ///         within that time.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    class LoginRoutine : RoutineBase
    {
        // These need to be set before LoginRoutine.Run is called
        public CredentialHandler Credentials = null;
        public NETConnection Connection = null;

        // Native functions
        [DllImport("symlibui.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern ulong Rtn_Login(IntPtr pNetConnection, string sUsername, string sPassword,
            int iBranch, StringBuilder sBuffer, StringBuilder sVersionBuffer, int iErrorSize, int iVersionSize);

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public LoginRoutine()
        ///
        /// \brief  Default constructor.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public LoginRoutine()
        {
            this.Name = "authenticate";
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public override RoutineBase GetNextRoutine()
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public override RoutineBase GetNextRoutine()
        {
            return new DownloadRoutine();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public override RtnState Run()
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public override RtnState Run()
        {
            // Check for valid paramters
            if (this.Credentials == null || this.Credentials.CredentialsInvalid ||
                this.Connection == null)
            {
                this.RoutineState = RtnState.Failed;
                return base.Run();
            }

            this.RoutineState = RtnState.Active;

            try
            {
                SymIO.Write(Level.Normal, "Logging in ..");
                DateTime start = DateTime.Now;

                // Attempt a login, error will be set if an error occurs
                StringBuilder error = new StringBuilder(4096);
                StringBuilder version = new StringBuilder(256);

                if ((Symmetry.ExpiryDate = LoginRoutine.Rtn_Login(this.Connection.GetNative(), this.Credentials.Username, 
                    this.Credentials.Password, Branching.GetBranch(), error, version, error.Capacity, version.Capacity)) != 0)
                {
                    Symmetry.Version = version.ToString();

                    this.RoutineState = RtnState.Finished;
                    SymIO.Write(Level.Good, "Logged in! ({0}ms)", Math.Round((DateTime.Now - start).TotalMilliseconds, 2));
                }
                else
                {
                    this.RoutineState = RtnState.Failed;
                    this.Credentials.CredentialsInvalid = true;

                    SymIO.Write(Level.Bad, "LoginErr: {0}", error.ToString());
                }
            }
            catch // SEHException
            {
                this.RoutineState = RtnState.Failed;
                this.Credentials.CredentialsInvalid = true;

                SymIO.Write(Level.Bad, "LoginErr: {0}", SymIO.FormatLastError());
            }

            return base.Run();
        }
    }
}
