using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Symmetry_GUI.IO
{
    enum Level
    {
        Good,   // [GREEN*] msg
        Bad,    // [RED*] msg
        Warning,// [YELLOW*] msg
        Debug,  // [YELLOW*] msg (only printed in debug buildmode)
        Normal, // [DARK_GREY*] msg
    }

    class SymIO
    {
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public static int GetLastError()
        ///
        /// \brief  Gets the last error from the C++ side.  The error variable is allocted in TLS, like
        /// 		with Win32's GetLastError.
        ///
        /// \return The last error.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        [DllImport("symlibui.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "GetLastErr")]
        public static extern int GetLastError();

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public static extern void SetLastError(int iErrorCode);
        ///
        /// \brief  Sets the last error, a varible returned by GetLastError (above). 
        ///
        /// \param  iErrorCode  The error code.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        [DllImport("symlibui.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "SetLastErr")]
        public static extern void SetLastError(int iErrorCode);

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public static string FormatError(int error)
        ///
        /// \brief  Formats an error by prefixing windows error-codes with 'E' and symmetry error codes with
        ///         "SE".  Symmetry error codes are made positive as well.
        ///
        /// \param  error   The error.
        ///
        /// \return The formatted error.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static string FormatError(int error)
        {
            if (error >= 0)
                return "E" + error;

            return "SE" + Math.Abs(error);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public static string FormatLastError()
        ///
        /// \brief  Gets and returns the last error as a formatted string.  See above for formatting.
        ///
        /// \return The formatted last error.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static string FormatLastError()
        {
            return FormatError(GetLastError());
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void Write(string text)
        ///
        /// \brief  Writes a string of text to an instance of the MainWindow.
        ///
        /// \param  text    The text to write.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static void Write(string text, params object[] format)
        {
            // Avoid null exceptions, this shouldn't really happen though - prints
            // should never occur before the main window is initialized
            if (Symmetry.Window != null)
                Symmetry.Window.WriteText(string.Format(text, format));
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void WriteNoColour(string text)
        ///
        /// \brief  Writes a string of text to an instance of the MainWindow without using colours.
        ///
        /// \param  text    The text.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static void WriteNoColour(string text, params object[] format)
        {
            if (Symmetry.Window != null)
                Symmetry.Window.WriteTextNoColour(string.Format(text, format));
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void Write(Level level, string text)
        ///
        /// \brief  Writes to an instance of the MainWindow using standard print formats.  The print format
        ///         is " [*] some text" where * is coloured coded depending on level paramater.
        ///         
        ///         If level is Level.Debug and the buildmode isn't beta, NO message will be printed.
        ///         
        ///         This function appends a newline character to the end of the message.
        ///
        /// \param  level   The level.
        /// \param  text    The text.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static void Write(Level level, string text, params object[] format)
        {
            if (Symmetry.Window == null)
                return;

#if !DEBUG
            if (level == Level.Debug)
                return;
#endif
            
            // Format text
            text = string.Format(text, format);

            // Print colour prefix
            Symmetry.Window.WriteText(" \x87[");
            switch (level)
            {
                case Level.Good:
                    Symmetry.Window.WriteText("\x8A*"); break;
                case Level.Bad:
                    Symmetry.Window.WriteText("\x8C*"); break;
                case Level.Warning:
                case Level.Debug:
                    Symmetry.Window.WriteText("\x8E*"); break;
                case Level.Normal:
                    Symmetry.Window.WriteText("\x88*"); break;
            }
            Symmetry.Window.WriteText("\x87] " + text + '\n');
        }
    }
}
