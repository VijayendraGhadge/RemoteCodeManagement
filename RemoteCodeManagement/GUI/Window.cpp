/////////////////////////////////////////////////////////////////////////////
// Window.cpp - C++\CLI implementation of WPF Application                  //
// ver 1.0															       //
// Language:    Visual C++, Visual Studio 2013						       //
// Platform:    Dell XPS L520, Core i7, Windows 7 SP3					   //
// Application: CSE 687 Project 4										   //
// Author:      Vijayendra Ghadge, Syracuse University	                   //
//              (315) 728 8953, vvgahdge@syr.edu	                       //
/////////////////////////////////////////////////////////////////////////////
/*
*  To run as a Windows Application:
*  - Set Project Properties > Linker > System > Subsystem to Windows
*  - Comment out int main(...) at bottom
*  - Uncomment int _stdcall WinMain() at bottom
*  To run as a Console Application:
*  - Set Project Properties > Linker > System > Subsytem to Console
*  - Uncomment int main(...) at bottom
*  - Comment out int _stdcall WinMain() at bottom
*/

#include "../Peer/Message.h"
#include "Window.h"


using namespace CppCliWindows;
using namespace System;


WPFCppCliDemo::WPFCppCliDemo()
{

	//enable and disable buttons
	hConnectButton->IsEnabled = true;
	hAddServerButton->IsEnabled = false;
	hClearAllButton->IsEnabled = false;

	hFileSearchButton->IsEnabled = false;
	hStringSearchButton->IsEnabled = false;
	hUploadFile->IsEnabled = false;
	hGetServerDirs->IsEnabled = false;
	hDownloadFile->IsEnabled = false;

	this->Loaded +=
		gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
	this->Closing +=
		gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
	hConnectButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::createPeer); 
	hGetClientDirs->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::getClientDirs);
	hGetServerDirs->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::getServerDirs);
	hUploadFile->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::UploadFile);
	hAddServerButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::PopulateList);
	hFileSearchButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::SearchFiles);
	hDownloadFile->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::DownloadFile);
	hStringSearchButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::SearchStrings);
	hClearAllButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::ClearAllLists);
	// set Window properties

	this->Title = "WPF Remote Code Management";
	this->Width = 800;
	this->Height = 600;

	// attach dock panel to Window

	this->Content = hDockPanel;
	hDockPanel->Children->Add(hStatusBar);
	hDockPanel->SetDock(hStatusBar, Dock::Bottom);
	hDockPanel->Children->Add(hGrid);

	// setup Window controls and views

	setUpTabControl();
	setUpStatusBar();
	//setUpSendMessageView();
	setUpConnectionView();
	setUpUploadDownloadView();
	setUpSearchView();
}

WPFCppCliDemo::~WPFCppCliDemo()
{
	 delete pChann_;
}

void WPFCppCliDemo::createPeer(Object^ sender, RoutedEventArgs^ args)
{
	Message M;
	String^ cport = hClientPortTextBox->Text;
	std::string cp = toStdString(cport);
	int cliPort = atoi(cp.c_str());
	bool faulty = false;
	if (cliPort < 1024 || cliPort>49151)
	{
		faulty = true;
		hStatus->Text = "Enter Valid Port Number";
	}

	if (faulty == false)
	{
		M.setSenderPort(cliPort);
	}
	String^ rIP = hServerIPTextBox->Text;
	std::string recIP = toStdString(rIP);
	M.setReceiverIP (recIP);
	String^ rPort = hServerPortTextBox->Text;
	std::string recPort = toStdString(rPort);
	int SerPort = atoi(recPort.c_str());
	M.setReceiverPort(SerPort);

	String^ uri = rIP + ":" + rPort;
	
	pObjFact = new ObjectFactory;
	pChann_ = pObjFact->createMockChannel();				// set up channel
	pChann_->begin(M);
	recvThread = gcnew Thread(gcnew ThreadStart(this, &WPFCppCliDemo::getMessage));
	recvThread->Start();
	hRemoteServerListBox->Items->Add(uri);
	hAllServersListBox->Items->Add(uri);
	hAvailableServerListBox->Items->Add(uri);
	hConnectButton->IsEnabled = false;
	hAddServerButton->IsEnabled = true;
	hClearAllButton->IsEnabled = true;
	hFileSearchButton->IsEnabled = true;
	hStringSearchButton->IsEnabled = true;
	hUploadFile->IsEnabled = true;
	hGetServerDirs->IsEnabled = true;
	hDownloadFile->IsEnabled = true;

}

