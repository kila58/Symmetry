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
#include "global.h"

#define FileExistsG(path) Lib::IO::FileExists(Var::CWD + "/" + string(path))
#define DirectoryExistsG(path) Lib::IO::DirectoryExists(Var::CWD + "/" + string(path))

#define CreateFileG(path) Lib::IO::CreateFile(Var::CWD + "/" + string(path))
#define CreateDirectoryG(path) Lib::IO::CreateDirectory(Var::CWD + "/" + string(path))

#define WriteFileG(path, data) Lib::IO::WriteFile(Var::CWD + "/" + string(path), data)
#define ReadFileG(path) Lib::IO::ReadFile(Var::CWD + "/" + string(path))
#define AppendFileG(path, data) Lib::IO::AppendFile(Var::CWD + "/" + string(path), data)

#define DeleteFileG(path) Lib::IO::DeleteFile(Var::CWD + "/" + string(path))
#define DeleteDirectoryG(path) Lib::IO::DeleteDirectory(Var::CWD + "/" + string(path))

#define IsBadDirChar(_x_) _x_ == '|' || _x_ == '<' || _x_ == '>' || _x_ == '\"' || _x_ == '?' || _x_ == '*' || _x_ == ':'
#define IsBadNameChar(_x_) _x_ == '|' || _x_ == '<' || _x_ == '>' || _x_ == '\"' || _x_ == '?' || _x_ == '*' || _x_ == ':' || _x_ == '/' || _x_ == '\\'