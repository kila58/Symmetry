/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

template <typename T>
class DynamicArray
{
private:

	void*	m_pAllocationBase;

	int		m_iElements;
	int		m_iAllocationSize;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	DynamicArray::DynamicArray()
	///
	/// \brief	Default constructor.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	DynamicArray()
	{
		this->m_pAllocationBase = malloc(sizeof(T));

		this->m_iElements = 0;
		this->m_iAllocationSize = 0;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	DynamicArray::~DynamicArray()
	///
	/// \brief	Destructor.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	~DynamicArray()
	{
		if (this->m_pAllocationBase)
			free(this->m_pAllocationBase);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	void DynamicArray::Push(T object)
	///
	/// \brief	Pushes an object into this array.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void Push(T object)
	{
		// Make sure we've enough space allocated for this new object
		this->m_pAllocationBase = realloc(this->m_pAllocationBase, this->m_iAllocationSize + sizeof(T));

		// Move this object to the end of the allocation chunk
		memcpy((void*)((dword)this->m_pAllocationBase + this->m_iAllocationSize), (void*)&object, sizeof(T));

		// Increase variables tracking the size of this dynamic array
		this->m_iAllocationSize += sizeof(T);
		this->m_iElements++;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	void DynamicArray::Push(T* object)
	///
	/// \brief	Pushes an object into this array.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void Push(T* pObject)
	{
		this->Push(*pObject);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	T* DynamicArray::Get(int iIndex)
	///
	/// \brief	Gets an element by index, returning a pointer.
	/// 		
	/// 		WARNING: After calling this->Push, the pointer returned by this may be invalid!  Push
	/// 		may reallocate the memory chunk.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	T* Get(int iIndex)
	{
		if (iIndex < 0 || iIndex > this->m_iElements)
			return NULL;

		return (T*)((dword)this->m_pAllocationBase + (iIndex * sizeof(T)));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	bool DynamicArray::Get(int iIndex, T*)
	///
	/// \brief	Gets an elemeny by index.  Safer than above because this function copies data rather
	/// 		than returning a pointer.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool Get(int iIndex, T* pObject)
	{
		if (iIndex < 0 || iIndex > this->m_iElements)
			return false;

		memcpy(pObject, (void*)((dword)this->m_pAllocationBase + (iIndex * sizeof(T))), sizeof(T));

		return true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	int DynamicArray::GetCount()
	///
	/// \brief	Gets the number of elements in this array.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	int GetCount()
	{
		return this->m_iElements;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	int DynamicArray::GetSize()
	///
	/// \brief	Gets the size of this array in bytes.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	int GetSize()
	{
		return this->m_iAllocationSize;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	void* DynamicArray::GetBase()
	///
	/// \brief	Gets the base of allocation for this array.  This may be reallocated when the size
	/// 		of this array changes.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void* GetBase()
	{
		return this->m_pAllocationBase;
	}
};