void WPFCppCliDemo::PopulateList(Object^ sender, RoutedEventArgs^ args)
{
	Message M;
	String^ cport = hClientPortTextBox->Text;
	std::string cp = toStdString(cport);
	int cliPort = atoi(cp.c_str());
	bool faulty = false;
	if (cliPort < 1024 || cliPort>49151)
	{
		faulty = true;
		hStatus->Text = "Enter Valid Port Number";
	}

	if (faulty == false)
	{
		M.setSenderPort(cliPort);
	}

	String^ rIP = hServerIPTextBox->Text;
	std::string recIP = toStdString(rIP);
	M.setReceiverIP(recIP);
	String^ rPort = hServerPortTextBox->Text;
	std::string recPort = toStdString(rPort);
	int SerPort = atoi(recPort.c_str());
	M.setReceiverPort(SerPort);
	String^ uri = rIP + ":" + rPort;
	hRemoteServerListBox->Items->Add(uri);
	hAllServersListBox->Items->Add(uri);
	hAvailableServerListBox->Items->Add(uri);
}

void WPFCppCliDemo::ClearAllLists(Object^ sender, RoutedEventArgs^ args)
{

	hRemoteServerListBox->Items->Clear();
	hAllServersListBox->Items->Clear();
	hAvailableServerListBox->Items->Clear();
	hConnectButton->IsEnabled = true;
	hAddServerButton->IsEnabled = false;
	hClearAllButton->IsEnabled = false;
}

void WPFCppCliDemo::setUpStatusBar()
{
	hStatusBar->Items->Add(hStatusBarItem);
	hStatus->Text = "very important messages will appear here";
	//status->FontWeight = FontWeights::Bold;
	hStatusBarItem->Content = hStatus;
	hStatusBar->Padding = Thickness(10, 2, 10, 2);
}

void WPFCppCliDemo::setUpTabControl()
{
	hGrid->Children->Add(hTabControl);
	hConnectTab->Header = "Connect";
	hSearchTab->Header = "Search";
	hUploadDownloadTab->Header = "Upload / Download";
	hTabControl->Items->Add(hConnectTab);
	hTabControl->Items->Add(hSearchTab);
	hTabControl->Items->Add(hUploadDownloadTab);
}


void WPFCppCliDemo::setTextBoxProperties()
{
	
}


void WPFCppCliDemo::setTextBlockProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hSendMessageGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hTextBlock1;
	hTextBlock1->Padding = Thickness(15);
	hTextBlock1->Text = "";
	hTextBlock1->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBlock1->FontWeight = FontWeights::Bold;
	hTextBlock1->FontSize = 16;
	hScrollViewer1->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewer1->Content = hBorder1;
	hSendMessageGrid->SetRow(hScrollViewer1, 0);
	hSendMessageGrid->Children->Add(hScrollViewer1);
}

std::string WPFCppCliDemo::toStdString(String^ pStr)
{
	std::string dst;
	for (int i = 0; i < pStr->Length; ++i)
		dst += (char)pStr[i];
	return dst;
}

String^ WPFCppCliDemo::toSystemString(std::string& str)
{
	StringBuilder^ pStr = gcnew StringBuilder();
	for (size_t i = 0; i < str.size(); ++i)
		pStr->Append((Char)str[i]);
	return pStr->ToString();
}

void WPFCppCliDemo::addText(String^ msg)
{
	hStatus->Text += msg + "\n";
}
	

void WPFCppCliDemo::getMessage()
{	while (true)							// recvThread runs this function
	{
		std::cout << "\n  receive thread calling getMessage()";
		Message msg = pChann_->fetch_ack_Client_Side();
		std::string Command = msg.getCommand();
		if (Command == "FTS")
		{	msg.setCommand("File Transfer Successful !");
			String^ sMsg = toSystemString(msg.getCommand());
			array<String^>^ args = gcnew array<String^>(1);
			args[0] = sMsg;
			Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::addText);
			Dispatcher->Invoke(act, args);  // must call addText on main UI thread
			continue;
		}
		if (Command == "ReceivedDirList")
		{	String^ sMsg = toSystemString(msg.ConstructHeader());
			array<String^>^ args = gcnew array<String^>(1);
			args[0] = sMsg;
			Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::receivedDirectoryList);
			Dispatcher->Invoke(act, args);  // must call addText on main UI thread
			continue;
		}
		if (Command == "ReceivedFiles")
		{	String^ sMsg = toSystemString(msg.ConstructHeader());
			array<String^>^ args = gcnew array<String^>(1);
			args[0] = sMsg;
			Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::receivedFilesList);
			Dispatcher->Invoke(act, args);  // must call addText on main UI thread
			continue;
		}
		if (Command == "ReceivedDir")
		{	String^ sMsg = toSystemString(msg.ConstructHeader());
			array<String^>^ args = gcnew array<String^>(1);
			args[0] = sMsg;
			Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::receivedDirList);
			Dispatcher->Invoke(act, args);  // must call addText on main UI thread
			continue;
		}
		if (Command == "ReceivedString")
		{	String^ sMsg = toSystemString(msg.ConstructHeader());
			array<String^>^ args = gcnew array<String^>(1);
			args[0] = sMsg;
			Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::receivedStringSearchList);
			Dispatcher->Invoke(act, args);  // must call addText on main UI thread
			continue;
		}
	}
}

