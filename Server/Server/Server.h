//
// Bachelor of Software Engineering (Artificial Intelligence)
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name : Server.h
// Description : Winsock 2 Socket Programming - Server Side - (Chat Application)
// Author : Rawinder Singh
// Mail : rawinder.singh@mds.ac.nz
//

// Library Includes
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>
#include <vector>
#include <cstring>
#include <algorithm> 

// Pragmas
#pragma comment (lib, "Ws2_32.lib")
#pragma warning( disable : 4996)

// Server class
class Server
{
public:

	// Struct definition for the Client
	struct client_type
	{
		int id;
		SOCKET socket;
	};

	// Class Methods
	bool InitializeWinsock();
	struct addrinfo Bind();
	
};