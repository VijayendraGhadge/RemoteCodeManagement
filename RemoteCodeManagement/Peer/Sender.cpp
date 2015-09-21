///////////////////////////////////////////////////////////////////
// Sender.cpp - A Class which encapsulates Socket operations     //
//              into easy Usage									 //
// ver 1.0                                                       //
// Language:    Visual C++, Visual Studio 2013                   //
// Platform:    Dell XPS L520, Core i7, Windows 7 SP3            //
// Application: CSE 687 Project 4                                //
// Author:      Vijayendra Ghadge, Syracuse University	         //
//              (315) 728 8953, vvgahdge@syr.edu	             //
///////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides fuctionality for Sockets-4.5:
*  - passing byte buffers (File)
*  - passing strings  (Header)
*
*  Required Files:
*  ---------------
*  Sender.cpp, Sender.h
*  Sockets.h, Sockets.cpp, AppHelpers.h, AppHelpers.cpp,BlockingQueue.h, BlockingQueue.cpp,FileSystem.h,FileSystem.cpp
*  Message.h, Message.cpp,Sockets.h,Sockets.cpp,WindowsHelpers.h,WindowsHelpers.cpp
*
*  Build Command:
*  --------------
*  devenv Sockets.sln /rebuild debug
*
*/
#include <iostream>
#include <thread>
#include "Sender.h"


//......Create Worker Thread..... = Dispatcher.
bool Sender::createSendThread(Message message)
{
	SendQ_.enQ(message);								/////.............After Message Received Enque in Blocking Que
	t = new thread(&Sender::WorkerThread, this);
	t->detach();
	return true;
}
//.......Function running on thread....... The Dispatcher.
bool Sender::WorkerThread()
{
	while (true)
	{
		Message msg = SendQ_.deQ();
		Send1(msg);
		Send2(msg);
		Send3(msg);
		Send4(msg);
	}
	Verbose::show("Send thread stopping");
	return true;
}

void Sender::Send1(Message msg)
{
	if (msg.getCommand() == "End Sender")
	{
		std::cout << "\nEnding Worker Thread Sender";
	}
	if (msg.getCommand() == "FileUpload")
	{
		while (!SC_.connect(msg.getReceiverIp(), msg.getReceiverPort()))     ////establishing connection!!!!!
		{
			Verbose::show("client waiting to connect");
			::Sleep(100);
		}
		Sender::UploadFile(msg);
		
	}
	if (msg.getCommand() == "File Upload Successful")
	{
		while (!SC_.connect(msg.getReceiverIp(), msg.getReceiverPort()))     ////establishing connection!!!!!
		{
			Verbose::show("client waiting to connect");
			::Sleep(100);
		}
		std::string x = msg.ConstructHeader();
		SC_.sendString(x);
		
	}
}

void Sender::Send2(Message msg)
{
	if (msg.getCommand() == "FileDownload")
	{
		while (!SC_.connect(msg.getReceiverIp(), msg.getReceiverPort()))     ////establishing connection!!!!!
		{
			Verbose::show("client waiting to connect");
			::Sleep(100);
		}
		std::string x = msg.ConstructHeader();
		SC_.sendString(x);
		
	}
	if (msg.getCommand() == "ACK")
	{
		while (!SC_.connect(msg.getReceiverIp(), msg.getReceiverPort()))     ////establishing connection!!!!!
		{
			Verbose::show("client waiting to connect");
			::Sleep(100);
		}
		std::string x = msg.ConstructHeader();
		SC_.sendString(x);
		
	}
	if (msg.getCommand() == "GetDirList")
	{
		while (!SC_.connect(msg.getReceiverIp(), msg.getReceiverPort()))     ////establishing connection!!!!!
		{
			Verbose::show("client waiting to connect");
			::Sleep(100);
		}
		std::string x = msg.ConstructHeader();
		SC_.sendString(x);
		
	}
}

