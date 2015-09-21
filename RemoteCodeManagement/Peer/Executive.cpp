/////////////////////////////////////////////////////////////////////////////
// Executive.cpp -Handle command line arguments,						   //
//				  control all commanding operations of all packages.	   //
// ver 1.0                                                                 //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Dell XPS L520, Core i7, Windows 7 SP3                      //
// Application: CSE 687 Project 4                                          //
// Author:      Vijayendra Ghadge, Syracuse University	                   //
//              (315) 728 8953, vvgahdge@syr.edu	                       //
/////////////////////////////////////////////////////////////////////////////

//Test Excutive for Project 3 OOD

#include "./Sockets.h"
#include "./Peer.h"
#include "Display.h"
#include "./Message.h"
#include <iostream>

using namespace::std;
//#ifdef EXECUTIVE
int main(int argc, char *argv[])
{
	Display d;
	std::string command=argv[1];
	std::string senderIp=argv[2];
	std::string senderPort = argv[3];
	std::string receiverIp=argv[4];
	std::string receiverPort = argv[5];
	std::string filePath=argv[6];
	int x = std::stoi(senderPort);
	int y = std::stoi(receiverPort);
	title("Server", '=');
	try
	{
		Verbose v(true);				//UnComment this to see Thread Creation and Sockets Connection Establishment and Thread Exits and Socket Closing.
		d.dispReq(3);
		d.dispReq(4);
		d.dispReq(5);
		d.dispReq(6);
		d.dispReq(7);
		d.dispstr("Bonus Requirement *********Concurrent string search in within source code files");
		Message M(command, x, receiverIp, y, filePath);
		Peer p;
		d.dispstr("Creating New Peer (Server)");
		p.begin(M);
		d.dispstr("To Force Close Connection.....Press Any Key************************************");
		std::cin.get();
		d.dispstr("********************************************************************************");
	}
	catch (std::exception& ex)
	{
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}

	std::cout << "\n\n";
	return 0;
}
//#endif
