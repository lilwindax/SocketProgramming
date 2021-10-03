//
// Bachelor of Software Engineering (Artificial Intelligence)
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name : Main.cpp
// Description : Winsock 2 Socket Programming - Server Side - (Chat Application)
// Author : Rawinder Singh
// Mail : rawinder.singh@mds.ac.nz
//

// Library Includes
#include"Main.h"

// Global Variable to store the IP Address
std::string IP_ADDRESS = "127.0.0.1";		// Change this to your IP Address

// Main Function 
int main()
{
	// Run the Server function and pass the IP Address 
	server_main(IP_ADDRESS);
}