void WPFCppCliDemo::clear(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  cleared message text");
	hStatus->Text = "Cleared message";
	hTextBlock1->Text = "";
}

void WPFCppCliDemo::setUpUploadDownloadView()
{
	Console::Write("\n  setting up Upload/Download view");
	hUploadDownloadViewGrid->Margin = Thickness(40);
	hUploadDownloadTab->Content = hUploadDownloadViewGrid;
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	//hRow1Def->Height = GridLength(75);
	hUploadDownloadViewGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hUDStackPanel1;
	//TextBlock^ hSpacer1 = gcnew TextBlock();
	hStatusBar->Padding = Thickness(20, 4, 20, 4);
	//hSpacer1->Width = 90;
	hRemoteServerListBox->Width = 110;
	hClientDirList->Width = 290;
	hClientDirList->SelectionMode = SelectionMode::Multiple;
	hUDStackPanel1->Children->Add(hClientDirList);
	hUDStackPanel1->Children->Add(hRemoteServerListBox);
	hUDStackPanel1->Children->Add(hServerDirList);
	hServerDirList->Width = 290;
	hServerDirList->SelectionMode = SelectionMode::Multiple;
	hServerDirList->HorizontalAlignment = System::Windows::HorizontalAlignment::Right;
	hUDStackPanel1->Orientation = Orientation::Horizontal;
	hUDStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hUploadDownloadViewGrid->SetRow(hBorder1, 0);
	hUploadDownloadViewGrid->Children->Add(hBorder1);
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hUploadDownloadViewGrid->RowDefinitions->Add(hRow2Def);
	hGetClientDirs->Content = "Get Directory List on Client";
	hGetClientDirs->Height = 25;
	hGetClientDirs->Width = 155;
	hGetClientDirs->BorderThickness = Thickness(2);
	hGetClientDirs->BorderBrush = Brushes::Black;	
	hUDStackPanel4->Orientation = Orientation::Vertical;
	hUDStackPanel4->Children->Add(hRemoteServerLabel);	
	hRemoteServerLabel->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	
	setupUploadDownloadViewProperties();
}

void WPFCppCliDemo::setupUploadDownloadViewProperties()
{

	hRemoteServerLabel->Content = "        Server list";
	hRemoteServerLabel->Width = 120;
	hRemoteServerLabel->Height = 120;
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 20;
	hUDStackPanel2->Children->Add(hGetClientDirs);
	hUDStackPanel2->Children->Add(hSpacer2);
	hUDStackPanel2->Children->Add(hUploadFile);
	hUploadFile->Height = 25;
	hUploadFile->Width = 120;
	hUploadFile->Content = "Upload File";
	hUDStackPanel2->Orientation = Orientation::Vertical;
	hUDStackPanel2->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hGetServerDirs->Content = "Get Directory List on Server";
	hGetServerDirs->Height = 25;
	hGetServerDirs->Width = 155;
	hGetServerDirs->BorderThickness = Thickness(2);
	hGetServerDirs->BorderBrush = Brushes::Black;
	TextBlock^ hSpacer3 = gcnew TextBlock();
	hSpacer2->Width = 20;
	hUDStackPanel3->Children->Add(hGetServerDirs);
	hUDStackPanel3->Children->Add(hSpacer3);
	hUDStackPanel3->Children->Add(hDownloadFile);
	hDownloadFile->Height = 25;
	hDownloadFile->Width = 120;
	hDownloadFile->Content = "Download File";
	hUDStackPanel3->Orientation = Orientation::Vertical;
	hUDStackPanel3->HorizontalAlignment = System::Windows::HorizontalAlignment::Right;
	hUploadDownloadViewGrid->SetRow(hUDStackPanel2, 1);
	hUploadDownloadViewGrid->SetRow(hUDStackPanel4, 1);
	hUploadDownloadViewGrid->SetRow(hUDStackPanel3, 1);
	hUploadDownloadViewGrid->Children->Add(hUDStackPanel2);
	hUploadDownloadViewGrid->Children->Add(hUDStackPanel3);
	hUploadDownloadViewGrid->Children->Add(hUDStackPanel4);
	hFolderBrowserDialog->ShowNewFolderButton = false;
	hFolderBrowserDialog->SelectedPath = System::IO::Directory::GetCurrentDirectory();
	hStatus->Text = "";

}


