#pragma once

#ifndef IERRORSH_GUARD
#define IERRORSH_GUARD

#include "IDebugLog.h"

__declspec(noreturn) static void IErrors_Halt(void);
void _AssertionFailed(const char * file, unsigned long line, const char * desc);
void _AssertionFailed_ErrCode(const char * file, unsigned long line, const char * desc, unsigned long long code);
void _AssertionFailed_ErrCode(const char * file, unsigned long line, const char * desc, const char * code);

//! Exit the program if the condition is not true
#define ASSERT(a)					do { if(!(a)) _AssertionFailed(__FILE__, __LINE__, #a); } while(0)
//! Exit the program if the condition is not true, with an error message
#define ASSERT_STR(a, b)			do { if(!(a)) _AssertionFailed(__FILE__, __LINE__, b); } while(0)
//! Exit the program if the condition is not true, reporting an error code
#define ASSERT_CODE(a, b)			do { if(!(a)) _AssertionFailed_ErrCode(__FILE__, __LINE__, #a, b); } while(0)
//! Exit the program if the condition is not true, reporting an error code and message
#define ASSERT_STR_CODE(a, b, c)	do { if(!(a)) _AssertionFailed_ErrCode(__FILE__, __LINE__, b, c); } while(0)
//! Exit the program with an error message
#define HALT(a)						do { _AssertionFailed(__FILE__, __LINE__, a); } while(0)
//! Exit the program with and error code and message
#define HALT_CODE(a, b)				do { _AssertionFailed_ErrCode(__FILE__, __LINE__, a, b); } while(0)

// based on the boost implementation of static asserts
template <bool x> struct StaticAssertFailure;
template <> struct StaticAssertFailure <true> { enum { a = 1 }; };
template <int x> struct static_assert_test { };

#define __MACRO_JOIN__(a, b)		__MACRO_JOIN_2__(a, b)
#define __MACRO_JOIN_2__(a, b)		__MACRO_JOIN_3__(a, b)
#define __MACRO_JOIN_3__(a, b)		a##b			//## Token-pasting: concatenate operator
#define __PREPRO_TOKEN_STR2__(a)	#a			//# Stringizing: double quotes operator
#define __PREPRO_TOKEN_STR__(a)		__PREPRO_TOKEN_STR2__(a)
#define __LOC__						__FILE__ "("__PREPRO_TOKEN_STR__(__LINE__)") : "
#ifdef __cplusplus
// __COUNTER__ increments by 1 each use in a source file
// ASSERT_COMPILE is a viable alt name to STATIC_ASSERT:  xkeycheck.h: #define static_assert EMIT WARNING C4005
//Consider swapping __COUNTER__ for __LINE__?
#define STATIC_ASSERT(a) typedef static_assert_test<sizeof(StaticAssertFailure<(bool)(a)>)>__MACRO_JOIN__(static_assert_typedef_,__COUNTER__ )
#else
#define STATIC_ASSERT(a) extern char __dummy[-1 + (2 * (int)(x))]
#endif

//Removed from ITypes.h
STATIC_ASSERT(sizeof(::Bitfield8)==1);
STATIC_ASSERT(sizeof(::Bitfield16)==2);
STATIC_ASSERT(sizeof(::Bitfield32)==4);


//consider
//#define VERIFY_EXPLICIT_CAST(from,to) ASSERT_COMPILE(sizeof(from) == sizeof(to))

#endif // IERRORSH_GUARD