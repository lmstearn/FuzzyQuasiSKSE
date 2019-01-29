#pragma once

#include "IPipeServer.h"
#include <iostream>

class IPipeClient
{
public:
	IPipeClient();
	virtual ~IPipeClient();

	bool	Open(const char * name);
	void	Close(void);
	
	bool	ReadMessage(UInt8 * buf, UInt32 length);
	bool	WriteMessage(IPipeServer::MessageHeader * msg);

private:
	HANDLE		m_pipe;
	// #ifdef _HAS_CXX17 ??
	std::string	m_name;
};
