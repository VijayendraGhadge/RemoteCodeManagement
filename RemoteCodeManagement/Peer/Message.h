#ifndef MESSAGE_H
#define MESSAGE_H
///////////////////////////////////////////////////////////////////
// Message.h -     TO create  Message and retrieve		         //
//                     information about the Client IP Address   //
//                     Server IP address and Port information.   //
// ver 1.1                                                       //
// Language:    Visual C++, Visual Studio 2013                   //
// Platform:    Dell XPS L520, Core i7, Windows 7 SP3            //
// Application: CSE 687 Project 3                                //
// Author:      Vijayendra Ghadge, Syracuse University	         //
//              (315) 728 8953, vvgahdge@syr.edu	             //
///////////////////////////////////////////////////////////////////
// Package Operations:                                           //
//  HTTP Message Header: Builds a HTTP Header which has diff     //
//                       attributes like IP Address Port Info    //
//                       type of content and Content length      //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package is used for CSE687 - Object Oriented Design
* Project #4 - Remote Code Magement Communication Channel to build Header.
*
Public Interface:
=================
Message msg;   //Create Message Obj
//Message msg("FileUpload", "localhost", 9080, "127.0.0.1", 9050, "../../../lj1018_1020_1022-HB-pnp-win64-en.exe");
msg.createMsg("FileUpload", "localhost", 9080, "127.0.0.1", 9050, "../../../lj1018_1020_1022-HB-pnp-win64-en.exe"); //Add Header Contents.
std::string temp = msg.ConstructHeader();//showHeader();
std::cout << temp;
inter.InterpretHeader(temp); //Interpret Header from String.
temp = inter.ConstructHeader();//showHeader(); //get header i nstring format.

* Required Files:
* ---------------
Message.h, Message .cpp, FileSystem.h, FileSystem.cpp
*
* Build Process:
* --------------
*   devenv Message.sln /debug rebuild
*/

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "./FileSystem.h"

using namespace std;
using namespace FileSystem;

class Message{
public:
	Message()
	{
		senderIp_ = "localhost";
	};
	Message(std::string cmd, int SenderPort, std::string ReceiverIp, int ReceiverPort, std::string FilePath)
	{
		createMsg(cmd, "localhost", SenderPort, ReceiverIp, ReceiverPort, FilePath);
	};
	
	void createMsg(std::string cmd, std::string SenderIp, int SenderPort, std::string ReceiverIp, int ReceiverPort, std::string FilePath);
	std::string ConstructHeader();
	bool InterpretHeader(std::string str);
//	std::string ConstructHeaderV2();
	//void InterpretHeaderV2(std::string str);
	//void InterpretHeaderV3(std::string str);

	//std::string Message::showHeader(){
	//	std::string temp;
	//	temp += "\n\tPOST ";
	//	temp += filepath_;
	//	temp += "\nCommand:";
	//	temp += command_;
	//	temp += " HTTP 1.0";
	//	temp += "\n\tClient IP:";
	//	temp += senderIp_;
	//	temp += ":";
	//	temp += ToString(senderPort_);
	//	temp += "\n\tServer IP:";
	//	temp += receiverIp_;
	//	temp += ":";
	//	temp += ToString(receiverPort_);
	//	//temp += "\n\tContent-Type:";
	//	//temp += msgtype;
	//	temp += "\n\tContent-Length:";
	//	temp += std::to_string(contentLength_);
	//	temp += "\n";
	//	return temp;

	//}
	//void Message::BuildBody(std::string body){
	//	//msgtype = type;
	////	msgcontent = body;
	//}
	//std::string Message::BuiltBody(){
	//	std::string temp;
	//	//temp += "\n\tMessage Type:";
	//	//temp += msgtype;
	////	temp += "\n\tMessage Body:";
	//	//temp += msgcontent;
	//	return temp;
	//};

	std::string Message::getCommand(){
		return command_;
	}
	std::string Message::getReceiverIp(){
		return receiverIp_;
	}
	std::string Message::getSenderIp(){
		return senderIp_;
	}
	std::string Message::getFileName(){
		return fileName_;
	}
	void Message::setFileName(std::string filenam){
		fileName_=filenam;
	}
	std::string Message::getFilePath(){
		return filepath_;
	}
	void Message::setFilePath(std::string filep){
		filepath_ = filep;
	}
	int Message::getReceiverPort(){
		return receiverPort_;
	}
	int Message::getSenderPort(){
		return senderPort_;
	}
	void Message::setReceiverPort(int port){
		receiverPort_ = port;
	}
	void Message::setReceiverIP(std::string IP){
		receiverIp_ = IP;
	}
	void Message::setSenderPort(int port){
		senderPort_ = port;
	}
	void Message::setSenderIP(std::string IP){
		senderIp_ = IP;
	}
	void Message::setCommand(std::string cmd){
		command_ = cmd;
	}

	void Message::setContentLength(int len){
		contentLength_ = len;
	}
	int Message::getContentLength(){
		return contentLength_;
	}


	std::string Message::ToString(int i){
		std::ostringstream conv;
		conv << i;
		return conv.str();
	}
private:
	std::string command_;
	std::string senderIp_;
	std::string receiverIp_;
	std::string filepath_;
	std::string fileName_;
	int contentLength_;
	int senderPort_;
	int receiverPort_;
};
#endif