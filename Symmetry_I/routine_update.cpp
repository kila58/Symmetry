/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "export_utils.h"
#include "virtualcontainer.h"

#include "file.h"
#include "mem.h"

using namespace Lib;

enum UpdateStatus
{
	Failed,
	Required,
	UpToDate,
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(UpdateStatus) Rtn_CheckUpdate(IO::VirtualContainer* vContainer)
///
/// \brief	Update routine.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(UpdateStatus) Rtn_CheckUpdate(IO::VirtualContainer* vContainer, wchar_t* cLibPath, wchar_t* cExePath)
{
	if (!vContainer || !cLibPath || !cExePath)
		return UpdateStatus::Failed;

	char* lib = (char*)cLibPath;
	char* exe = (char*)cExePath;

	for (uint32 i = 0; i < vContainer->GetList().size(); i++)
	{
		IO::VirtualFile* file = vContainer->GetList()[i];

		// Injector files shouldn't be encrypted, don't support encryption on injector files
		if (file->IsInjectorFile && !file->Encrypted)
		{
			string path = file->Name;

			char cname[MAX_PATH];
			char cext[16];

			// Extract base filename and extension
			_splitpath_s(path.c_str(), 
				NULL, NULL,			// drive
				NULL, NULL,			// directory
				cname, MAX_PATH,		// filename
				cext, sizeof(cext));	// extenstion

			// Join name and extension
			string name = cname;
			name.append(cext);

			// Replacement engine doesn't need to exist
			if (name == "reng.exe")
				continue;

			// File needs to exist
			if (!IO::FileExists(name))
				return UpdateStatus::Required;
			
			// File needs to match
			if (Mem::GetHash(IO::ReadFile(name)) !=
				file->Hash)
				return UpdateStatus::Required;

		}
	}

	return UpdateStatus::UpToDate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(UpdateStatus) Rtn_Update(IO::VirtualContainer* vContainer)
///
/// \brief	Update install routine.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(bool) Rtn_Update(IO::VirtualContainer* vContainer, wchar_t* cLibPath, wchar_t* cExePath,
	wchar_t* wcUsername, wchar_t* wcPassword, int iBranch)
{
	if (!vContainer || !cLibPath || !cExePath)
		return UpdateStatus::Failed;

	char* lib		= (char*)cLibPath;
	char* exe		= (char*)cExePath;
	char* username	= (char*)wcUsername;
	char* password	= (char*)wcPassword;

	for (uint32 i = 0; i < vContainer->GetList().size(); i++)
	{
		IO::VirtualFile* file = vContainer->GetList()[i];

		// Injector files shouldn't be encrypted, don't support encryption on injector files
		if (file->IsInjectorFile && !file->Encrypted)
		{
			string path = file->Name;

			char cname[MAX_PATH];
			char cext[16];

			// Extract base filename and extension
			_splitpath_s(path.c_str(),
				NULL, NULL,				// drive
				NULL, NULL,				// directory
				cname, MAX_PATH,		// filename
				cext, sizeof(cext));	// extenstion

			// Join name and extension
			string name = cname;

			// Replacement engine doesn't need original extension
			if (name != "reng")
				name.append(cext);

			name.append(".syuf");

			// Delete temporary files that have stolen our name
			if (IO::FileExists(name))
			{
				if (!IO::DeleteFile(name))
					return false;
			}

			// Create file temporary files
			if (!IO::WriteFile(name, file->Data))
				return false;
		}
	}

	// Start the replacement engine
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));

	char arg[MAX_PATH];

	sprintf(arg, "\"%s\" \"%s\" \"%s\" \"%s\" %d",
		exe, lib, username, password, iBranch);

	if (!CreateProcess("reng.syuf", arg, NULL,
		NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
		return false;

	return true;
}