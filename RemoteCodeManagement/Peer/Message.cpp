///////////////////////////////////////////////////////////////////
// Message.cpp - To create HTTP Message and retrieve			 //
//               information about the Client IP Address         //
//               Server IP address and Port information.	     //
// ver 1.0                                                       //
// Language:    Visual C++, Visual Studio 2013                   //
// Platform:    Dell XPS L520, Core i7, Windows 7 SP3            //
// Application: CSE 687 Project 3 ,4                              //
// Author:      Vijayendra Ghadge, Syracuse University	         //
//              (315) 728 8953, vvgahdge@syr.edu	             //
///////////////////////////////////////////////////////////////////
// Package Operations:                                           //
// -------------------                                           //
// HTTPMessage :     Sets IP Address Port number of for the HTTP //
//                 message. Retrieves values of IP Address and   //
//                 port numbers of the sender and client.        //
//                    Contains a body and Type of content.       //
///////////////////////////////////////////////////////////////////
#include "Message.h"

//
//void Message::InterpretHeaderV3(std::string str)
//{
//	int startLoc = 0;
//	int size = 0;
//	bool flag = false;
//	std::vector < std::string > vec ;
//	for (size_t i = 0; i <= str.length(); i++)
//	{
//		str.find_first_of(":");
//		if (str[i] != '\n')
//		{
//			continue;
//			flag = false;
//		}
//		else
//		{
//			flag = true;
//			size = i - startLoc;
//			vec.push_back(str.substr(startLoc, size));
//			startLoc = i+1;
//		}
//	}
//			int  q= vec.size();
//			if (flag == true && q == 7)
//            {	
//			command_ = vec[0];
//			senderIp_ = vec[1];
//			senderPort_ = std::stoi(vec[2]);
//			receiverIp_ = vec[3];
//			receiverPort_ = std::stoi(vec[4]);
//			fileName_ = vec[5];
//			contentLength_ = std::stoi(vec[6]);
//		}
//	
//	
//}

//void Message::InterpretHeaderV2(std::string str)
//{
//
//	int startLoc = 0;
//	int size = 0;
//	//bool flag = false;
//	std::vector < std::string > vec;
//	for (size_t i = 0; i <= str.length(); i++)
//	{
//
//		if (str[i] != '\n')
//		{
//			continue;
//		//	flag = false;
//		}
//		else
//		{
//		//	flag = true;
//			size = i - startLoc;
//			vec.push_back(str.substr(startLoc, size));
//			startLoc = i + 1;
//		}
//	}
//	//if (flag == true)
//	//{
//		command_ = vec[0];
//		senderIp_ = vec[1];
//		senderPort_ = std::stoi(vec[2]);
//		receiverIp_ = vec[3];
//		receiverPort_ = std::stoi(vec[4]);
//		fileName_ = vec[5];
//		contentLength_ = std::stoi(vec[6]);
//	//}
//}

//..........Interpret Header From String...............

bool Message::InterpretHeader(std::string str)
{

	int startLoc = 0;
	int size = 0;
	bool flag = false;
	std::vector < std::string > vec;
	for (size_t i = 0; i <= str.length(); i++)
	{

		if (str[i] != '\n')
		{
			continue;
			flag = false;
		}
		else
		{
			flag = true;
			size = i - startLoc;
			vec.push_back(str.substr(startLoc, size));
			startLoc = i + 1;
		}
	}
	int  q= vec.size();
	if (flag == true&&q==7)
	{
		std::vector < std::string > v;
		for (auto a : vec)
		{
			std::size_t pos = a.find(":");
			v.push_back(a.substr(pos+2));
		}
		int  q = vec.size();
		if (q == 7)
		{
			command_ = v[0];
			senderIp_ = v[1];
			senderPort_ = std::stoi(v[2]);
			receiverIp_ = v[3];
			receiverPort_ = std::stoi(v[4]);
			fileName_ = v[5];
			contentLength_ = std::stoi(v[6]);
			return true;
		}
	}
	return false;
}

//
//std::string Message::ConstructHeaderV2()
//{
//	std::string header;
//	header += command_;
//	header += "\n";
//	header += senderIp_;
//	header += "\n";
//	header += ToString(senderPort_);
//	header += "\n";
//	header += receiverIp_;
//	header += "\n";
//	header += ToString(receiverPort_);
//	header += "\n";
//	header += fileName_;
//	header += "\n";
//	header += std::to_string(contentLength_);
//	header += "\n";
//	return header;
//
//}

//...........Construct Sring of Header.........
std::string Message::ConstructHeader()
{
	std::string header;
	header += "Command : ";
	header += command_;
	header += "\n";
	header += "SenderIP : ";
	header += senderIp_;
	header += "\n";
	header += "SenderPort : ";
	header += ToString(senderPort_);
	header += "\n";
	header += "ReceiverIP : ";
	header += receiverIp_;
	header += "\n";
	header += "ReceiverPort : ";
	header += ToString(receiverPort_);
	header += "\n";
	header += "FileName : ";
	header += fileName_;
	header += "\n";
	header += "ContentLength : ";
	header += std::to_string(contentLength_);
	header += "\n";
	return header;
}

//.........Set Values of Message Header/............

void Message::createMsg(std::string cmd, std::string SenderIp, int SenderPort, std::string ReceiverIp, int ReceiverPort, std::string FilePath)
{
	command_ = cmd;
	//if (SenderIp == "localhost" || SenderIp == "LocalHost" || SenderIp == "Localhost" || SenderIp == "LOCALHOST")
	//{
	//	senderIp_ = "127.0.0.1";
	//}
	//else
	//{
	//	senderIp_ = SenderIp;
	//}
	senderIp_ = SenderIp;

	senderPort_ = SenderPort;

	receiverIp_ = ReceiverIp;
	/*if (ReceiverIp == "localhost" || ReceiverIp == "LocalHost" || ReceiverIp == "Localhost" || ReceiverIp == "LOCALHOST")
	{
		receiverIp_ = "127.0.0.1";
	}
	else
	{
		receiverIp_ = ReceiverIp;
	}*/
	receiverPort_ = ReceiverPort;

	filepath_ = Path::getFullFileSpec(FilePath);
	fileName_ = Path::getName(filepath_);

	/*std::ifstream myfile(filepath_, std::ifstream::ate | std::ifstream::binary);
	if (myfile.is_open())
	{
		contentLength_ = size_t(myfile.tellg());
	}
	else
	{
		cerr << "Cannot open file:" << filepath_ << endl;
	}*/

	contentLength_ = -1;
}
// Test Stub -----
#ifdef TEST_MESSAGE
int main(){
	Message msg;
	//Message msg("FileUpload", "localhost", 9080, "127.0.0.1", 9050, "../../../lj1018_1020_1022-HB-pnp-win64-en.exe");
	msg.createMsg("FileUpload", "localhost", 9080, "127.0.0.1", 9050, "../../../lj1018_1020_1022-HB-pnp-win64-en.exe");
	//msg.BuildBody("Sample", "Text")''
	//	msg.BuiltBody();
	//msg.getreceiverIp();
	//msg.getsenderIp();
	//std::string sss=msg.getPath();
	//std::cout << sss;
	//msg.getServerPort();
	//msg.getClientPort();
	//msg.setPort(8050);
	//msg.setIP("127.0.0.8");
	std::string temp = msg.ConstructHeader();//showHeader();
	std::cout << temp;

	Message inter;
	inter.InterpretHeader(temp);
	temp = inter.ConstructHeader();//showHeader();
	std::cout << temp;


		std::cout << "\n\n";
}
#endif