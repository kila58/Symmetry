using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Symmetry_GUI.Interop
{
    class FileContainer
    {
        private IntPtr FileContainerPointer = IntPtr.Zero;

        // Native functions
        [DllImport("symlibui.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr FileContainer_New();

        [DllImport("symlibui.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void FileContainer_Delete(IntPtr pFileContainer);

        [DllImport("symlibui.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int FileContainer_GetFileCount(IntPtr pFileContainer);

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public FileContainer()
        ///
        /// \brief  Default constructor.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public FileContainer()
        {
            this.FileContainerPointer = FileContainer.FileContainer_New();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public ~FileContainer()
        ///
        /// \brief  Finaliser.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        ~FileContainer()
        {
            this.Dispose();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void Dispose()
        ///
        /// \brief  Deletes the file container.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public void Dispose()
        {
            if (this.FileContainerPointer != IntPtr.Zero)
                FileContainer.FileContainer_Delete(this.FileContainerPointer);

            this.FileContainerPointer = IntPtr.Zero;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public IntPtr GetNative()
        ///
        /// \brief  Gets the native net file container pointer, used when a native function wants to use
        ///         the file container.
        ///
        /// \return The native file container pointer.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public IntPtr GetNative()
        {
            return this.FileContainerPointer;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public int GetFileCount()
        ///
        /// \brief  Returns the amount of files in the C++ FileContainer class.
        ///
        /// \return The file count.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public int GetFileCount()
        {
            if (this.FileContainerPointer == IntPtr.Zero)
                return -1;

            return FileContainer.FileContainer_GetFileCount(this.FileContainerPointer);
        }
    }
}
