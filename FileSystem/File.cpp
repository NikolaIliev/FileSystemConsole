#include "File.h"
#include <cstring>
#include <iostream>
using namespace std;

File::File() : FileSystemMember()
{
	data = new char[8];
	strcpy_s(data, 8, "No info");
}

File::~File()
{
	deleteFile();
}

File::File(const File& f) : FileSystemMember(f)
{
	copyFile(f);
}

File& File::operator=(const File& f)
{
	if(this != &f)
	{
		FileSystemMember::operator=(f);
		deleteFile();
		copyFile(f);
	}
	return *this;
}

File::File(Folder* parentFolder, const char* data, const char* name, const char* creatorName) 
	: FileSystemMember(parentFolder, name, creatorName)
{
	
	this->data = new char[strlen(data) + 1];
	strcpy_s(this->data, strlen(data) + 1, data);
	if(parentFolder != NULL)
	{
		parentFolder->addFile(*this, false);
	}
}

void File::deleteFile()
{
	delete[] data;
}

void File::copyFile(const File& f)
{
	int dataLength = strlen(f.data);
	data = new char[dataLength + 1];
	strcpy_s(data, dataLength + 1, f.data);
}

void File::setData(const char* newData)
{
	if(newData != NULL)
	{
		delete[] data;
		int newDataLength = strlen(newData);
		data = new char[newDataLength + 1];
		strcpy_s(data, newDataLength + 1, newData);
		updateTimeOfLastEdit();
	}
}

const char* File::getData() const
{
	return data;
}

void File::printInfo() const
{
	cout<<"-------------------";
	cout<<"Printing info for '"<<getName()<<"' :\n";
	cout<<"Date and time of creation: "<<getTimeOfCreation()<<endl;
	cout<<"Date and time of last edit: "<<getTimeOfLastEdit()<<endl;
	cout<<"Name of creator: "<<getCreatorName()<<endl;
}