//
// Bachelor of Software Engineering (Artificial Intelligence)
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name : Main.h
// Description : Winsock 2 Socket Programming - Client Side - (Chat Application)
// Author : Rawinder Singh
// Mail : rawinder.singh@mds.ac.nz
//

// Library Includes
#include "Client.h"

// Global variable definition 
#define DEFAULT_BUFLEN 512            
#define DEFAULT_PORT "9631"
constexpr int Port = 9631;

// Struct definition for the Client
struct client_type
{
    SOCKET socket;
    int id;
    char received_message[DEFAULT_BUFLEN];
};

std::mutex _Mutex;
int counter = 0;

// Function to process each of the connected clients
int client_processing(client_type& new_client)
{
    while (1)
    {
        // Lock the mutex before accessing the common resources
        _Mutex.lock();

        memset(new_client.received_message, 0, DEFAULT_BUFLEN);

        if (new_client.socket != 0)
        {
            // Recv the command from the server and store in tempmsg
            int iResult = recv(new_client.socket, new_client.received_message, DEFAULT_BUFLEN, 0);

            if (iResult != SOCKET_ERROR)
                std::cout << new_client.received_message << std::endl;


            else
            {
                std::cout << "recv() failed: " << WSAGetLastError() << std::endl;
                break;
            }

        }

        // Unlock the mutex post accessing the common resources
        _Mutex.unlock();

        // If the Client self-intiates Quit Command - Close socket and clean up
        std::string ServerMessage = "";
        ServerMessage = new_client.received_message;
        if (ServerMessage == "QUIT")
        {
            closesocket(new_client.socket);
            WSACleanup();
            exit(0);
        }
    }

    if (WSAGetLastError() == WSAECONNRESET)
        std::cout << "The server has disconnected" << std::endl;

    return 0;
}

// Client function
int client_main(std::string IP_ADDRESS)
{
    Client _client;
    // Initialize Winsock
    struct addrinfo* result = NULL, * ptr = NULL;
    std::string sent_message = "";
    client_type client = { INVALID_SOCKET, -1, "" };
    int iResult = 0;
    std::string message;
    
    _client.InitializeWinsock();

    struct addrinfo tIP4addr = _client.Bind();

    // Resolve the server address and port
    iResult = getaddrinfo(IP_ADDRESS.c_str(), DEFAULT_PORT, &tIP4addr, &result);
    if (iResult != 0) {
        std::cout << "getaddrinfo() failed with error: " << iResult << std::endl;
        WSACleanup();
        system("pause");
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        client.socket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (client.socket == INVALID_SOCKET) {
            std::cout << "socket() failed with error: " << WSAGetLastError() << std::endl;
            WSACleanup();
            system("pause");
            return 1;
        }

        // Connect to server.
        iResult = connect(client.socket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(client.socket);
            client.socket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (client.socket == INVALID_SOCKET) {
        std::cout << "Unable to connect to server!" << std::endl;
        WSACleanup();
        system("pause");
        return 1;
    }


    //Obtain confirmation from Server that it is connected and ready 
    recv(client.socket, client.received_message, DEFAULT_BUFLEN, 0);
    message = client.received_message;
    std::cout << "Confirmation from Server that it is connected and ready: " << message << std::endl;

    if (message != "Server is full")
    {
        client.id = atoi(client.received_message);

        // Initialize the Thread for processing 
        std::thread my_thread(client_processing, std::ref(client));

        while (1)
        {
            // Provide the client the abilty to send HTTP COMMANDS
            getline(std::cin, sent_message);
            iResult = send(client.socket, sent_message.c_str(), strlen(sent_message.c_str()), 0);

            if (iResult <= 0)
            {
                std::cout << "send() failed: " << WSAGetLastError() << std::endl;
                break;
            }
        }

        // Shutdown the connection since no more data will be sent
        my_thread.detach();
    }
    else
        std::cout << client.received_message << std::endl;


    std::cout << "Shutting down socket..." << std::endl;
    iResult = shutdown(client.socket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        std::cout << "shutdown() failed with error: " << WSAGetLastError() << std::endl;
        closesocket(client.socket);
        WSACleanup();
        system("pause");
        return 1;
    }

    // Close Socket 
    closesocket(client.socket);

    // Clean up Winsock 
    WSACleanup();

    system("pause");
    return 0;
}