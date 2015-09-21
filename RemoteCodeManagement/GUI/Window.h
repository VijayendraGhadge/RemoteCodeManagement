#ifndef WINDOW_H
#define WINDOW_H
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
*  Package Operations:
*  -------------------
*  This package demonstrates how to build a C++\CLI WPF application.  It
*  provides one class, WPFCppCliDemo, derived from System::Windows::Window
*  that is compiled with the /clr option to run in the Common Language
*  Runtime, and another class MockChannel written in native C++ and compiled
*  as a DLL with no Common Language Runtime support.
*
*  The window class hosts, in its window, a tab control with three views, two
*  of which are provided with functionality that you may need for Project #4.
*  It loads the DLL holding MockChannel.  MockChannel hosts a send queue, a
*  receive queue, and a C++11 thread that reads from the send queue and writes
*  the deQ'd message to the receive queue.
*
*  The Client can post a message to the MockChannel's send queue.  It hosts
*  a receive thread that reads the receive queue and dispatches any message
*  read to a ListBox in the Client's FileList tab.  So this Demo simulates
*  operations you will need to execute for Project #4.
*
*  Required Files:
*  ---------------
*  Window.h, Window.cpp, MochChannel.h, MochChannel.cpp,
*  Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp
*
*  Build Command:
*  --------------
*  devenv CppCli-WPF-App.sln
*  - this builds C++\CLI client application and native mock channel DLL
*
*  Maintenance History:
*  --------------------
*  ver 2.0 : 15 Apr 2015
*  - completed message passing demo with moch channel
*  - added FileBrowserDialog to show files in a selected directory
*  ver 1.0 : 13 Apr 2015
*  - incomplete demo with GUI but not connected to mock channel
*/
/*
* Create C++/CLI Console Application
* Right-click project > Properties > Common Language Runtime Support > /clr
* Right-click project > Add > References
*   add references to :
*     System
*     System.Windows.Presentation
*     WindowsBase
*     PresentatioCore
*     PresentationFramework
*/


using namespace System;
using namespace System::Text;
using namespace System::Windows;
using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   // TextBlock formatting
using namespace System::Windows::Controls;                // TabControl
using namespace System::Windows::Controls::Primitives;    // StatusBar
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;
#include "../Channel/Channel.h"
#include <iostream>


