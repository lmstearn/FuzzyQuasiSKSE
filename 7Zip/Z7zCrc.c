/* 7zCrc.c -- CRC32 init
2017-06-06 : Igor Pavlov : Public domain */

#include "Z7zCrc.h"



typedef UNInt32 (MY_FAST_CALL *CRC_FUNC)(UNInt32 v, const void *data, size_t size, const UNInt32 *table);

CRC_FUNC g_CrcUpdateT4;
CRC_FUNC g_CrcUpdateT8;
CRC_FUNC g_CrcUpdate;

UNInt32 g_CrcTable[256 * CRC_NUM_TABLES];

UNInt32 MY_FAST_CALL CrcUpdate(UNInt32 v, const void *data, size_t size)
{
  return g_CrcUpdate(v, data, size, g_CrcTable);
}

UNInt32 MY_FAST_CALL CrcCalc(const void *data, size_t size)
{
  return g_CrcUpdate(CRC_INIT_VAL, data, size, g_CrcTable) ^ CRC_INIT_VAL;
}

UNInt32 MY_FAST_CALL CrcUpdateT1(UNInt32 v, const void *data, size_t size, const UNInt32 *table)
{
  const Byte *p = (const Byte *)data;
  const Byte *pEnd = p + size;
  for (; p != pEnd; p++)
    v = CRC_UPDATE_BYTE_2(v, *p);
  return v;
}

void MY_FAST_CALL CrcGenerateTable(void)
{
  UNInt32 i;
  for (i = 0; i < 256; i++)
  {
    UNInt32 r = i;
    unsigned j;
    for (j = 0; j < 8; j++)
      r = (r >> 1) ^ (kCrcPoly & ((UNInt32)0 - (r & 1)));
    g_CrcTable[i] = r;
  }
  for (i = 256; i < 256 * CRC_NUM_TABLES; i++)
  {
    UNInt32 r = g_CrcTable[(size_t)i - 256];
    g_CrcTable[i] = g_CrcTable[r & 0xFF] ^ (r >> 8);
  }

  #if CRC_NUM_TABLES < 4
  
  g_CrcUpdate = CrcUpdateT1;
  
  #else
 
  #ifdef MY_CPU_LE

    g_CrcUpdateT4 = CrcUpdateT4;
    g_CrcUpdate = CrcUpdateT4;

    #if CRC_NUM_TABLES >= 8
      g_CrcUpdateT8 = CrcUpdateT8;
  
      #ifdef MY_CPU_X86_OR_AMD64
      if (!CPU_Is_InOrder())
      #endif
        g_CrcUpdate = CrcUpdateT8;
    #endif

  #else
  {
    #ifndef MY_CPU_BE
    UNInt32 k = 0x01020304;
    const Byte *p = (const Byte *)&k;
    if (p[0] == 4 && p[1] == 3)
    {
      g_CrcUpdateT4 = CrcUpdateT4;
      g_CrcUpdate = CrcUpdateT4;
      #if CRC_NUM_TABLES >= 8
      g_CrcUpdateT8 = CrcUpdateT8;
      g_CrcUpdate = CrcUpdateT8;
      #endif
    }
    else if (p[0] != 1 || p[1] != 2)
      g_CrcUpdate = CrcUpdateT1;
    else
    #endif
    {
      for (i = 256 * CRC_NUM_TABLES - 1; i >= 256; i--)
      {
        UNInt32 x = g_CrcTable[(size_t)i - 256];
        g_CrcTable[i] = CRC_UINT32_SWAP(x);
      }
      g_CrcUpdateT4 = CrcUpdateT1_BeT4;
      g_CrcUpdate = CrcUpdateT1_BeT4;
      #if CRC_NUM_TABLES >= 8
      g_CrcUpdateT8 = CrcUpdateT1_BeT8;
      g_CrcUpdate = CrcUpdateT1_BeT8;
      #endif
    }
  }
  #endif

  #endif
}
