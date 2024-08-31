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
NAMESPACE_LIB(IO)

class Stream
{
protected:

	// Underlying stringstream buffer
	stringstream	m_ssStream;

	// Auto-flush
	bool			m_bAutoFlush;

	// "Events"
	virtual void	OnBytesOut(uchar*, uint64);
	virtual void 	OnBytesIn(uchar*, uint64);

	// Used in some internal string functions.  Network communcation
	// is done with a C# server that uses Binary(Reader/Writer), their
	// version of these functions are protected so it's protected here too.
	void	Write7BitEncodedInt(int);
	int		Read7BitEncodedInt();

public:

	Stream();
	Stream(string);
	~Stream();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn template <typename T> T Stream::Read()
	///
	/// \brief Reads T from the stream.
	///
	/// \tparam typename T Type to read.
	///
	/// \return .
	////////////////////////////////////////////////////////////////////////////////////////////////////

	template <typename T>
	T				Read()
	{
		T t; this->OnBytesIn( reinterpret_cast<uchar*>(&t), sizeof( T ) );
		return t;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	template<typename T> void Stream::Write(T tValue)
	///
	/// \brief	Writes tValue into the stream.
	///
	/// \tparam	typename T	Type to write
	/// \param	tValue	The value.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename T>
	void			Write(T tValue)
	{
		this->OnBytesOut( reinterpret_cast<uchar*>( &tValue ), sizeof( tValue ) );
		if (this->m_bAutoFlush) this->Flush();
	}

	// Flushing functions
	virtual void	Flush();
	void			SetAutoFlush(bool);

	// Write functions
	void			Write(int16);
	void			Write(int32);
	void			Write(int64);

	void			Write(uint16);
	void			Write(uint32);
	void			Write(uint64);

	void			Write(double);
	void			Write(float);

	// Length is written as a 7bit encoded integer
	void			Write(string, bool bSendLength = true);
	void			Write(char_c*);

	void			Write(uchar);
	void			Write(char);

	void			Write(uchar*, uint32 iLength = 0);

	void			Write(bool);

	// Read functions
	int16				ReadInt16();
	int32				ReadInt32();
	int64				ReadInt64();

	uint16				ReadUInt16();
	uint32				ReadUInt32();
	uint64				ReadUInt64();

	double				ReadDouble();
	float				ReadFloat();

	string				ReadString(uint32);

	// Conforms with C#'s BinaryReader.ReadString.  The length of the 
	// string is read as a 7bit encoded integer.
	string				ReadString();

	void				ReadChars(char*, uint32);

	uchar				ReadByte();
	char				ReadChar();

	void				ReadBytes(uchar*, uint32);

	string				ReadUntil(char, bool bInclude = true);
	string				ReadLine();

	bool				ReadBool();

	// Other functions
	void			Seek(streamoff);
	streamoff		Tell();

	streamoff		GetStreamSize();
	stringstream	GetSStream();

};

NAMESPACE_END2