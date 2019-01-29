#include "IDatabase.h"

template<class DataType> DataType *	IDatabase<DataType>::Get(UInt64 key)
{
	key &= kGUIDMask;

	if(!key)
		return NULL;

	DataMapType::iterator	iter = theDataMap.find(key);

	return (iter == theDataMap.end()) ? NULL : &((*iter).second);
}

template <class DataType> DataType *	IDatabase<DataType>::Alloc(UInt64 key)
{
	key &= kGUIDMask;

	if(!key)
		return NULL;

	DataMapType::iterator	iter = theDataMap.find(key);

	return (iter == theDataMap.end()) ? &theDataMap[key] : NULL;
}

template <class DataType> DataType *	IDatabase<DataType>::Alloc(UInt64 *key)
{
	UInt64	newKey = newKeyHint;

	do
	{
		if(!newKey)
			newKey++;

		DataMapType::iterator	iter = theDataMap.find(newKey);

		// is 'newKey' unused?
		if(iter == theDataMap.end())
		{
			*key = newKey;
			newKeyHint = (newKey + 1) & kGUIDMask;
			return &theDataMap[newKey];
		}
		else
		{
			++iter;
			if(iter == theDataMap.end())
			{
				newKey = 1;
			}
			else
			{
				UInt64	nextKey = (newKey + 1) & kGUIDMask;
				if(iter->first != nextKey)
				{
					*key = nextKey;
					newKeyHint = (nextKey + 1) & kGUIDMask;
					return &theDataMap[nextKey];
				}
			}
		}
	}
	while(1);

	*key = 0;

	return NULL;
}

template<class DataType> void	IDatabase<DataType>::Delete(UInt64 key)
{
	if(key)
	{
		key &= kGUIDMask;

		theDataMap.erase(key);

		newKeyHint = key;
	}
}

template<class DataType> void IDatabase <DataType>::Save(IDataStream * stream)
{
	stream->Write32(theDataMap.size());
	stream->Write64(newKeyHint);

	for(DataMapType::iterator iter = theDataMap.begin(); iter != theDataMap.end(); iter++)
	{
		stream->Write64((*iter).first);
		stream->WriteBuf(&((*iter).second), sizeof(DataType));
	}
}

template<class DataType> void IDatabase <DataType>::Load(IDataStream * stream)
{
	UInt32	numEntries = stream->Read32();
	newKeyHint = stream->Read64();

	theDataMap.clear();

	for(UInt32 i = 0; i < numEntries; i++)
	{
		UInt64	key = stream->Read64();
		stream->ReadBuf(&(theDataMap[key]), sizeof(DataType));
	}
}

template<class DataType> bool IDatabase<DataType>::SaveToFile(char * name)
{
	IFileStream	stream;

	if(stream.Create(name))
	{
		Save(&stream);
		return true;
	}

	return false;
}

template <class DataType> bool IDatabase<DataType>::LoadFromFile(char * name)
{
	IFileStream	stream;

	if(stream.Open(name))
	{
		Load(&stream);
		return true;
	}

	return false;
}

