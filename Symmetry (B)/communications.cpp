/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "communications.h"

#include "global.h"

#include "symmetry.h"

NAMESPACE_CORE(IO)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	Communications::Communications(HANDLE hIn, HANDLE hOut)
///
/// \brief	Constructor.
///
/// \param	hIn 	Handle of the anonymous pipe server's input.
/// \param	hOut	Handle of the anonymous pipe server's output.
////////////////////////////////////////////////////////////////////////////////////////////////////

Communications::Communications(HANDLE hIn, HANDLE hOut)
{
	this->m_nIPCClient = new Lib::IO::AnonPipeClient();
	this->m_nIPCClient->Connect(hIn, hOut);
	this->m_nIPCClient->SetAutoFlush(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	Communications::~Communications()
///
/// \brief	Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////

Communications::~Communications()
{
	DeleteAndNull(this->m_nIPCClient);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void Communications::Recv()
///
/// \brief	Receives a single IPC message from the injector.
////////////////////////////////////////////////////////////////////////////////////////////////////

void Communications::Recv()
{
	Lib::IO::Stream* pipe = this->m_nIPCClient;

	// Shouldn't happen?
	if (!pipe)
		return;

	switch (pipe->ReadInt16())
	{
	case Lib::IO::IPC_INIT:
	{
		// CWD
		Var::CWD = pipe->ReadString();

		// AES
		Lib::IO::AES256KeySet aes = { 0 };
		for (uint32 i = 0; i < sizeof(Lib::IO::AES256KeySet); i++)
			((uchar*)(&aes))[i] = pipe->ReadByte();

		Class::VirtualC->SetKeySet(aes);

		// Username
		Var::Username = pipe->ReadString();

		// Expiry date
		Var::ExpiryDate = pipe->ReadInt64();

		// Version
		Var::Version = pipe->ReadString();

		// Resource count
		Var::ResourceCount = pipe->ReadUInt32();

		break;
	}
	case Lib::IO::IPC_SNDFILE:
	{
		Lib::IO::VirtualFile* v = new Lib::IO::VirtualFile;

		v->Name = pipe->ReadString();
		v->Hash = pipe->ReadString();

		v->Encrypted = pipe->ReadBool();
		v->Mapped = pipe->ReadBool();
		v->LuaAutorun = pipe->ReadBool();

		int size = pipe->ReadInt32();
		v->Data.resize(size);

		for (int i = 0; i < size; i++)
			v->Data[i] = pipe->ReadByte();

		if (Mem::GetHash(v->Data) == v->Hash)
			Class::VirtualC->AddFile(v);

		break;
	}
	case Lib::IO::IPC_EXIT:
		// Exit
		throw Lib::IO::Exception(-1);
		break;
	case Lib::IO::IPC_CONINPUT:
	{
		for (CommandArgs args : Class::Console->ParseCommand(pipe->ReadString()))
		{
			Class::Console->PerformCommand(args);
		}
		break;
	}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void Communications::Send(Lib::IO::NetMsgOut& nMsg)
///
/// \brief	Send a network message to the injector via IPC.
///
/// \param [in,out]	nMsg	The message.
////////////////////////////////////////////////////////////////////////////////////////////////////

void Communications::Send(Lib::IO::NetMsgOut& nMsg)
{
	nMsg.Send(this->m_nIPCClient);
}

NAMESPACE_END2

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn void SetInputHandle(HANDLE* hpInput)
///
/// \brief	Sets the duplicated handle for the input pipe.  The IPC class uses this pipe
/// 		to read data from the injector.
///
/// \param [in,out]	hpInput	The pipe handle.
////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" __declspec(dllexport) void SetInputHandle(HANDLE* hpInput)
{
	Var::IPCInputPipe = *hpInput;

	// Any of the three variables could be received last
	if (Var::IPCInputPipe && Var::IPCOutputPipe && Var::IPCInjectorHandle)
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)IPCInit, NULL, NULL, NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	extern "C" void SetOutputHandle(HANDLE* hpOutput)
///
/// \brief	Sets the duplicated handle for the output pipe.  The IPC class uess this pipe
/// 		to send messages to the injector.
///
/// \param [in,out]	hpOutput	The pipe handle.
////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" __declspec(dllexport) void SetOutputHandle(HANDLE* hpOutput)
{
	Var::IPCOutputPipe = *hpOutput;

	// Any of the three variables could be received last
	if (Var::IPCInputPipe && Var::IPCOutputPipe && Var::IPCInjectorHandle)
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)IPCInit, NULL, NULL, NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	extern "C" void SetOutputHandle(HANDLE* hpOutput)
///
/// \brief	
///
/// \param [in,out]	hpOutput	The pipe handle.
////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" __declspec(dllexport) void SetInjectorPID(dword* dwProcID)
{
	if (!dwProcID || !*dwProcID)
		return;

	Var::IPCInjectorHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, *dwProcID);

	// Any of the three variables could be received last
	if (Var::IPCInputPipe && Var::IPCOutputPipe && Var::IPCInjectorHandle)
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)IPCInit, NULL, NULL, NULL);
}