void WPFCppCliDemo::DownloadFile(Object^ sender, RoutedEventArgs^ args)
{	bool flag = true;
	if (hRemoteServerListBox->SelectedIndex == -1){		hStatus->Text = " Select Remote Server !";	}
	else{	flag = false;	hStatus->Text = "Downloading Files !\n"; }
	if (!flag)
	{	if (hServerDirList->SelectedItems->Count > 0)
			for each(String^ value in hServerDirList->SelectedItems)
			{
			Message M;
			String^ URI;
			for each(String^ val in hRemoteServerListBox->SelectedItems)URI = val;
			std::string serverIP = toStdString(URI);
			std::size_t pos = serverIP.find(":");      // position of : in str
			std::string serverPort = serverIP.substr(pos + 1);
			serverIP.resize(pos);
			String^ cport = hClientPortTextBox->Text;
			std::string cp = toStdString(cport);
			int cliPort = atoi(cp.c_str());
			M.setSenderPort(cliPort);
			M.setReceiverIP(serverIP);
			int SerPort = atoi(serverPort.c_str());
			M.setReceiverPort(SerPort);
			M.setCommand("FileDownload");
			M.setFileName(toStdString(value));
			pChann_->postMessage(M);
			}
	}

}

void WPFCppCliDemo::UploadFile(Object^ sender, RoutedEventArgs^ args)
{
	bool flag = true;
	if (hRemoteServerListBox->SelectedIndex == -1)	{ hStatus->Text = " Select Remote Server !"; }
	else	{ flag = false; 		hStatus->Text = "Uploading Files !\n"; }
	if (!flag)
	{	if (hClientDirList->SelectedItems->Count > 0)
			for each(String^ value in hClientDirList->SelectedItems)
			{if (toStdString(value).substr(0, 2) == "<>")
				{
				Console::WriteLine(L"\n Directory  selected");
				hStatus->Text = "Select File to Upload ...  Or Multiple Files..... Cannot Upload Directories";
				flag = true;
				}
				if (!flag)
				{
				Message M;
				String^ URI;
				for each(String^ val in hRemoteServerListBox->SelectedItems)URI = val;
				std::string serverIP = toStdString(URI);
				std::size_t pos = serverIP.find(":");      // position of : in str
				std::string serverPort = serverIP.substr(pos + 1);
				serverIP.resize(pos);
				String^ cport = hClientPortTextBox->Text;
				std::string cp = toStdString(cport);
				int cliPort = atoi(cp.c_str());
				M.setSenderPort(cliPort);
				M.setReceiverIP(serverIP);
				int SerPort = atoi(serverPort.c_str());
				M.setReceiverPort(SerPort);
				M.setCommand("FileUpload");
				std::string temp;
				temp = Path::getName(toStdString(value));
				M.setFileName(temp);
				temp =toStdString(value);
				M.setFilePath(temp);				
				pChann_->postMessage(M);
				}
			}
	}
}


void WPFCppCliDemo::getServerDirs(Object^ sender, RoutedEventArgs^ args)
{
	bool flag = true;
	if(hRemoteServerListBox->SelectedIndex == -1)
	{
		hStatus->Text = " Select Remote Server !";
	}
	else
	{
		flag = false;
	}
	if (!flag)
	{
		hServerDirList->Items->Clear();
		Message M;
		String^ URI;
		for each(String^ val in hRemoteServerListBox->SelectedItems)URI = val;
		std::string serverIP = toStdString(URI);
		std::size_t pos = serverIP.find(":");      // position of : in str
		std::string serverPort = serverIP.substr(pos + 1);
		serverIP.resize(pos);
		String^ cport = hClientPortTextBox->Text;
		std::string cp = toStdString(cport);
		int cliPort = atoi(cp.c_str());
		bool faulty = false;
		if (cliPort < 1024 || cliPort>49151)
		{
			faulty = true;
			hStatus->Text = "Enter Valid Client Port Number";
		}
		if (faulty == false)
		{
			M.setSenderPort(cliPort);
		}
		M.setReceiverIP(serverIP);
		int SerPort = atoi(serverPort.c_str());
		M.setReceiverPort(SerPort);

		M.setCommand("GetDirList");
		pChann_->postMessage(M);
	}
}

