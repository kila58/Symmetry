using Symmetry_GUI.Core;
using Symmetry_GUI.Interop;
using Symmetry_GUI.IO;
using Symmetry_GUI.UI;
using Symmetry_GUI.Util;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Symmetry_GUI
{
    static class Symmetry
    {
        public static MainWindow Window = null;
        public static Thread MainThread = null;

        public static ulong     ExpiryDate  = ulong.MaxValue;
        public static string    Version     = "";

        [DllImport("kernel32.dll", EntryPoint = "LoadLibraryA")]
        private static extern IntPtr LoadLibraryA(string sModuleName);

        [DllImport("kernel32.dll")]
        private static extern void FreeLibrary(IntPtr pModule);
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn static void Main()
        ///
        /// \brief  Main entry-point for this application.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        [STAThread]
        static void Main()
        {
            RoutineHandler rtnHandler = new RoutineHandler();

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            Symmetry.Window = new MainWindow();
            Symmetry.Window.FormClosed += new FormClosedEventHandler((sender, args) =>
                {
                    rtnHandler.Cleanup();
                    Environment.Exit(0); // nasty call needed, can't interupt C++ routines
                });
            Symmetry.Window.HandleCreated += new EventHandler((sender, args) =>
                {
                    // "Main" thread
                    Symmetry.MainThread = new Thread(() =>
                    {
                        rtnHandler.RunRoutines();
                        Environment.Exit(0); // nasty call needed, can't interupt C++ routines
                    });
                    Symmetry.MainThread.Start();
                });

            // Load C++ library
            IntPtr lib = LoadLibraryA(GetCPPLibName());

            Signals.RegisterHandler();

            if (lib == IntPtr.Zero)
            {
                MessageBox.Show("Library not found!");
                return;
            }

            // UI Thread
            Application.Run(Symmetry.Window);

            // Unload C++ library
            FreeLibrary(lib);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn publuc static string GetCPPLibName()
        ///
        /// \brief  Gets C++ library name.
        ///
        /// \return The C++ library name.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static string GetCPPLibName()
        {
#if !DEBUG
            // -library not supported in public builds
            return "symlibui.dll";
#else
            if (!CommandLine.HasArgVal("-libpath"))
                return "symlibui.dll";

            return CommandLine.GetArg("-libpath") + "\\symlibui.dll";
#endif
        }
    }
}
