using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Symmetry_GUI.UI
{
    public partial class CredentialPrompt : Form
    {
        public string Username
        {
            get
            {
                return this.UsernameText.Text;
            } 
        }

        public string Password
        {
            get
            {
                return this.PasswordText.Text;
            }
        }

        // True if valid information was written into the text boxes before the form was closed.
        public bool ValidInformation = false;

        public CredentialPrompt(string username = "", string password = "")
        {
            InitializeComponent();

            this.UsernameText.Text = username;
            this.PasswordText.Text = password;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn private void Accept_Click(object sender, EventArgs e)
        ///
        /// \brief  Event handler. Called by when the accept button is pressed.
        ///
        /// \param  sender  Source of the event.
        /// \param  e       Event information.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        private void Accept_Click(object sender, EventArgs e)
        {
            if (this.Username != "" && this.Password != "")
            {
                this.ValidInformation = true;
                this.Close();
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn private void UsernameText_KeyDown(object sender, KeyEventArgs e)
        ///
        /// \brief  Event handler. Called by UsernameText when a key is pressed.
        ///
        /// \param  sender  Source of the event.
        /// \param  e       Key event information.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        private void UsernameText_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
                this.PasswordText.Focus();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn private void PasswordText_KeyDown(object sender, KeyEventArgs e)
        ///
        /// \brief  Event handler. Called by PasswordText when a key is pressed.
        ///
        /// \param  sender  Source of the event.
        /// \param  e       Key event information.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        private void PasswordText_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
                this.Accept_Click(this.Accept, null);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn private void CredentialPrompt_FormClosing(object sender, FormClosingEventArgs e)
        ///
        /// \brief  Event handler. Called by CredentialPrompt when the close button is pressed.
        ///
        /// \param  sender  Source of the event.
        /// \param  e       Form closing event information.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        private void CredentialPrompt_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing && !this.ValidInformation)
            {
                if (MessageBox.Show(this, "Are you sure you want to exit?", "Notice", MessageBoxButtons.YesNo) ==
                    DialogResult.Yes)
                {
                    if (Symmetry.Window != null && Symmetry.Window.Handle != IntPtr.Zero)
                    {
                        Symmetry.Window.Close();
                    } 
                }
                else
                    e.Cancel = true;
            }
        }
    }
}
