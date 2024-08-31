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
#include "net_connection.h"

using namespace Lib;

// Here for reconnections
EXPORT(uint64) Rtn_Login(IO::NETConnection* nNet, wchar_t* wcUsername, wchar_t* wcPassword, int iBranch,
	char* cErrBuf, char* cVersionBuf, int iErrBufSize, int iVersionBufSize);