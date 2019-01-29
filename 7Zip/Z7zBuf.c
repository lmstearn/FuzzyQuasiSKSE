/* 7zBuf.c -- Byte Buffer
2017-04-03 : Igor Pavlov : Public domain */

#include "Precomp.h"

#include "Z7zBuf.h"

void Buf_Init(CBuf *p)
{
  p->data = 0;
  p->size = 0;
}

int Buf_Create(CBuf *p, size_t size, ISzAllocPtr alloc)
{
  p->size = 0;
  if (size == 0)
  {
    p->data = 0;
    return 1;
  }
  p->data = (Byte *)ISzAlloc_Alloc(alloc, size);
  if (p->data)
  {
    p->size = size;
    return 1;
  }
  return 0;
}

void Buf_Free(CBuf *p, ISzAllocPtr alloc)
{
  ISzAlloc_Free(alloc, p->data);
  p->data = 0;
  p->size = 0;
}

void DynBuf_Construct(CDynBuf * p)
{
}

void DynBuf_SeekToBeg(CDynBuf * p)
{
}

int DynBuf_Write(CDynBuf * p, const Byte * buf, size_t size, ISzAllocPtr alloc)
{
	return 0;
}
void DynBuf_Free(CDynBuf *p, ISzAllocPtr alloc)
{
	ISzAlloc_Free(alloc, p->data);
	p->data = 0;
	p->size = 0;
	p->pos = 0;
}

