///////////////////////////////////////////////////////////////////
// Receiver.cpp -A Class which encapsulates Socket operations    //
//              into easy Usage									 //
// ver 1.0                                                       //
// Language:    Visual C++, Visual Studio 2013                   //
// Platform:    Dell XPS L520, Core i7, Windows 7 SP3            //
// Application: CSE 687 Project 3 ,4                             //
// Author:      Vijayendra Ghadge, Syracuse University	         //
//              (315) 728 8953, vvgahdge@syr.edu	             //
///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Receiver.cpp - Receiving Strings, Buffers							   //
/*
*  Package Operations:
*  -------------------
*  This package provides fuctionality for Sockets-4.5:
*  - passing byte buffers (File)
*  - passing strings  (Header)
*
*  Required Files:
*  ---------------
*  Receiver.cpp, Receiver.h
*  Sockets.h, Sockets.cpp, AppHelpers.h, AppHelpers.cpp,BlockingQueue.h, BlockingQueue.cpp,FileSystem.h,FileSystem.cpp
*  Message.h, Message.cpp,Sockets.h,Sockets.cpp,WindowsHelpers.h,WindowsHelpers.cpp
*
*  Build Command:
*  --------------
*  devenv Receiver.sln /rebuild debug
*/
#include "Receiver.h"

//----< Client Handler thread starts running this function >-----------------

void Receiver::operator()(Socket& socket_)
{
	while (true)
	{
		Directory::setCurrentDirectory(serverRootDir);
		// interpret test command
		std::string header = socket_.recvString();
		if (header.size() != 0)
		{
			std::string command = interpretCommand(header);

			if (command == "")
				continue;
			//Verbose::show("command: " + command);

			if (command == "FileUpload")
			{
				Message temp;
				temp.InterpretHeader(header);
				if (fileUpload(socket_, temp))
				{
					Message m;
					m.InterpretHeader(header);
					LastPacket(m);					
					//Verbose::show("----fileupload test passed", always);			
				}
				else
					Verbose::show("----File Upload Chunks failed", always);
				continue; // go back and get another command
			}

			Receive1(command, header);
			Receive2(command, header);
			Receive3(command, header);
		}			
		continue; // go back and get another command
		
	}
	Verbose::show("Receiver socket connection closing");
	socket_.shutDown();
	socket_.close();
	Verbose::show("Receiver thread terminating");
}

void Receiver::LastPacket(Message m)
{
	if (m.getContentLength() < 10240)
	{
		Message ms = m;
		ms.setCommand("FUS");
		ReceiveQ_.enQ(ms);
	}
}

void Receiver::Receive1(std::string command, std::string header)
{
	if (command == "File Upload Successful")
	{
		std::string head = header;
		Message temp;
		temp.InterpretHeader(head);
		std::cout << "\nFile Upload Successful***************************";
		ReceiveQ_.enQ(temp);
	}
	if (command == "ACK")
	{
		std::string head = header;
		Message temp;
		temp.InterpretHeader(head);
		std::cout << "\nFile Download Successful***************************";
		ReceiveQ_.enQ(temp);
	}
	if (command == "FileDownload")
	{
		std::string head = header;
		Message temp;
		temp.InterpretHeader(head);
		ReceiveQ_.enQ(temp);
	}
	if (command == "ReceivedDirList")
	{
		std::string head = header;
		Message temp;
		temp.InterpretHeader(head);
		ReceiveQ_.enQ(temp);
	}

}

void Receiver::Receive2(std::string command, std::string header)
{
	if (command == "GetDirList")
	{
		std::string head = header;
		Message temp;
		temp.InterpretHeader(header);
		temp.setFileName(getfilestructure());
		ReceiveQ_.enQ(temp);	
	}
	if (command == "GetFiles")
	{
		std::string head = header;
		Message temp;
		temp.InterpretHeader(head);
		std::size_t pos = temp.getFileName().find("?");      // position of ? in str
		std::string pattern = temp.getFileName().substr(pos + 1); \
		std::string path = temp.getFileName();
		path.resize(pos);
		std::string result = getfiles(path, pattern);
		temp.setFileName(result);
		ReceiveQ_.enQ(temp);
		
	}
	if (command == "ReceivedFiles")
	{
		std::string head = header;
		Message temp;
		temp.InterpretHeader(head);
		ReceiveQ_.enQ(temp);
		
	}
}

