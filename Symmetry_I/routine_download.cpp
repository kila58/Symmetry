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
#include "virtualcontainer.h"

using namespace Lib;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(bool) Rtn_Download(IO::NETConnection* nNet, IO::VirtualContainer* vContainer,
/// 	int iBranch, int* ipFileCount)
///
/// \brief	Download routine.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(bool) Rtn_Download(IO::NETConnection* nNet, IO::VirtualContainer* vContainer, int iBranch,
	int* ipFileCount)
{
	if (!nNet || !vContainer || !ipFileCount)
		return false;

	IO::Stream* net = nNet->GetStream();

	net->Write((short)IO::NET_REQAES);

	// Set AES key set 
	// 16	16
	// key  iv
	IO::AES256KeySet aes = {0};
	for (uint32 i = 0; i < sizeof(IO::AES256KeySet); i++)
		((uchar*)(&aes))[i] = net->ReadByte();

	vContainer->SetKeySet(aes);

	// Request files
	net->Write((short)IO::NET_REQFILES);

	// Tell the server what branch we're using
	net->Write(iBranch);

	if (!net->ReadBool())
		return false;

	*ipFileCount = net->ReadInt32();

	while (vContainer->GetList().size() != *ipFileCount)
		nNet->Recv();

	return true;
}