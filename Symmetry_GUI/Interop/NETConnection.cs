using Symmetry_GUI.Core;
using Symmetry_GUI.Util;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Symmetry_GUI.Interop
{
    class NETConnection : ConnectionBase
    {
        [DllImport("symlibui.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr NETConnection_New();

        [DllImport("symlibui.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void NETConnection_Delete(IntPtr pNetConnection);

        [DllImport("symlibui.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void NETConnection_Recv(IntPtr pNetConnection);

        [DllImport("symlibui.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void NETConnection_Send(IntPtr pNetConnection, NetMsg nMsg, IntPtr pData, int iLen);

        [DllImport("symlibui.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void NETConnection_SetContainer(IntPtr pNetConnection, IntPtr pFileContainer);

        [DllImport("symlibui.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern byte NETConnection_Reconnect(IntPtr pNetConnection, string sUsername, string sPassword, 
            int iBranch);

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public NETConnection()
        ///
        /// \brief  Default constructor.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public NETConnection()
        {
            // Create a new instance of the native net connection and 
            // stores it as a pointer
            this.ConnectionPointer = NETConnection.NETConnection_New();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn ~NETConnection()
        ///
        /// \brief  Finaliser.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        ~NETConnection()
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
            NETConnection.NETConnection_Delete(this.ConnectionPointer);

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

            NETConnection.NETConnection_Recv(this.ConnectionPointer);
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
        /// \brief  Sets the VirtualContainer (FileContainer) used by the native net connection.
        ///
        /// \param  container   The container.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public void SetContainer(FileContainer container)
        {
            if (container != null)
                NETConnection.NETConnection_SetContainer(this.GetNative(), container.GetNative());
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void Reconnect(CredentialHandler credentials)
        ///
        /// \brief  Reconnects and reauthenticates a connection that had disconnected previously.
        ///
        /// \param  credentials The credentials.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public bool Reconnect(CredentialHandler credentials)
        {
            return NETConnection.NETConnection_Reconnect(this.GetNative(), 
                credentials.Username, credentials.Password, Branching.GetBranch()) != 0;
        }
    }
}
