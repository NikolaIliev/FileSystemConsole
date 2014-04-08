#ifndef FILE_H
#define FILE_H
#include "FileSystemMember.h"
#include "Folder.h"

class File : public FileSystemMember
{
public:
	File();
	~File();
	File(const File&);
	File& operator=(const File&);
	//Name, Creator name, Parent folder, Data
	File(Folder* parent, const char* data, const char* name, const char* creatorName);
	void setData(const char*);
	const char* getData() const;
	void printInfo() const;
private:
	char* data; //this represents the data of the current file. Basically all files are *.txt(data is always text)
	void deleteFile();	//this is used in the destructor and operator= for effieciency(no copy-paste)
	void copyFile(const File&); //this is used in the copy constructor and operator= for effieciency(no copy-paste)
};

#endif