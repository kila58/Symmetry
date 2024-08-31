using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace Symmetry_GUI.UI
{
    // With help from:
    // http://stackoverflow.com/questions/10237781/get-current-scroll-position-from-rich-text-box-control
    
    public partial class RichTextAutoScroll : RichTextBox
    {
        private const int WM_VSCROLL = 0x115;
        private const int WM_MOUSEWHEEL = 0x20A;
        private const int WM_USER = 0x400;
        private const int SB_VERT = 1;
        private const int EM_SETSCROLLPOS = WM_USER + 222;
        private const int EM_GETSCROLLPOS = WM_USER + 221;

        [DllImport("user32.dll")]
        private static extern bool GetScrollRange(IntPtr hWnd, int nBar, out int lpMinPos, out int lpMaxPos);

        [DllImport("user32.dll")]
        private static extern IntPtr SendMessage(IntPtr hWnd, Int32 wMsg, UInt32 wParam, ref Point lParam);

        private Dictionary<char, Color> ColourFormats = new Dictionary<char,Color>()
        {
            {'\x82', Color.DarkGreen},
            {'\x83', Color.DarkCyan},
            {'\x84', Color.DarkRed},
            {'\x85', Color.DeepPink},
            {'\x86', Color.GreenYellow},
            {'\x88', Color.DarkGray},
            {'\x87', Color.Gray},
            {'\x89', Color.Blue},
            {'\x8A', Color.Green},
            {'\x8B', Color.Cyan},
            {'\x8C', Color.Red},
            {'\x8D', Color.Pink},
            {'\x8E', Color.Yellow},
            {'\x8F', Color.White}
        };

        public RichTextAutoScroll()
        {
            InitializeComponent();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void ScrollToBottom()
        ///
        /// \brief  Scrolls the vertical scrollbar to the it's bottom-most position.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public void ScrollToBottom()
        {
            if (this.Handle == IntPtr.Zero)
                return;

            int min, max;
            GetScrollRange(this.Handle, SB_VERT, out min, out max);

            // No point doing anything if the scrollbar can't scroll
            if (max == 0)
                return;

            Point point = new Point(0, max - this.Height);
            SendMessage(this.Handle, EM_SETSCROLLPOS, 0, ref point);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void AppendText(string Text)
        ///
        /// \brief  Overrides RichTexBox.AppendText. Overridden behaviour includes automatic scrolling
        ///         after the text is appended.
        ///
        /// \param  Text    The text.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public new void AppendText(string Text)
        {
            try
            {
                this.Invoke(new Action(() =>
                {
                    base.AppendText(Text);
                    this.ScrollToBottom();
                }));
            }
            catch (Exception) { }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public void AppendColouredText(string Text)
        ///
        /// \brief  Appends a coloured text using format specifies in the ColourFormats dictionary
        ///
        /// \param  Text    The text.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public void AppendColouredText(string Text)
        {
            try
            {
                this.Invoke(new Action(() =>
                {
                    foreach (char c in Text.ToCharArray())
                    {
                        if (this.ColourFormats.ContainsKey(c))
                        {
                            this.SelectionColor = ColourFormats[c];
                        }
                        else
                            this.AppendText(c.ToString());
                    }
                }));
            }
            catch (Exception) { }
        }
    }
}
