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
#include "client.h"

NAMESPACE_LIB(IO)

class NETConnection : public BaseConnection
{
private:

	// Connection with master server
	IO::NetworkClient*		m_nNetClient;

public:

	// Recv/Send netmsgs
	void Recv();
	void Send(IO::NetMsgOut&);

	// Reconnect
	void Reconnect();

	IO::Stream* GetStream();

	// Constructor/Destructor
	NETConnection();
	~NETConnection();
};

NAMESPACE_END2