using Symmetry_GUI.Interop;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Symmetry_GUI.Core
{
    class CommandQueue
    {
        // Singleton instance
        private static CommandQueue Instance;

        // List of commands
        private List<string> Queue = new List<string>();

        // Active IPC connection that can be used to send IPC message
        public IPCConnection CommandConnection = null;

        public bool CanProcess = false;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public CommandQueue GetInstance()
        ///
        /// \brief  Gets or creates a CommandQueue instance.
        ///
        /// \return The instance.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static CommandQueue GetInstance()
        {
            if (Instance == null)
                Instance = new CommandQueue();

            return Instance;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void AddCommand(string command)
        ///
        /// \brief  Adds command to the queue, if CanProces is true this command will run ProcessQueue
        ///         too.
        ///
        /// \param  command The command.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public void AddCommand(string command)
        {
            this.Queue.Add(command);

            if (this.CanProcess)
                this.ProcessQueue();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void ProcessQueue()
        ///
        /// \brief  Processes the queue.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public void ProcessQueue()
        {
            if (this.CommandConnection == null)
                return;

            foreach (string command in this.Queue)
            {
                try
                {
                    this.CommandConnection.SendInput(command);
                }
                catch
                {
                    this.CommandConnection = null;
                    this.CanProcess = false;
                    break;
                }
            }
            this.Queue.Clear();
        }
    }
}
