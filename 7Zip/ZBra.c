/* Bra.c -- Converters for RISC code
2017-04-04 : Igor Pavlov : Public domain */

#include "Precomp.h"

#include "ZBra.h"
EXTERN_C_BEGIN
extern "C" SizeT ARM_Convert(Byte *data, SizeT size, UInt32 ip, int encoding)
{
  Byte *p;
  const Byte *lim;
  size &= ~(size_t)3;
  ip += 4;
  p = data;
  lim = data + size;

  if (encoding)

  for (;;)
  {
    for (;;)
    {
      if (p >= lim)
        return p - data;
      p += 4;
      if (p[-1] == 0xEB)
        break;
    }
    {
      UInt32 v = GetUi32(p - 4);
      v <<= 2;
        v += ip + (UInt32)(p - data);
      v >>= 2;
      v &= 0x00FFFFFF;
      v |= 0xEB000000;
      SetUi32(p - 4, v);
    }
  }

  for (;;)
  {
    for (;;)
    {
      if (p >= lim)
        return p - data;
      p += 4;
      if (p[-1] == 0xEB)
        break;
    }
    {
      UInt32 v = GetUi32(p - 4);
      v <<= 2;
        v -= ip + (UInt32)(p - data);
      v >>= 2;
      v &= 0x00FFFFFF;
      v |= 0xEB000000;
      SetUi32(p - 4, v);
    }
  }
}


extern "C" SizeT ARMT_Convert(Byte *data, SizeT size, UInt32 ip, int encoding)
{
  Byte *p;
  const Byte *lim;
  size &= ~(size_t)1;
  p = data;
  lim = data + size - 4;

  if (encoding)
  
  for (;;)
  {
    UInt32 b1;
    for (;;)
    {
      UInt32 b3;
      if (p > lim)
        return p - data;
      b1 = p[1];
      b3 = p[3];
      p += 2;
      b1 ^= 8;
      if ((b3 & b1) >= 0xF8)
        break;
    }
    {
      UInt32 v =
             ((UInt32)b1 << 19)
          + (((UInt32)p[1] & 0x7) << 8)
          + (((UInt32)p[-2] << 11))
          + (p[0]);

      p += 2;
      {
        UInt32 cur = (ip + (UInt32)(p - data)) >> 1;
          v += cur;
      }

      p[-4] = (Byte)(v >> 11);
      p[-3] = (Byte)(0xF0 | ((v >> 19) & 0x7));
      p[-2] = (Byte)v;
      p[-1] = (Byte)(0xF8 | (v >> 8));
    }
  }
  
  for (;;)
  {
    UInt32 b1;
    for (;;)
    {
      UInt32 b3;
      if (p > lim)
        return p - data;
      b1 = p[1];
      b3 = p[3];
      p += 2;
      b1 ^= 8;
      if ((b3 & b1) >= 0xF8)
        break;
    }
    {
      UInt32 v =
             ((UInt32)b1 << 19)
          + (((UInt32)p[1] & 0x7) << 8)
          + (((UInt32)p[-2] << 11))
          + (p[0]);

      p += 2;
      {
        UInt32 cur = (ip + (UInt32)(p - data)) >> 1;
          v -= cur;
      }

      /*
      SetUi16(p - 4, (UInt16)(((v >> 11) & 0x7FF) | 0xF000));
      SetUi16(p - 2, (UInt16)(v | 0xF800));
      */
      
      p[-4] = (Byte)(v >> 11);
      p[-3] = (Byte)(0xF0 | ((v >> 19) & 0x7));
      p[-2] = (Byte)v;
      p[-1] = (Byte)(0xF8 | (v >> 8));
    }
  }
}


extern "C" SizeT PPC_Convert(Byte *data, SizeT size, UInt32 ip, int encoding)
{
  Byte *p;
  const Byte *lim;
  size &= ~(size_t)3;
  ip -= 4;
  p = data;
  lim = data + size;

  for (;;)
  {
    for (;;)
    {
      if (p >= lim)
        return p - data;
      p += 4;
      /* if ((v & 0xFC000003) == 0x48000001) */
      if ((p[-4] & 0xFC) == 0x48 && (p[-1] & 3) == 1)
        break;
    }
    {
      UInt32 v = GetBe32(p - 4);
      if (encoding)
        v += ip + (UInt32)(p - data);
      else
        v -= ip + (UInt32)(p - data);
      v &= 0x03FFFFFF;
      v |= 0x48000000;
      SetBe32(p - 4, v);
    }
  }
}


