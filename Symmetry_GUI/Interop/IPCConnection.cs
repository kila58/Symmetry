using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Symmetry_GUI.Interop
{
    class IPCConnection : ConnectionBase
    {
        [DllImport("symlibui.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr IPCConnection_New();

        [DllImport("symlibui.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void IPCConnection_Delete(IntPtr pIPCConnection);

        [DllImport("symlibui.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void IPCConnection_Recv(IntPtr pIPCConnection);

        [DllImport("symlibui.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void IPCConnection_Send(IntPtr pIPCConnection, NetMsg nMsg, IntPtr pData, int iLen);

        [DllImport("symlibui.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void IPCConnection_SetContainer(IntPtr pIPCConnection, IntPtr pFileContainer);

        [DllImport("symlibui.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void IPCConnection_SendInput(IntPtr pIPCConnection, string sInput);

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public IPCConnection()
        ///
        /// \brief  Default constructor.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public IPCConnection()
        {
            // Create a new instance of the native net connection and 
            // stores it as a pointer
            this.ConnectionPointer = IPCConnection.IPCConnection_New();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn ~IPCConnection()
        ///
        /// \brief  Finaliser.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        ~IPCConnection()
        {
            this.Dispose();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void Dispose()
        ///
        /// \brief  Destroys the connection.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public void Dispose()
        {
            // Delete connection created in constructor
            IPCConnection.IPCConnection_Delete(this.ConnectionPointer);

            // Make sure we know we can't use it anymore
            this.ConnectionPointer = IntPtr.Zero;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public override void RecvMsg()
        ///
        /// \brief  See ConnectionBases's brief description.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public override void RecvMsg()
        {
            // Connection pointer not initialized or destroyed
            if (this.ConnectionPointer == IntPtr.Zero)
                return;

            IPCConnection.IPCConnection_Recv(this.ConnectionPointer);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public override void SendMsg(NetMsg msg, byte[] data)
        ///
        /// \brief  See ConnectionBases's brief description.
        ///
        /// \param  msg     The message.
        /// \param  data    The data.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public override void SendMsg(NetMsg msg, byte[] data)
        {
            // Connection pointer not initialized or destroyed
            if (this.ConnectionPointer == IntPtr.Zero)
                return;

            // Copy bytes into a allocated block of unmanaged memory
            IntPtr buf = Marshal.AllocHGlobal(data.Length);
            Marshal.Copy(data, 0, buf, data.Length);

            NETConnection.NETConnection_Send(this.ConnectionPointer, msg, buf, data.Length);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void SetContainer(FileContainer container)
        ///
        /// \brief  Sets the VirtualContainer (FileContainer) used by the native ipc connection.
        ///
        /// \param  container   The container.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public void SetContainer(FileContainer container)
        {
            if (container != null)
                IPCConnection.IPCConnection_SetContainer(this.GetNative(), container.GetNative());
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void SendInput(string input)
        ///
        /// \brief  Sends a line of input to the binary for it to process.
        ///
        /// \param  input   The input.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public void SendInput(string input)
        {
            // Connection pointer not initialized or destroyed
            if (this.ConnectionPointer == IntPtr.Zero)
                return;

            IPCConnection.IPCConnection_SendInput(this.GetNative(), input);
        }
    }
}