namespace CppCliWindows
{
	ref class WPFCppCliDemo : Window
	{
		//Peer refrence //Channel
		IChannel* pChann_;
		ObjectFactory* pObjFact;

		//Controlds for Search View
		Grid^ hSearchViewGrid = gcnew  Grid();
		Label^ hPatternLabel = gcnew Label();
		TextBox^ hPatternTextBox = gcnew TextBox();
		StackPanel^ hSearchViewStackPanel0 = gcnew StackPanel();
		StackPanel^ hSearchViewStackPanel1 = gcnew StackPanel();
		StackPanel^ hSearchViewStackPanel2 = gcnew StackPanel();
		StackPanel^ hSearchViewStackPanel3 = gcnew StackPanel();
		StackPanel^ hSearchViewStackPanel4 = gcnew StackPanel();
		Label^ hStringLabel = gcnew Label();
		Label^ hServerLabel = gcnew Label();
		TextBox^ hStringTextBox = gcnew TextBox();
		Label^ hServerPathsLabel = gcnew Label();
		Label^ hResultLabel = gcnew Label();
		ListBox^ hPathListBox = gcnew ListBox();
		ListBox^ hResultListBox = gcnew ListBox();
		ListBox^ hAvailableServerListBox = gcnew ListBox();
		Button^ hFileSearchButton = gcnew Button();
		Button^ hStringSearchButton = gcnew Button();

		// Controls for Window
		DockPanel^ hDockPanel = gcnew DockPanel();      // support docking statusbar at bottom
		Grid^ hGrid = gcnew Grid();
		TabControl^ hTabControl = gcnew TabControl();
		TabItem^ hFileListTab = gcnew TabItem();
		TabItem^ hConnectTab = gcnew TabItem();
		TabItem^ hUploadDownloadTab = gcnew TabItem();
		TabItem^ hSearchTab = gcnew TabItem();
		StatusBar^ hStatusBar = gcnew StatusBar();
		StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
		TextBlock^ hStatus = gcnew TextBlock();


		// Controls for Connect View
		Grid^ hConnectViewGrid= gcnew Grid();
		Label^ hClientPortLabel = gcnew Label();
		TextBox^ hClientPortTextBox = gcnew TextBox();
		Label^ hServerPortLabel = gcnew Label();
		TextBox^ hServerPortTextBox = gcnew TextBox();
		Label^ hServerIPLabel = gcnew Label();
		TextBox^ hServerIPTextBox = gcnew TextBox();
		Button^ hConnectButton = gcnew Button();
		Button^ hAddServerButton = gcnew Button();
		Button^ hClearAllButton = gcnew Button();
		ListBox^ hAllServersListBox = gcnew ListBox();
		StackPanel^ hConnectStackPanel1 = gcnew StackPanel();
		StackPanel^ hConnectStackPanel2 = gcnew StackPanel();
		StackPanel^ hConnectStackPanel3 = gcnew StackPanel();

		//controls for upload download view 

		Grid^ hUploadDownloadViewGrid = gcnew Grid();
		StackPanel^ hUDStackPanel1 = gcnew StackPanel();
		ListBox^ hClientDirList = gcnew ListBox();
		ListBox^ hServerDirList = gcnew ListBox();
		Button^ hGetClientDirs = gcnew Button();
		Button^ hUploadFile = gcnew Button();
		StackPanel^ hUDStackPanel2 = gcnew StackPanel();
		Button^ hDownloadFile = gcnew Button();
		Button^ hGetServerDirs = gcnew Button();
		StackPanel^ hUDStackPanel3 = gcnew StackPanel();
		ListBox^ hRemoteServerListBox = gcnew ListBox();
		StackPanel^ hUDStackPanel4 = gcnew StackPanel();
		Label^ hRemoteServerLabel = gcnew Label();

		// Controls for SendMessage View
		

		Grid^ hSendMessageGrid = gcnew Grid();
		/*Button^ hSendButton = gcnew Button();
		Button^ hClearButton = gcnew Button();*/
		TextBlock^ hTextBlock1 = gcnew TextBlock();
		TextBox^ hTextBoxServerIP = gcnew TextBox();
		ScrollViewer^ hScrollViewer1 = gcnew ScrollViewer();
		StackPanel^ hStackPanel1 = gcnew StackPanel();

		//String^ msgText = "FileUpload localhost 9070 localhost 9080";

		// Controls for FileListView View

	
	public:
		WPFCppCliDemo();
		~WPFCppCliDemo();
	
	private:
		std::string toStdString(String^ pStr);
		String^ toSystemString(std::string& str);
		void setTextBlockProperties();
		void setTextBoxProperties();
	//	void setButtonsProperties();

		
		void createPeer(Object^ sender, RoutedEventArgs^ args);
		void clear(Object^ sender, RoutedEventArgs^ args);
		void OnLoaded(Object^ sender, RoutedEventArgs^ args);
		void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);
		void UploadFile(Object^ sender, RoutedEventArgs^ args);
		void DownloadFile(Object^ sender, RoutedEventArgs^ args);
		void getClientDirs(Object^ sender, RoutedEventArgs^ args);
		void getServerDirs(Object^ sender, RoutedEventArgs^ args);
		void PopulateList(Object^ sender, RoutedEventArgs^ args);
		void SearchFiles(Object^ sender, RoutedEventArgs^ args);
		void SearchStrings(Object^ sender, RoutedEventArgs^ args);
		void ClearAllLists(Object^ sender, RoutedEventArgs^ args);



		void receivedDirectoryList(String^);
		void receivedFilesList(String^);
		void receivedDirList(String^);
		void receivedStringSearchList(String^);


		Forms::FolderBrowserDialog^ hFolderBrowserDialog = gcnew Forms::FolderBrowserDialog();
		Button^ hFolderBrowseButton = gcnew Button();
		// receive thread

		Thread^ recvThread;


		void setUpStatusBar();
		void setUpTabControl();
		//void setUpSendMessageView();
		void setUpConnectionView();
		void setUpUploadDownloadView();
		void setUpSearchView();
		void setupSearchPatternProperties();
		void setupSearchStringProperties();
		void setupSearchLabelProperties();
		void setupSearchListBoxProperties();
		void setupSearchButtonProperties();
		void setUpConnectViewClientProperties();
		void setUpConnectViewServerProperties();
		void setUpConnectViewButtonProperties();
		void setUpConnectViewAllServersListBoxProperties();
		void setupUploadDownloadViewProperties();

		void addText(String^ msg);
		void getMessage();
	};
}


#endif