/////////////////////////////////////////////////////////////////////////////
// FileManager.cpp - Manage the efficient File Information Structure       //
// ver 1.0                                                                 //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Dell XPS L520, Core i7, Windows 7 SP3                      //
// Application: CSE 687 Project 1                                          //
// Author:      Vijayendra Ghadge, Syracuse University	                   //
//              (315) 728 8953, vvgahdge@syr.edu	                       //
/////////////////////////////////////////////////////////////////////////////
#include "FileManager.h"
#include <string>
#include <iostream>
using namespace::std;


//.................Add Patterns to vector patterns...................
void FileManager::addPattern(const string& patt)
{
	if (patterns_.size() == 1 && patterns_[0] == "*.*")
	{
		patterns_.pop_back();
		patterns_.push_back(patt);
	}
	else
	{
		patterns_.push_back(patt); //handle for *.cpp , *.cpp
	}
}
//................Call to recure function at gibven path Recursively...................
void FileManager::search()
{
	find(path_);
}

//................Search at path Non-Recursively to get the files at that path..................
void FileManager::nonrecursivefind(const string& path)
{
	store_.save(path);
	for (auto patt : patterns_)
	{
		vector <string> files = FileSystem::Directory::getFiles(path, patt);
		for (auto f : files)
		{
			store_.save(f, path);
		}
	}
}

//...............Recurse function to search path Recursively to get files at path and also inside the directories inside of the path...................
void FileManager::find(const string& path)
{
	store_.save(path);
	for (auto patt : patterns_)											//store only files of interested type
	{
		vector <string> files = FileSystem::Directory::getFiles(path,patt);			
		for (auto f : files)
		{
			store_.save(f, path);											//Store files at path
		}
	}
		vector<string> dir = FileSystem::Directory::getDirectories(path);	
		for (auto d : dir)
		{
			if (d == "." || d == "..")
			{
				continue;
			}
			else
			{
				string current_path = path + "\\" + d;				//Enter one level down and recurse the function
				find(current_path);
			}

		}
}
//................Search text inside file of created catalogue....................
vector<string>FileManager::textsearch(const string& text)
{
	vector<string> files_found;
	for (iterator it = store_.begin(); it != store_.end(); ++it)
	{
		for (auto se : it->second)
		{
			string search_path = se->c_str();
			search_path = search_path + "\\" + it->first;				//for all paths of each file in Catalogue 

			FileSystem::File text_file(search_path);
			text_file.open(FileSystem::File::in);					//Open The file

			if (text_file.isGood())
			{
				std::string search_text = text_file.readAll(true);		//readall the file content 
				if (search_text.find(text) != string::npos)				//if search text is found in file 
				{
					files_found.push_back(search_path);						//store path and file name to vector for printing later.
				}
			}
			text_file.close();
		}
	
	}
	return files_found;
}

#ifdef TEST_FILEMANAGER
int main()
{
	cout << "\nTesting Filemanager";
	DataStore ds;
	string path = ".";
	const string pattern="*.*";
	FileManager fm(path,ds); 
	fm.addPattern(pattern);
	fm.search();
	fm.nonrecursivefind(path);
	string text="vijayendra";
	fm.textsearch(text);

	return 0;
}
#endif