/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "apipe_server.h"

NAMESPACE_LIB(IO)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	AnonPipeServer::~AnonPipeServer()
///
/// \brief	Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////

AnonPipeServer::~AnonPipeServer()
{
	this->m_bClosing = true;
	dword placeholder;

	// We can't close the handles if we're reading something on another
	// thread
	if (this->m_dwReading > 0)
	{
		char* buf = (char*)malloc(this->m_dwReading);
		memset(buf, NULL, this->m_dwReading);

		if (!WriteFile(this->m_hPipeOutWrite, buf, this->m_dwReading, &placeholder, NULL))
		{
			free(buf);
			return;
		}
		else 
			free(buf);
	}

	// We can't close the handles if we're writing something on another
	// thread, writing will halt until something reads it
	if (this->m_dwWriting > 0)
	{
		char* buf = (char*)malloc(this->m_dwWriting);
		
		if (!ReadFile(this->m_hPipeInRead, buf, this->m_dwWriting, &placeholder, NULL))
		{
			free(buf);
			return;
		}
		else 
			free(buf);
	}

	if (this->m_hPipeInRead != INVALID_HANDLE_VALUE)
		CloseHandle(this->m_hPipeInRead);
		
	if (this->m_hPipeInWrite != INVALID_HANDLE_VALUE)
		CloseHandle(this->m_hPipeInWrite);

	if (this->m_hPipeOutRead != INVALID_HANDLE_VALUE)
		CloseHandle(this->m_hPipeOutRead);

	if (this->m_hPipeOutWrite != INVALID_HANDLE_VALUE)
		CloseHandle(this->m_hPipeOutWrite);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void AnonPipeServer::Listen(HANDLE* hpPipeIn, HANDLE* hpPipeOut)
///
/// \brief	Creates an anonymous and returns input and output handles for that pipe.
///
/// \exception	NetworkException	Thrown when a Network error condition occurs.
///
/// \param [in,out]	hpPipeIn 	If non-null, the input pipe.
/// \param [in,out]	hpPipeOut	If non-null, the output pipe.
////////////////////////////////////////////////////////////////////////////////////////////////////

void AnonPipeServer::Listen(HANDLE* hpPipeIn, HANDLE* hpPipeOut)
{
	this->m_hPipeInRead = INVALID_HANDLE_VALUE;
	this->m_hPipeInWrite = INVALID_HANDLE_VALUE;
	
	this->m_hPipeOutRead = INVALID_HANDLE_VALUE;
	this->m_hPipeOutWrite = INVALID_HANDLE_VALUE;

	// Input
	if (!CreatePipe(&this->m_hPipeInRead, &this->m_hPipeInWrite, NULL, 1024))
		throw Exception(GetLastError());

	// Output
	if (!CreatePipe(&this->m_hPipeOutRead, &this->m_hPipeOutWrite, NULL, 1024))
		throw Exception(GetLastError());

	if (this->m_hPipeInRead == INVALID_HANDLE_VALUE || this->m_hPipeInWrite == INVALID_HANDLE_VALUE)
		throw Exception(GetLastError());

	if (this->m_hPipeOutRead == INVALID_HANDLE_VALUE || this->m_hPipeOutWrite == INVALID_HANDLE_VALUE)
		throw Exception(GetLastError());

	*hpPipeIn = this->m_hPipeInRead;
	*hpPipeOut = this->m_hPipeOutWrite;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void AnonPipeServer::Flush()
///
/// \brief	Overrides IO::Stream::Flush.  We'll write any information from m_ssStream into the
/// 		output stream provided by CreatePiepe.
///
/// \exception	NetworkException	Thrown when a Network error condition occurs.
////////////////////////////////////////////////////////////////////////////////////////////////////

void AnonPipeServer::Flush()
{
	if (this->m_bClosing)
		throw IO::Exception(-3);

	string data = this->m_ssStream.str();
	dword written = 0;

	this->m_dwWriting = data.length();

	// Write the m_ssStream buffer into the pipe
	if (!WriteFile(this->m_hPipeInWrite, data.c_str(), data.length(), &written, NULL) || written != data.size())
		throw Exception(GetLastError());

	this->m_dwWriting = NULL;

	// This is how you clear stringstreams apparently
	this->m_ssStream.str(string());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void AnonPipeServer::OnBytesIn(uchar* cData, uint64 iSize)
///
/// \brief	Overrides IO::Stream::OnBytesIn.
///
/// \exception	NetworkException	Thrown when a Network error occurs.
///
/// \param [in,out]	cData	Pointer to the data to write to.
/// \param	iSize		 	Amount of data in bytes.
////////////////////////////////////////////////////////////////////////////////////////////////////

void AnonPipeServer::OnBytesIn(uchar* cData, uint64 iSize)
{
	if (this->m_bClosing)
		throw IO::Exception(-3);

	dword read = 0;

	this->m_dwReading = (dword)iSize;

	if (!ReadFile(this->m_hPipeOutRead, cData, (dword)iSize, &read, NULL) || read != iSize)
		throw Exception(GetLastError());

	this->m_dwReading = NULL;
}

NAMESPACE_END2