void WPFCppCliDemo::receivedDirectoryList(String^ str)
{
	hStatus->Text = "Received Directory List";
	Message M;
	M.InterpretHeader(toStdString(str));
	hServerDirList->Items->Clear();
	std::string list=M.getFileName();
	int startLoc = 0;
	int size = 0;
	bool flag = false;
	std::vector < std::string > vec;
	for (size_t i = 0; i <= list.length(); i++)
	{
		if (list[i] != '?')
		{
			continue;
			flag = false;
		}
		else
		{
			flag = true;
			size = i - startLoc;
			vec.push_back(list.substr(startLoc, size));
			startLoc = i + 1;
		}
	}
	int  q = vec.size();
	if (q!=0)
	{
		
		for (auto a : vec)
		{
			std::string temp = a;
			hServerDirList->Items->Add(toSystemString(temp));
		}

	}
}
void WPFCppCliDemo::getClientDirs(Object^ sender, RoutedEventArgs^ args)
{
	//std::cout << "\n  Browsing for folder";
	hClientDirList->Items->Clear();
	System::Windows::Forms::DialogResult X;
	X= hFolderBrowserDialog->ShowDialog();
	if (X == System::Windows::Forms::DialogResult::OK)
	{
		String^ path = hFolderBrowserDialog->SelectedPath;
		//std::cout << "\n  opening folder \"" << toStdString(path) << "\"";
		array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.*");
		for (int i = 0; i < files->Length; ++i)
			hClientDirList->Items->Add(files[i]);
		array<String^>^ dirs = System::IO::Directory::GetDirectories(path);
		for (int i = 0; i < dirs->Length; ++i)
			hClientDirList->Items->Add(L"<> " + dirs[i]);
	}
}

void WPFCppCliDemo::setUpConnectViewClientProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(100);
	hConnectViewGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->Width = 160;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hClientPortLabel->Content = "Client Listener Port Number : ";
	hBorder1->Child = hClientPortLabel;
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 50;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hClientPortTextBox->Text = "9070";
	hBorder2->Child = hClientPortTextBox;
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 150;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hConnectButton->Content = "Establish Connection";
	hBorder3->Child = hConnectButton;
	hConnectStackPanel1->Children->Add(hBorder1);
	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 20;
	hConnectStackPanel1->Children->Add(hSpacer1);
	hConnectStackPanel1->Children->Add(hBorder2);
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 20;
	hConnectStackPanel1->Children->Add(hSpacer2);
	hConnectStackPanel1->Children->Add(hBorder3);
	hConnectStackPanel1->Orientation = Orientation::Horizontal;
	hConnectStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hConnectViewGrid->SetRow(hConnectStackPanel1, 0);
	hConnectViewGrid->Children->Add(hConnectStackPanel1);
}

void WPFCppCliDemo::setUpConnectViewServerProperties()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hConnectViewGrid->RowDefinitions->Add(hRow2Def);
	//label, text box and button
	Border^ hBorder1 = gcnew Border();
	hBorder1->Width = 65;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hServerIPLabel->Content = "Server IP:";
	hBorder1->Child = hServerIPLabel;
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 100;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hServerIPTextBox->Text = "localhost";
	hBorder2->Child = hServerIPTextBox;
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 160;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hServerPortLabel->Content = "Server Listen Port Number:";
	hBorder3->Child = hServerPortLabel;
	Border^ hBorder4 = gcnew Border();
	hBorder4->Width = 50;
	hBorder4->Height = 30;
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hServerPortTextBox->Text = "9080";
	hBorder4->Child = hServerPortTextBox;
	hConnectStackPanel2->Children->Add(hBorder1);
	hConnectStackPanel2->Children->Add(hBorder2);
	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 10;
	hConnectStackPanel2->Children->Add(hSpacer1);
	hConnectStackPanel2->Children->Add(hBorder3);
	hConnectStackPanel2->Children->Add(hBorder4);
	hConnectStackPanel2->Orientation = Orientation::Horizontal;
	hConnectStackPanel2->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hConnectViewGrid->SetRow(hConnectStackPanel2, 1);
	hConnectViewGrid->Children->Add(hConnectStackPanel2);
}


void WPFCppCliDemo::setUpConnectViewButtonProperties()
{
	RowDefinition^ hRow5Def = gcnew RowDefinition();
	hRow5Def->Height = GridLength(75);
	hConnectViewGrid->RowDefinitions->Add(hRow5Def);
	//button and button

	Border^ hBorder1 = gcnew Border();
	hBorder1->Width = 120;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hAddServerButton->Content = "Add To Server List";
	hBorder1->Child = hAddServerButton;

	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hClearAllButton->Content = "Clear All Server Lists";
	hBorder2->Child = hClearAllButton;


	hConnectStackPanel3->Children->Add(hBorder1);
	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 10;
	hConnectStackPanel3->Children->Add(hSpacer1);
	hConnectStackPanel3->Children->Add(hBorder2);
	hConnectStackPanel3->Orientation = Orientation::Horizontal;
	hConnectStackPanel3->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hConnectViewGrid->SetRow(hConnectStackPanel3, 2);
	hConnectViewGrid->Children->Add(hConnectStackPanel3);
}
void WPFCppCliDemo::setUpConnectViewAllServersListBoxProperties()
{
	hConnectViewGrid->Margin = Thickness(20);
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	//hRow1Def->Height = GridLength(75);
	hConnectViewGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hAllServersListBox;
	hConnectViewGrid->SetRow(hBorder1, 3);
	hConnectViewGrid->Children->Add(hBorder1);
}

