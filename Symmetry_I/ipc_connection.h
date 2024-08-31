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
#include "base_connection.h"
#include "apipe_server.h"

NAMESPACE_LIB(IO)

class IPCConnection : public BaseConnection
{
private:
	// Anon pipe handles
	HANDLE m_hOut, m_hIn;

	// Anon pipe server
	IO::AnonPipeServer* m_pIPCPipe;

public:

	void Recv();
	void Send(IO::NetMsgOut&);

	IO::Stream* GetStream();

	// Constructor/Destructor
	IPCConnection();
	~IPCConnection();

	bool SendHandles(HANDLE* hIn, HANDLE* hOut);
};

NAMESPACE_END2