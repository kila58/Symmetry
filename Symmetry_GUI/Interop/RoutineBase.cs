using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Symmetry_GUI.Interop
{
    enum RtnState 
    {
        Active,     // routine is actively performing some task
        Finished,   // routine is finished, another routine can be performed
        Failed,     // routine failed, retry or exit
        Exit,       // routine wants symmetry to explicitly exit
        Waiting,    // routine is waiting to be started
    }

    class RoutineBase
    {
        public RtnState RoutineState = RtnState.Waiting;
        public string Name = "RtnBase";

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public virtual RoutineBase GetNextRoutine()
        ///
        /// \brief  Returns the routine that should be performed next.  If that routine is null, no
        ///         routine should be performed.  In the case of a null routine, the application should
        ///         exit, routines should only finish when symmetry does.
        ///
        /// \return The next routine.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public virtual RoutineBase GetNextRoutine()
        {
            return null;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public virtual RtnState Run()
        ///
        /// \brief  Runs the routine, this should be overrun by seperate routine.
        ///
        /// \return RoutineState after the routine finished processing.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public virtual RtnState Run()
        {
            // Don't really need to do this here at all, especially since as 
            // the code to call this (return base.Run()) isn't really shorter than:
            return this.RoutineState;
        }
    }
}
