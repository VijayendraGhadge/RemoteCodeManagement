#ifndef FILEMANAGER_H
#define FILEMANAGER_H
/////////////////////////////////////////////////////////////////////////////
// FileManager.h - Manage the Efficient File Information Structure         //
// ver 1.0                                                                 //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Dell XPS L520, Core i7, Windows 7 SP3                      //
// Application: CSE 687 Project 1                                          //
// Author:      Vijayendra Ghadge, Syracuse University	                   //
//              (315) 728 8953, vvgahdge@syr.edu	                       //
/////////////////////////////////////////////////////////////////////////////

/*

Module Purpose:
===============
This module is responsible for managing information between FileSystem and our new Efficient DataStore .
This module handle all insertion ,deletion, modification of the Defined Datastore using STL Container operations and iterators.

Public Interface:
=================
Use STL containers and to operate using STL fuctions on the containers { Set, Map, List}
DataStore ds;
Filemanager fm(ds,path)				//Publicly available Constructor to Create Filemanager Object by stealing Datastores iterators.
String pattern;
fm.addPattern(pattern);				//Function to puch back patterns to vector of patterns
fm.search();						//Search recuresively at path 
fm.find(const string& path);		//Search recuresively at path
fm.nonrecursivefind(const string& path);  //Search non-recuresively at path
vector<string> found;					
found=textsearch(string searchtext);			//Find search-text inside files of datastore 

Build Process:
==============
Required files
- DataStore.cpp, DataStore.h,FileSystem.h, FileSystem.cpp,FileManager.h, FileManager.cpp

Build commands:
==============
- devenv Project1-Soln.sln /rebuild debug

Maintenance History:
====================
ver 1.0 : 2/13/2015

*/
#include "../Peer/FileSystem.h"
#include "../DataStore/DataStore.h"
#include <string>
using namespace::std;

class FileManager
{
public:
	using iterator = DataStore::iterator; //FileManager steals datastore's iterator
	using patterns = vector <string>;

	FileManager(const string& path, DataStore& ds) :path_(path), store_(ds)// accept path and datastore
	{	
		patterns_.push_back("*.*");
	}

	void addPattern(const string& patt);
	void search();
	void find(const string& path);
	void nonrecursivefind(const string& path);
	vector<string> textsearch(const string &text);

private:
	string path_;
	DataStore& store_;
	patterns patterns_;
};

#endif