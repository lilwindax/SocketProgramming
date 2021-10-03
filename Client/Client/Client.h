//
// Bachelor of Software Engineering (Artificial Intelligence)
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name : Client.h
// Description : Winsock 2 Socket Programming - Client Side - (Chat Application)
// Author : Rawinder Singh
// Mail : rawinder.singh@mds.ac.nz
//

// Library Includes
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <cstdlib>

// Pragmas
#pragma comment (lib, "Ws2_32.lib")

// Client class 
class Client
{
	
protected:
	// Struct definition for the Client
	struct client_type
	{
		int id;
		SOCKET socket;
	};

public:
	// Class Methods
	bool InitializeWinsock();
	struct addrinfo Bind();


};
