/* 7zCrc.h -- CRC32 calculation
2013-01-18 : Igor Pavlov : Public domain */

#ifndef __7Z_CRC_H
#define __7Z_CRC_H

#include "Precomp.h"
#include "ZCpuArch.h"


#define CRC_UPDATE_BYTE_2(crc, b) (table[((crc) ^ (b)) & 0xFF] ^ ((crc) >> 8))
#define kCrcPoly 0xEDB88320

#ifdef MY_CPU_LE
#define CRC_NUM_TABLES 8
#else
#define CRC_NUM_TABLES 9

#define CRC_UINT32_SWAP(v) ((v >> 24) | ((v >> 8) & 0xFF00) | ((v << 8) & 0xFF0000) | (v << 24))

UNInt32 MY_FAST_CALL CrcUpdateT1_BeT4(UNInt32 v, const void *data, size_t size, const UNInt32 *table);
UNInt32 MY_FAST_CALL CrcUpdateT1_BeT8(UNInt32 v, const void *data, size_t size, const UNInt32 *table);
#endif

#ifndef MY_CPU_BE
UNInt32 MY_FAST_CALL CrcUpdateT4(UNInt32 v, const void *data, size_t size, const UNInt32 *table);
UNInt32 MY_FAST_CALL CrcUpdateT8(UNInt32 v, const void *data, size_t size, const UNInt32 *table);
#endif


EXTERN_C_BEGIN

extern UNInt32 g_CrcTable[];

/* Call CrcGenerateTable one time before other CRC functions */
void MY_FAST_CALL CrcGenerateTable(void);

#define CRC_INIT_VAL 0xFFFFFFFF
#define CRC_GET_DIGEST(crc) ((crc) ^ CRC_INIT_VAL)
#define CRC_UPDATE_BYTE(crc, b) (g_CrcTable[((crc) ^ (b)) & 0xFF] ^ ((crc) >> 8))

UNInt32 MY_FAST_CALL CrcUpdate(UNInt32 crc, const void *data, size_t size);
UNInt32 MY_FAST_CALL CrcCalc(const void *data, size_t size);

EXTERN_C_END

#endif
