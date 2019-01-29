#include "ICriticalSection.h"

ICriticalSection::ICriticalSection()	{ InitializeCriticalSection(&critSection); }
ICriticalSection::~ICriticalSection()	{ DeleteCriticalSection(&critSection); }

void	ICriticalSection::Enter(void)		{ EnterCriticalSection(&critSection); }
void	ICriticalSection::Leave(void)		{ LeaveCriticalSection(&critSection); }
bool	ICriticalSection::TryEnter(void)	{ return TryEnterCriticalSection(&critSection) != 0; }