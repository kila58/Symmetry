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
#include "stream.h"

NAMESPACE_LIB(IO)

// Server TCP stream
class NetworkServer : public Stream
{
private:

	// Client and server sockets
	SOCKET m_sServerSocket;
	SOCKET m_sClientSocket;

public:
	NetworkServer();
	~NetworkServer();

	// Generic
	void Listen(string sPort = "0", int iTimeout = 3000);

	// Overridden
	virtual void Flush();
	virtual void OnBytesIn(uchar*, uint64);

	// Other
	void	SetTimeout(int);
	int		GetPort();

};

NAMESPACE_END2