void WPFCppCliDemo::setUpConnectionView()
{
	Console::Write("\n  setting up Connection view");
	hConnectTab->Content = hConnectViewGrid;
	setUpConnectViewClientProperties();
	setUpConnectViewServerProperties();
	setUpConnectViewButtonProperties();
	setUpConnectViewAllServersListBoxProperties();
}

void WPFCppCliDemo::setupSearchPatternProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(75);
	hSearchViewGrid->RowDefinitions->Add(hRow1Def);
	//label and text box 
	Border^ hBorder1 = gcnew Border();
	hBorder1->Width = 150;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hPatternLabel->Content = "File Patterns Or File Name :";
	hBorder1->Child = hPatternLabel;

	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 150;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hPatternTextBox->Text = "*.*";
	hBorder2->Child = hPatternTextBox;

	hSearchViewStackPanel0->Children->Add(hAvailableServerListBox);
	hSearchViewStackPanel0->Children->Add(hServerLabel);
	hServerLabel->Width = 200;
	hServerLabel->Height = 30;
	hServerLabel->Content = "                    Server List";
	hAvailableServerListBox->Width = 200;
	hAvailableServerListBox->Height = 50;
	hSearchViewStackPanel0->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hSearchViewStackPanel1->Children->Add(hBorder1);	

	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 10;
	hSearchViewStackPanel1->Children->Add(hSpacer2);
	hSearchViewStackPanel1->Children->Add(hBorder2);
	hSearchViewStackPanel1->Orientation = Orientation::Horizontal;
	hSearchViewStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hSearchViewGrid->SetRow(hSearchViewStackPanel0, 0);
	hSearchViewGrid->Children->Add(hSearchViewStackPanel0);
	hSearchViewGrid->SetRow(hSearchViewStackPanel1, 0);
	hSearchViewGrid->Children->Add(hSearchViewStackPanel1);
}


void WPFCppCliDemo::setupSearchStringProperties()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hSearchViewGrid->RowDefinitions->Add(hRow2Def);
	//label and text box 

	Border^ hBorder1 = gcnew Border();
	hBorder1->Width = 120;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hStringLabel->Content = "Text string:";
	hBorder1->Child = hStringLabel;

	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hStringTextBox->Text = "";
	hBorder2->Child = hStringTextBox;

	hSearchViewStackPanel2->Children->Add(hBorder1);
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 10;
	hSearchViewStackPanel2->Children->Add(hSpacer2);
	hSearchViewStackPanel2->Children->Add(hBorder2);
	hSearchViewStackPanel2->Orientation = Orientation::Horizontal;
	hSearchViewStackPanel2->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hSearchViewGrid->SetRow(hSearchViewStackPanel2, 1);
	hSearchViewGrid->Children->Add(hSearchViewStackPanel2);
}
void WPFCppCliDemo::setupSearchLabelProperties()
{
	RowDefinition^ hRow5Def = gcnew RowDefinition();
	hRow5Def->Height = GridLength(75);
	hSearchViewGrid->RowDefinitions->Add(hRow5Def);
	//label and text box 

	Border^ hBorder1 = gcnew Border();
	hBorder1->Width = 120;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hServerPathsLabel->Content = "Server Paths";
	hBorder1->Child = hServerPathsLabel;

	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hResultLabel->Content = "Resultant Files";
	hBorder2->Child = hResultLabel;

	hSearchViewStackPanel3->Children->Add(hBorder1);
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 470;
	hSearchViewStackPanel3->Children->Add(hSpacer2);
	hSearchViewStackPanel3->Children->Add(hBorder2);
	hSearchViewStackPanel3->Orientation = Orientation::Horizontal;
	hSearchViewStackPanel3->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hSearchViewGrid->SetRow(hSearchViewStackPanel3, 2);
	hSearchViewGrid->Children->Add(hSearchViewStackPanel3);
}
void WPFCppCliDemo::setupSearchListBoxProperties()
{
	RowDefinition^ hRow3Def = gcnew RowDefinition();
	//hRow3Def->Height = GridLength(75);
	hSearchViewGrid->RowDefinitions->Add(hRow3Def);

	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Width = 360;
	hBorder1->Child = hPathListBox;
	hPathListBox->SelectionMode = SelectionMode::Multiple;
	hBorder1->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;

	Border^ hBorder2 = gcnew Border();
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Width = 360;
	hBorder2->Child = hResultListBox;
	hResultListBox->SelectionMode = SelectionMode::Multiple;
	hBorder2->HorizontalAlignment = System::Windows::HorizontalAlignment::Right;

	hSearchViewGrid->SetRow(hBorder1, 3);
	hSearchViewGrid->Children->Add(hBorder1);
	hSearchViewGrid->SetRow(hBorder2, 3);
	hSearchViewGrid->Children->Add(hBorder2);

}
void WPFCppCliDemo::setupSearchButtonProperties()
{
	RowDefinition^ hRow4Def = gcnew RowDefinition();
	hRow4Def->Height = GridLength(75);
	hSearchViewGrid->RowDefinitions->Add(hRow4Def);
	//button and button

	Border^ hBorder1 = gcnew Border();
	hBorder1->Width = 120;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hStringSearchButton->Content = "Text Search !";
	hBorder1->Child = hStringSearchButton;

	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hFileSearchButton->Content = "File Search !";
	hBorder2->Child = hFileSearchButton;

	hSearchViewStackPanel4->Children->Add(hBorder1);
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 10; 
	hSearchViewStackPanel4->Children->Add(hSpacer2);
	hSearchViewStackPanel4->Children->Add(hBorder2);
	hSearchViewStackPanel4->Orientation = Orientation::Horizontal;
	hSearchViewStackPanel4->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hSearchViewGrid->SetRow(hSearchViewStackPanel4, 4);
	hSearchViewGrid->Children->Add(hSearchViewStackPanel4);
}

