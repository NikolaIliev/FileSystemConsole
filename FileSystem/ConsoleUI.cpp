#include "ConsoleUI.h"
#include "FileSystemMember.h"
#include "File.h"
#include "Folder.h"
#include <cstring>
#include <iostream>
using namespace std;

ConsoleUI::ConsoleUI()
{
	Folder* root = new Folder(NULL, "Root", "Nikola Iliev");
	rootFolder = root;
	currentFolder = root;
	clipboard = new Folder(NULL, "Clipboard", "Nikola Iliev");
	int pathLength = strlen(root->name);
	currentFileSystemPath = new char[pathLength + 3];
	numberOfFileSystemMembersInCurrentFolder = 0;
	strcpy_s(currentFileSystemPath, pathLength + 3, root->name);
	strcat_s(currentFileSystemPath, pathLength + 3, ":\\");
}

ConsoleUI::~ConsoleUI()
{
	delete rootFolder;
	delete clipboard;
	delete[] currentFileSystemPath;
}

ConsoleUI::ConsoleUI(const ConsoleUI& cUI)
{
	rootFolder = new Folder();
	*rootFolder = *cUI.rootFolder;
	currentFolder = new Folder();
	*currentFolder = *cUI.currentFolder;
	*clipboard = *cUI.clipboard;
	int copyPathLength = strlen(cUI.currentFileSystemPath);
	currentFileSystemPath = new char[copyPathLength + 1];
	strcpy_s(currentFileSystemPath, copyPathLength + 1, cUI.currentFileSystemPath);
}

ConsoleUI& ConsoleUI::operator=(const ConsoleUI& cUI)
{
	if(this != &cUI)
	{
		delete rootFolder;
		delete clipboard;
		delete[] currentFileSystemPath; 
		rootFolder = new Folder();
		*rootFolder = *cUI.rootFolder;	//cpy
		currentFolder = new Folder();
		*currentFolder = *cUI.currentFolder;
		*clipboard = *cUI.clipboard;
		int copyPathLength = strlen(cUI.currentFileSystemPath);
		currentFileSystemPath = new char[copyPathLength + 1];
		strcpy_s(currentFileSystemPath, copyPathLength + 1, cUI.currentFileSystemPath);
	}
	return *this;
}

void ConsoleUI::lifetime()
{
	char userInput[100] = "";
	while(strcmp(userInput, "exit"))
	{
		bool properInput = false;
		system("cls");
		print(currentFolder);
		do
		{
			cin.getline(userInput, 100);
			properInput = processUserInput(userInput);
		} 
		while(properInput == false);
	}
}

