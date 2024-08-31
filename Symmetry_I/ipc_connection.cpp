/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#pragma once
#include "ipc_connection.h"
#include "mem.h"

#include "signal.h"

#include "export_utils.h"
#include "netmsg_ipc_exit.h"

NAMESPACE_LIB(IO)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	IPCConnection::IPCConnection()
///
/// \brief	Default constructor.
/// 		
/// 		Throws IO::NetworkException
////////////////////////////////////////////////////////////////////////////////////////////////////

IPCConnection::IPCConnection()
{
	this->m_hOut = NULL;
	this->m_hOut = NULL;

	this->m_pIPCPipe = new IO::AnonPipeServer();
	this->m_pIPCPipe->Listen(&this->m_hIn, &this->m_hOut);
	this->m_pIPCPipe->SetAutoFlush(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	IPCConnection::~IPCConnection()
///
/// \brief	Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////

IPCConnection::~IPCConnection()
{
	DeleteAndNull(this->m_pIPCPipe);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool IPCConnection::SendHandles(HANDLE* hIn, HANDLE* hOut)
///
/// \brief	Duplicates the anonymous pipe server's handles so that they can be used in another
/// 		process.
///
/// \param [in,out]	hIn 	Process' ID.
/// \param [in,out]	hOut	If non-null, the out.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool IPCConnection::SendHandles(HANDLE* hIn, HANDLE* hOut)
{
	// Static? perhaps.  But so is NETConnection and it's 
	// #define'd IP endpoint
	dword id = Mem::GetProcessIDByName(GAME_PROCNAME);

	if (!id)
		return false;

	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, id);

	if (process == INVALID_HANDLE_VALUE)
		return false;

	if (!DuplicateHandle(GetCurrentProcess(), this->m_hIn, process, hIn, NULL, FALSE, DUPLICATE_SAME_ACCESS))
		return false;

	if (!DuplicateHandle(GetCurrentProcess(), this->m_hOut, process, hOut, NULL, FALSE, DUPLICATE_SAME_ACCESS))
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void IPCConnection::Recv()
///
/// \brief	Receives a single network message from the IPC connection.  This function will halt
/// 		if there's no network message to be received and will return when a network message
/// 		is sent from the otherside.
/// 		
/// 		Throws IO::NetworkException.
////////////////////////////////////////////////////////////////////////////////////////////////////

void IPCConnection::Recv()
{
	IO::Stream* pipe = this->m_pIPCPipe;

	short type = pipe->ReadInt16();
	switch (type)
	{
	case IPC_PRINT:
	{
		string msg = pipe->ReadString();
		Signal::Send((SignalType)pipe->ReadInt32(), msg.c_str(), msg.length());
		break;
	}
	case IPC_CLEAR:
		Signal::Send(SIGNAL_CLEAR);
		break;
	case IPC_EXIT:
		// Tells C# to exit it's shit
		throw IO::Exception(-1);
	case IPC_ADDCOMMAND:
	case IPC_REMCOMMAND:
		string msg = pipe->ReadString();
		Signal::Send(type == IPC_ADDCOMMAND ? SIGNAL_ADDCOMMAND : SIGNAL_REMCOMMAND,
			msg.c_str(), msg.length());
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void IPCConnection::Send(IO::NetMsgOut& nMsg)
///
/// \brief	Sends a network message to the other side of this IPC connection.  This function will
/// 		halt until the other side reads all of the data that was sent.
/// 		
/// 		Throws IO::NetworkException.
///
/// \param [in,out]	nMsg	The message.
////////////////////////////////////////////////////////////////////////////////////////////////////

void IPCConnection::Send(IO::NetMsgOut& nMsg)
{
	nMsg.Send(this->m_pIPCPipe);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	IO::Stream* IPCConnection::GetStream()
///
/// \brief	Returns the underlying IO::Stream used by the ipc connection.
///
/// \return	See above.
////////////////////////////////////////////////////////////////////////////////////////////////////

IO::Stream* IPCConnection::GetStream()
{
	return this->m_pIPCPipe;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(IPCConnection*) IPCConnection_New()
///
/// \brief	Binding.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(IPCConnection*) IPCConnection_New()
{
	return new IPCConnection();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(void) IPCConnection_Delete(IPCConnection* nNet)
///
/// \brief	Binding.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(void) IPCConnection_Delete(IPCConnection* nNet)
{
	if (nNet)
		delete nNet;

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(void) IPCConnection_Recv(IPCConnection* nNet)
///
/// \brief	Binding.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(void) IPCConnection_Recv(IPCConnection* nNet)
{
	if (nNet)
		nNet->Recv();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(void) IPCConnection_Send(IPCConnection* nNet, int iType, char* cData, int iLen)
///
/// \brief	Binding.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(void) IPCConnection_Send(IPCConnection* nNet, int iType, char* cData, int iLen)
{
	if (!nNet)
		return;

	nNet->GetStream()->Write((short)iType);
	nNet->GetStream()->Write((uchar*)cData, (uint32)iLen);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(void) IPCConnection_SetContainer(IPCConnection* nNet,
/// 	IO::VirtualContainer* vContainer)
///
/// \brief	Binding.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(void) IPCConnection_SetContainer(IPCConnection* nNet, IO::VirtualContainer* vContainer)
{
	if (!nNet || !vContainer)
		return;

	nNet->SetContainer(vContainer);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(void) IPCConnection_SendInput(IPCConnection* nNet, wchar_t* wcInput)
///
/// \brief	Binding.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(void) IPCConnection_SendInput(IPCConnection* nNet, wchar_t* wcInput)
{
	if (!nNet || !wcInput)
		return;

	nNet->GetStream()->Write((short)IPC_CONINPUT);
	nNet->GetStream()->Write(string((char*)wcInput));
}

NAMESPACE_END2