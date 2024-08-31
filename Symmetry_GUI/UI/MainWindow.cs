using Symmetry_GUI.Core;
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
    public partial class MainWindow : Form
    {
        private List<string> CommandHistory = new List<string>();
        private string StringBeforeCommand = "";
        private int CommandHistoryIndex = -1;

        public MainWindow()
        {
            InitializeComponent();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void AddCommand(string command)
        ///
        /// \brief  Adds a command to the console input's auto complete string collection.
        ///
        /// \param  command The command.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public void AddCommand(string command)
        {
            if (!this.ConsoleInput.AutoCompleteCustomSource.Contains(command))
                this.ConsoleInput.AutoCompleteCustomSource.Add(command);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void RemoveCommand(string command)
        ///
        /// \brief  Removes a command from the console input's auto complete string collection.
        ///
        /// \param  command The command.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public void RemoveCommand(string command)
        {
            if (this.ConsoleInput.AutoCompleteCustomSource.Contains(command))
                this.ConsoleInput.AutoCompleteCustomSource.Remove(command);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn protected override bool ProcessTabKey(bool forward)
        ///
        /// \brief  Processes pressing the tab key.
        ///
        /// \param  forward true to cycle forward through the controls in the
        ///                 <see cref="T:System.Windows.Forms.ContainerControl" />; otherwise, false.
        ///
        /// \return true if a control is selected; otherwise, false.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        protected override bool ProcessTabKey(bool forward)
        {
            if (this.ConsoleInput.Focused & this.ConsoleInput.SelectionLength > 0
                && this.ConsoleInput.SelectionStart > 0)
            {
                this.ConsoleInput.Text += " ";
                this.ConsoleInput.SelectionStart = this.ConsoleInput.Text.Length;
                this.ConsoleInput.SelectionLength = 0;
            }
            return false;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn private void MainWindow_Resize(object sender, EventArgs e)
        ///
        /// \brief  Event handler. Called by MainWindow when it's resized.
        ///
        /// \param  sender  Source of the event.
        /// \param  e       Event information.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        private void MainWindow_Resize(object sender, EventArgs e)
        {
            this.ConsoleOutput.Height = ClientRectangle.Height - (this.ConsoleInput.Height);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn private void ConsoleInput_KeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
        ///
        /// \brief  Event handler. Called by ConsoleInput when a key is pressed.
        ///
        /// \param  sender  Source of the event.
        /// \param  e       Key event information.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        private void ConsoleInput_KeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            switch (e.KeyCode)
            {
                case Keys.Enter:
                    CommandQueue.GetInstance().AddCommand(this.ConsoleInput.Text);

                    if (!this.CommandHistory.Contains(this.ConsoleInput.Text))
                        this.CommandHistory.Add(this.ConsoleInput.Text);

                    this.ConsoleInput.Clear();

                    this.CommandHistoryIndex = -1;
                    this.StringBeforeCommand = "";

                    break;
                case Keys.Down:
                case Keys.Up:
                    {
                        if (this.CommandHistoryIndex == -1)
                        {
                            this.CommandHistoryIndex = this.CommandHistory.Count - 1;
                            this.StringBeforeCommand = this.ConsoleInput.Text;
                        }
                            
                        if (e.KeyCode == Keys.Down && this.CommandHistory.Count > 0)
                        {
                            // There's no more commands later in the history list,
                            // so we can check if we typed something before up/down
                            if (this.CommandHistoryIndex == (this.CommandHistory.Count - 1))
                            {
                                // If we did type something before pressing up or down 
                                // we'll set the text to that
                                if (this.StringBeforeCommand != "")
                                {
                                    this.ConsoleInput.Text = this.StringBeforeCommand;
                                    this.ConsoleInput.SelectionStart = this.ConsoleInput.Text.Length;
                                    this.ConsoleInput.SelectionLength = 0;
                                }
                            }
                            else
                            {
                                this.CommandHistoryIndex++;

                                this.ConsoleInput.Text = this.CommandHistory[this.CommandHistoryIndex];
                                this.ConsoleInput.SelectionStart = this.ConsoleInput.Text.Length;
                                this.ConsoleInput.SelectionLength = 0;
                            }
                        }
                        else if (this.CommandHistory.Count > 0)
                        {
                            if (this.CommandHistoryIndex >= 0)
                            {
                                this.ConsoleInput.Text = this.CommandHistory[this.CommandHistoryIndex];
                                this.ConsoleInput.SelectionStart = this.ConsoleInput.Text.Length;
                                this.ConsoleInput.SelectionLength = 0;

                                if (this.CommandHistoryIndex != 0)
                                    this.CommandHistoryIndex--;
                            }
                        }

                        e.Handled = true;
                        break;
                    }
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn private void ConsoleOutput_KeyDown(object sender, KeyEventArgs e)
        ///
        /// \brief  Event handler. Called by ConsoleOutput when a key is pressed
        ///
        /// \param  sender  Source of the event.
        /// \param  e       Key event information.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        private void ConsoleOutput_KeyDown(object sender, KeyEventArgs e)
        {
            if ((Control.ModifierKeys & Keys.Control) == Keys.Control) return;
            if ((Control.ModifierKeys & Keys.ControlKey) == Keys.ControlKey) return;
            if ((Control.ModifierKeys & Keys.LControlKey) == Keys.LControlKey) return;
            if ((Control.ModifierKeys & Keys.RControlKey) == Keys.RControlKey) return;
            if ((Control.ModifierKeys & Keys.Shift) == Keys.Shift) return;
            if ((Control.ModifierKeys & Keys.ShiftKey) == Keys.ShiftKey) return;
            if ((Control.ModifierKeys & Keys.RShiftKey) == Keys.RShiftKey) return;
            if ((Control.ModifierKeys & Keys.LShiftKey) == Keys.LShiftKey) return;
            if ((Control.ModifierKeys & Keys.PageDown) == Keys.PageDown) return;
            if ((Control.ModifierKeys & Keys.PageUp) == Keys.PageUp) return;

            this.ConsoleInput.Focus();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void WriteText(string text)
        ///
        /// \brief  Writes text to the console output window, using colours.
        ///
        /// \param  text    The text.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public void WriteText(string text)
        {
            this.ConsoleOutput.AppendColouredText(text);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void WriteTextNoColour(string text)
        ///
        /// \brief  Writes text to the console output window WITHOUT using colours.
        ///
        /// \param  text    The text.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public void WriteTextNoColour(string text)
        {
            this.ConsoleOutput.AppendText(text);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void Clear()
        ///
        /// \brief  Clears this object to its blank/initial state.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public void Clear()
        {
            this.ConsoleOutput.Clear();
        }

        private void ConsoleOutput_KeyPress(object sender, KeyPressEventArgs e)
        {

        }
    }
}
