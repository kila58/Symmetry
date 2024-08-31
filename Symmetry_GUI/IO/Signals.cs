using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Symmetry_GUI.IO
{
    enum SignalType
    {
        PrintGood,
        PrintBad,
        PrintWarning,
        PrintDebug,
        PrintNormal,
        Clear,
        AddCommand,
        RemoveComand
    };

    class Signals
    {
        private delegate void SignalFnD(SignalType sType, IntPtr sArg, int iLength);
        
        // Native functions
        [DllImport("symlibui.dll", CallingConvention=CallingConvention.StdCall)]
        private static extern void SetSignalFn(SignalFnD fn);

        // Needs to be a static, so the garbage collector won't 
        // clean up this delegate
        private static SignalFnD HandlerInstance;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public static void RegisterHandler()
        ///
        /// \brief  Registers the handler for C++ signals.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static void RegisterHandler()
        {
            HandlerInstance = new SignalFnD(Handler);
            SetSignalFn(HandlerInstance);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn private static void Handler(SignalType type, IntPtr data, int length)
        ///
        /// \brief  Handles signals.
        ///
        /// \param  type    The type.
        /// \param  data    The data buffer.
        /// \param  length  The length of the data buffer.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        private static void Handler(SignalType type, IntPtr data, int length)
        {
            if (Symmetry.Window == null)
                return;

            // All of this idiocy is required for some reason because my colours
            // get changed when marshal'd to managed strings
            StringBuilder sbuf = new StringBuilder();

            byte[] buf = new byte[length];
            Marshal.Copy(data, buf, 0, length);

            foreach (byte b in buf)
                sbuf.Append((char)b);

            switch (type)
            {
                case SignalType.Clear:
                    Symmetry.Window.Invoke(new Action(() => Symmetry.Window.Clear()));
                    break;
                case SignalType.AddCommand:
                        Symmetry.Window.Invoke(new Action(() => Symmetry.Window.AddCommand(sbuf.ToString())));
                        break;
                case SignalType.RemoveComand:
                        Symmetry.Window.Invoke(new Action(() => Symmetry.Window.RemoveCommand(sbuf.ToString())));
                        break;
                case SignalType.PrintGood:
                case SignalType.PrintBad:
                case SignalType.PrintWarning:
                case SignalType.PrintDebug:
                case SignalType.PrintNormal:
                    Symmetry.Window.Invoke(new Action(() => SymIO.Write((Level)type, sbuf.ToString())));
                    break;
            }
        }
    }
}
