#ifndef FILESYSTEMMEMBER_H
#define FILESYSTEMMEMBER_H
class Folder;

class FileSystemMember
{
public:
	FileSystemMember();
	~FileSystemMember();
	FileSystemMember(const FileSystemMember&);
	FileSystemMember& operator=(const FileSystemMember&);
	FileSystemMember(Folder*, const char*, const char*);
	void setName(const char*);
	const char* getName() const;
	void setCreatorName(const char*);
	const char* getCreatorName() const;
	void setParentFolder(Folder*);
	Folder* getParentFolder() const;
	void updateTimeOfCreation();
	void updateTimeOfLastEdit();
	const char* getTimeOfCreation() const;
	const char* getTimeOfLastEdit() const;

	
	Folder* parentFolder;
protected:
	void copyDetails(const FileSystemMember&);
private:
	void deleteDetails();
	char* name;
	char* creatorName;
	char* timeOfCreation;
	char* timeOfLastEdit;
	friend class ConsoleUI;
};

#endif