bool ConsoleUI::processUserInput(const char* userInput, int size)
{
	bool processingWentOK = false;	//the method will keep asking for input as long as this remains false
									//..it will remain false if the userInput differentiates from the known commands
	//below are the commands recognised by the ConsoleUI class
	//EXIT
	if(!strcmp(userInput, "exit"))		//exits the program
	{
		processingWentOK = true;
	}

	//HELP -- prints out the known commands

	else if(!strcmp(userInput, "help"))
	{
		cout<<"\nIf there are files or folders in the current folder, you will see them preceded by a unique number."
			<<" You can use it to access the chosen file/folder.\n\n"
			<<"Commands:\n\n"
			<<"-create folder\n\n"
			<<"-create file\n\n"
			<<"-delete folder\n\n"
			<<"-delete file\n\n"
			<<"-copy folder - choose a folder and it will be added to the clipboard\n\n"
			<<"-copy file - choose a file and it will be added to the clipboard\n\n"
			<<"-paste - all the contents of the clipboard will be copied in the current folder. this empties the clipboard\n\n"
			<<"-edit file - you can edit its data(text)\n\n" 
			<<"-rename folder\n\n"
			<<"-rename file\n\n"
			<<"-print info - prints creator name, time of creation and time of last edit/rename\n\n"
			<<"-exit\n\n";
		system("pause");
		processingWentOK = true;
	}

	//GO BACK -- go back to the previous folder

	else if((strlen(userInput) == 1) && (userInput[0] == '0') && (currentFolder != rootFolder)) //if input is 0
	{
		if(currentFolder != rootFolder)
		{
			back();
		}
		processingWentOK = true;
	}

	//ACCESS OBJECT -- enter a folder or print out the data of a file -- this method is used if 
	//the user input is a double-digit number

	else if((strlen(userInput) == 2) && (userInput[0] >= '0') && (userInput[0] <= '9') && (userInput[1] >= '0') && (userInput[1] <= '9')) //if input is 10-99
	{
		int number = ((int)(userInput[1]) - 48) + ((int)(userInput[0]) - 48) * 10;
		if(number <= numberOfFileSystemMembersInCurrentFolder)
		{
			if(number <= currentFolder->folderCount) //number is always >= 1
			{
				accessFolder(number);
			}
			else
			{
				accessFile(number);
			}
			processingWentOK = true;
		}
	}

	//ACCESS OBJECT -- for single-digit numbers -- this method and the one above it can be optimised (merged)

	else if((strlen(userInput) == 1) && (userInput[0] >= '1') && (userInput[0] <= '9')) //if input is a digit
	{
		int number = (int)(userInput[0]) - 48;
		if(number <= numberOfFileSystemMembersInCurrentFolder)
		{
			if(number <= currentFolder->folderCount) //number is always >= 1
			{
				accessFolder(number);
			}
			else
			{
				accessFile(number);
			}
			processingWentOK = true;
		}
	}

	//CREATE FOLDER

	else if(!strcmp(userInput, "create folder"))
	{
		createFolder();
		processingWentOK = true;
	}

	//CREATE FILE

	else if(!strcmp(userInput, "create file"))
	{
		createFile();
		processingWentOK = true;
	}

	//DELETE FOLDER

	else if(!strcmp(userInput, "delete folder"))
	{
		if(currentFolder->folderCount == 0)
		{
			cout<<"\nThere are no folders in the current folder!\n";
			system("pause");
		}
		else
		{
			deleteFolder(askForFolderNumber());
		}
		processingWentOK = true;
	}

	//DELETE FILE

	else if(!strcmp(userInput, "delete file"))
	{
		if(currentFolder->fileCount == 0)
		{
			cout<<"\n\nThere's no files in this folder!\n\n";
			system("pause");
		}
		else
		{
			deleteFile(askForFileNumber());
		}
		processingWentOK = true;
	}

	//COPY FILE

	else if(!strcmp(userInput, "copy file"))
	{
		if(currentFolder->fileCount == 0)
		{
			cout<<"\n\nThere're no files in this folder!\n\n";
			system("pause");
		}
		else
		{
			copyFile(askForFileNumber());
		}
		processingWentOK = true;
	}

	//COPY FOLDER

	else if(!strcmp(userInput, "copy folder"))
	{
		if(currentFolder->folderCount == 0)
		{
			cout<<"\n\nThere're no folders in the current folder!\n\n";
			system("pause");
		}
		else
		{
			copyFolder(askForFolderNumber());
		}
		processingWentOK = true;
	}

	//PASTE

	else if(!strcmp(userInput, "paste"))
	{
		paste();
		processingWentOK = true;
	}

	//PRINT INFO

	else if(!strcmp(userInput, "print info"))
	{
		if(currentFolder->isEmpty())
		{
			cout<<"\n\nFolder is empty. Nothing to print info about!\n\n";
			system("pause");
		}
		else
		{
			int number = 0;
			do
			{
				cout<<"Choose an object to print info for by typing its corresponding number: ";
				cin>>number;
				cin.clear();
				cin.ignore( numeric_limits <streamsize> ::max(), '\n' );
			}
			while((number < 0) || (number > numberOfFileSystemMembersInCurrentFolder));
			if(number <= currentFolder->folderCount)
			{
				int positionInFolderArray = number - 1;
				system("cls");
				currentFolder->nestedFolders[positionInFolderArray]->printInfo();
				system("pause");
			}
			else
			{
				int positionInFileArray = number - currentFolder->folderCount - 1;
				system("cls");
				currentFolder->nestedFiles[positionInFileArray]->printInfo();
				system("pause");
			}
		}
		processingWentOK = true;
	}

	//RENAME FOLDER

	else if(!strcmp(userInput, "rename folder"))
	{
		if(currentFolder->folderCount == 0)
		{
			cout<<"\n\nThere's no folders in this folder!\n\n";
			system("pause");
		}
		else
		{
			renameFolder(askForFolderNumber());
		}
		processingWentOK = true;
	}
	
	//RENAME FILE

	else if(!strcmp(userInput, "rename file"))
	{
		if(currentFolder->fileCount == 0)
		{
			cout<<"\n\nThere's no files in this folder!\n\n";
			system("pause");
		}
		else
		{
			renameFile(askForFileNumber());
		}
		processingWentOK = true;
	}
	
	//EDIT FILE

	else if(!strcmp(userInput, "edit file"))
	{
		if(currentFolder->fileCount == 0)
		{
			cout<<"There are no files in this folder!\n\n";
			system("pause");
		}
		else
		{
			editFile(askForFileNumber());
		}
		processingWentOK = true;
	}
	return processingWentOK;
}

