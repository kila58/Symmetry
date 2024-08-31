using Symmetry_GUI.IO;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.AccessControl;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Symmetry_GUI.Util
{
    class BackgroundTask : EventWaitHandle
    {
        private Thread TaskThread;

        public int Error;
        public string Name;

        public BackgroundTask(Action action, string name) :
            base(false, EventResetMode.ManualReset)
        {
            this.TaskThread = new Thread(() =>
                {
                    try
                    {
                        action();
                    }
                    catch
                    {
                        // nothing much
                    }
                    this.Error = SymIO.GetLastError();
                    this.Set();
                });

            this.TaskThread.Name = "TaskThread";
            this.TaskThread.IsBackground = true;
            this.TaskThread.Start();

            this.Name = name;
        }
    }
}
