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


class IpcPrint : public NetMsgOut
{
private:
	string	m_String;
	int		m_iLevel;

public:

	IpcPrint(int iLevel, string sText)
	{
		this->m_String = sText;
		this->m_iLevel = iLevel;
	}

	virtual void Send(IO::Stream* sNet)
	{
		sNet->Write((short)IO::IPC_PRINT);
		sNet->Write(this->m_String);
		sNet->Write(this->m_iLevel);
	}
};

NAMESPACE_END2