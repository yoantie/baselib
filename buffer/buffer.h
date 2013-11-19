template<typename T, int t_nFixedBytes = 128>
class CTempBuffer
{
public:
	CTempBuffer() : m_p(NULL)
	{
	}

	CTempBuffer(size_t nElements) : m_p(NULL)
	{
		Allocate(nElements);
	}

	~CTempBuffer()
	{
		if(m_p != reinterpret_cast<T*>(m_abFixedBuffer))
			free(m_p);
	}

	operator T*() const
	{
		return m_p;
	}

	T* operator ->() const
	{
		ATLASSERT(m_p != NULL);
		return m_p;
	}

	T* Allocate(size_t nElements)
	{
		ATLASSERT(nElements <= (SIZE_MAX / sizeof(T)));
		return AllocateBytes(nElements * sizeof(T));
	}

	T* AllocateBytes(size_t nBytes)
	{
		ATLASSERT(m_p == NULL);
		if(nBytes > t_nFixedBytes)
			m_p = static_cast<T*>(malloc(nBytes));
		else
			m_p = reinterpret_cast<T*>(m_abFixedBuffer);

		return m_p;
	}

private:
	T* m_p;
	BYTE m_abFixedBuffer[t_nFixedBytes];
};
