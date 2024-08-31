using Symmetry_GUI.UI;
namespace Symmetry_GUI.UI
{
    partial class MainWindow
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.ConsoleInput = new System.Windows.Forms.TextBox();
            this.ConsoleOutput = new Symmetry_GUI.UI.RichTextAutoScroll();
            this.SuspendLayout();
            // 
            // ConsoleInput
            // 
            this.ConsoleInput.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Append;
            this.ConsoleInput.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.CustomSource;
            this.ConsoleInput.BackColor = System.Drawing.Color.Black;
            this.ConsoleInput.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.ConsoleInput.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.ConsoleInput.Font = new System.Drawing.Font("Courier New", 10F);
            this.ConsoleInput.ForeColor = System.Drawing.SystemColors.WindowFrame;
            this.ConsoleInput.Location = new System.Drawing.Point(0, 497);
            this.ConsoleInput.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.ConsoleInput.Name = "ConsoleInput";
            this.ConsoleInput.Size = new System.Drawing.Size(956, 26);
            this.ConsoleInput.TabIndex = 1;
            this.ConsoleInput.KeyDown += new System.Windows.Forms.KeyEventHandler(this.ConsoleInput_KeyDown);
            // 
            // ConsoleOutput
            // 
            this.ConsoleOutput.BackColor = System.Drawing.Color.Black;
            this.ConsoleOutput.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.ConsoleOutput.Dock = System.Windows.Forms.DockStyle.Top;
            this.ConsoleOutput.Font = new System.Drawing.Font("Courier New", 10F);
            this.ConsoleOutput.ForeColor = System.Drawing.SystemColors.WindowFrame;
            this.ConsoleOutput.Location = new System.Drawing.Point(0, 0);
            this.ConsoleOutput.Margin = new System.Windows.Forms.Padding(0);
            this.ConsoleOutput.Name = "ConsoleOutput";
            this.ConsoleOutput.ReadOnly = true;
            this.ConsoleOutput.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
            this.ConsoleOutput.Size = new System.Drawing.Size(956, 495);
            this.ConsoleOutput.TabIndex = 0;
            this.ConsoleOutput.Text = "";
            this.ConsoleOutput.KeyDown += new System.Windows.Forms.KeyEventHandler(this.ConsoleOutput_KeyDown);
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(956, 523);
            this.Controls.Add(this.ConsoleInput);
            this.Controls.Add(this.ConsoleOutput);
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.Name = "MainWindow";
            this.Text = "Symmetry Console";
            this.Resize += new System.EventHandler(this.MainWindow_Resize);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox ConsoleInput;
        private RichTextAutoScroll ConsoleOutput;
    }
}

