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
#include "netmsg_base.h"

NAMESPACE_LIB(IO)

class IpcConIn : public NetMsgOut
{
private:
	string m_String;

public:

	IpcConIn(string sText)
	{
		this->m_String = sText;
	}

	virtual void Send(IO::Stream* sNet)
	{
		sNet->Write((short)IO::IPC_CONINPUT);
		sNet->Write(this->m_String);
	}
};

NAMESPACE_END2