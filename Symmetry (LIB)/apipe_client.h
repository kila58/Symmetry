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

// Named pipe server
class AnonPipeClient : public Stream
{
private:

	HANDLE	m_hPipeIn, m_hPipeOut;
	bool	m_bConnected;

public:

	~AnonPipeClient();

	// Generic
	void Connect(HANDLE hPipeIn, HANDLE hPipeOut);

	// Overridden
	virtual void Flush();
	virtual void OnBytesIn(uchar*, uint64);

	bool		IsConnected();
};

NAMESPACE_END2