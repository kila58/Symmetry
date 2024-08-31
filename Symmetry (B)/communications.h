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
NAMESPACE_CORE(IO)

// Actually IPC communications in disguise!
class Communications
{
private:
	Lib::IO::AnonPipeClient* m_nIPCClient;

public:

	void Recv();
	void Send(Lib::IO::NetMsgOut&);

	Lib::IO::Stream* GetStream();

	// Constructor/Destructor
	Communications(HANDLE hIn, HANDLE hOut);
	~Communications();

};

NAMESPACE_END2