void WPFCppCliDemo::setUpSearchView()
{
	Console::Write("\n  setting up Search view");
	hSearchViewGrid->Margin = Thickness(20);
	hSearchTab->Content = hSearchViewGrid;
	setupSearchPatternProperties();
	setupSearchStringProperties();
	setupSearchLabelProperties();
	setupSearchListBoxProperties();
	setupSearchButtonProperties();
}

void WPFCppCliDemo::SearchFiles(Object^ sender, RoutedEventArgs^ args)
{	bool flag = true;
	if (hAvailableServerListBox->SelectedIndex == -1){	hStatus->Text = " Select Remote Server !";	}
	else{ 	flag = false;		hStatus->Text = "Searching !\n";	}
	Message M;
	String^ URI;
		if (hPathListBox->SelectedItems->Count > 0 && flag)
		{
			for each(String^ value in hPathListBox->SelectedItems)
			{	for each(String^ val in hAvailableServerListBox->SelectedItems)URI = val;
				std::string serverIP = toStdString(URI);		std::size_t pos = serverIP.find(":");      std::string serverPort = serverIP.substr(pos + 1);
				serverIP.resize(pos);		String^ cport = hClientPortTextBox->Text;		std::string cp = toStdString(cport);	int cliPort = atoi(cp.c_str());
				M.setSenderPort(cliPort); M.setReceiverIP(serverIP);	int SerPort = atoi(serverPort.c_str()); 	M.setReceiverPort(SerPort);
				M.setCommand("GetFiles");
				M.setFileName(toStdString(value + "?" + hPatternTextBox->Text));
				pChann_->postMessage(M);
				M.setCommand("GetDir");
				pChann_->postMessage(M);
				flag = true;
			}
			hPathListBox->Items->Clear();
		}
		else
		{
			if (!flag)
			{
				hPathListBox->Items->Clear();
				for each(String^ val in hAvailableServerListBox->SelectedItems)URI = val;
				std::string serverIP = toStdString(URI);		std::size_t pos = serverIP.find(":");      std::string serverPort = serverIP.substr(pos + 1);
				serverIP.resize(pos);		String^ cport = hClientPortTextBox->Text;		std::string cp = toStdString(cport);	int cliPort = atoi(cp.c_str());
				M.setSenderPort(cliPort); M.setReceiverIP(serverIP);	int SerPort = atoi(serverPort.c_str()); 	M.setReceiverPort(SerPort);
				M.setCommand("GetFiles");
				M.setFileName(toStdString("?" + hPatternTextBox->Text));
				pChann_->postMessage(M);
				M.setCommand("GetDir");
				pChann_->postMessage(M);
				hPathListBox->Items->Clear();
			}
		}
}

void WPFCppCliDemo::receivedFilesList(String^ str)
{
	hStatus->Text = "Received File List !";
	Message M;
	M.InterpretHeader(toStdString(str));
	hResultListBox->Items->Clear();
	hServerDirList->Items->Clear();
	std::string list = M.getFileName();
	int startLoc = 0;
	int size = 0;
	bool flag = false;
	std::vector < std::string > vec;
	for (size_t i = 0; i <= list.length(); i++)
	{
		if (list[i] != '?')
		{
			continue;
			flag = false;
		}
		else
		{
			flag = true;
			size = i - startLoc;
			vec.push_back(list.substr(startLoc, size));
			startLoc = i + 1;
		}
	}
	int  q = vec.size();
	if (q != 0)
	{

		for (auto a : vec)
		{
			std::string temp = a;
			hResultListBox->Items->Add(toSystemString(temp));
			hServerDirList->Items->Add(toSystemString(temp));
		}

	}
}


