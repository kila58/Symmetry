/**                  ====================                      *
*      _____                                __               *
*     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
*     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
*    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
*   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
*        /____/                                  /____/      *
*   - by Defc0n                                              *
*                  ====================                      **/

#include "client.h"
#include "mem.h"

NAMESPACE_LIB(IO)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void NetworkClient::Connect(string sAddress, int iTimeout)
///
/// \brief	Connects to sAddress using the TCP protocol.  This function is blocking, this
/// 		function CAN halt.  Any read operations done after this can also halt, but they won't
/// 		halt any longer than iTimeout milliseconds.
///
/// \exception	NetworkException   	Thrown when a Network error occurs.
/// \exception	BadAddressException	Thrown when a Bad Address error occurs.
///
/// \param	sAddress	The address to connect to, must contain ip and port seperated by a ":".
/// 					Example: 127.0.0.1:69.
/// \param	iTimeout	Timeout for connections, reading and writing in milliseconds.
////////////////////////////////////////////////////////////////////////////////////////////////////

void NetworkClient::Connect(string sAddress, int iTimeout)
{
	WSADATA wsa;
	this->m_sSocket = INVALID_SOCKET;

	addrinfo *res, hints;

	int iSepPos = sAddress.find(":");

	// Split the ip from the port
	string	ip		= sAddress.substr(0, iSepPos);
	string	port	= sAddress.substr(iSepPos + 1, sAddress.length()).c_str();

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		WSACleanup();
		throw Exception(WSAGetLastError() ? WSAGetLastError() : BAD_WSA);
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family		= AF_INET;
	hints.ai_socktype	= SOCK_STREAM;
	hints.ai_protocol	= IPPROTO_TCP;

	// Resolve ip:port
	if (getaddrinfo(ip.c_str(), port.c_str(), &hints, &res) != 0)
	{
		WSACleanup();
		throw Exception(WSAGetLastError() ? WSAGetLastError() : BAD_ADDRINFO);
	}

	// Create the client socket
	this->m_sSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	if (this->m_sSocket == INVALID_SOCKET)
	{
		WSACleanup();
		throw Exception(WSAGetLastError() ? WSAGetLastError() : BAD_SOCKET);
	}

	if (iTimeout > 0)
		this->SetTimeout(iTimeout);

	// Connect the client socket to the socket server
	if (connect(this->m_sSocket, res->ai_addr, res->ai_addrlen) == SOCKET_ERROR)
	{
		closesocket(this->m_sSocket);
		this->m_sSocket = INVALID_SOCKET;
	}

	// The socket will be an INVALID_SOCKET here if none of the addresses above worked
	if (this->m_sSocket == INVALID_SOCKET)
	{
		WSACleanup();
		throw Exception(WSAGetLastError() ? WSAGetLastError() : BAD_CONNECT);
	}

	freeaddrinfo(res);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	NetworkClient::NetworkClient()
///
/// \brief	Default constructor.  This is only here to call the Stream constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////

NetworkClient::NetworkClient() : Stream()
{
	this->m_sSocket = SOCKET_ERROR;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	NetworkClient::~NetworkClient()
///
/// \brief	Destructor.  Cleans up shit
////////////////////////////////////////////////////////////////////////////////////////////////////

NetworkClient::~NetworkClient()
{
	if (this->m_sSocket != SOCKET_ERROR)
	{
		shutdown(this->m_sSocket, SD_BOTH);
		closesocket(this->m_sSocket);

		WSACleanup();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void NetworkClient::Flush()
///
/// \brief	Overrides IO::Stream::Flush.  We'll write any information in m_ssStream here.
///
/// \exception	TimeoutException	Thrown when a Timeout error occurs.
/// \exception	NetworkException	Thrown when a Network error occurs.
////////////////////////////////////////////////////////////////////////////////////////////////////

void NetworkClient::Flush()
{
	string data = this->m_ssStream.str();
	
	if (send(this->m_sSocket, data.c_str(), data.length(), 0) != data.length())
		throw Exception(WSAGetLastError() ? WSAGetLastError() : BAD_SEND);

	// This is how you clear stringstreams apparently
	this->m_ssStream.str(string());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void NetworkClient::OnBytesIn(uchar* cData, uint64 iSize)
///
/// \brief	Overrides IO::Stream:OnBytesIn.
///
/// \exception	TimeoutException	Thrown when a Timeout error occurs.
/// \exception	NetworkException	Thrown when a Network error occurs.
///
/// \param [in,out]	cData	A pointer to a buffer where read text will be stored.
/// \param	iSize		 	The amount of bytes to read.
////////////////////////////////////////////////////////////////////////////////////////////////////

void NetworkClient::OnBytesIn(uchar* cData, uint64 iSize)
{
	if (recv(this->m_sSocket, (char*)cData, (int)iSize, 0) != iSize)
		throw Exception(WSAGetLastError() ? WSAGetLastError() : BAD_READ);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void NetworkClient::SetTimeout(int iTimeout)
///
/// \brief	Sets the read &amp; write timeout for this connection.
///
/// \param	iTimeout	Timeout in milliseconds.
////////////////////////////////////////////////////////////////////////////////////////////////////

void NetworkClient::SetTimeout(int iTimeout)
{
	setsockopt(this->m_sSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&iTimeout, sizeof(int));
	setsockopt(this->m_sSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&iTimeout, sizeof(int));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int NetworkClient::SetOption(int iLevel, int int iOptName, char* pVal, int iSize)
///
/// \brief	Sets an option on the underlying socket.
////////////////////////////////////////////////////////////////////////////////////////////////////

int NetworkClient::SetOption(int iLevel, int iOptName, char* pVal, int iSize)
{
	return setsockopt(this->m_sSocket, iLevel, iOptName, pVal, iSize);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int NetworkClient::GetPort()
///
/// \brief	Returns the local port for the socket client.  If the function fails it will return -
/// 		1.
///
/// \return	The port.
////////////////////////////////////////////////////////////////////////////////////////////////////

int NetworkClient::GetPort()
{
	sockaddr_in info;
	int len = sizeof(info);

	if (getsockname(this->m_sSocket, (sockaddr*)&info, &len) != SOCKET_ERROR)
	{
		return info.sin_port;
	}

	return -1;
}

NAMESPACE_END2