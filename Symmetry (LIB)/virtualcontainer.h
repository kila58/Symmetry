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
NAMESPACE_LIB(IO)

// Represents a single virtual file in the virtual container
struct VirtualFile
{
	string Name;
	string Hash;

	bool Encrypted;
	bool Mapped;
	bool LuaAutorun;
	bool IsInjectorFile;

	string Data;
};

// AES key set, for encryption/decryption
struct AES256KeySet
{
	uchar Key[16];
	uchar IV[16];
};

// Stores virtual files
class VirtualContainer
{
private:
	vector<VirtualFile*>	m_vVirtualFiles;
	AES256KeySet			m_aesKeySet;

public:

	VirtualContainer();

	void AddFile(VirtualFile*);
	void RemoveFile(VirtualFile*);

	VirtualFile*	FindByName(string);
	VirtualFile*	FindByHash(string);

	void			SetKeySet(AES256KeySet&);
	AES256KeySet&	GetKeySet();

	vector<VirtualFile*>	GetList();
};

NAMESPACE_END2