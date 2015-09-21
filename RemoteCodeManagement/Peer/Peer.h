#ifndef PEER_H
#define PEER_H

///////////////////////////////////////////////////////////////////
// Receiver.h - a container of XmlElement nodes                  //
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
Project 4 Remote Server Code management channel for creating peerIt basically has sender and receiver.
*
Public Interface:
=================
Message M("FileUpload", 9070, "localhost", 9070, "../zoo.JPG");   // send on same port and receive on same port .
Peer p;   //peer obj
p.getMessge(M); //get message ofr peer to send
p.start(); //Start sender and receiver.
p.send(M); //Send message from peer.
//p.end();   // end peer (sender,receiver)
p.postmessage(M); //enque message to send 
p.fetch_ack_Client_Side();  //deque acknowledgements
* Required Files:
* ---------------
Sender.h,sender.cpp
*  Receiver.cpp, Receiver.h
*  Sockets.h, Sockets.cpp, AppHelpers.h, AppHelpers.cpp,BlockingQueue.h, BlockingQueue.cpp,FileSystem.h,FileSystem.cpp
*  Message.h, Message.cpp,Sockets.h,Sockets.cpp,WindowsHelpers.h,WindowsHelpers.cpp
*
* Build Process:
* --------------
*   devenv Peer.sln /debug rebuild

*/
#include "Sockets.h"
#include "AppHelpers.h"
#include "Sender.h"
#include "Receiver.h"
#include "Message.h"
#include "FileSystem.h"
#include <iostream>
#include <thread>
#include <mutex>
#include "Cpp11-BlockingQueue.h"
#include "../Channel/Channel.h"

class Peer : public  IChannel
{
public:
	Peer()
	{
		Peer::createAckThread();  //Start Worker Thread
	}
	Peer(Message M)
	{
		Peer::createAckThread();  //Start Worker Thread
		Peer::begin(M);
	}
	~Peer()
	{
		Peer::end();
	}
	void Peer::begin(Message Ms)      //start connection
	{
		
		receiver_.start(receiver_, Ms, Socket::IP6);
		sender_.start(Ms);
	}
	void Peer::end()
	{
		receiver_.closeConnection();
		sender_.closeConnection();
	}
	void Peer::postMessage(Message msg){ sender_.send_message(msg); }
	Message Peer::fetch_ack_Client_Side(){ return ackQ_.deQ(); }
private:
	bool Peer::createAckThread();   // run operation on a thread 
	bool Peer::ackThread();		//Will last till life time of mainthread to ack commands.
	void Peer::closeAckThread();		//end acknowledgement thread
	void Peer::Check1(Message);
	void Peer::Check2(Message);
	void Peer::Check3(Message);
	void Peer::Check4(Message);
	void Peer::Check5(Message);
	Message Peer::Invertor(Message M);
	Sender sender_;
	Receiver receiver_;
	BlockingQueue <Message> ackQ_;
	std::thread* t;
};
#endif
