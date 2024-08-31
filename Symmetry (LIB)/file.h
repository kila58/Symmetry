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

#undef DeleteFile
#undef CreateFile
#undef CreateDirectory

////////////////////////////////////////////////////////////////////////////////////////////////////
///									  == STATIC FUNCTIONS == 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool	FileExists(string);
bool	DirectoryExists(string);

bool	CreateFile(string);
bool	CreateDirectory(string);

bool	WriteFile(string, string);
string	ReadFile(string);
void	AppendFile(string, string);

bool	DeleteFile(string);
bool	DeleteDirectory(string);

void	MakeStringSafe(string&);
void	MakeDirSafe(string&);

NAMESPACE_END2