void ConsoleUI::back()
{
	updateCurrentFileSystemPathBackwards();
	currentFolder = currentFolder->parentFolder;
	updateNumberOfFileSystemMembersInCurrentFolder();
}

void ConsoleUI::editFile(int position)
{
	char newData[100];
	system("cls");
	cout<<"Enter new data(text) for the file '"<<currentFolder->nestedFiles[position]->name<<"':\n";
	do
	{
		cin.getline(newData, 100);
	}
	while(strlen(newData) == 0);
	currentFolder->nestedFiles[position]->setData(newData);
	currentFolder->nestedFiles[position]->updateTimeOfLastEdit();
}

void ConsoleUI::deleteFile(int position)
{
	currentFolder->removeFile(*currentFolder->nestedFiles[position]);
	updateNumberOfFileSystemMembersInCurrentFolder();
}

void ConsoleUI::deleteFolder(int position)
{
	currentFolder->removeFolder(*(currentFolder->nestedFolders[position]));
	updateNumberOfFileSystemMembersInCurrentFolder();
}

void ConsoleUI::renameFile(int position)
{
	char name[100];
	system("cls");
	cout<<"New name for file '"<<currentFolder->nestedFiles[position]->name<<"': ";
	do
	{
		cin.getline(name, 100);
	}
	while(strlen(name) == 0);
	currentFolder->nestedFiles[position]->setName(name);
	currentFolder->nestedFiles[position]->updateTimeOfLastEdit();
}

void ConsoleUI::renameFolder(int position)
{
	char name[100];
	system("cls");
	cout<<"New name for folder '"<<currentFolder->nestedFolders[position]->name<<"': ";
	do
	{
		cin.getline(name, 100);
	}
	while(strlen(name) == 0);
	currentFolder->nestedFolders[position]->setName(name);
	currentFolder->nestedFolders[position]->updateTimeOfLastEdit();
}

void ConsoleUI::accessFolder(int position)
{
	int positionInFolderArray = position - 1;
	currentFolder = currentFolder->nestedFolders[positionInFolderArray];
	updateNumberOfFileSystemMembersInCurrentFolder();
	updateCurrentFileSystemPathForwards();
}

void ConsoleUI::accessFile(int position)
{
	int positionInFileArray = position - currentFolder->folderCount - 1;
	system("cls");
	cout<<"Reading file '"<<currentFolder->nestedFiles[positionInFileArray]->getName()<<"' :\n";
	cout<<currentFolder->nestedFiles[positionInFileArray]->getData()<<endl<<endl;
	system("pause");
}

void ConsoleUI::createFile()
{
	char fileName[100], creatorName[100], data[100];
	system("cls");
	cout<<"Need file details:\n"
		<<"File name: ";
	do
	{
		cin.getline(fileName, 100);
	}
	while(strlen(fileName) == 0);
	cout<<"\nYour name: ";
	do
	{
		cin.getline(creatorName, 100);
	}
	while(strlen(creatorName) == 0);
	cout<<"\nFile data(text): ";
	do
	{
		cin.getline(data, 100);
	}
	while(strlen(data) == 0);
	File* newFile = new File(currentFolder, data, fileName, creatorName);
	updateNumberOfFileSystemMembersInCurrentFolder();
}

void ConsoleUI::createFolder()
{
	char folderName[100], creatorName[100];
	system("cls");
	cout<<"Need folder details:\n"
		<<"Folder name: ";
	do
	{
		cin.getline(folderName, 100);
	}
	while(strlen(folderName) == 0);
	cout<<"\nYour name: ";
	do
	{
		cin.getline(creatorName, 100);
	}
	while(strlen(creatorName) == 0);
	Folder* newFolder = new Folder(currentFolder, folderName, creatorName);
	updateNumberOfFileSystemMembersInCurrentFolder();
}

