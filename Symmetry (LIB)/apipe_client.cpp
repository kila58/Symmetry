/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "apipe_client.h"

NAMESPACE_LIB(IO)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	AnonPipeClient::~AnonPipeClient()
///
/// \brief	Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////

AnonPipeClient::~AnonPipeClient()
{
	// These handles should be duplicated from the server's handles
	// so that the destructor on the server doesn't clean them up before
	// we do here, or vice versa

	if (this->m_hPipeIn != INVALID_HANDLE_VALUE)
		CloseHandle(this->m_hPipeIn);

	if (this->m_hPipeOut != INVALID_HANDLE_VALUE)
		CloseHandle(this->m_hPipeOut);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void AnonPipeClient::Connect(HANDLE hPipeIn, HANDLE hPipeOut)
///
/// \brief	Connects to an anonymous pipe server, using an input and output pipe handle.
///
/// \exception	NetworkException	Thrown when a Network error condition occurs.
///
/// \param	hPipeIn 	The pipe name.
/// \param	hPipeOut	Handle of the pipe out.
////////////////////////////////////////////////////////////////////////////////////////////////////

void AnonPipeClient::Connect(HANDLE hPipeIn, HANDLE hPipeOut)
{
	this->m_hPipeIn = hPipeIn;
	this->m_hPipeOut = hPipeOut;

	if (this->m_hPipeIn == INVALID_HANDLE_VALUE || this->m_hPipeOut == INVALID_HANDLE_VALUE)
		throw Exception(GetLastError());

	this->m_bConnected = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void AnonPipeClient::Flush()
///
/// \brief	Overrides IO::Stream::Flush.  Writes data into the output pipe.
///
/// \exception	NetworkException	Thrown when a Network error condition occurs.
////////////////////////////////////////////////////////////////////////////////////////////////////

void AnonPipeClient::Flush()
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
/// \brief	Overrides IO::Stream::OnBytesIn.  Reads from the input pipe.
///
/// \exception	NetworkException	Thrown when a Network error occurs.
///
/// \param [in,out]	cData	Pointer to the data to write to.
/// \param	iSize		 	Amount of data in bytes.
////////////////////////////////////////////////////////////////////////////////////////////////////

void AnonPipeClient::OnBytesIn(uchar* cData, uint64 iSize)
{
	dword read;

	if (!ReadFile(this->m_hPipeIn, cData, (dword)iSize, &read, NULL) || read != iSize)
		throw Exception(GetLastError());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool AnonPipeClient::IsConnected()
///
/// \brief	Returns true if the pipe client is connected to a pipe server, false otherwise.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool AnonPipeClient::IsConnected()
{
	return this->m_bConnected;
}

NAMESPACE_END2