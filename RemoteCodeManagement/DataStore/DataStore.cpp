/////////////////////////////////////////////////////////////////////////////
//DataStore.cpp -Provide Efficient Structure to hold FileSystem in volatile//
//				 memory and perform functionality to operate on that data  //
// ver 1.0                                                                 //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Dell XPS L520, Core i7, Windows 7 SP3                      //
// Application: CSE 687 Project 1                                          //
// Author:      Vijayendra Ghadge, Syracuse University	                   //
//              (315) 728 8953, vvgahdge@syr.edu	                       //
/////////////////////////////////////////////////////////////////////////////

#include "DataStore.h"   //"" for local headers
#include <iostream>    // system file in .....

#ifdef TEST_DATASTORE

int main()

{
	std::cout << "\n Testing DataStore";
	DataStore ds;
	ds.save("one");
	ds.save("two");
	ds.save("three");
	DataStore::iterator iter = ds.begin();

	for (auto item : ds)
	{
		std::cout << "\n  " << item.first;
	}
	std::cout << "\n\n";

	return 0;
}

#endif

//.................Save File Path to Datastore Set...................
void DataStore::save(const string& filespec) {
	paths.insert(filespec);
}

//.................Save Filename to Map key and path inside Path set and chain the pathiter to respective ListofIteratores to Datastore map value ...................
void DataStore::save(const string& filename, const string& path) {
	//save(path);
	PathIter pathiter;
	ListOfIters listofiters;
	iterator iter;
	int flag = 0;
	
		pathiter = paths.find(path);						//find paths existing in set

		if (store.size() == 0)								//if map is empty insert file name and path iter in listofiters in key and value fields respectively.
		{
			listofiters.push_back(pathiter);
			store.insert({ filename, listofiters });
		}
		else
		{
			for (iter = store.begin(); iter != store.end(); ++iter)   
			{
				flag = 0;
				if (filename == iter->first)					//Search Filename if existiing in Map 
				{
					iter->second.push_back(pathiter);			//pushback to listofiter the path iter of path
					flag = 1;
					break;
				}
			}
			if (flag==0)											//else create new entry in Map
				{
					listofiters.push_back(pathiter);
					store.insert({ filename, listofiters });
				}			
		}
}
