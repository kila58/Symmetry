/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "pipe_client.h"

NAMESPACE_LIB(IO)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void PipeClient::~PipeClient()
///
/// \brief	Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////

PipeClient::~PipeClient()
{
	if (this->m_hPipeIn != INVALID_HANDLE_VALUE)
		CloseHandle(this->m_hPipeIn);

	if (this->m_hPipeOut != INVALID_HANDLE_VALUE)
		CloseHandle(this->m_hPipeOut);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void PipeClient::Connect(string cPipe)
///
/// \brief	Listens for pipe connections.  The name of the pipe will be sPipe + "in" or "out"
/// 		depending on the direction of traffic.  Both pipes are created.
///
/// \param	cPipe	The pipe name.
////////////////////////////////////////////////////////////////////////////////////////////////////

void PipeClient::Connect(string sPipeName)
{
	string domain = "\\\\.\\pipe\\";

	string in = domain, out = domain;

	// Reverse in/out directions so the pipe we're reading from is
	// the outputting pipe on the server
	in += sPipeName; in += "out";
	out += sPipeName; out += "in";

	// Create a pipe to read and a pipe to write data to and from
	this->m_hPipeIn = CreateFile(in.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	this->m_hPipeOut = CreateFile(out.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (this->m_hPipeIn == INVALID_HANDLE_VALUE || this->m_hPipeOut == INVALID_HANDLE_VALUE)
		throw Exception(GetLastError());

	this->m_bConnected = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void PipeClient::Flush()
///
/// \brief	Overrides IO::Stream::Flush.  Writes data into the pipe made in the constructor.
///
/// \exception	NetworkException	Thrown when a Network error condition occurs.
////////////////////////////////////////////////////////////////////////////////////////////////////

void PipeClient::Flush()
{
	string data = this->m_ssStream.str();
	dword written;

	if (!WriteFile(this->m_hPipeOut, data.c_str(), data.length(), &written, NULL) || written != data.length())
		throw Exception(GetLastError());

	// This is how you clear stringstreams apparently
	this->m_ssStream.str(string());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void PipeClient::OnBytesIn(uchar* cData, uint64 iSize)
///
/// \brief	Overrides IO::Stream::OnBytesIn.  Reads from the pipe made in the constructor.
///
/// \exception	NetworkException	Thrown when a Network error occurs.
///
/// \param [in,out]	cData	Pointer to the data to write to.
/// \param	iSize		 	Amount of data in bytes.
////////////////////////////////////////////////////////////////////////////////////////////////////

void PipeClient::OnBytesIn(uchar* cData, uint64 iSize)
{
	dword read;

	if (!ReadFile(this->m_hPipeIn, cData, (dword)iSize, &read, NULL) || read != iSize)
		throw Exception(GetLastError());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool PipeClient::IsConnected()
///
/// \brief	Returns true if the pipe client is connected to a pipe server, false otherwise.
///
/// \return	.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool PipeClient::IsConnected()
{
	return this->m_bConnected;
}

NAMESPACE_END2