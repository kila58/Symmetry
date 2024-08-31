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
#include "error.h"

NAMESPACE_LIB(IO)

////////////////////////////////////////////////////////////////////////////////////////////////////
///									     == MACRO/DEFINITIONS == 
////////////////////////////////////////////////////////////////////////////////////////////////////

#define SYM_SERVER_PORT ":11337"

#if 0
#define SYM_SERVER_ADDRESS "192.168.1.150"
#else
#define SYM_SERVER_ADDRESS "login.symhack.com"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
///									      == ENUMS/TYPEDEFS == 
////////////////////////////////////////////////////////////////////////////////////////////////////

// DEPERACTED
/*
enum NetMsg
{
	// Master server
	PING			= 0x1,	// Refreshes the master server's timeout for us
	NET_REQAES,				// Requests AES keys
	NET_REQFILES,			// Requests files, each file is sent back in a new net message
	NET_UPDATE,				// Sent to us from the master server, occurs serverside when svn updates
	NET_SNDFILE,			// Sent for each file on the server that's meant to be sent to us, only sentafter NET_REQFILES
	NET_CHECKINJ,			// Checks injector files

	// IPC
	IPC_RELAY,				// NET_UPDATE and any other relayed netmsgs are relayed via IPC using this NetMsg
	IPC_PRINT,				// Prints everywhere.  If sent from the injector it will print ingame, works the same in reverse
	IPC_CONINPUT,			// Relays console, this is only relayed when the end relaying the input doesn't handle it too
	IPC_CLEAR,				// Clears any console buffer that belongs to the target of the IPC msg
	IPC_GET_VARS,			// Used by the binary to request that a global variable be sent to it
	IPC_REQFILES,			// Sent from the binary to the injector to request virtul files that it needs
	IPC_SNDFILE,			// Represents a single file sent from the injector to the binary
	IPC_CONTAB,				// Sent to the binary from the injector, handles autocompletion
};*/

// Network messages exchanged with binary and injector
enum IPCMsg
{
	IPC_PRINT,		// b -> i: prints a message on the injector's console window
	IPC_CONINPUT,	// i -> b: processes a line of input on the binary
	IPC_CLEAR,		// b -> i: clears the injector's console window
	IPC_SNDFILE,	// i -> b: sends a single file to the binary
	IPC_CONTAB,		// i -> b: sends a string to the binary, the binary sends an autocompleted string back
	IPC_INIT,		// i -> b: tells the binary that an initialization process has started, prerequisites will be sent after this
	IPC_EXIT,		// i -- b: signals an exit routine
	IPC_ADDCOMMAND,	// b -> i: adds a command to the console's autocomplete table
	IPC_REMCOMMAND, // b -> i: removes a command from the console's autocomplete table
};

enum NETMsg
{
	PING,			// i -> s: keeps an active login connection alive
	NET_REQAES,		// i -> s: requests generated aes key & iv from the server
	NET_REQFILES,	// i -> s: requests files from the server
	NET_UPDATE,		// s -> i: notifaction of svn update
	NET_SNDFILE,	// s -> i: sends a single file from the server to the injector
	NET_SUBEND,		// s -> i: tells the injector that our subscription ended
};

////////////////////////////////////////////////////////////////////////////////////////////////////
///										== CLASS/STRUCTURES == 
////////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: move this to a relevant file
class Exception
{
public:
	Exception(int iError)
	{
		SetLastErrorA(iError);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	int Exception::GetErrorCode()
	///
	/// \brief	Gets error code passed in through the constructor.
	///
	/// \return	The error code.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	int GetErrorCode()
	{
		return GetLastErrorA();
	}
};

NAMESPACE_END2