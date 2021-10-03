//
// Bachelor of Software Engineering (Artificial Intelligence)
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name : Main.h
// Description : Winsock 2 Socket Programming - Server Side - (Chat Application)
// Author : Rawinder Singh
// Mail : rawinder.singh@mds.ac.nz
//

// Library Includes
#include "Server.h"

// Global variable definition 
#define DEFAULT_PORT "9631"
#define DEFAULT_BUFLEN 512
std::string InitialGet = "Bindu is the best";

// Struct definition for the Client
struct client_type
{
    int id;
    SOCKET socket;
};

const char OPTION_VALUE = 1;
const int MAX_CLIENTS = 5;


// Function to process each of the connected clients
int client_processing(client_type& new_client, std::vector<client_type>& client_array, std::thread& thread)
{
    Server server_;
    // Local variables to store the information from the connected client
    std::string msg{};
    char tempmsg[DEFAULT_BUFLEN] = "";

    // Connected Session with the connected client 
    while (1)
    {;
    
        memset(tempmsg, 0, DEFAULT_BUFLEN);

        if (new_client.socket != 0)
        {
            // Recv the command from the client store in tempmsg
            int iResult = recv(new_client.socket, tempmsg, DEFAULT_BUFLEN, 0);

            if (iResult != SOCKET_ERROR)
            {
                if (strcmp("", tempmsg))
                    msg = "Client #" + std::to_string(new_client.id) + ": " + tempmsg;

                std::string _TEMPMSG = std::string(tempmsg);

                // ------------------ HTTP COMMANDS ---------------------

                // Client requests TRANSLATE - Convert the recevied message to uppercase
                if (_TEMPMSG == "TRANSLATE")
                {
                    std::string tOK = "200 OK";
                    send(new_client.socket, tOK.c_str(), strlen(tOK.c_str()), 0);

                    std::cout << "Client #" << std::to_string(new_client.id) << " sends TRANSLATE" << std::endl;

                }
                if (_TEMPMSG.find('.') != std::string::npos)
                {
                    std::string SaveMessage = _TEMPMSG;

                    int toupper(int c);
                    // Convert string to upper case
                    for_each(_TEMPMSG.begin(), _TEMPMSG.end(), [](char& c) {
                        c = ::toupper(c);
                        });
                    send(new_client.socket, _TEMPMSG.c_str(), strlen(_TEMPMSG.c_str()), 0);

                }

                // Client requests GET - Send the default message back to the Client
                if (_TEMPMSG == "GET")
                {
                    std::string tOK = "200 OK";
                    send(new_client.socket, tOK.c_str(), strlen(tOK.c_str()), 0);

                    send(new_client.socket, InitialGet.c_str(), strlen(InitialGet.c_str()), 0);

                    std::cout << "Client #" << std::to_string(new_client.id) << " sends GET" << std::endl;
                }

                // Client requests PUT - Store the sent message into a saved buffer the code does this at the being of loop
                if (_TEMPMSG == "PUT")
                {
                    std::string tOK = "200 OK";
                    send(new_client.socket, tOK.c_str(), strlen(tOK.c_str()), 0);

                    std::cout << "Client #" << std::to_string(new_client.id) << " sends PUT" << std::endl;
                }

                // Client requests QUIT - Quit the client from the server and clean up 
                if (_TEMPMSG == "QUIT")
                {
                    std::string tOK = "200 OK";
                    std::string Quit = "QUIT";
                    send(new_client.socket, tOK.c_str(), strlen(tOK.c_str()), 0);
                    send(new_client.socket, Quit.c_str(), strlen(Quit.c_str()), 0);

                    std::cout << "Client #" << std::to_string(new_client.id) << " sends QUIT" << std::endl;

                    msg = "Client #" + std::to_string(new_client.id) + " Disconnected";

                    std::cout << msg << std::endl;

                    closesocket(new_client.socket);
                    closesocket(client_array[new_client.id].socket);
                    client_array[new_client.id].socket = INVALID_SOCKET;

                    break;
                }

                // Client requests OPTIONS - Get the client information about the requests that can send
                if (_TEMPMSG == "OPTIONS")
                {
                    std::string tOK = "200 OK";
                    std::string Options = "Allow: TRANSLATE, GET, PUT, OPTIONS & QUIT ";
                    send(new_client.socket, tOK.c_str(), strlen(tOK.c_str()), 0);
                    send(new_client.socket, Options.c_str(), strlen(Options.c_str()), 0);

                    std::cout << "Client #" << std::to_string(new_client.id) << " sends OPTIONS" << std::endl;
                }

                // Send 400 error to client if valid request is made
                else if (_TEMPMSG != "OPTIONS" && _TEMPMSG != "GET" && _TEMPMSG != "PUT" && _TEMPMSG != "TRANSLATE" && _TEMPMSG != "QUIT" && _TEMPMSG != ".")
                {
                    if (_TEMPMSG.find('.') != std::string::npos)
                    {

                    }
                    else
                    {
                        std::string ErrorMessage = "400 Command not valid";
                        send(new_client.socket, ErrorMessage.c_str(), strlen(ErrorMessage.c_str()), 0);
                    }
                }

                //Broadcast that message to the other clients
                for (int i = 0; i < MAX_CLIENTS; i++)
                {
                    if (client_array[i].socket != INVALID_SOCKET)
                        if (new_client.id != i)
                            iResult = send(client_array[i].socket, msg.c_str(), strlen(msg.c_str()), 0);
                }
            }
            else
            {
                // If the client disconnects without notice
                msg = "Client #" + std::to_string(new_client.id) + " Disconnected";

                std::cout << msg << std::endl;

                // Close socket and clean up
                closesocket(new_client.socket);
                closesocket(client_array[new_client.id].socket);
                client_array[new_client.id].socket = INVALID_SOCKET;

                //Broadcast the disconnection message to the other clients
                for (auto i = 0; i < MAX_CLIENTS; i++)
                {
                    if (client_array[i].socket != INVALID_SOCKET)
                        iResult = send(client_array[i].socket, msg.c_str(), strlen(msg.c_str()), 0);
                }

                break;
            }
        }
    }

    // Detach the Thread once service is statisfied 
    thread.detach();


    return 0;
}