void Receiver::Receive3(std::string command, std::string header)
{
	if (command == "GetDir")
	{
		std::string head = header;
		Message temp;
		temp.InterpretHeader(head);
		std::size_t pos = temp.getFileName().find("?");      // position of ":" in str
		std::string pattern = temp.getFileName().substr(pos + 1); \
		std::string path = temp.getFileName();
		path.resize(pos);
		std::string result = getdirs(path);
		temp.setFileName(result);
		ReceiveQ_.enQ(temp);
		
	}
	if (command == "ReceivedDir")
	{
		std::string head = header;
		Message temp;
		temp.InterpretHeader(head);
		ReceiveQ_.enQ(temp);
		
	}
	if (command == "SearchString")
	{
		std::string head = header;
		Message temp;
		temp.InterpretHeader(head);
		std::size_t pos = temp.getFileName().find("?");      // position of ":" in str
		std::string str = temp.getFileName().substr(pos + 1); \
		std::string path = temp.getFileName();
		path.resize(pos);
		std::string result = searchString(path, str);
		temp.setFileName(result);
		ReceiveQ_.enQ(temp);
	}
	if (command == "ReceivedString")
	{
		std::string head = header;
		Message temp;
		temp.InterpretHeader(head);
		ReceiveQ_.enQ(temp);
	}
}

//........Function to Interpret Command from reaceived string haeder.......
std::string Receiver::interpretCommand(std::string head)
{
	Message header;
	std::string cmd;
	if (header.InterpretHeader(head))
	{
	//	Msg_ = header;
		cmd = header.getCommand();
	}
	if (cmd == "")
	{
		std::cout << "Empty Command\n";
	}
	return cmd;
}
//...........Closes Socket Connection...... And Enque End Receiver Message
void Receiver::closeConnection()
{
	Message X;
	X.setCommand("End Receiver");
	ReceiveQ_.enQ(X);
	Verbose::show("\n  Server calling Listen shutdown\n");
	SL_->close(); //SL_->shutDown();
}

// ........... Downloads File At Receiver......(header ...File block.....header.....FIle block........)
bool Receiver::fileUpload(Socket& socket_,Message &m)
{
	//title("Trying to Upload File on server");

	const int BufLen = 10480;//Msg_.getContentLength();
	Socket::byte buffer[BufLen];
	bool ok;

	int filesize = m.getContentLength();

	while (true)
	{
		if (socket_.bytesWaiting() == 0)
			break;
		ok = socket_.recv(filesize, buffer);
		if (socket_ == INVALID_SOCKET || !ok)
			return false;

		std::ofstream out(m.getFileName(), std::ios::binary | std::ios::app | std::ios::ate);


		for (size_t i = 0; i < size_t(filesize); ++i)
		{
			out << buffer[i];
			//std::cout << buffer[i]<<"\n";
		}

		out.close();

		return true;
	}
	return true;
}
//.........................Feteches dir structure from server....
std::string Receiver::getfilestructure()
{
	std::string currentdir, paths,slash;
	currentdir = serverRootDir;
	
	slash = "\\";
	DataStore ds;
	FileManager fm(currentdir, ds);
	fm.find(currentdir);
	
	std::set <std::string> x;

	for (auto fs : ds)
	{
		for (auto l : fs.second)
		{
			x.insert(l->c_str()+slash+fs.first);
		}
	}

	for (auto i : x)
	{
		paths = paths + i + "?";
	}

		paths = parsepath(paths, serverRootDir);
		return paths;

}

