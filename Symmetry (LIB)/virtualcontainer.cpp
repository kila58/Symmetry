/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "virtualcontainer.h"
#include "mem.h"

NAMESPACE_LIB(IO)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	VirtualFree::~VirtualContainer()
///
/// \brief	Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////

VirtualContainer::VirtualContainer()
{
	memset(&this->m_aesKeySet, NULL, sizeof(AES256KeySet));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void VirtualContainer::AddFile(VirtualFile* vFile)
///
/// \brief	Adds a file to the virtual container.
///
/// \param [in,out]	vFile	The file to add.
////////////////////////////////////////////////////////////////////////////////////////////////////

void VirtualContainer::AddFile(VirtualFile* vFile)
{
	if (vFile != NULL)
		this->m_vVirtualFiles.push_back(vFile);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void VirtualContainer::RemoveFile(VirtualFile* vFile)
///
/// \brief	Removes vFile from the virtual container, if it exists inside it.
///
/// \param [in,out]	vFile	The file to remove.
////////////////////////////////////////////////////////////////////////////////////////////////////

void VirtualContainer::RemoveFile(VirtualFile* vFile)
{
	for (uint32 i = 0; i < this->m_vVirtualFiles.size(); i++)
	{
		if (this->m_vVirtualFiles[i] == vFile)
		{
			this->m_vVirtualFiles.erase(this->m_vVirtualFiles.begin() + i);
			delete vFile;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	VirtualFile* VirtualContainer::FindByName(string sName)
///
/// \brief	Returns a virtual file inside the container by name.
///
/// \param	sName	The name.
///
/// \return	A file by sName, NULL if it wasn't found.
////////////////////////////////////////////////////////////////////////////////////////////////////

VirtualFile* VirtualContainer::FindByName(string sName)
{
	for (uint32 i = 0; i < this->m_vVirtualFiles.size(); i++)
	{
		if (this->m_vVirtualFiles[i]->Name == sName)
		{
			return this->m_vVirtualFiles[i];
		}
	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	VirtualFile* VirtualContainer::FindByHash(string sHash)
///
/// \brief	Returns a virtual file inside the container by data hash.
///
/// \param	sHash	The hash.
///
/// \return A file by sHash, NULL if it wasn't found.
////////////////////////////////////////////////////////////////////////////////////////////////////

VirtualFile* VirtualContainer::FindByHash(string sHash)
{
	for (uint32 i = 0; i < this->m_vVirtualFiles.size(); i++)
	{
		if (this->m_vVirtualFiles[i]->Hash == sHash)
		{
			return this->m_vVirtualFiles[i];
		}
	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void VirtualContainer::SetKeySet(AES256KeySet* aesKeySet)
///
/// \brief	Sets 256bit AES key and IV stored in the virtual container.
////////////////////////////////////////////////////////////////////////////////////////////////////

void VirtualContainer::SetKeySet(AES256KeySet& aesKeySet)
{
	this->m_aesKeySet = aesKeySet;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	AES256KeySet* VirtualContainer::GetKeySet()
///
/// \brief	Gets key set set with VirtualContainer::SetKeySet.
///
/// \return	The AES key set (m_aesKeySet).
////////////////////////////////////////////////////////////////////////////////////////////////////

AES256KeySet& VirtualContainer::GetKeySet()
{
	return this->m_aesKeySet;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	vector<VirtualFile*> VirtualContainer::GetList()
///
/// \brief	Gets the list of files inside this container.
///
/// \return	.
////////////////////////////////////////////////////////////////////////////////////////////////////

vector<VirtualFile*> VirtualContainer::GetList()
{
	return this->m_vVirtualFiles;
}

NAMESPACE_END2