#pragma once

#include <map>
#include "IDataStream.h"
#include "IFilestream.h"

template <class DataType> class IDatabase
{
	public:
		typedef std::map <UInt64, DataType>		DataMapType;
		typedef typename DataMapType::iterator	DataMapIterator;
		
		static const UInt64	kGUIDMask = 0x0FFFFFFFFFFFFFFF;

		IDatabase()	{ newKeyHint = 1; }
		virtual ~IDatabase()	{ }

		DataType *	Get(UInt64 key);

		DataType *	Alloc(UInt64 key);

		DataType *	Alloc(UInt64 * key);

		void		Delete(UInt64 key);

		void		Save(IDataStream * stream);
		void		Load(IDataStream * stream);

		bool		SaveToFile(char * name);
		bool		LoadFromFile(char * name);

		DataMapType &GetDMData(void) 	{ return theDataMap; }
		DataMapIterator	Begin(void)		{ return theDataMap.begin(); }
		DataMapIterator	End(void)		{ return theDataMap.end(); }
		UInt32			Length(void)	{ return theDataMap.size(); }

	private:
		DataMapType	theDataMap;
		UInt64		newKeyHint;
};