// Server function
inline int server_main(std::string IP_ADDRESS)
{
    Server _server;

    // Initialize Winsock
    struct addrinfo* server = NULL;
    SOCKET server_socket = INVALID_SOCKET;
    std::string msg = "Server is ready...";
    std::vector<client_type> client(MAX_CLIENTS);
    int num_clients = 0;
    int temp_client_ids = -1;
    std::thread thread_[MAX_CLIENTS];

    _server.InitializeWinsock();
    
    struct addrinfo tIP4addr = _server.Bind();

    // Setup the Server using the user defined IP Address 
    getaddrinfo(IP_ADDRESS.c_str(), DEFAULT_PORT, &tIP4addr, &server);

    // Tell Winsock the socket is for listening 
    server_socket = socket(server->ai_family, server->ai_socktype, server->ai_protocol);

    // Setup socket options
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &OPTION_VALUE, sizeof(int)); //Make it possible to re-bind to a port that was used within the last 2 minutes
    setsockopt(server_socket, IPPROTO_TCP, TCP_NODELAY, &OPTION_VALUE, sizeof(int)); //Used for interactive programs

    // Assign an address to the server socket
    bind(server_socket, server->ai_addr, (int)server->ai_addrlen);

    // Tell Winsock the socket is for listening 
    listen(server_socket, SOMAXCONN);

    std::cout << "Server is running successfully awaiting connection from clients..." << std::endl;

    //Initialize the client list
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        client[i] = { -1, INVALID_SOCKET };
    }

    while (1)
    {
        // Define Client info
        sockaddr_in client_info = { 0 };
        int addr = sizeof(client_info);

        SOCKET incoming = INVALID_SOCKET;
        incoming = accept(server_socket, (struct sockaddr*)&client_info, &addr);

        // Get the IP address from the Client and Print
        char* ip = inet_ntoa(client_info.sin_addr);

        std::cout << "Server: got connection from client " << ip << std::endl;


        if (incoming == INVALID_SOCKET) continue;

        // Reset the number of clients
        num_clients = -1;

        // Create a temporary client id for the next client
        temp_client_ids = -1;
        for (auto i = 0; i < MAX_CLIENTS; i++)
        {
            if (client[i].socket == INVALID_SOCKET && temp_client_ids == -1)
            {
                client[i].socket = incoming;
                client[i].id = i;
                temp_client_ids = i;
            }

            if (client[i].socket != INVALID_SOCKET)
                num_clients++;

        }

        if (temp_client_ids != -1)
        {
            send(client[temp_client_ids].socket, msg.c_str(), strlen(msg.c_str()), 0);

            // Create a thread process for that client
            thread_[temp_client_ids] = std::thread(client_processing, std::ref(client[temp_client_ids]), std::ref(client), std::ref(thread_[temp_client_ids]));
        }
        else
        {
            msg = "Server is full";
            send(incoming, msg.c_str(), strlen(msg.c_str()), 0);
            std::cout << msg << std::endl;
        }
    } 


    // Close listening socket
    closesocket(server_socket);

    // Close client socket
    for (auto i = 0; i < MAX_CLIENTS; i++)
    {
        thread_[i].detach();
        closesocket(client[i].socket);
    }

    // Clean up Winsock
    WSACleanup();
    std::cout << "Program has ended successfully" << std::endl;

    system("pause");
    return 0;
}

