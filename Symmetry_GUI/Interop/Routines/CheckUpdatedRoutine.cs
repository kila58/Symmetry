using Symmetry_GUI.IO;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Symmetry_GUI.Interop.Routines
{
    class CheckUpdatedRoutine : RoutineBase
    {
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public CheckUpdatedRoutine()
        ///
        /// \brief  Default constructor.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public CheckUpdatedRoutine()
        {
            this.Name = "update-check";
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public override RoutineBase GetNextRoutine()
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public override RoutineBase GetNextRoutine()
        {
            return new LoginRoutine();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public override RtnState Run()
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public override RtnState Run()
        {
            this.RoutineState = RtnState.Active;

            try
            {
                // Shit way to check?  Perhaps.
                if (File.Exists("reng.syuf") || File.Exists("reng"))
                {
                    File.Delete("reng.syuf");
                    File.Delete("reng");

                    SymIO.Write(Level.Good, "Updated succesfully!");
                }

                this.RoutineState = RtnState.Finished;
            }
            catch (UnauthorizedAccessException)
            {
                this.RoutineState = RtnState.Failed;
                SymIO.Write(Level.Bad, "UpdateErr: Access denied in directory {0} ({1})", Environment.CurrentDirectory, "reng");
            }
            
            return base.Run();
        }

    }
}
