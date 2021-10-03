//
// Bachelor of Software Engineering (Artificial Intelligence)
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name : Server.cpp
// Description : Winsock 2 Socket Programming - Server Side - (Chat Application)
// Author : Rawinder Singh
// Mail : rawinder.singh@mds.ac.nz
//

// Library Include
#include "Server.h"

// Function to Initialize Winsock
bool Server::InitializeWinsock()
{
    int iResult = 0;
    WSADATA wsaData;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup() failed with error: " << iResult << std::endl;
        return 1;
    }

    return true;
}

// Function to Bind
addrinfo Server::Bind()
{
    struct addrinfo tIP4addr;

    // Bind the ip address and port to a socket
    ZeroMemory(&tIP4addr, sizeof(tIP4addr));
    tIP4addr.ai_family = AF_INET;
    tIP4addr.ai_socktype = SOCK_STREAM;
    tIP4addr.ai_protocol = IPPROTO_TCP;
    tIP4addr.ai_flags = AI_PASSIVE;

    return tIP4addr;
}




