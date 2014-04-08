#include "File.h"
#include "Folder.h"
#include "FileSystemMember.h"
#include <ctime>
#include <iostream>
using namespace std;

Folder::Folder() : FileSystemMember()
{
	nestedFiles = NULL;
	nestedFolders = NULL;
	folderCount = 0;
	folderCapacity = 0;
	fileCount = 0;
	fileCapacity = 0;
}

Folder::~Folder()
{
	deleteFolder();
}

Folder::Folder(const Folder& f) : FileSystemMember(f)
{
	copyFolder(f);
}

Folder& Folder::operator=(const Folder& f)
{
	if(this != &f)
	{
		FileSystemMember::operator=(f);
		deleteFolder();
		copyFolder(f);
	}
	return *this;
}

Folder::Folder(Folder* parentFolder, const char* name, const char* creatorName)
	: FileSystemMember(parentFolder, name, creatorName)
{
	nestedFiles = new File*[2];
	for(int i = 0; i < 2; i++)
	{
		nestedFiles[i] = new File();
	}
	nestedFolders = new Folder*[2];
	for(int i = 0; i < 2; i++)
	{
		nestedFolders[i] = new Folder();
	}
	folderCount = 0;
	folderCapacity = 2;
	fileCount = 0;
	fileCapacity = 2;
	if(parentFolder != NULL)
	{
		parentFolder->addFolder(*this, false);
	}
}

void Folder::addFile(File& f, bool copy)
{
	int position = containsFile(f);
	if(position != -1)
	{
		cout<<"A file with the name '"<<f.getName()<<"' already exists in the current folder.\n";
		system("pause");
	}
	else
	{
		if(fileCount == fileCapacity)
		{
			increaseFileCapacity();
		}
		if(copy)
		{
			*nestedFiles[fileCount] = f;
			nestedFiles[fileCount]->parentFolder = this;
		}
		else
		{
			f.parentFolder = this;
			nestedFiles[fileCount] = &f;
		}
		fileCount++;
	}
}

bool Folder::removeFilePointer(File& f)
{
	bool removed = false;
	int position = containsFile(f);
	if(position == -1)
	{
		cout<<"The file '"<<f.getName()<<"' you are trying to delete does not exist in the current location.\n";
		system("pause");
	}
	else
	{
		if(fileCount == fileCapacity / 4)
		{
			decreaseFileCapacity();
		}
		for(int i = position; i < fileCount - 1; i++)
		{
			nestedFiles[i] = nestedFiles[i + 1];
		}
		fileCount--;
		removed = true;
	}
	return removed;
}

void Folder::removeFile(File& f)
{
	if(removeFilePointer(f))
	{
		delete &f;
	}
}

void Folder::increaseFileCapacity()
{
	File** copyFileArray = new File*[fileCapacity];
	for(int i = 0; i < fileCount; i++)
	{
		copyFileArray[i] = nestedFiles[i];
	}
	delete[] nestedFiles;
	nestedFiles = new File*[fileCapacity * 2];
	fileCapacity *= 2;
	for(int i = 0; i < fileCount; i++)
	{
		nestedFiles[i] = copyFileArray[i];
	}
}

void Folder::decreaseFileCapacity()
{
	File** copyFileArray = new File*[fileCapacity];
	for(int i = 0; i < fileCount; i++)
	{
		copyFileArray[i] = nestedFiles[i];
	}
	delete[] nestedFiles;
	nestedFiles = new File*[fileCapacity / 2];
	fileCapacity /= 2;
	for(int i = 0; i < fileCount; i++)
	{
		nestedFiles[i] = copyFileArray[i];
	}
}

File** Folder::getNestedFiles() const
{
	return nestedFiles;
}

Folder** Folder::getNestedFolders() const
{
	return nestedFolders;
}

void Folder::print() const
{
	if(isEmpty())
	{
		cout<<"Folder is empty.\n";
	}
	else
	{
		int counter = 1;
		if(folderCount > 0)
		{
			cout<<"Folders: \n";
			for(int i = 0; i < folderCount; i++)
			{
				cout<<counter<<": ";
				cout<<nestedFolders[i]->getName()<<endl;
				counter++;
			}
		}
		if(fileCount > 0)
		{
			cout<<"\nFiles: \n";
			for(int i = 0; i < fileCount; i++)
			{
				cout<<counter<<": ";
				cout<<nestedFiles[i]->getName()<<endl;
				counter++;
			}
		}
	}
}

bool Folder::isEmpty() const
{
	bool empty = (fileCount == 0 && folderCount == 0);
	return empty;
}

int Folder::containsFile(const File& f) const
{
	int contains = -1;
	for(int i = 0; i < fileCount; i++)
	{
		if(!strcmp(nestedFiles[i]->getName(), f.getName()))
		{
			contains = i;
			break;
		}
	}
	return contains;
}

