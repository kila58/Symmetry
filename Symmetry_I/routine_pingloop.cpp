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
#include "netmsg_ping.h"

using namespace Lib;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(bool) Rtn_Download(IO::NETConnection* nNet, IO::VirtualContainer* vContainer,
/// 	int* ipFileCount)
///
/// \brief	Ping loop routine.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(void) Rtn_PingLoop(IO::NETConnection* nNet)
{
	if (!nNet)
		return;

	nNet->Send(IO::NetPing());
}