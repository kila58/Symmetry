/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "export_utils.h"
#include "net_connection.h"

// CryptoPP
#include "cryptlib.h"
#include "rsa.h"
#include "osrng.h"

#include "mem.h"

using namespace Lib;
using namespace CryptoPP;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(uint64) Rtn_Login(IO::NETConnection* nNet, wchar_t* wcUsername,
/// 	wchar_t* wcPassword, int iBranch, char* cErrBuf, char* cVersionBuf, int iErrBufSize,
/// 	int iVersionBufSize)
///
/// \brief	Login routine.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(uint64) Rtn_Login(IO::NETConnection* nNet, wchar_t* wcUsername, wchar_t* wcPassword, int iBranch,
	char* cErrBuf, char* cVersionBuf, int iErrBufSize, int iVersionBufSize)
{
	if (nNet == NULL)
		return false;

	IO::Stream* net = nNet->GetStream();

	int size;

	AutoSeededRandomPool		rng;
	RSAES_OAEP_SHA_Encryptor	pub;

	// Read exponent bytes
	size = net->ReadInt32();
	uchar* buf = (uchar*)malloc(size);
	net->ReadBytes(buf, size);

	Integer exponent(buf, (size_t)size);

	// Read modulus bytes
	size = net->ReadInt32();
	buf = (uchar*)realloc(buf, size);
	net->ReadBytes(buf, size);

	Integer modulus(buf, (size_t)size);

	// Set the RSA encryptor's exponent and modulus
	RSAFunction& r = pub.AccessKey();
	r.SetModulus(modulus);
	r.SetPublicExponent(exponent);

	string encryptedUsername, encryptedPassword;

	// Hash the password
	string password = Mem::GetMD5((char*)wcPassword);

	// Encrypt username and password
	StringSource((char*)wcUsername, true, new PK_EncryptorFilter(rng, pub, new StringSink(encryptedUsername)));
	StringSource((char*)password.c_str(), true, new PK_EncryptorFilter(rng, pub, new StringSink(encryptedPassword)));

	// Write encrypted username/password into the server socket
	net->Write((int)encryptedUsername.length());
	net->Write(encryptedUsername, false);

	net->Write((int)encryptedPassword.length());
	net->Write(encryptedPassword, false);

	// Write branch
	net->Write(iBranch);

	bool result = net->ReadBool();

	// Exponent/modulus size
	free(buf);

	// If we failed to login, they'll be an error
	if (!result)
	{
		// Read the error from the server
		string error = net->ReadString();

		// Copy no more than the iErrBufSize characters into cErrBuf
		int size = ((int)error.length() + 1) <= iErrBufSize ? 
			((int)error.length() + 1) : iErrBufSize;

		memcpy(cErrBuf, error.c_str(), size);
	}
	else
	{	
		// Version only available in non-public builds
		if (iBranch)
		{
			// Read version from server
			string version = net->ReadString();

			// Copy no more than the iVersionBufSize characters into cVersionBuf
			int size = ((int)version.length() + 1) <= iVersionBufSize ?
				((int)version.length() + 1) : iVersionBufSize;

			memcpy(cVersionBuf, version.c_str(), size);
		}

		// Return subscription length
		return net->ReadUInt64();
	}

	return false;
}