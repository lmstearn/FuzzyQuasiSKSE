#pragma once
#include "IDebugLog.h"
#include "ICriticalSection.h"

template <typename T, ConstUInt32 tSize> class IMemPool
{
public:
	IMemPool() : m_free(NULL), m_alloc(NULL)
	{
		Reset();
	}

	~IMemPool()	{ Clear(); }

	void	Reset(void);
	T *	Allocate(void);
	void	Free(T * obj);
	
	UInt32	GetSize(void)	{ return tSize; }

	T *		Begin(void);
	T *		Next(T * obj);
	void	Dump(void);

	bool	Full(void)
	{
		return m_free == NULL;
	}

	bool	Empty(void)
	{
		return m_alloc == NULL;
	}
	void	Clear(void);

private:
	struct PoolItem
	{
		UInt8		obj[sizeof(T)];
		PoolItem	* next;

		T *			GetObj(void)	{ return reinterpret_cast <T *>(obj); }
	};

	PoolItem	m_items[tSize];
	PoolItem	* m_free;
	PoolItem	* m_alloc;
};

template<typename T, ConstUInt32 tSize> class IBasicMemPool
{
public:
	IBasicMemPool()
	:m_free(NULL)
	{
		Reset();
	}

	~IBasicMemPool()	{ }

	void	Reset(void);
	T *		Allocate(void);
	void	Free(T * obj);
	UInt32	GetSize(void)	{ return tSize; }

	bool	Full(void)
	{
		return m_free == NULL;
	}

	UInt32	GetIdx(T * obj);

	T *		GetByID(UInt32 id)
	{
		return m_items[id].GetObj();
	}

private:
	union PoolItem
	{
		UInt8		obj[sizeof(T)];
		PoolItem	* next;

		T *			GetObj(void)	{ return reinterpret_cast <T *>(obj); }
	};

	PoolItem	m_items[tSize];
	PoolItem	* m_free;
};

template<typename T, ConstUInt32 tSize> class IThreadSafeBasicMemPool
{
public:
	IThreadSafeBasicMemPool()
	:m_free(NULL)
	{
		Reset();
	}

	~IThreadSafeBasicMemPool()	{ }

	void	Reset(void);
	T *		Allocate(void);
	void	Free(T * obj);
	UInt32	GetSize(void)	{ return tSize; }

	bool	Full(void)
	{
		return m_free == NULL;
	}

private:
	union PoolItem
	{
		UInt8		obj[sizeof(T)];
		PoolItem	* next;

		T *			GetObj(void)	{ return reinterpret_cast <T *>(obj); }
	};

	PoolItem	m_items[tSize];
	PoolItem	* m_free;

	ICriticalSection	m_mutex;
};

void Test_IMemPool(void);
