#include "IMemPool.h"

template<typename T, ConstUInt32 tSize> void IMemPool<T, tSize>::Reset(void)
{
	for(UInt32 i = 0; i < tSize - 1; i++)
	{
		m_items[i].next = &m_items[i + 1];
	}

	m_items[tSize - 1].next = NULL;
	m_free = m_items;
	m_alloc = NULL;
}

template<typename T, UInt32 tSize> T* IMemPool<T, tSize>::Allocate(void)
{
	if(m_free)
	{
		PoolItem	* item = m_free;
		m_free = m_free->next;

		item->next = m_alloc;
		m_alloc = item;

		T	* obj = item->GetObj();

		new (obj) T;
		return obj;
	}

	return NULL;
}

template<typename T, UInt32 tSize> void IMemPool<T, tSize>::Free(T * obj)
{
	PoolItem	* item = reinterpret_cast <PoolItem *>(obj);

	if(item == m_alloc)
	{
		m_alloc = item->next;
	}
	else
	{
		PoolItem	* traverse = m_alloc;
		while(traverse->next != item)
			traverse = traverse->next;
		traverse->next = traverse->next->next;
	}

	item->next = m_free;
	m_free = item;

	obj->~T();
}

template<typename T, UInt32 tSize> T *IMemPool<T, tSize>::Begin(void)
{
	T	* result = NULL;

	if(m_alloc)
		result = m_alloc->GetObj();

	return result;
}
template<typename T, UInt32 tSize> T *IMemPool<T, tSize>::Next(T * obj)
{
	PoolItem	* item = reinterpret_cast <PoolItem *>(obj);
	PoolItem	* next = item->next;
	T			* result = NULL;

	if(next)
		result = next->GetObj();

	return result;
}

template<typename T, UInt32 tSize> void IMemPool<T, tSize>::Dump(void)
{
	gLog.Indent();

	_DMESSAGE("free:");
	gLog.Indent();
	for(PoolItem * traverse = m_free; traverse; traverse = traverse->next)
		_DMESSAGE("%08X", traverse);
	gLog.Outdent();

	_DMESSAGE("alloc:");
	gLog.Indent();
	for(PoolItem * traverse = m_alloc; traverse; traverse = traverse->next)
		_DMESSAGE("%08X", traverse);
	gLog.Outdent();

	gLog.Outdent();
}

template<typename T, UInt32 tSize> void IMemPool<T, tSize>::Clear(void)
{
	while(m_alloc)
		Free(m_alloc->GetObj());
}
//IBasicMemPool

template<typename T, UInt32 tSize> void IBasicMemPool<T, tSize>::Reset(void)
{
	for(UInt32 i = 0; i < tSize - 1; i++)
	{
		m_items[i].next = &m_items[i + 1];
	}

	m_items[tSize - 1].next = NULL;
	m_free = m_items;
}

template<typename T, UInt32 tSize> T *IBasicMemPool<T, tSize>::Allocate(void)
{
	if(m_free)
	{
		PoolItem	* item = m_free;
		m_free = m_free->next;

		T	* obj = item->GetObj();

		new (obj) T;
		return obj;
	}
	return NULL;
}

template<typename T, UInt32 tSize> void IBasicMemPool<T, tSize>::Free(T * obj)
{
	obj->~T();

	PoolItem	* item = reinterpret_cast <PoolItem *>(obj);

	item->next = m_free;
	m_free = item;
}

template<typename T, UInt32 tSize> UInt32 IBasicMemPool<T, tSize>::GetIdx(T * obj)
{
	PoolItem	* item = reinterpret_cast <PoolItem *>(obj);
	return item - m_items;
}

//IThreadSafeBasicMemPool
template<typename T, UInt32 tSize> void IThreadSafeBasicMemPool<T, tSize>::Reset(void)
{
	m_mutex.Enter();

	for(UInt32 i = 0; i < tSize - 1; i++)
	{
		m_items[i].next = &m_items[i + 1];
	}

	m_items[tSize - 1].next = NULL;
	m_free = m_items;

	m_mutex.Leave();
}
template<typename T, UInt32 tSize> T * IThreadSafeBasicMemPool<T, tSize>::Allocate(void)
{
	T	* result = NULL;

	m_mutex.Enter();

	if(m_free)
	{
		PoolItem	* item = m_free;
		m_free = m_free->next;

		m_mutex.Leave();

		result = item->GetObj();

		new (result) T;
	}
	else
	{
		m_mutex.Leave();
	}

	return result;
}

template<typename T, UInt32 tSize> void IThreadSafeBasicMemPool<T, tSize>::Free(T * obj)
{
	obj->~T();

	PoolItem	* item = reinterpret_cast <PoolItem *>(obj);

	m_mutex.Enter();

	item->next = m_free;
	m_free = item;

	m_mutex.Leave();
}


//Testing
void Test_IMemPool(void)
{
	IMemPool <UInt32, 3>	pool;

	_DMESSAGE("main: pool test");
	gLog.Indent();

	_DMESSAGE("start");
	pool.Dump();

	UInt32	* data0, * data1, * data2;

	data0 = pool.Allocate();
	_DMESSAGE("alloc0 = %08X", data0);
	pool.Dump();

	data1 = pool.Allocate();
	_DMESSAGE("alloc1 = %08X", data1);
	pool.Dump();

	data2 = pool.Allocate();
	_DMESSAGE("alloc2 = %08X", data2);
	pool.Dump();

	_DMESSAGE("free0 %08X", data0);
	pool.Free(data0);
	pool.Dump();

	data0 = pool.Allocate();
	_DMESSAGE("alloc0 = %08X", data0);
	pool.Dump();

	_DMESSAGE("free2 %08X", data2);
	pool.Free(data2);
	pool.Dump();

	_DMESSAGE("done");
	pool.Dump();

	gLog.Outdent();
}
