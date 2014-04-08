#ifndef CONSOLEUI_H
#define CONSOLEUI_H
#include "FileSystemMember.h"

class ConsoleUI
{
public:
	ConsoleUI();
	~ConsoleUI();
	ConsoleUI(const ConsoleUI&);
	ConsoleUI& operator=(const ConsoleUI&);

	void lifetime();	//keeps the program alive, until the user inputs 'exit'
	void back();
	bool processUserInput(const char* userInput, int size = 100);
	void accessFolder(int position);
	void accessFile(int position);
	void createFolder();
	void createFile();
	void renameFolder(int position);
	void renameFile(int position);
	void deleteFolder(int position);
	void deleteFile(int position);
	void copyFile(int position);
	void copyFolder(int position);
	void paste();
	void editFile(int position);
	void print(Folder*);

private:
	Folder* rootFolder;
	Folder* currentFolder;
	Folder* clipboard;	//used for copy-pasting
	char* currentFileSystemPath;
	int numberOfFileSystemMembersInCurrentFolder;
	void updateCurrentFileSystemPathForwards();
	void updateCurrentFileSystemPathBackwards();	//updates the path i.e. Root:\games\Diablo III\ etc.
	void updateNumberOfFileSystemMembersInCurrentFolder();
	void cleanClipboard();
	int askForFileNumber() const;	//used for user input
	int askForFolderNumber() const;
};

#endif