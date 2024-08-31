/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "export_utils.h"
#include "error.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(int) GetLastErr()
///
/// \brief	Returns a integer used as an alternative to GetLastError in win32.  The Win32 last
/// 		error is reset when an exception is thrown, which makes it inapplicable.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(int) GetLastErr()
{
	return GetLastErrorA();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(void) SetLastErr(int iError)
///
/// \brief	Matching SetLastError function for the GetLastError function above.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(void) SetLastErr(int iError)
{
	SetLastErrorA(iError);
}