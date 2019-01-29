#pragma once

#include <map>

// t_key must be a numeric type
// ### you can't create a range taking up the entire range of t_key
// ### (could be done by switching from start/length -> start/end)
template <typename t_key, typename t_data> class IRangeMap : public IRangeMap<t_key, t_data>
{
public:
	struct Entry
	{
		bool	Contains(t_key addr, t_key base)
		{
			return (addr >= base) && (addr <= (base + length - 1));
		}

		t_key	length;
		t_data	data;
	};

	typedef std::map <t_key, Entry>			EntryMapType;
	typedef typename EntryMapType::iterator	Iterator;

	IRangeMap<t_key, t_data>()
	{
		//
	}

	virtual ~IRangeMap<t_key, t_data>()
	{
		//
	}

	void	Clear(void)
	{
		m_entries.clear();
	}

	t_data * Add(t_key start, t_key length);
	
	t_data * Lookup(t_key addr, t_key * base = NULL, t_key * length = NULL);

	bool	Erase(t_key addr, t_key * base = NULL, t_key * length = NULL);
	
	t_key GetDataRangeLength(t_data * data);

	typename IRangeMap<t_key, t_data>::EntryMapType::iterator	LookupIter(t_key addr);

	typename IRangeMap<t_key, t_data>::EntryMapType::iterator	Begin(void);

	typename IRangeMap<t_key, t_data>::EntryMapType::iterator	End(void);
	
private:
	IRangeMap<t_key,t_data>:: EntryMapType m_entries = NULL;
};

/*
template <typename t_key, typename t_data>
IRangeMap<typename t_key, typename t_data>* new_IRangeMap()
{
	return new IRangeMap<t_key, t_data>();
}
*/

