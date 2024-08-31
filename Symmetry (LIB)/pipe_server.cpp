/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "pipe_server.h"

NAMESPACE_LIB(IO)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	PipeServer::~PipeServer()
///
/// \brief	Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////

PipeServer::~PipeServer()
{
	if (this->m_hPipeIn != INVALID_HANDLE_VALUE)
	{
		DisconnectNamedPipe(this->m_hPipeIn);
		CloseHandle(this->m_hPipeIn);
	}

	if (this->m_hPipeOut != INVALID_HANDLE_VALUE)
	{
		DisconnectNamedPipe(this->m_hPipeOut);
		CloseHandle(this->m_hPipeOut);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void PipeServer::Listen(char_c* cPipe)
///
/// \brief	Opens a pipe connection, this call will block whilst it waits for a client.
///
/// \param [in,out]	cPipe	The pipe name.
////////////////////////////////////////////////////////////////////////////////////////////////////

void PipeServer::Listen(string sPipeName)
{
	this->m_bConnected = false;

	string domain = "\\\\.\\pipe\\";

	string in = domain, out = domain;

	in += sPipeName; in += "in";
	out += sPipeName; out += "out";

	// Create a pipe to read and a pipe to write data to and from
	this->m_hPipeIn		= CreateNamedPipe(in.c_str(), PIPE_ACCESS_DUPLEX, PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 1024, 1024, 0, NULL);
	this->m_hPipeOut	= CreateNamedPipe(out.c_str(), PIPE_ACCESS_DUPLEX, PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 1024, 1024, 0, NULL);

	if (this->m_hPipeIn == INVALID_HANDLE_VALUE || this->m_hPipeOut == INVALID_HANDLE_VALUE)
		throw Exception(GetLastError());

	if (!ConnectNamedPipe(this->m_hPipeIn, NULL) && GetLastError() != ERROR_PIPE_CONNECTED)
	{
		CloseHandle(this->m_hPipeIn);
		throw Exception(GetLastError());
	}

	if (!ConnectNamedPipe(this->m_hPipeOut, NULL) && GetLastError() != ERROR_PIPE_CONNECTED)
	{
		CloseHandle(this->m_hPipeOut);
		throw Exception(GetLastError());
	}

	this->m_bConnected = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void PipeServer::Flush()
///
/// \brief	Overrides IO::Stream::Flush.  We'll write any information from m_ssStream
/// 		into a newly connected named pipe.
///
/// \exception	NetworkException	Thrown when a Network error condition occurs.
////////////////////////////////////////////////////////////////////////////////////////////////////

void PipeServer::Flush()
{
	string data = this->m_ssStream.str();
	dword written = 0;

	// Write the m_ssStream buffer into the pipe
	if (!WriteFile(this->m_hPipeOut, data.c_str(), data.length(), &written, NULL) || written != data.size())
		throw Exception(GetLastError());

	// Flush the pipe
	FlushFileBuffers(this->m_hPipeOut);

	// This is how you clear stringstreams apparently
	this->m_ssStream.str(string());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void PipeServer::OnBytesIn(uchar* cData, uint64 iSize)
///
/// \brief	Overrides IO::Stream::OnBytesIn.  We'll open a pipe, wait for a connection and read
/// 		iSize bytes from the pipe into cData.
///
/// \exception	NetworkException	Thrown when a Network error occurs.
///
/// \param [in,out]	cData	Pointer to the data to write to.
/// \param	iSize		 	Amount of data in bytes.
////////////////////////////////////////////////////////////////////////////////////////////////////

void PipeServer::OnBytesIn(uchar* cData, uint64 iSize)
{
	dword read = 0;

	if (!ReadFile(this->m_hPipeIn, cData, (dword)iSize, &read, NULL) || read != iSize)
		throw Exception(GetLastError());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool PipeServer::IsConnected()
///
/// \brief	Returns true if the pipe server is connected to a pipe client, false otherwise.
///
/// \return	.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool PipeServer::IsConnected()
{
	return this->m_bConnected;
}

NAMESPACE_END2