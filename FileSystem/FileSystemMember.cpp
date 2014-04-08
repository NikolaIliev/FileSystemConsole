#include "FileSystemMember.h"
#include <cstring>
#include <ctime>
#include <iostream>
using namespace std;
FileSystemMember::FileSystemMember()
{
	name = new char[8];
	creatorName = new char[8];
	parentFolder = NULL;
	timeOfCreation = new char[8];
	timeOfLastEdit = new char[8];
	strcpy_s(name, 8, "No info");
	strcpy_s(creatorName, 8, "No info");
	strcpy_s(timeOfCreation, 8, "No info");
	strcpy_s(timeOfLastEdit, 8, "No info");
}

FileSystemMember::~FileSystemMember()
{
	deleteDetails();
}

FileSystemMember::FileSystemMember(const FileSystemMember& ffsd)
{
	copyDetails(ffsd);
}

FileSystemMember& FileSystemMember::operator=(const FileSystemMember& ffsd)
{
	if(this != &ffsd)
	{
		deleteDetails();
		copyDetails(ffsd);
	}
	return *this;
}

FileSystemMember::FileSystemMember(Folder* parentFolder, const char* name, const char* creatorName)
{
	this->name = new char[strlen(name) + 1];
	this->creatorName = new char[strlen(creatorName) + 1];
	strcpy_s(this->name, strlen(name) + 1, name);
	strcpy_s(this->creatorName, strlen(creatorName) + 1, creatorName);	//names
	this->parentFolder = parentFolder;									//parent folder

	time_t result = time(NULL);											
	timeOfCreation = new char[27];
	timeOfLastEdit = new char[27];		
	strcpy_s(timeOfCreation, 27, ctime(&result));
	strcpy_s(timeOfLastEdit, 27, ctime(&result));						//times
}

//private
void FileSystemMember::deleteDetails()
{
	delete[] name;
	delete[] creatorName;
	parentFolder = NULL;
	delete[] timeOfCreation;
	delete[] timeOfLastEdit;
}

void FileSystemMember::copyDetails(const FileSystemMember& ffsd)
{
	int lengthOfName = strlen(ffsd.name);
	int lengthOfCreatorName = strlen(ffsd.creatorName);
	name = new char[lengthOfName + 1];
	creatorName = new char[lengthOfCreatorName + 1];
	timeOfCreation = new char[27];
	timeOfLastEdit = new char[27];
	strcpy_s(name, lengthOfName + 1, ffsd.name);
	strcpy_s(creatorName, lengthOfCreatorName + 1, ffsd.creatorName);
	strcpy_s(timeOfCreation, 27, ffsd.timeOfCreation);
	strcpy_s(timeOfLastEdit, 27, ffsd.timeOfLastEdit);
	parentFolder = ffsd.parentFolder;
}

void FileSystemMember::setName(const char* n)
{
	if(n != NULL)
	{
		delete[] name;
		int newNameLength = strlen(n);
		name = new char[newNameLength + 1];
		strcpy_s(name, newNameLength + 1, n);
	}
	else
	{
		name = NULL;
	}
	updateTimeOfLastEdit();
}

const char* FileSystemMember::getName() const
{
	return name;
}

void FileSystemMember::setCreatorName(const char* n)
{
	if(n != NULL)
	{
		delete[] creatorName;
		int newNameLength = strlen(n);
		creatorName = new char[newNameLength + 1];
		strcpy_s(creatorName, newNameLength + 1, n);
	}
	else
	{
		creatorName = NULL;
	}
}

const char* FileSystemMember::getCreatorName() const
{
	return creatorName;
}

void FileSystemMember::setParentFolder(Folder* pF)
{
	parentFolder = pF;
}

Folder* FileSystemMember::getParentFolder() const
{
	return parentFolder;
}

void FileSystemMember::updateTimeOfCreation()
{
	time_t result = time(NULL);				
	delete[] timeOfCreation;
	timeOfCreation = new char[27];	
	strcpy_s(timeOfCreation, 27, ctime(&result));
}

void FileSystemMember::updateTimeOfLastEdit()
{
	time_t result = time(NULL);				
	delete[] timeOfLastEdit;
	timeOfLastEdit = new char[27];	
	strcpy_s(timeOfLastEdit, 27, ctime(&result));
}

const char* FileSystemMember::getTimeOfCreation() const
{
	return timeOfCreation;
}

const char* FileSystemMember::getTimeOfLastEdit() const
{
	return timeOfLastEdit;
}