int Folder::containsFolder(const Folder& f) const
{
	int contains = -1;
	for(int i = 0; i < folderCount; i++)
	{
		if(!strcmp(nestedFolders[i]->getName(), f.getName()))
		{
			contains = i;
			break;
		}
	}
	return contains;
}

void Folder::addFolder(Folder& f, bool copy)
{
	int position = containsFolder(f);
	if(position != -1)
	{
		cout<<"The folder '"<<f.getName()<<"' already exists in the current folder.";
		system("pause");
	}
	else
	{
		if(folderCount == folderCapacity)
		{
			increaseFolderCapacity();
		}
		if(copy)
		{
			*nestedFolders[folderCount] = f;
			nestedFolders[folderCount]->parentFolder = this;
		}
		else
		{
			f.parentFolder = this;
			nestedFolders[folderCount] = &f;
		}
		folderCount++;
	}
}

bool Folder::removeFolderPointer(Folder& f)
{
	bool removed = false;
	int position = containsFolder(f);
	if(position == -1)
	{
		cout<<"The folder '"<<f.getName()<<"' you are trying to delete does not exist in the current location.\n";
		system("pause");
	}
	else
	{
		if(folderCount == folderCapacity / 4)
		{
			decreaseFolderCapacity();
		}
		for(int i = position; i < folderCount - 1; i++)
		{
			nestedFolders[i] = nestedFolders[i + 1];
		}
		folderCount--;
		removed = true;
	}
	return removed;
}

void Folder::removeFolder(Folder& f)
{
	if(removeFolderPointer(f))
	{
		f.deleteFolder();
	}
}

void Folder::increaseFolderCapacity()
{
	Folder** copyFolderArray = new Folder*[folderCapacity];
	for(int i = 0; i < folderCount; i++)
	{
		copyFolderArray[i] = nestedFolders[i];
	}
	delete[] nestedFolders;
	nestedFolders = new Folder*[folderCapacity * 2];
	folderCapacity *= 2;
	for(int i = 0; i < folderCount; i++)
	{
		nestedFolders[i] = copyFolderArray[i];
	}
}

void Folder::decreaseFolderCapacity()
{
	Folder** copyFolderArray = new Folder*[folderCapacity];
	for(int i = 0; i < folderCount; i++)
	{
		copyFolderArray[i] = nestedFolders[i];
	}
	delete[] nestedFolders;
	nestedFolders = new Folder*[folderCapacity / 2];
	folderCapacity /= 2;
	for(int i = 0; i < folderCount; i++)
	{
		nestedFolders[i] = copyFolderArray[i];
	}
}

void Folder::copyFolder(const Folder& cpyFolder)
{
	fileCount = cpyFolder.fileCount;
	fileCapacity = cpyFolder.fileCapacity;
	folderCount = cpyFolder.folderCount;
	folderCapacity = cpyFolder.folderCapacity;
	updateTimeOfCreation();
	updateTimeOfLastEdit();
	nestedFiles = new File*[fileCapacity];
	for(int i = 0; i < fileCapacity; i++)
	{
		nestedFiles[i] = new File();			//allocating memory
	}
	for(int i = 0; i < fileCount; i++)
	{
		*nestedFiles[i] = *cpyFolder.nestedFiles[i];
	}
	nestedFolders = new Folder*[folderCapacity];
	for(int i = 0; i < folderCapacity; i++)
	{
		nestedFolders[i] = new Folder();			//allocating memory
	}
	for(int i = 0; i < folderCount; i++)
	{
		nestedFolders[i]->copyDetails(*cpyFolder.nestedFolders[i]);
		nestedFolders[i]->setParentFolder(this);
		nestedFolders[i]->copyFolder(*cpyFolder.nestedFolders[i]);
	}
}

void Folder::deleteFolder()
{
	if(isEmpty())
	{
		delete[] nestedFolders;
		delete[] nestedFiles;
		fileCount = 0;
		fileCapacity = 0;
		folderCount = 0;
		folderCapacity = 0;
	}
	else
	{
		for(int i = 0; i < folderCount; i++)
		{
			delete nestedFolders[i];
		}
		for(int i = 0; i < fileCount; i++)
		{
			delete nestedFiles[i];
		}
		delete[] nestedFiles;
		delete[] nestedFolders;
	}
}

int Folder::getFileCount() const
{
	return fileCount;
}

int Folder::getFolderCount() const
{
	return folderCount;
}

void Folder::printInfo() const
{
	cout<<"-------------------";
	cout<<"Printing info for '"<<getName()<<"' :\n";
	cout<<"Date and time of creation: "<<getTimeOfCreation()<<endl;
	cout<<"Date and time of last edit: "<<getTimeOfLastEdit()<<endl;
	cout<<"Name of creator: "<<getCreatorName()<<endl;
	cout<<"Number of nested folders: "<<folderCount<<endl<<endl;
	cout<<"Number of nested files: "<<fileCount<<endl<<endl;
}