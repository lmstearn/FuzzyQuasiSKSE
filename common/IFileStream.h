#pragma once
#ifndef IFILESTREAMH_GUARD
#define IFILESTREAMH_GUARD
#include <direct.h>
#include "IDataStream.h"

/**
 *	An input file stream
 */
class IFileStream : public IDataStream
{
	public:
		IFileStream();
		IFileStream(const char * name);
		~IFileStream();

		bool	Open(const char * name);
		bool	BrowseOpen(void);

		bool	Create(const char * name);
		bool	BrowseCreate(const char * defaultName = NULL, const char * defaultPath = NULL, const char * title = NULL);

		void	Close(void);

		HANDLE	GetHandle(void)	{ return theFile; }

		virtual void	ReadBuf(void * buf, UINT32 inLength);
		virtual void	WriteBuf(const void * buf, UINT32 inLength);
		virtual void	SetOffset(INT64 inOffset);

		// can truncate. implicitly seeks to the end of the file
		void	SetLength(UINT64 length);

		static void	MakeAllDirs(const char * path);
		static char * ExtractFileName(char * path);

	protected:
		HANDLE	theFile;
};

#endif //IFILESTREAMH_GUARD
