#pragma once
#include <Windows.h>

class ICriticalSection
{
	public:
		ICriticalSection();
		~ICriticalSection();

		void	Enter(void);
		void	Leave(void);
		bool	TryEnter(void);

	private:
		CRITICAL_SECTION	 critSection;
};

class IScopedCriticalSection
{
public:
	IScopedCriticalSection(ICriticalSection * cs)
		:m_cs(cs)
	{
		m_cs->Enter();
	}

	~IScopedCriticalSection()
	{
		m_cs->Leave();
	}

private:
	IScopedCriticalSection(); // undefined

	ICriticalSection * m_cs;
};
