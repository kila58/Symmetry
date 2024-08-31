/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

  /*   ///////////////////////////////////////
	       ____                     __
  //	  / __/_ ____ _  __ _  ___ / /_______ __	//
  //	 _\ \/ // /  ' \/  ' \/ -_) __/ __/ // /	//
  //	/___/\_, /_/_/_/_/_/_/\__/\__/_/  \_, /		//
  //		/___/                        /___/		//

	   ////////////////////////////////////////     */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Symmetry.Console
{
    class CommandHandler
    {
        delegate void Command(string commandName, string argString, string[] args);

        private Dictionary<string, Command> CommandDictionary = new Dictionary<string, Command>();

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public Commands()
        ///
        /// \brief  Default constructor.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public CommandHandler()
        {
            // Start the command thread (calls Console.ReadLine a lot, needs to be done in it's own thread)
            new Thread(() => this.StartReadCommands()).Start();

            // Find each class inside Console.Commands
            Type[] classes = Assembly.GetExecutingAssembly().GetTypes().Where(c => 
                {
                    return c.Namespace.StartsWith("Symmetry.Console.Commands");
                }).ToArray();

            foreach (Type cl in classes)
            {
                // Find each method and it as a command
                foreach (MethodInfo mf in cl.GetMethods().Where(m => m.IsStatic))
                {
                    Log.Print("adding {0}", mf.Name);

                    CommandDictionary.Add(mf.Name, (Command)Delegate.CreateDelegate(typeof(Command), mf));
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void StartReadCommands()
        ///
        /// \brief  Starts a while loop that'll repeatedly read commands from the console,
        ///         command data is passed into ProcessCommand.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public void StartReadCommands()
        {
            string command = string.Empty;

            // It'll never be null but yolo
            while ((command = System.Console.ReadLine()) != null)
            {
                string[] parts = command.Split(' ');

                string argString = string.Empty;
                string[] args = new string[parts.Length - 1];
 
                // Process parts
                for (int i = 0; i < parts.Length; i++ )
                {
                    if (i > 0)
                        args[i - 1] = parts[i];

                    argString += parts[i] + " ";
                }

                this.ProcessCommand(parts[0], argString.TrimEnd(' '), args);
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void ProcessCommand(string commandName, string argString, string[] args)
        ///
        /// \brief  Process the command.
        ///
        /// \param  commandName Name of the command.
        /// \param  argString   All of the arguments in one string.
        /// \param  args        The arguments.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public void ProcessCommand(string commandName, string argString, string[] args)
        {
            if (this.CommandDictionary.ContainsKey(commandName))
            {
                this.CommandDictionary[commandName].Invoke(commandName, argString, args);
            }
            else
            {
                Log.Print(LogFlags.NOLOG, "Command '{0}' does not exist!", commandName);
            }
        }

    }
}