extern "C" SizeT SPARC_Convert(Byte *data, SizeT size, UInt32 ip, int encoding)
{
  Byte *p;
  const Byte *lim;
  size &= ~(size_t)3;
  ip -= 4;
  p = data;
  lim = data + size;

  for (;;)
  {
    for (;;)
    {
      if (p >= lim)
        return p - data;
      /*
      v = GetBe32(p);
      p += 4;
      m = v + ((UInt32)5 << 29);
      m ^= (UInt32)7 << 29;
      m += (UInt32)1 << 22;
      if ((m & ((UInt32)0x1FF << 23)) == 0)
        break;
      */
      p += 4;
      if ((p[-4] == 0x40 && (p[-3] & 0xC0) == 0) ||
          (p[-4] == 0x7F && (p[-3] >= 0xC0)))
        break;
    }
    {
      UInt32 v = GetBe32(p - 4);
      v <<= 2;
      if (encoding)
        v += ip + (UInt32)(p - data);
      else
        v -= ip + (UInt32)(p - data);
      
      v &= 0x01FFFFFF;
      v -= (UInt32)1 << 24;
      v ^= 0xFF000000;
      v >>= 2;
      v |= 0x40000000;
      SetBe32(p - 4, v);
    }
  }
}
#define Test86MSByte(b) ((((b) + 1) & 0xFE) == 0)

extern "C" SizeT x86_Convert(Byte *data, SizeT size, UInt32 ip, UInt32 *state, int encoding)
{
	SizeT pos = 0;
	UInt32 mask = *state & 7;
	if (size < 5)
		return 0;
	size -= 4;
	ip += 5;

	for (;;)
	{
		Byte *p = data + pos;
		const Byte *limit = data + size;
		for (; p < limit; p++)
			if ((*p & 0xFE) == 0xE8)
				break;

		{
			SizeT d = (SizeT)(p - data - pos);
			pos = (SizeT)(p - data);
			if (p >= limit)
			{
				*state = (d > 2 ? 0 : mask >> (unsigned)d);
				return pos;
			}
			if (d > 2)
				mask = 0;
			else
			{
				mask >>= (unsigned)d;
				if (mask != 0 && (mask > 4 || mask == 3 || Test86MSByte(p[(size_t)(mask >> 1) + 1])))
				{
					mask = (mask >> 1) | 4;
					pos++;
					continue;
				}
			}
		}

		if (Test86MSByte(p[4]))
		{
			UInt32 v = ((UInt32)p[4] << 24) | ((UInt32)p[3] << 16) | ((UInt32)p[2] << 8) | ((UInt32)p[1]);
			UInt32 cur = ip + (UInt32)pos;
			pos += 5;
			if (encoding)
				v += cur;
			else
				v -= cur;
			if (mask != 0)
			{
				unsigned sh = (mask & 6) << 2;
				if (Test86MSByte((Byte)(v >> sh)))
				{
					v ^= (((UInt32)0x100 << sh) - 1);
					if (encoding)
						v += cur;
					else
						v -= cur;
				}
				mask = 0;
			}
			p[1] = (Byte)v;
			p[2] = (Byte)(v >> 8);
			p[3] = (Byte)(v >> 16);
			p[4] = (Byte)(0 - ((v >> 24) & 1));
		}
		else
		{
			mask = (mask >> 1) | 4;
			pos++;
		}
	}
}

extern "C" SizeT IA64_Convert(Byte *data, SizeT size, UInt32 ip, int encoding)
{
	SizeT i;
	if (size < 16)
		return 0;
	size -= 16;
	i = 0;
	do
	{
		unsigned m = ((UInt32)0x334B0000 >> (data[i] & 0x1E)) & 3;
		if (m)
		{
			m++;
			do
			{
				Byte *p = data + (i + (size_t)m * 5 - 8);
				if (((p[3] >> m) & 15) == 5
					&& (((p[-1] | ((UInt32)p[0] << 8)) >> m) & 0x70) == 0)
				{
					unsigned raw = GetUi32(p);
					unsigned v = raw >> m;
					v = (v & 0xFFFFF) | ((v & (1 << 23)) >> 3);

					v <<= 4;
					if (encoding)
						v += ip + (UInt32)i;
					else
						v -= ip + (UInt32)i;
					v >>= 4;

					v &= 0x1FFFFF;
					v += 0x700000;
					v &= 0x8FFFFF;
					raw &= ~((UInt32)0x8FFFFF << m);
					raw |= (v << m);
					SetUi32(p, raw);
				}
			} while (++m <= 4);
		}
		i += 16;
	} while (i <= size);
	return i;
}
EXTERN_C_END


