///////////////////////////////////////////////////////////////////
// Peer.cpp - a container of SENDER,RECEIVER					 //
// ver 1.0                                                       //
// Language:    Visual C++, Visual Studio 2013                   //
// Platform:    Dell XPS L520, Core i7, Windows 7 SP3            //
// Application: CSE 687 Project 4                                //
// Author:      Vijayendra Ghadge, Syracuse University	         //
//              (315) 728 8953, vvgahdge@syr.edu	             //
///////////////////////////////////////////////////////////////////
//#define IN_DLL
#include"Peer.h"

//......Create Worker Thread..... = Dispatcher.
bool Peer::createAckThread()
{
	t = new thread(&Peer::ackThread, this);
	t->detach();
	return true;
}

//.......Function running on thread....... The Dispatcher.
bool Peer::ackThread()
{
	while (true)
	{
		Message msg = receiver_.get_acknowledgement();
		Check1(msg);
		Check2(msg);
		Check3(msg);
		Check4(msg);
		Check5(msg);
	}
	Verbose::show("\n  Acknowledge Thread Ended \n");
	return true;
}


void Peer::Check1(Message msg)
{

	if (msg.getCommand() == "End Receiver")
	{
		std::cout << "\nEnding Worker Thread (Receivers dispatcher)";
		Message M = msg;
		M = Invertor(M);
		M.setCommand("End Sender");
		postMessage(msg);
	//	break;
	}
	if (msg.getCommand() == "FUS")
	{
		//std::cout << "File Upload Succeeded received";			
		Message M = msg;
		M = Invertor(M);
		M.setCommand("File Upload Successful");
		postMessage(M);
	//	continue;
	}
	if (msg.getCommand() == "File Upload Successful")
	{
		Message M = msg;
		M.setCommand("FTS");
		ackQ_.enQ(M);
		M = Invertor(M);
		M.setCommand("ACK");
		postMessage(M);
	//	continue;
	}

}

void Peer::Check2(Message msg)
{
	if (msg.getCommand() == "FileDownload")
	{
		//std::cout << "File Upload Succeeded received";
		Message M = msg;
		M.setCommand("FileUpload");
		std::string FILEPATH = Path::getFullFileSpec(M.getFileName());
		M.setFilePath(FILEPATH);
		M.setFileName(Path::getName(M.getFilePath()));
		M = Invertor(M);
		postMessage(M);
	//	continue;
	}
	if (msg.getCommand() == "ACK")
	{
		Message M = msg;
		M.setCommand("FTS");
		ackQ_.enQ(M);
		//continue;
	}
	if (msg.getCommand() == "GetDirList")
	{
		//std::cout << "File Upload Succeeded received";
		Message M = msg;
		M = Invertor(M);
		M.setCommand("ReceivedDirList");
		postMessage(M);
	//	continue;
	}
}

void Peer::Check3(Message msg)
{
	if (msg.getCommand() == "ReceivedDirList")
	{
		Message M = msg;
		ackQ_.enQ(M);
	//	continue;

	}
	if (msg.getCommand() == "GetFiles")
	{
		Message M = msg;
		M = Invertor(M);
		M.setCommand("ReceivedFiles");
		postMessage(M);
	//	continue;

	}
	if (msg.getCommand() == "ReceivedFiles")
	{
		Message M = msg;
		ackQ_.enQ(M);
	//	continue;
	}

}
void Peer::Check4(Message msg)
{
	if (msg.getCommand() == "GetDir")
	{
		Message M = msg;
		M = Invertor(M);
		M.setCommand("ReceivedDir");
		postMessage(M);
//		continue;

	}
	if (msg.getCommand() == "ReceivedDir")
	{
		Message M = msg;
		ackQ_.enQ(M);
	//	continue;
	}
	if (msg.getCommand() == "SearchString")
	{
		Message M = msg;
		M = Invertor(M);
		M.setCommand("ReceivedString");
		postMessage(M);
//		continue;

	}
}
void Peer::Check5(Message msg)
{
	if (msg.getCommand() == "ReceivedString")
	{
		Message M = msg;
		ackQ_.enQ(M);
	//	continue;

	}
	if (msg.getCommand() == "End Sender")
	{
		Message M = msg;
		M = Invertor(M);
		postMessage(msg);
	//	continue;
	}
	//else
	//{
	//	continue;
	//}
}

//
Message Peer::Invertor(Message M)   //inverts sender and receiver IP,Port number
{
	std::string temp;
	int tem;
	temp = M.getReceiverIp();
	M.setReceiverIP(M.getSenderIp());
	M.setSenderIP(temp);

	tem = M.getReceiverPort();
	M.setReceiverPort(M.getSenderPort());
	M.setSenderPort(tem);
	return M;
}

//
//#ifdef TEST_PEER
//int main()
//{
//	title("Testing Peer", '=');
//
//	try
//	{
//		Verbose v(true);
//		Message M("FileUpload", 9080, "localhost", 9080, "../zoo.JPG");   // send on same port and receive on same port .
//		Peer p;
//		p.start(M);
//		//std::cin.get();
//		//Message N("XYZ", 9080, "localhost", 9090, "../zoo2.JPG");   // send on same port and receive on same port .
//		//Peer pee;
//		//pee.start(N);
//		title("CLient Sending File to Server");
//		title("To Force Connection close Press Any Key");
//		std::cin.get();
//
//	}
//	catch (std::exception& ex)
//	{
//		Verbose::show("  Exception caught:", always);
//		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
//	}
//
//}
//#endif