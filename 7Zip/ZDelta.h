/* Delta.h -- Delta converter
2013-01-18 : Igor Pavlov : Public domain */

#ifndef __DELTA_H
#define __DELTA_H
#include "Precomp.h"
#include "7zTypes.h"

#define DELTA_STATE_SIZE 256

EXTERN_C_BEGIN

#define DELTA_STATE_SIZE 256

void Delta_Init(Byte *state);
void Delta_Encode(Byte *state, unsigned delta, Byte *data, SizeT size);
void Delta_Decode(Byte *state, unsigned delta, Byte *data, SizeT size);

EXTERN_C_END

#endif
