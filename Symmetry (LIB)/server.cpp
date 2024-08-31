/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "server.h"
#include "net.h"

NAMESPACE_LIB(IO)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void NetworkServer::Listen(string sPort, int iTimeout)
///
/// \brief	Listens for a TCP connection on sPort.  A single client connection is accepted if
/// 		this succeeds, this function will wait indefinitely until a connection is made.
///
/// \exception	NetworkException   	Thrown when a Network error occurs.
/// \exception	BadAddressException	Thrown when a Bad Address error occurs.
///
/// \param	sPort   	The port to listen on.
/// \param	iTimeout	Timeout for reading and writing in milliseconds.
////////////////////////////////////////////////////////////////////////////////////////////////////

void NetworkServer::Listen(string sPort, int iTimeout)
{
	WSADATA wsa;

	this->m_sClientSocket = INVALID_SOCKET;
	this->m_sServerSocket = INVALID_SOCKET;

	addrinfo* res, hints;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		WSACleanup();
		throw Exception(WSAGetLastError());
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve ip:port
	if (getaddrinfo(NULL, sPort.c_str(), &hints, &res) != 0)
	{
		WSACleanup();
		throw Exception(WSAGetLastError());
	}

	this->m_sServerSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	if (this->m_sServerSocket == INVALID_SOCKET)
	{
		freeaddrinfo(res); WSACleanup();
		throw Exception(WSAGetLastError());
	}

	// Bind the server to the port specified in the constructor
	if (::bind(this->m_sServerSocket, res->ai_addr, res->ai_addrlen) == SOCKET_ERROR)
	{
		closesocket(this->m_sServerSocket); freeaddrinfo(res); WSACleanup();
		throw Exception(WSAGetLastError());
	}

	// We don't need the adress info anymore
	freeaddrinfo(res);

	if (listen(this->m_sServerSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(this->m_sServerSocket); WSACleanup();
		throw Exception(WSAGetLastError());
	}

	// Accept a single client socket
	this->m_sClientSocket = accept(this->m_sServerSocket, NULL, NULL);
	if (this->m_sClientSocket == INVALID_SOCKET)
	{
		closesocket(this->m_sServerSocket); WSACleanup();
		throw Exception(WSAGetLastError());
	}

	if (iTimeout > 0)
		this->SetTimeout(iTimeout);

	// No longer need server socket
	closesocket(this->m_sServerSocket);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	NetworkServer::NetworkServer()
///
/// \brief	Default constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////

NetworkServer::NetworkServer() : Stream()
{
	this->m_sClientSocket = SOCKET_ERROR;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	NetworkServer::~NetworkServer()
///
/// \brief	Destructor.  Cleans up shit.
////////////////////////////////////////////////////////////////////////////////////////////////////

NetworkServer::~NetworkServer()
{
	// Flush any anything
	this->Flush();

	if (this->m_sClientSocket != SOCKET_ERROR)
	{
		shutdown(this->m_sClientSocket, SD_BOTH);
	}

	closesocket(this->m_sClientSocket);
	WSACleanup();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void NetworkServer::Flush()
///
/// \brief	Overrides IO::Stream::Flush.  We'll write any information in m_ssStream here.
///
/// \exception	TimeoutException	Thrown when a Timeout error occurs.
/// \exception	NetworkException	Thrown when a Network error occurs.
////////////////////////////////////////////////////////////////////////////////////////////////////

void NetworkServer::Flush()
{
	string data = this->m_ssStream.str();

	if (this->m_sClientSocket == SOCKET_ERROR)
		return;

	if (send(this->m_sClientSocket, data.c_str(), data.length(), 0) != data.length())
		throw Exception(WSAGetLastError());

	// This is how you clear stringstreams apparently
	this->m_ssStream.str(string());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void NetworkServer::OnBytesIn(uchar* cData, uint64 iSize)
///
/// \brief	Overrides IO::Stream:OnBytesIn.
///
/// \exception	TimeoutException	Thrown when a Timeout error occurs.
/// \exception	NetworkException	Thrown when a Network error occurs.
///
/// \param [in,out]	cData	A pointer to a buffer where read text will be stored.
/// \param	iSize		 	The amount of bytes to read.
////////////////////////////////////////////////////////////////////////////////////////////////////

void NetworkServer::OnBytesIn(uchar* cData, uint64 iSize)
{
	if (this->m_sClientSocket == SOCKET_ERROR)
		return;

	if (recv(this->m_sClientSocket, (char*)cData, (int)iSize, 0) != iSize)
		throw Exception(WSAGetLastError());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void NetworkServer::SetTimeout(int iTimeout)
///
/// \brief	Sets the read & write timeout for this server.
///
/// \param	iTimeout	Timeout in milliseconds.
////////////////////////////////////////////////////////////////////////////////////////////////////

void NetworkServer::SetTimeout(int iTimeout)
{
	setsockopt(this->m_sClientSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&iTimeout, sizeof(int));
	setsockopt(this->m_sClientSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&iTimeout, sizeof(int));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int NetworkServer::GetPort()
///
/// \brief	Returns the local port for the socket server.  If the function fails
/// 		it will return -1.
///
/// \return	The port.
////////////////////////////////////////////////////////////////////////////////////////////////////

int NetworkServer::GetPort()
{
	sockaddr_in info;
	int len = sizeof(info);

	if (getsockname(this->m_sServerSocket, (sockaddr*)&info, &len) != SOCKET_ERROR)
	{
		return info.sin_port;
	}
	
	return -1;
}

NAMESPACE_END2