void WPFCppCliDemo::receivedDirList(String^ str)
{
	hStatus->Text = "Received Directory List !";
	Message M;
	M.InterpretHeader(toStdString(str));
	hPathListBox->Items->Clear();
	std::string list = M.getFileName();
	int startLoc = 0;
	int size = 0;
	bool flag = false;
	std::vector < std::string > vec;
	for (size_t i = 0; i <= list.length(); i++)
	{
		if (list[i] != '?')
		{
			continue;
			flag = false;
		}
		else
		{
			flag = true;
			size = i - startLoc;
			vec.push_back(list.substr(startLoc, size));
			startLoc = i + 1;
		}
	}
	int  q = vec.size();
	if (q != 0)
	{

		for (auto a : vec)
		{
			std::string temp = a;
			hPathListBox->Items->Add(toSystemString(temp));
		}

	}
}

void WPFCppCliDemo::SearchStrings(Object^ sender, RoutedEventArgs^ args)
{	bool flag = true;
	if (hAvailableServerListBox->SelectedIndex == -1) 	{		hStatus->Text = " Select Remote Server !";	}
	else{		flag = false;		hStatus->Text = "Searching Strings !\n";	}
	Message M;
	String^ URI;
	if (!flag)
	{
		if (hPathListBox->SelectedItems->Count > 0)
			for each(String^ value in hPathListBox->SelectedItems)
			{	for each(String^ val in hAvailableServerListBox->SelectedItems)URI = val;
				std::string serverIP = toStdString(URI);		std::size_t pos = serverIP.find(":");      std::string serverPort = serverIP.substr(pos + 1);
				serverIP.resize(pos);		String^ cport = hClientPortTextBox->Text;		std::string cp = toStdString(cport);	int cliPort = atoi(cp.c_str());
				M.setSenderPort(cliPort); M.setReceiverIP(serverIP);	int SerPort = atoi(serverPort.c_str()); 	M.setReceiverPort(SerPort);
				M.setCommand("SearchString");
				M.setFileName(toStdString(value + "?" + hStringTextBox->Text));
				pChann_->postMessage(M);
				M.setCommand("GetDir");
				pChann_->postMessage(M);
				flag = true;
			}
		hPathListBox->Items->Clear();
	}
	if (!flag)
	{
		hPathListBox->Items->Clear();
		for each(String^ val in hAvailableServerListBox->SelectedItems)URI = val;
		std::string serverIP = toStdString(URI);		std::size_t pos = serverIP.find(":");      std::string serverPort = serverIP.substr(pos + 1);
		serverIP.resize(pos);		String^ cport = hClientPortTextBox->Text;		std::string cp = toStdString(cport);	int cliPort = atoi(cp.c_str());
		M.setSenderPort(cliPort); M.setReceiverIP(serverIP);	int SerPort = atoi(serverPort.c_str()); 	M.setReceiverPort(SerPort);
		M.setCommand("SearchString");
		M.setFileName(toStdString("?" + hStringTextBox->Text));
		pChann_->postMessage(M);
		M.setCommand("GetDir");
		pChann_->postMessage(M);
		hPathListBox->Items->Clear();
	}
}

void WPFCppCliDemo::receivedStringSearchList(String^ str)
{
	hStatus->Text = "Received Search Results !";
	Message M;
	M.InterpretHeader(toStdString(str));
	hResultListBox->Items->Clear();
	hServerDirList->Items->Clear();
	std::string list = M.getFileName();
	int startLoc = 0;
	int size = 0;
	bool flag = false;
	std::vector < std::string > vec;
	for (size_t i = 0; i <= list.length(); i++)
	{
		if (list[i] != '?')
		{
			continue;
			flag = false;
		}
		else
		{
			flag = true;
			size = i - startLoc;
			vec.push_back(list.substr(startLoc, size));
			startLoc = i + 1;
		}
	}
	int  q = vec.size();
	if (q != 0)
	{

		for (auto a : vec)
		{
			std::string temp = a;
			hResultListBox->Items->Add(toSystemString(temp));
			hServerDirList->Items->Add(toSystemString(temp));
		}

	}
}


void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  Window loaded");
}
void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
	Console::Write("\n  Window closing");
}

[STAThread]
//int _stdcall WinMain()
int main(array<System::String^>^ args)
{
	Console::WriteLine(L"\n Starting WPFCppCliDemo");

	Application^ app = gcnew Application();
	app->Run(gcnew WPFCppCliDemo());
	Console::WriteLine(L"\n\n");
}