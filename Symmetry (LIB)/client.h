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

#include "net.h"
#include "stream.h"

NAMESPACE_LIB(IO)

// Client TCP stream
class NetworkClient : public Stream
{
private:

	// Socket connection instance
	SOCKET m_sSocket;

public:
	NetworkClient();
	~NetworkClient();

	// Generic
	void Connect(string, int iTimeout = 3000);

	// Overridden
	virtual void Flush();
	virtual void OnBytesIn(uchar*, uint64);

	// Other
	void	SetTimeout(int);
	int		SetOption(int iLevel, int iOptName, char* pVal, int iSize);
	int		GetPort();
};

// Error definitons
#define BAD_WSA			-1
#define BAD_ADDRINFO	-2
#define BAD_SOCKET		-3
#define BAD_CONNECT		-4
#define BAD_SEND		-5
#define BAD_READ		-6

NAMESPACE_END2