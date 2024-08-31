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
class AnonPipeServer : public Stream
{
private:

	HANDLE	m_hPipeOutRead, m_hPipeOutWrite;
	HANDLE	m_hPipeInRead, m_hPipeInWrite;

	dword	m_dwReading;
	dword	m_dwWriting;

	bool	m_bClosing;

public:

	~AnonPipeServer();

	// Generic
	void Listen(HANDLE* hpPipeIn, HANDLE* hpPipeOut);

	// Overridden
	virtual void Flush();
	virtual void OnBytesIn(uchar*, uint64);
};

NAMESPACE_END2