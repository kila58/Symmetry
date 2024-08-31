using Symmetry_GUI.UI;
using Symmetry_GUI.Util;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Symmetry_GUI.Core
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \class  CredentialHandler
    ///
    /// \brief  Credential handler.  This class handles the credentials used to login with, if the 
    ///         credentials are invalid, CredentialsInvalid should be set to true, so another login
    ///         attempt to request new ones.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    class CredentialHandler
    {
        public string   Username = "";
        public string   Password = "";
        public bool     CredentialsInvalid = false;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public CredentialHandler()
        ///
        /// \brief  Default constructor.  This will retrieve username and password from the command-line,
        ///         anything that's not found on the command-line will be requested by
        ///         RequestNewCredentials.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public CredentialHandler()
        {
            string[] args = Environment.GetCommandLineArgs();

            if (!CommandLine.HasArgVal("-username"))
                this.Username = "";
            else
                this.Username = CommandLine.GetArg("-username");

            if (!CommandLine.HasArgVal("-password"))
                this.Password = "";
            else
                this.Password = CommandLine.GetArg("-password");

            // If we're missing some credentials, request for new ones
            // keeping ones that we already know
            if (this.Username == "" || this.Password == "")
                this.RequestNewCredentials(true);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void RequestNewCredentials(bool useOldCredentials)
        ///
        /// \brief  Requests new credentials.  If useOldCredentials is true, this function will request 
        ///         only credentials that aren't set in the class.  (By filling out the fields automatically,
        ///         they can still be changed.)
        ///
        /// \param  useOldCredentials   See above.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public void RequestNewCredentials(bool useOldCredentials)
        {
            if (Symmetry.Window == null)
                return;

            Symmetry.Window.Invoke(new Action(() =>
                {
                    CredentialPrompt prompt = new CredentialPrompt(useOldCredentials ? this.Username : "",
                        useOldCredentials ? this.Password : "");

                    prompt.ShowDialog(Symmetry.Window);

                    if (prompt.ValidInformation)
                    {
                        this.Username = prompt.Username;
                        this.Password = prompt.Password;

                        this.CredentialsInvalid = false;
                    }
                    else
                        this.CredentialsInvalid = true;

                }));
        }
    }
}
