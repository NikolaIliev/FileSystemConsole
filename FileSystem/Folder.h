#ifndef FOLDER_H
#define FOLDER_H
#include "FileSystemMember.h"

class File;

class Folder : public FileSystemMember
{
public:
	Folder();
	~Folder();
	Folder(const Folder&);
	Folder& operator=(const Folder&);

	//Parent Folder, name, creator name
	Folder(Folder*, const char*, const char*);
	File** getNestedFiles() const;
	Folder** getNestedFolders() const;
	int getFileCount() const;
	int getFolderCount() const;
	void addFile(File&, bool copy = true);
	bool removeFilePointer(File&);	//nestedFiles is an array of pointers to files. This is used when removing a file
									//as the first step in the process -- to delete the pointer and then rearrange the remaining
									//pointers properly
	void removeFile(File&);			//this actually removes the file from the heap
	int containsFile(const File&) const;
	void addFolder(Folder&, bool copy = true);
	bool removeFolderPointer(Folder&);	//same as removeFilePointer
	void removeFolder(Folder&);
	int containsFolder(const Folder&) const;	
	bool isEmpty() const;
	void print() const;
	void printInfo() const;	//prints the FileSystemMember data-members -- name, creator name, time of creation, time of last edit
private:
	Folder** nestedFolders;	//each folder can have a random amount of nested folders and files in it..
	File** nestedFiles;		//
	int folderCapacity;		//these 4 variables are used to track the above arrays' capacity and actual Files/Folders in them
	int folderCount;		//meaning that nestedFolders could have space for 2 folders in it, but only have 1 actual folder 
	int fileCapacity;		//nested inside -- 
	int fileCount;
	void deleteFolder();			//used in destructor and operator=
	void increaseFileCapacity();	//the arrays nestedFolders and nestedFiles start off with a capacity of 2
	void decreaseFileCapacity();	//every time you fill up the array, it doubles in size
	void increaseFolderCapacity();	//every time the array starts getting empty, it halves in size
	void decreaseFolderCapacity();
	void copyFolder(const Folder&);	//used in copy constructor and operator=
	friend class ConsoleUI;
};


#endif