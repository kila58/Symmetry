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
#include "generic.h"
#include "net.h"
#include "netmsg_base.h"

#include "virtualcontainer.h"

NAMESPACE_LIB(IO)

class BaseConnection
{
protected:

	// Instance of a virtualcontainer, needed by both Net and IPC connections
	IO::VirtualContainer*	m_vContainer;

public:

	// Both throw IO::NetworkException!
	virtual void Recv(){};
	virtual void Send(IO::NetMsgOut&){};

	// Set/Get virtualcontainer
	void					SetContainer(IO::VirtualContainer* vContainer);
	IO::VirtualContainer*	GetContainer();
};

NAMESPACE_END2