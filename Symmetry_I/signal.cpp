/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "signal.h"
#include "export_utils.h"

NAMESPACE_LIB(Signal)

// Instance of above function
SendSignal_t SendSignalNET = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void Send(SignalType stSig, const char* cArgBuf, int iLength)
///
/// \brief	Sends a "signal" to the C# side.  With a single string argument.
///
/// \param	stSig		   	The signal type.
/// \param [in,out]	cArgBuf	A string argument. Null-terminated.
/// \param	iLength		   	Zero-based index of the length.
////////////////////////////////////////////////////////////////////////////////////////////////////

void Send(SignalType stSig, const char* cArgBuf, int iLength)
{
	if (SendSignalNET)
		SendSignalNET(stSig, cArgBuf, iLength);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(void) SetSignalFn()
///
/// \brief	Sets the function used to send signals from C++ to C#.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(void) SetSignalFn(SendSignal_t fn)
{
	SendSignalNET = fn;
}

NAMESPACE_END2