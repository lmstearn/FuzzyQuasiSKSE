#pragma once

struct IInterlockedLong
{
	public:
		long	Increment(void);
		long	Decrement(void);
		long	Get(void);
		long	Set(long in);
		long	TrySetIf(long newValue, long expectedOldValue);

		// interlock variable semantics
		bool	Claim(void);
		bool	Release(void);

	private:
		volatile long	value;
};
