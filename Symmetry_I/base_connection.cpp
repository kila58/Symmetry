/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "base_connection.h"

NAMESPACE_LIB(IO)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void BaseConnection::SetContainer(IO::VirtualContainer* vContainer)
///
/// \brief	Sets the virtual container.
////////////////////////////////////////////////////////////////////////////////////////////////////

void BaseConnection::SetContainer(IO::VirtualContainer* vContainer)
{
	this->m_vContainer = vContainer;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	IO::VirtualContainer* BaseConnection::GetContainer()
///
/// \brief	Gets the virtual container.
////////////////////////////////////////////////////////////////////////////////////////////////////

IO::VirtualContainer* BaseConnection::GetContainer()
{
	return this->m_vContainer;
}

NAMESPACE_END2