void Sender::Send3(Message msg)
{
	if (msg.getCommand() == "ReceivedDirList")
	{
		while (!SC_.connect(msg.getReceiverIp(), msg.getReceiverPort()))     ////establishing connection!!!!!
		{
			Verbose::show("client waiting to connect");
			::Sleep(100);
		}
		std::string x = msg.ConstructHeader();
		SC_.sendString(x);
		
	}
	if (msg.getCommand() == "GetFiles")
	{
		while (!SC_.connect(msg.getReceiverIp(), msg.getReceiverPort()))     ////establishing connection!!!!!
		{
			Verbose::show("client waiting to connect");
			::Sleep(100);
		}
		std::string x = msg.ConstructHeader();
		SC_.sendString(x);
		
	}
	if (msg.getCommand() == "ReceivedFiles")
	{
		while (!SC_.connect(msg.getReceiverIp(), msg.getReceiverPort()))     ////establishing connection!!!!!
		{
			Verbose::show("client waiting to connect");
			::Sleep(100);
		}
		std::string x = msg.ConstructHeader();
		SC_.sendString(x);
		
	}
}

void Sender::Send4(Message msg)
{
	if (msg.getCommand() == "ReceivedDir")
	{
		while (!SC_.connect(msg.getReceiverIp(), msg.getReceiverPort()))     ////establishing connection!!!!!
		{
			Verbose::show("client waiting to connect");
			::Sleep(100);
		}
		std::string x = msg.ConstructHeader();
		SC_.sendString(x);
		
	}
	if (msg.getCommand() == "GetDir")
	{
		while (!SC_.connect(msg.getReceiverIp(), msg.getReceiverPort()))     ////establishing connection!!!!!
		{
			Verbose::show("client waiting to connect");
			::Sleep(100);
		}
		std::string x = msg.ConstructHeader();
		SC_.sendString(x);
		
	}
	if (msg.getCommand() == "SearchString")
	{
		while (!SC_.connect(msg.getReceiverIp(), msg.getReceiverPort()))     ////establishing connection!!!!!
		{
			Verbose::show("client waiting to connect");
			::Sleep(100);
		}
		std::string x = msg.ConstructHeader();
		SC_.sendString(x);
		
	}
	if (msg.getCommand() == "ReceivedString")
	{
		while (!SC_.connect(msg.getReceiverIp(), msg.getReceiverPort()))     ////establishing connection!!!!!
		{
			Verbose::show("client waiting to connect");
			::Sleep(100);
		}
		std::string x = msg.ConstructHeader();
		SC_.sendString(x);
		
	}
}

//.........Uploads File ffrom Sender side by sending Header and then file Chunks until end of file by just modyfying header's contentlenght  each time
bool Sender::UploadFile(Message msg)
{

	std::string FilePath = msg.getFilePath();
	const int BufLen = 10240;
	Socket::byte buffer[BufLen];
	std::streamoff bytesRead;
	std::ifstream file(FilePath, std::ios::in | std::ios::binary);
	if (file.is_open())
	{
		file.seekg(0, std::ios::end);
		std::streamoff remaining = file.tellg();			//Get File Size
		file.seekg(0, std::ios::beg);
		while (file.good())
		{
			file.read(buffer, min(BufLen, int(remaining)));
			bytesRead = min(BufLen, remaining);
			msg.setContentLength(int(bytesRead));            //set content length everytime in header 
			//msg.setCommand("FileUpload");
			std::string Header = msg.ConstructHeader();		//send this header
			SC_.sendString(Header);
			SC_.send(int(bytesRead), buffer);				//send Bytes Read.
			remaining -= bytesRead;
			if (bytesRead < 10240) break;
		}
		file.close();
		return true;
	}
	else
	{
		std::cout << "File Already Open OR File Not Found";
	}
	return false;

}
//...........Establishes Connection with Receiver............
//void Sender::establishConnection(std::string receiverIP, int Rport)
//{
//	while (!SC_.connect(receiverIP, Rport))
//	{
//		Verbose::show("client waiting to connect");
//		::Sleep(100);
//	}
//}

//.........Close Socket Connection............
void Sender::closeConnection()
{
	Message X;
	X.setCommand("End Sender");
	SendQ_.enQ(X);
	Verbose::show("\n  Client calling send shutdown \n");
	SC_.shutDownSend();  //OR // SC_.close();
}

//----< test stub starts here >----------------------------------------------


//----< demonstration >------------------------------------------------------

#ifdef TEST_SENDER
int main()
{
	title("Testing Socket Client", '=');
	try
	{
		Verbose v(true);

		Sender S;
		Message M("FileUpload", 9070, "localhost", 9080, "../hi.JPG");
		S.start(M);
		title("Client sending File...Be Patient...Connection will close in time");
		title("To Force Connection close Press Any Key");
		std::cin.get();
		S.closeConnection();
	}
	catch (std::exception& ex)
	{
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}
#endif