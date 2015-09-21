#ifndef SENDER_H
#define SENDER_H

///////////////////////////////////////////////////////////////////
// Sender.h - a encapsule over Socet for easy use                //
// ver 1.0                                                       //
// Language:    Visual C++, Visual Studio 2013                   //
// Platform:    Dell XPS L520, Core i7, Windows 7 SP3            //
// Application: CSE 687 Project 4                                //
// Author:      Vijayendra Ghadge, Syracuse University	         //
//              (315) 728 8953, vvgahdge@syr.edu	             //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
-----------------------
* This package is used for CSE687 - Object Oriented Design
* Project #4 - Message Passing Communication Channel to Create SocketConnecter,Establish connection,End connction,Thread Operations.
Public Interface:
=================

Sender S;
Message M("FileUpload", 9070, "localhost", 9080, "../zoo.JPG"); //Message Obj
S.establishConnection(M.getReceiverIp(), M.getReceiverPort());  //Establish connection
S.getMessge(M); //get Messgase Header to me sent.
S.start();      //start Sender on thread.
S.closeConnection(); //end connection

* Required Files:
* ---------------
*  Sender.cpp, Sender.h
*  Sockets.h, Sockets.cpp, AppHelpers.h, AppHelpers.cpp,BlockingQueue.h, BlockingQueue.cpp,FileSystem.h,FileSystem.cpp
*  Message.h, Message.cpp,Sockets.h,Sockets.cpp,WindowsHelpers.h,WindowsHelpers.cpp
*
* Build Process:
* --------------
*   devenv Sender.sln /debug rebuild
*/
#include "./Sockets.h"
#include "./AppHelpers.h"
#include "./Message.h"
#include "./Cpp11-BlockingQueue.h"

using namespace ApplicationHelpers;

class Sender//:public IMockChannel
{
public:
	Sender(){};
	~Sender()
	{
		Sender::closeConnection();
	};
	void Sender::closeConnection();
	void Sender::start(Message msg){ Sender::createSendThread(msg); }  //Start Worker Thread
	void Sender::send_message(Message msg){ SendQ_.enQ(msg); }

private:
	bool Sender::createSendThread(Message);   // run operation on a thread 
	bool Sender::WorkerThread();		//Will last till life time of mainthread to send commands.
	bool Sender::UploadFile(Message);          //Uploads File ....will send ( header then file chunks ) until end of file.
	void Sender::Send1(Message);
	void Sender::Send2(Message);
	void Sender::Send3(Message);
	void Sender::Send4(Message);
	SocketSystem SS_;
	SocketConnecter SC_;
	BlockingQueue <Message> SendQ_;
	std::thread* t;
};
#endif