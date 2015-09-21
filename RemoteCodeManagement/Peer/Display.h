#ifndef DISPLAY_H
#define DISPLAY_H
/////////////////////////////////////////////////////////////////////////////
// Display.h - Provide Fuctions to Display to Console					   //
// ver 1.0                                                                 //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Dell XPS L520, Core i7, Windows 7 SP3                      //
// Application: CSE 687 Project 2                                          //
// Author:      Vijayendra Ghadge, Syracuse University	                   //
//              (315) 728 8953, vvgahdge@syr.edu	                       //
/////////////////////////////////////////////////////////////////////////////

/*

Module Purpose:
===============
This module is responsible for displaying  outputs ,write output string,display strings and messages to console.

Public Interface:
=================
Display d;
d.dispReq(100);  //display requirement number string
d.dispstr("foo"); ///display string
Build Process:
==============
Required files
- Sockets.h,Sockets.cpp, Display.h, Display.cpp;

Build commands:
==============
- devenv Display.sln /rebuild debug

Maintenance History:
====================
ver 1.0 : 3/24/2015

*/
#include "./Sockets.h"
#include<iostream>

class Display
{
public:
	void dispReq(int n);
	void dispstr(std::string x);
private:
};
#endif