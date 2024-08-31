/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "virtualcontainer_exp.h"
#include "export_utils.h"

// Global virtualcontainer instance
IO::VirtualContainer* g_pVirtualContainer = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(IO::VirtualContainer*) FileContainer_New()
///
/// \brief	Creates a new VirtualContainer instance.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(IO::VirtualContainer*) FileContainer_New()
{
	// Bad idea?
	return new IO::VirtualContainer();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(void) FileContainer_Delete(IO::VirtualContainer* vcContainer)
///
/// \brief	Deletes a VirtualContainer instance.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(void) FileContainer_Delete(IO::VirtualContainer* vcContainer)
{
	// Dangerous?
	if (vcContainer)
		delete vcContainer;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(int) FileContainer_GetFileCount(IO::VirtualContainer* vcContainer)
///
/// \brief	Returns the amount of files in a VirtualContainer.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(int) FileContainer_GetFileCount(IO::VirtualContainer* vcContainer)
{
	return (int)vcContainer->GetList().size();
}