void ConsoleUI::copyFile(int position)
{
	clipboard->addFile(*currentFolder->nestedFiles[position]);
}

void ConsoleUI::copyFolder(int position)
{
	clipboard->addFolder(*currentFolder->nestedFolders[position]);
}

void ConsoleUI::paste()
{
	if(clipboard->isEmpty())
	{
		cout<<"\n\nClipboard is empty!\n\n";
	}
	else
	{
		system("cls");
		for(int i = 0; i < clipboard->fileCount; i++)
		{
			cout<<"Pasting file #"<<i + 1<<"...\n";
			currentFolder->addFile(*clipboard->nestedFiles[i]);
		}
		for(int i = 0; i < clipboard->folderCount; i++)
		{
			cout<<"Pasting folder #"<<i + 1<<"...\n";
			currentFolder->addFolder(*clipboard->nestedFolders[i]);
		}
	}
	updateNumberOfFileSystemMembersInCurrentFolder();
	cleanClipboard();
	system("pause");
}

void ConsoleUI::print(Folder* f)
{
	cout<<"____________\n";
	cout<<"You are viewing "<<currentFileSystemPath<<endl;
	if(currentFolder != rootFolder)
	{
		cout<<"0: Go back\n";
	}
	f->print();
	cout<<"____________\n";
	cout<<"Input: ";
} //differentiates from the Folder::print() because it also prints the file system path

void ConsoleUI::updateNumberOfFileSystemMembersInCurrentFolder()
{
	numberOfFileSystemMembersInCurrentFolder = currentFolder->fileCount + currentFolder->folderCount;
}

void ConsoleUI::updateCurrentFileSystemPathForwards()
{
	int pathLength = strlen(currentFileSystemPath);
	int addonLength = strlen(currentFolder->name);
	char* copyPath = new char[pathLength + 1];
	strcpy_s(copyPath, pathLength + 1, currentFileSystemPath);
	currentFileSystemPath = new char[pathLength + addonLength + 3];
	strcpy_s(currentFileSystemPath, pathLength + addonLength + 3, copyPath);
	strcat_s(currentFileSystemPath, pathLength + addonLength + 3, currentFolder->name);
	strcat_s(currentFileSystemPath, pathLength + addonLength + 3, "\\");
	delete[] copyPath;
}

void ConsoleUI::updateCurrentFileSystemPathBackwards()
{
	int pathLength = strlen(currentFileSystemPath); //Root:\games\ - 12
	int currentFolderNameLength = strlen(currentFolder->name); //games - 5
	int numberOfCharactersLeft = pathLength - currentFolderNameLength - 1; // 12 - 5 - 1 = 6; Root:\ 
	char* copyPath = new char[pathLength + 1];
	strcpy_s(copyPath, pathLength + 1, currentFileSystemPath);
	delete[] currentFileSystemPath;
	currentFileSystemPath = new char[numberOfCharactersLeft + 1];
	strcpy_s(currentFileSystemPath, 1, "");
	strncat(currentFileSystemPath, copyPath, numberOfCharactersLeft);
}

void ConsoleUI::cleanClipboard()
{
	delete clipboard;
	clipboard = new Folder(NULL, "Clipboard", "Nikola Iliev");
}

int ConsoleUI::askForFileNumber() const
{
	int number = 0;
	do
	{
		cout<<"Choose a file by typing its corresponding number: ";
		cin>>number;
		cin.clear();
		cin.ignore( numeric_limits <streamsize> ::max(), '\n' );
	}
	while(number < 0  || number > numberOfFileSystemMembersInCurrentFolder || number <= currentFolder->folderCount);
	int positionInFileArray = number - currentFolder->folderCount - 1;
	return positionInFileArray;
}

int ConsoleUI::askForFolderNumber() const
{
	int number = 0;
	do
	{
	cout<<"Choose a folder by typing its corresponding number: ";
	cin>>number;
	cin.clear();
	cin.ignore( numeric_limits <streamsize> ::max(), '\n' );
	}
	while((number > numberOfFileSystemMembersInCurrentFolder) || (number > currentFolder->folderCount) || (number < 0));
	int positionInFolderArray = number - 1;
	return positionInFolderArray;
}