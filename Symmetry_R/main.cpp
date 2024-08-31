/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <windows.h>
#include <SetupAPI.h>
#include <stdlib.h>
#include <stdio.h>
#include <Psapi.h>
#include <tlhelp32.h>
#include <vdmdbg.h>
#include <conio.h>
using namespace std;

enum ProcFileErr
{
	PROC_CONFLICT,
	PROC_RENAME_FAIL,
	PROC_NO_EXTENSION,
	PROC_SUCCESS
};

enum ProcUpdateErr
{
	PROC_PARENT_OPEN,
	PROC_BAD_FIND_HANDLE,
	PROC_BAD_FILE_PROCESS,
	PROC_BAD_RENAME,
	PROC_BAD_RELAUNCH,
	SUCCESS
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	inline bool exists(const std::string& name)
///
/// \brief	Checks if a file exists.
////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool exists(const std::string& name) {
	ifstream f(name.c_str());
	if (f.good()) {
		f.close();
		return true;
	}
	else {
		f.close();
		return false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	DWORD GetParentProcessId()
///
/// \brief	Gets the parent's process ID. 
/// 		https://gist.github.com/253013/d47b90159cf8ffa4d92448614b748aa1d235ebe4
////////////////////////////////////////////////////////////////////////////////////////////////////

DWORD GetParentProcessId()
{
	HANDLE hSnapshot;
	PROCESSENTRY32 pe32;
	DWORD ppid = 0, pid = GetCurrentProcessId();

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	__try{
		if (hSnapshot == INVALID_HANDLE_VALUE) __leave;

		ZeroMemory(&pe32, sizeof(pe32));
		pe32.dwSize = sizeof(pe32);
		if (!Process32First(hSnapshot, &pe32)) __leave;

		do{
			if (pe32.th32ProcessID == pid){
				ppid = pe32.th32ParentProcessID;
				break;
			}
		} while (Process32Next(hSnapshot, &pe32));

	}
	__finally{
		if (hSnapshot != INVALID_HANDLE_VALUE) CloseHandle(hSnapshot);
	}
	return ppid;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool WaitForParent()
///
/// \brief	Waits for the parent process to exit.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool WaitForParent(DWORD dwWait)
{
	DWORD id = GetParentProcessId();

	if (!id)
		return false;

	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, id);
	
	if (handle == INVALID_HANDLE_VALUE)
		return false;

	WaitForSingleObject(handle, dwWait);
	CloseHandle(handle);
		
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	ProcFileErr _ProcessFile(string sFileName)
///
/// \brief	Process the file described by name.
///
/// \param	sFileName	Filename of the file.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////////////////////////

ProcFileErr ProcessFile(string sFileName)
{
	// Don't need to process current directory
	if (sFileName == ".")
		return ProcFileErr::PROC_SUCCESS;

	// Don't need to process previous directory
	if (sFileName == "..")
		return ProcFileErr::PROC_SUCCESS;

	char name[MAX_PATH];
	char extension[MAX_PATH];

	// Extract base filename and extension
	_splitpath_s(sFileName.c_str(),
		NULL, NULL,						// drive
		NULL, NULL,						// directory
		name, sizeof(name),				// filename
		extension, sizeof(extension));	// extenstion

	// C++ strings
	string str_name = name;
	string str_ext = extension;

	// Don't need to process non .syuf files
	if (str_ext != ".syuf")
		return ProcFileErr::PROC_SUCCESS;

	// New name has to not exist
	if (exists(str_name))
	{
		if (!DeleteFile(str_name.c_str()))
			return ProcFileErr::PROC_CONFLICT;
	}

	// Renaming file must be succesful
	if (rename(sFileName.c_str(), str_name.c_str()) != 0)
		return ProcFileErr::PROC_RENAME_FAIL;

	return ProcFileErr::PROC_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int PerformUpdate(int argc, char** argv)
///
/// \brief	Performs the update.
////////////////////////////////////////////////////////////////////////////////////////////////////

ProcUpdateErr PerformUpdate(char* pszExe, char* pszLib = "", char* pszUsername = "",
	char* pszPassword = "", char* pszBranch = "")
{
	// We can't replace the parent's files if the process is running, 
	// wait for 30s max for it to finish
	if (!WaitForParent(30000))
		return ProcUpdateErr::PROC_PARENT_OPEN;

	WIN32_FIND_DATA fd;
	HANDLE find = FindFirstFile("*", &fd);

	if (find == INVALID_HANDLE_VALUE)
		return ProcUpdateErr::PROC_BAD_FIND_HANDLE;

	// Rename all remove .syuf from everything
	do
	{
		ProcFileErr status = ProcessFile(fd.cFileName);
		if (status != ProcFileErr::PROC_SUCCESS)
		{
#ifdef BUILDMODE_BETA
			MessageBox(0, ("ProcessFileErrror: " + to_string(status)).c_str(), "Update Error", 0);
#endif
		}
	} while (FindNextFile(find, &fd));
	FindClose(find);

	// Make sure the injector has the same name
	if (exists("symmetry.exe") && string(pszExe) != "")
	{
		char name[MAX_PATH];
		char extension[MAX_PATH];

		// Extract base filename and extension
		_splitpath_s(pszExe,
			NULL, NULL,						// drive
			NULL, NULL,						// directory
			name, sizeof(name),				// filename
			extension, sizeof(extension));	// extenstion
		
		string str_name = string(name) + string(extension);

		if (rename("symmetry.exe", str_name.c_str()) != 0)
			return ProcUpdateErr::PROC_BAD_RENAME;
	}

	// Make sure the library has the same name
	if (exists("symlibui.dll") && string(pszLib) != "")
	{
		char name[MAX_PATH];
		char extension[MAX_PATH];

		// Extract base filename and extension
		_splitpath_s(pszLib,
			NULL, NULL,						// drive
			NULL, NULL,						// directory
			name, sizeof(name),				// filename
			extension, sizeof(extension));	// extenstion

		string str_name = string(name) + string(extension);

		if (rename("symlibui.dll", str_name.c_str()) != 0)
			return ProcUpdateErr::PROC_BAD_RENAME;
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));

	char arg[0xFF];
	if (strlen(pszBranch) > 0)
	{
		sprintf(arg, "-username \"%s\" -password \"%s\" -branch %s", pszUsername,
			pszPassword, pszBranch);
	}
	else
	{
		sprintf(arg, "-username \"%s\" -password \"%s\"", pszUsername,
			pszPassword);
	}

	// Restart injector
	CreateProcess(pszExe, arg, NULL,
		NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

	return ProcUpdateErr::SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int main(int argc, char** argv)
///
/// \brief	Main entry-point for this application.
///
/// \param	argc	Number of command-line arguments.
/// \param	argv	Array of command-line argument strings.
///
/// \return	Exit-code for the process - 0 for success, else an error code.
////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
	if (argc <= 0)
		return EXIT_FAILURE;

	ProcUpdateErr code;
	switch (argc)
	{
	case 1:	// exepath
		code = PerformUpdate(argv[0]); break;
	case 2: // exepath + libpath
		code = PerformUpdate(argv[0], argv[1]); break;
	case 3:	// exepath + libpath + username
		code = PerformUpdate(argv[0], argv[1], argv[2]); break;
	case 4: // exepath + libpath + username + password
		code = PerformUpdate(argv[0], argv[1], argv[2], argv[3]); break;
	case 5: // exepath + libpath + username + password + branch
		code = PerformUpdate(argv[0], argv[1], argv[2], argv[3], argv[4]); break;
	}

	if (code == ProcUpdateErr::SUCCESS)
		return EXIT_SUCCESS;

#ifdef BUILDMODE_BETA
	MessageBox(0, ("ProcUpdateErr: " + to_string(code)).c_str(), "Update Error", 0);
#endif

	return EXIT_FAILURE;
}