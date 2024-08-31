/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "net_connection.h"
#include "mem.h"

#include "signal.h" // temp

#include "virtualcontainer.h"
#include "virtualcontainer_exp.h"

#include "export_utils.h"

#include "netmsg_ping.h"

// Required routines for reconnections
#include "routine_login.h"
#include "routine_pingloop.h"

NAMESPACE_LIB(IO)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	NETConnection::NETConnection()
///
/// \brief	Default constructor.
/// 		
/// 		Throws IO::NetworkException.
////////////////////////////////////////////////////////////////////////////////////////////////////

NETConnection::NETConnection()
{
	this->m_nNetClient = new IO::NetworkClient();
	this->m_nNetClient->Connect(SYM_SERVER_ADDRESS SYM_SERVER_PORT, 0);
	this->m_nNetClient->SetAutoFlush(true);

	// Timeouts 10s
	this->m_nNetClient->SetTimeout(10000);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void NETConnection::Reconnect()
///
/// \brief	Reconnects this network connection.
////////////////////////////////////////////////////////////////////////////////////////////////////

void NETConnection::Reconnect()
{
	DeleteAndNull(this->m_nNetClient);

	this->m_nNetClient = new IO::NetworkClient();
	this->m_nNetClient->Connect(SYM_SERVER_ADDRESS SYM_SERVER_PORT, 0);
	this->m_nNetClient->SetAutoFlush(true);

	// Timeouts 10s
	this->m_nNetClient->SetTimeout(10000);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	IPCConnection::~IPCConnection()
///
/// \brief	Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////

NETConnection::~NETConnection()
{
	DeleteAndNull(this->m_nNetClient);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void IPCConnection::Recv()
///
/// \brief	Receives a single network message from the net connection.  This function will halt
/// 		if there's no network message to be received and will return when a network message
/// 		is sent from the otherside.
/// 		
/// 		Throws IO::NetworkException.
////////////////////////////////////////////////////////////////////////////////////////////////////

void NETConnection::Recv()
{
	IO::Stream* net = this->m_nNetClient;

	switch (net->ReadInt16())
	{
	case NET_UPDATE:
	{
		string msg = "RECEIVED NETWORK UPDATE";
		Signal::Send(SIGNAL_PRINT_DEBUG, msg.c_str(), msg.length());
		break;
	}
	case NET_SNDFILE:
	{
		IO::VirtualFile* v = new IO::VirtualFile;

		// Name/hash
		v->Name = net->ReadString();
		v->Hash = net->ReadString();

		// Flagst
		v->Encrypted = net->ReadBool();
		v->Mapped = net->ReadBool();
		v->LuaAutorun = net->ReadBool();
		v->IsInjectorFile = net->ReadBool();

		// Data
		int size = net->ReadInt32();
		v->Data.resize(size);

		for (int i = 0; i < size; i++)
			v->Data[i] = net->ReadByte();

		if (Mem::GetHash(v->Data) == v->Hash)
		{
			net->Write(true);	
		
			if (this->m_vContainer)
				this->m_vContainer->AddFile(v);
		}
		else
		{
			net->Write(false);
		}

		break;
	}
	case NET_SUBEND:
		Signal::Send(SIGNAL_SUBEND);
		break;
	case PING:

		// Reply instantly, server is delayed will resend this request in x seconds
		net->Write((short)PING);
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void NETConnection::Send(IO::NetMsgOut& nMsg)
///
/// \brief	Sends a network message to the other side of this net connection.  This function will
/// 		halt until the other side reads all of the data that was sent.
/// 		
/// 		Throws IO::Exception.
///
/// \param [in,out]	nMsg	The message.
////////////////////////////////////////////////////////////////////////////////////////////////////

void NETConnection::Send(IO::NetMsgOut& nMsg)
{
	nMsg.Send(this->m_nNetClient);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	IO::Stream* NETConnection::GetStream()
///
/// \brief	Returns the underlying IO::Stream used by the network connection.
///
/// \return	See above.
////////////////////////////////////////////////////////////////////////////////////////////////////

IO::Stream* NETConnection::GetStream()
{
	return this->m_nNetClient;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(NETConnection*) NETConnection_New()
///
/// \brief	Binding.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(NETConnection*) NETConnection_New()
{
	return new NETConnection();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(void) NETConnection_Delete(NETConnection* nNet)
///
/// \brief	Binding.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(void) NETConnection_Delete(NETConnection* nNet)
{
	if (nNet)
		delete nNet;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(void) NETConnection_Recv(NETConnection* nNet)
///
/// \brief	Binding.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(void) NETConnection_Recv(NETConnection* nNet)
{
	if (nNet)
		nNet->Recv();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(void) NETConnection_Send(NETConnection* nNet, int iType, char* cData, int iLen)
///
/// \brief	Binding.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(void) NETConnection_Send(NETConnection* nNet, int iType, char* cData, int iLen)
{
	if (!nNet)
		return;

	nNet->GetStream()->Write((short)iType);
	nNet->GetStream()->Write((uchar*)cData, (uint32)iLen);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(void) NETConnection_SetContainer()
///
/// \brief	Binding.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(void) NETConnection_SetContainer(NETConnection* nNet, IO::VirtualContainer* vContainer)
{
	if (!nNet || !vContainer)
		return;

	nNet->SetContainer(vContainer);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(bool) NETConnection_Reconnect(NETConnection* nNet, wchar_t* wcUsername,
/// 	wchar_t* wcPassword, int iBranch)
///
/// \brief	Binding.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(bool) NETConnection_Reconnect(NETConnection* nNet, wchar_t* wcUsername, wchar_t* wcPassword, int iBranch)
{
	if (!nNet)
		return false;

	nNet->Reconnect();

	// All we want to do is log in again and throw errors if it fails
	char err[0xFFF];
	char ver[0xFFF];

	bool result = Rtn_Login(nNet, wcUsername, wcPassword, iBranch,
		err, ver, sizeof(err), sizeof(ver)) > 0;

	// Restart ping loop, only if the login was succesful
	if (result)
		Rtn_PingLoop(nNet);

	// Return login results
	return result;
}

NAMESPACE_END2