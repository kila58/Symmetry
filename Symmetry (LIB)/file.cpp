/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "file.h"
#include "mem.h"

NAMESPACE_LIB(IO)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn bool FileExists(string sFileName)
///
/// \brief Uses ifstreams (f.good()) to query if a file exists.
///
/// \param sFileName Name of the file.
///
/// \return True if the file exists, false oterhwise
////////////////////////////////////////////////////////////////////////////////////////////////////

bool FileExists(string sFileName)
{
	MakeDirSafe(sFileName);

	ifstream f(sFileName);
	if (f.good())
	{
		f.close();
		return true;
	}
	else 
	{
		f.close();
		return false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn bool DirectoryExists(string sDir)
///
/// \brief Uses GetFileAttributesA to query if a directory exists.
///
/// \param sDir The dir.
///
/// \return True if the direcotry exists, false otherwise.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool DirectoryExists(string sDir)
{
	MakeDirSafe(sDir);

	dword wAttributes = GetFileAttributesA(sDir.c_str());

	if (wAttributes == INVALID_FILE_ATTRIBUTES)
		return false;

	return (wAttributes & FILE_ATTRIBUTE_DIRECTORY) == 1 ? true : false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn bool CreateFile(string sFileName)
///
/// \brief Creates an empty file.
///
/// \param sFileName Name of the file.
///
/// \return True if it was created succesfully, false otherwise.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CreateFile(string sFileName)
{
	MakeDirSafe(sFileName);

	ofstream s(sFileName);

	if (!s.good())
		return false;

	s.close();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn bool CreateDirectory(string sFileName)
///
/// \brief Creates a directory using CreateDirectoryA.
///
/// \param sDirName Name of the directory.
///
/// \return True if it succeeds, false otherwise.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CreateDirectory(string sDirName)
{
	MakeDirSafe(sDirName);

	string build = "";
	for (uint32 i = 0; i < sDirName.length(); i++)
	{
		// To avoid creating directories like "C:/"
		// NOTE: DirectoryExists would mark C:/ as a non-existant directory and
		// CreateDirectoryA would fail to create it
		if (build.length() > 3 || build[1] != ':')
		{
			// If we hit a directory indication (/ or \) we can
			// create the directory path we've built (as long the directory doesn't exist)
			if (sDirName[i] == '\\' || sDirName[i] == '/' && !DirectoryExists(build))
			{
				CreateDirectoryA(build.c_str(), NULL);
			}
		}

		build += sDirName[i];
	}

	// Create final directory too (again if it doesn't exist)
	// (this would happen anyway if there was a trailing slash)
	if (!DirectoryExists(sDirName))
		CreateDirectoryA(sDirName.c_str(), NULL);

	return DirectoryExists(sDirName.c_str()) == 1 ? true : false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn bool WriteFile(string, string)
///
/// \brief Writes sData to a file (sFileName).  The file will be created if it doesn't exist.
///
/// \param sFileName The first parameter.
/// \param sData The second parameter.
///
/// \return True if it succeeds, false otherwise.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool WriteFile(string sFileName, string sData)
{
	MakeDirSafe(sFileName);

	ofstream s(sFileName, ios::binary | ios::out);

	if (!s.good())
		return false;

	s << sData;
	s.close();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn string ReadFile(string sFileName)
///
/// \brief Returns the contents of a file (sFileName)
///
/// \param sFileName Name of the file to read.
///
/// \return The contents of the file.  An empty string will be returned if the file
/// 		couldn't be read.
////////////////////////////////////////////////////////////////////////////////////////////////////

string ReadFile(string sFileName)
{
	MakeDirSafe(sFileName);

	stringstream stream;

	if (!FileExists(sFileName))
		return "";

	ifstream f(sFileName, ios::in | ios::binary);

	if (!f.good())
		return "";

	stream << f.rdbuf();

	f.close();
	
	return stream.str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void AppendFile(string sFileName, string sText)
///
/// \brief	Appends the sText so a file.  The file will be created if it doesn't exist.
///
/// \param	sFileName	Name of the file to append to.
/// \param	sText	 	The text
/// 					
////////////////////////////////////////////////////////////////////////////////////////////////////

void AppendFile(string sFileName, string sText)
{
	MakeDirSafe(sFileName);

	ofstream f(sFileName, ios::out | ios::binary | ios::app);

	if (!f.good())
		return;

	f << sText;
	f.close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn bool DeleteFile(string sFileName)
///
/// \brief Deletes a file (sFileName) using DeleteFileA.
///
/// \param sFileName Name of the file to delete
///
/// \return True if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool DeleteFile(string sFileName)
{
	MakeDirSafe(sFileName);
	return DeleteFileA(sFileName.c_str()) == 1 ? true : false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn bool DeleteDirectory(string sDirName)
///
/// \brief Deletes a directory (sDirName) using RemoveDirectoryA.
///
/// \param sDirName Pathname of the directory.
///
/// \return True if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool DeleteDirectory(string sDirName)
{
	MakeDirSafe(sDirName);
	return RemoveDirectoryA(sDirName.c_str()) == 1 ? true : false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void MakeStringSafe(string& sPath)
///
/// \brief	Makes a string safe for files.  This removes device names, directory traversal
/// 		characters and bad-path characters.  This assumes the path is one directory or file
/// 		name, this will not make a full path safe.
///
/// \param	sPath	File path.
////////////////////////////////////////////////////////////////////////////////////////////////////

void MakeStringSafe(string& sPath)
{
	// Get rid of bad path characters
	Mem::ReplaceString(sPath, "|", "-");
	Mem::ReplaceString(sPath, "<", "-");
	Mem::ReplaceString(sPath, ">", "-");
	Mem::ReplaceString(sPath, "\"", "-");
	Mem::ReplaceString(sPath, "?", "-");
	Mem::ReplaceString(sPath, "*", "-");
	Mem::ReplaceString(sPath, ":", "-");

	// Fix device file names
	if (Mem::IsRegexMatch(sPath, "COM[1-9]"))
		sPath = "DCOM";

	if (Mem::IsRegexMatch(sPath, "LPT[1-8]"))
		sPath = "DLPT";

	if (Mem::IsRegexMatch(sPath, "NUL"))
		sPath = "DNUL";

	if (Mem::IsRegexMatch(sPath, "CON"))
		sPath = "DCON";

	if (Mem::IsRegexMatch(sPath, "PRN"))
		sPath = "DPRN";

	if (Mem::IsRegexMatch(sPath, "AUX"))
		sPath = "DAUX";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void MakeDirSafe(string& sDirectory)
///
/// \brief	Calls MakeStringSafe on each folder in a directory.
///
/// \param [in,out]	sDirectory	Pathname of the directory.
////////////////////////////////////////////////////////////////////////////////////////////////////

void MakeDirSafe(string& sDirectory)
{
	string build;
	string part;
	for (char c : sDirectory)
	{
		if (c == '\\' || c == '/')
		{
			// Don't make drive names safe
			if (!Mem::IsRegexMatch(part + c, "[a-z]:[/|\\\\]"))
				MakeStringSafe(part);

			// Add part and directory seperator to build
			build += part;
			build += c;

			// Reset part
			part = "";
		}
		else
			part += c;
	}

	if (part != "")
		MakeStringSafe(part);

	sDirectory = build + part;
}

NAMESPACE_END2