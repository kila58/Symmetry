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

#include "ipc_connection.h"
#include "virtualcontainer.h"

#include "mem.h"
#include "manmap.h"

#include <direct.h>

using namespace Lib;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(void) Rtn_Init(IO::IPCConnection* nNet, IO::VirtualContainer* vContainer,
/// 	wchar_t* wcUsername, time_t tExpiryDate, dword dwModuleBaseAddr)
///
/// \brief	Initialize routine.
///
/// \exception	IO::NetworkException	Thrown when a Network error condition occurs.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(void) Rtn_Init(IO::IPCConnection* nNet, IO::VirtualContainer* vContainer, wchar_t* wcUsername, 
	time_t tExpiryDate, wchar_t* wcVersion, dword dwModuleBaseAddr)
{
	if (!nNet || !vContainer || !wcUsername)
		throw IO::Exception(-1);

	IO::Stream* ipc = nNet->GetStream();

	uint32 id		= Mem::GetProcessIDByName(GAME_PROCNAME);
	uint32 _id		= GetCurrentProcessId();

	// This will duplicate the handles in the target process, though
	// we'll still need to send these values to the target process so it
	// knows what handles are our anonymous pipe handles
	HANDLE in, out;
	nNet->SendHandles(&in, &out);

	dword setinj	= dwModuleBaseAddr;
	dword setin		= dwModuleBaseAddr;
	dword setout	= dwModuleBaseAddr;

	// We need these two functions that exported by the binary to 
	// "send" the anonymous pipe duplicated input and output handles
	for (IO::VirtualFile* v : vContainer->GetList())
	{
		// Should only be one mapped file
		if (v->Mapped)
		{
			string data = v->Data;

			if (v->Encrypted)
			{
				// Decrypt if required
				data = Mem::AESDecrypt(v->Data, vContainer->GetKeySet().Key,
					vContainer->GetKeySet().IV);
			}

			setinj	+= (dword)ManualMap::ResolveExport(data, "SetInjectorPID");
			setin	+= (dword)ManualMap::ResolveExport(data, "SetInputHandle");
			setout	+= (dword)ManualMap::ResolveExport(data, "SetOutputHandle");

			break;
		}
		
	}

	// Call remote functions and return false if they fail
	if (setin == dwModuleBaseAddr || setout == dwModuleBaseAddr || setinj == dwModuleBaseAddr)
		throw IO::Exception(-2);

	if (!Mem::CallRemoteFunction(id, setinj, &_id, sizeof(uint32)))
		throw IO::Exception(-3);

	if (!Mem::CallRemoteFunction(id, setin, &in, sizeof(HANDLE)))
		throw IO::Exception(-4);

	if (!Mem::CallRemoteFunction(id, setout, &out, sizeof(HANDLE)))
		throw IO::Exception(-5);

	// Now that the binary knows about our handles used for commmunication, we can
	// send variables it needs to operate

	// Determine current working directory 
	char* cwd = _getcwd(NULL, MAX_PATH);

	// Count resources that the injector's meant to have
	uint32 rcount = 0;
	for (IO::VirtualFile* v : vContainer->GetList())
	{
		if (!v->Mapped && !v->IsInjectorFile)
			rcount++;
	}

	// Send variable prerequisite chunk
	// SE3 ERROR HERE
	ipc->Write((short)IO::IPC_INIT);
	/* CWD		*/	ipc->Write(string(cwd));
	/* AES		*/	ipc->Write((uchar*)&vContainer->GetKeySet(), sizeof(IO::AES256KeySet));
	/* USERNAME	*/	ipc->Write(string((char*)wcUsername));
	/* EXPIRY	*/	ipc->Write(tExpiryDate);
	/* VERSION	*/  ipc->Write(string((char*)wcVersion));
	/* RCOUNT	*/	ipc->Write(rcount);

	// Allocated with malloc
	free(cwd);

	// Send files that they need
	for (IO::VirtualFile* v : vContainer->GetList())
	{
		if (v->Mapped || v->IsInjectorFile) 
			continue;

		ipc->Write((short)IO::IPC_SNDFILE);

		ipc->Write(v->Name);
		ipc->Write(v->Hash);

		ipc->Write(v->Encrypted);
		ipc->Write(v->Mapped);
		ipc->Write(v->LuaAutorun);

		ipc->Write(v->Data.size());

		for (uint32 j = 0; j < v->Data.size(); j++)
			ipc->Write((uchar)v->Data[j]);
	}
}