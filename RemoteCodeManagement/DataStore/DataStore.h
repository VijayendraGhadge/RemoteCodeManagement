#ifndef DATASTORE_H
#define DATASTORE_H
/////////////////////////////////////////////////////////////////////////////
// DataStore.h - Provide Efficient Structure to hold FileSystem in volatile//
//				 memory and perform functionality to operate on that data  //
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
This module is responsible for creating an efficient Data Structure,
to store FileSystem Structure and operate on it.

Public Interface:
=================
Use STL containers and to operate using STL fuctions on the containers { Set, Map, List}
DataStore& store_; .store_.save(f, path); /////save (const string& filename,const string& path);
DataStore& store_; .store_.save(path); /////save (const string& filespec);
int file, dir; file = ds.number_of_files(); dir = ds.number_of_directories();

Build Process:
==============
Required files
- DataStore.cpp, DataStore.h

Build commands:
==============
- devenv Project1-Soln.sln /rebuild debug

Maintenance History:
====================
ver 1.0 : 2/13/2015

*/
#include <set>
#include <list>
#include <map>
#include <string>
using namespace::std;


class DataStore
{
public:
	using Path = string;
	using Paths = set < Path > ;
	using PathIter = Paths::iterator;
	using ListOfIters = list < PathIter > ;
	using File = string;
	using Store = map < File, ListOfIters > ;
	using iterator = Store::iterator;
	void save(const string& filespec);
	void save(const string& filename, const string& path);
	int number_of_files(){return store.size();}
	int number_of_directories(){return paths.size();}
	iterator begin()
	{
		return store.begin();
	}
	iterator end()
	{
		return store.end();
	}
private:
	Store store;
	Paths paths;
};
#endif
