#include "IInterlockedLong.h"

long IInterlockedLong::Increment(void)	{ return InterlockedIncrement(&value); }
long	IInterlockedLong::Decrement(void)	{ return InterlockedDecrement(&value); }
long	IInterlockedLong::Get(void)		{ return value; }
long	IInterlockedLong::Set(long in)	{ return InterlockedExchange(&value, in); }
long	IInterlockedLong::TrySetIf(long newValue, long expectedOldValue)
{ return InterlockedCompareExchange(&value, newValue, expectedOldValue); }

// interlock variable semantics
bool	IInterlockedLong::Claim(void)		{ return TrySetIf(1, 0) == 0; }
bool	IInterlockedLong::Release(void)	{ return TrySetIf(0, 1) == 1; }




// all functions are inlined
