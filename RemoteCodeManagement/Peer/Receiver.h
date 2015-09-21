#ifndef RECEIVER_H
#define RECEIVER_H
///////////////////////////////////////////////////////////////////
// Receiver.h - A receiver node				                     //
// ver 1.0                                                       //
// Language:    Visual C++, Visual Studio 2013                   //
// Platform:    Dell XPS L520, Core i7, Windows 7 SP3            //
// Application: CSE 687 Project 4                                //
// Author:      Vijayendra Ghadge, Syracuse University	         //
//              (315) 728 8953, vvgahdge@syr.edu	             //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package is used for CSE687 - Object Oriented Design
* Project #4 - Message Passing Communication Channel to Create SocketListner,End connction,Thread Operations.
Public Interface:
=================
Receiver cp; //Create Receiver Obj
cp.start(cp,9080, Socket::IpVer::IP6); Start receiver thread by grabbing connecction
cp.closeConnection(); //Close socket connection
* Required Files:
* ---------------
*  Receiver.cpp, Receiver.h
*  Sockets.h, Sockets.cpp, AppHelpers.h, AppHelpers.cpp,BlockingQueue.h, BlockingQueue.cpp,FileSystem.h,FileSystem.cpp
*  Message.h, Message.cpp,Sockets.h,Sockets.cpp,WindowsHelpers.h,WindowsHelpers.cpp
*
* Build Process:
* --------------
*   devenv Receiver.sln /debug rebuild
*/
#include "./sockets.h"
#include "./AppHelpers.h"
#include <iostream>
#include <thread>
#include <mutex>
#include "Cpp11-BlockingQueue.h"
#include "./Message.h"
#include "../FileManager/FileManager.h"
#include "../DataStore/DataStore.h"
using namespace ApplicationHelpers;

class Receiver //: public ISendr
{

public:
	/////////////////////////////////////////////////////////////////////////////
	// Server's client handler class
	// - must be callable object so we've built as a functor
	// - passed to SocketListener::start(CallObject& co)
	// - Client handling thread starts by calling operator()
	Receiver()
	{
		serverRootDir = Directory::getCurrentDirectory();
	};
	~Receiver()
	{
		Receiver::closeConnection();
	}

	void Receiver::operator()(Socket& socket_);
	void Receiver::closeConnection();
	void Receiver::start(Receiver &cp, Message msg, Socket::IpVer typeOfProtocol){
		SL_ = new SocketListener(msg.getSenderPort(), typeOfProtocol);
		SL_->start(cp);
	}
	Message Receiver::get_acknowledgement(){return ReceiveQ_.deQ();}
private:

	SocketSystem SS_;
	SocketListener *SL_;
	std::string Receiver::interpretCommand(std::string);

	std::string Receiver::getdirs(std::string);
	std::string Receiver::getfiles(std::string,std::string);
	std::string Receiver::getAllfiles();
	std::string Receiver::getfilestructure();
	std::string Receiver::parsepath(std::string, std::string);
	std::string Receiver::searchString(std::string path, std::string searchText);
	void Receiver::Receive1(std::string, std::string);
	void Receiver::Receive2(std::string, std::string);
	void Receiver::Receive3(std::string, std::string);
	void Receiver::LastPacket(Message);

	std::string Receiver::serverRootDir;				//Server Root Directory
	bool fileUpload(Socket& socket_, Message &m);    //Upload file Fn
	bool fileDownload(Socket& socket_, Message &m);    //Download file Fn
	BlockingQueue <Message> ReceiveQ_;
};
#endif