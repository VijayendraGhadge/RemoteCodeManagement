/////////////////////////////////////////////////////////////////////////////
// Display.cpp - Provide Fuctions to Display on Console					   //
// ver 1.0															       //
// Language:    Visual C++, Visual Studio 2013						       //
// Platform:    Dell XPS L520, Core i7, Windows 7 SP3					   //
// Application: CSE 687 Project 4										   //
// Author:      Vijayendra Ghadge, Syracuse University	                   //
//              (315) 728 8953, vvgahdge@syr.edu	                       //
/////////////////////////////////////////////////////////////////////////////

#include"Display.h"


//----< display requirements >----------------------------------------
void Display::dispReq(int n)
{
	std::cout << "\n********************* Demostarting Requirement # " << n << " ****************************\n";
}
//----< display passed string >----------------------------------------
void Display::dispstr(std::string s)
{
	std::cout << "\n................................................................................\n";
	std::cout << s << "\n";
}

#ifdef TEST_DISPLAY
int main()
{
	std::cout << "\nTesting Display";
	Display d;
	d.dispReq(100);
	d.dispstr("foo");
	return 0;
}
#endif