//.........................Feteches all files from server....
std::string Receiver::getAllfiles()
{
	std::string currentdir, paths, slash;
	currentdir = serverRootDir;

	slash = "\\";
	DataStore ds;
	FileManager fm(currentdir, ds);
	fm.find(currentdir);

	std::set <std::string> x;

	for (auto fs : ds)
	{
		for (auto l : fs.second)
		{
			x.insert(l->c_str() + slash + fs.first);
		}
	}

	for (auto i : x)
	{
		paths = paths + i + "?";
	}

	paths = parsepath(paths, serverRootDir);
	return paths;

}
//.........................Feteches files from server....
std::string Receiver::getfiles(std::string path,std::string patt)
{	std::string currentdir, paths, slash;
	if (path == ""){		currentdir = serverRootDir;	}
	else{	Directory::setCurrentDirectory(Path::getFullFileSpec(path));	currentdir = Directory::getCurrentDirectory();	}
	if (patt == "")patt = "*.*";	
	patt = patt + " ";
	int startLoc = 0;
	int size = 0;
	bool flag = false;
	std::vector < std::string > vec;
	for (size_t i = 0; i < patt.length(); ++i)
	{	if (patt[i] != ' ')
		{			flag = false;
					continue;
		}
		else
		{
			flag = true;
			size = i - startLoc;
			vec.push_back(patt.substr(startLoc, size));
			startLoc = i + 1;
		}
	}
	slash = "\\";
	DataStore ds;
	FileManager fm(currentdir, ds);
	for (auto a : vec)
	{
		fm.addPattern(a);
	}
	fm.find(currentdir);
	std::set <std::string> x;
	for (auto fs : ds)
	{
		for (auto l : fs.second)
		{
			x.insert(l->c_str() + slash + fs.first);
		}
	}
	for (auto i : x)
	{
		paths = paths + i + "?";
	}
	paths = parsepath(paths, serverRootDir);
	return paths;
}
//.........................Feteches dir from server....
std::string Receiver::getdirs(std::string path)
{
	std::string currentdir, paths, slash;
	std::vector <std::string> vec;
	if (path.find("?") != std::string::npos) {
		currentdir = serverRootDir;
	}

	if (path == "")
	{
		currentdir = serverRootDir;
	}
	else
	{
		Directory::setCurrentDirectory(Path::getFullFileSpec(path));
		currentdir = Directory::getCurrentDirectory();

	}

	slash = "\\";
	DataStore ds;
	FileManager fm(currentdir, ds);
	fm.find(currentdir);
	std::set <std::string> x;

	for (auto fs : ds)
	{
		for (auto l : fs.second)
		{
			x.insert(l->c_str());
		}
	}

	for (auto i: x)
	{
		paths = paths + i + "?";
	}

	paths = parsepath(paths, serverRootDir);
	return paths;
}

//.........................Search text in files from server....
std::string Receiver::searchString(std::string path,std::string searchText)
{
	std::string currentdir, paths, slash;
	std::vector <std::string> vec;
	
	if (path == "")
	{
		currentdir = serverRootDir;
	}
	else
	{
		Directory::setCurrentDirectory(Path::getFullFileSpec(path));
		currentdir = Directory::getCurrentDirectory();

	}

	slash = "\\";
	DataStore ds;
	FileManager fm(currentdir, ds);
	fm.find(currentdir);
	
	std::vector <std::string> result = fm.textsearch(searchText);
	
	for (auto l : result)
	{
		paths = paths + l + "?";
	}
	paths = parsepath(paths, serverRootDir);
	return paths;
}
//...................................clip of server absolute path to relative root dir path ......
std::string Receiver::parsepath(std::string str,std::string substr )
{
	size_t pos;
	while ((pos = str.find(substr)) != string::npos)
	{
		string s3 = str.substr(0, pos);
		if ((pos + substr.length() + 1) < str.length())
			s3 += str.substr(pos + substr.length() + 1);
		str = s3;
	}

	return str;
}

//----< demonstration >------------------------------------------------------
#ifdef TEST_RECEIVER
int main()
{
	title("Testing Socket Server", '=');

	try
	{
		Verbose v(true);
		Receiver cp;
		Message M("FileUpload", 9070, "localhost", 9080, "../zoo.JPG");
		//	Verbose::show("press key to exit: ", always);
		cp.start(cp,M,Socket::IP4);
		title("Server Uploading File...Be Patient...Connection will close in time");
		title("To Force Connection close Press Any Key");
		std::cin.get();
		cp.closeConnection();
	

	}
	catch (std::exception& ex)
	{
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}
#endif
