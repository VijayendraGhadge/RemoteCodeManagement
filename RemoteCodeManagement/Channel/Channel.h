#ifndef MOCKCHANNEL_H
#define MOCKCHANNEL_H

///////////////////////////////////////////////////////////////////
// Channel.h - Emulating a channel between WPF and C++ code      //
// ver 1.0                                                       //
// Language:    Visual C++, Visual Studio 2013                   //
// Platform:    Dell XPS L520, Core i7, Windows 7 SP3            //
// Application: CSE 687 Project 3                                //
// Author:      Vijayendra Ghadge, Syracuse University	         //
//              (315) 728 8953, vvgahdge@syr.edu	             //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package is used for CSE687 - Object Oriented Design Project 4  Just contains pure virtual functions to construct the bridge between managed C++ and native c++
* Project #4 - Remote Code Management to Create Mock Channel Between WPF and C++
Public Interface:
=================
Refer Inherited from class.
* Required Files:
* ---------------
*  Channel.h, Channel.cpp
*  Sockets.h, Sockets.cpp, AppHelpers.h, AppHelpers.cpp,BlockingQueue.h, BlockingQueue.cpp,FileSystem.h,FileSystem.cpp
*  Message.h, Message.cpp,Sockets.h,Sockets.cpp,WindowsHelpers.h,WindowsHelpers.cpp
*
* Build Process:
* --------------
*   devenv Receiver.sln /debug rebuild
*/
#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include <string>
struct IChannel
{
public:
  virtual void begin(Message M) = 0;
  virtual void end() = 0;
  virtual void postMessage(Message msg)=0;
  virtual Message fetch_ack_Client_Side() = 0;
};
extern "C" {
	struct ObjectFactory
	{
		DLL_DECL IChannel* createMockChannel();
	};
}
#endif


