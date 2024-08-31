using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Symmetry_GUI.Interop
{
    enum NetMsg
    {
        Ping
    };

    class ConnectionBase
    {
        protected IntPtr ConnectionPointer = IntPtr.Zero;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public IntPtr GetNative()
        ///
        /// \brief  Gets the native net connection pointer, used when a native function wants to use
        ///         the connection.
        ///
        /// \return The native connection pointer.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public IntPtr GetNative()
        {
            return this.ConnectionPointer;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public virtual void RecvMsg()
        ///
        /// \brief  Receives a single message, this function doesn't really do all that much, since as
        ///         network messages are handled by c++.  However, calling the receive function via C#
        ///         makes it possible to use C# threads, which is much more managable from a C# host
        ///         process.
        ///         
        ///         The return value is non-zero if a failure occured somewhere.  Negative values are C#
        ///         side errors, postive values are C++ side Windows Errors.
        ///         
        ///         This function does not perform anything by default and is overridden by IPCConnection
        ///         and NETConnection.
        ///         
        ///         EDIT: 21/10/2014: errors are now thrown, return type is void.

        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public virtual void RecvMsg()
        {
            // Not implemented
            return;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public virtual void SendMsg(NetMsg msg, byte[] data)
        ///
        /// \brief  Sends a network message, not actually used anywhere since as all sending is done by
        ///         routines, that are all native.
        ///
        /// \param  msg     The message.
        /// \param  data    The data.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public virtual void SendMsg(NetMsg msg, byte[] data)
        {
            // Not implemented
            return;
        }
    }
}
