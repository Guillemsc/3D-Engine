#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

#include "Module.h"

class FileSystem : public Module
{
public:
	FileSystem(bool start_enabled = true);
	~FileSystem();

	bool Awake();
	bool Start();
	bool Update();
	bool CleanUp();

	bool CopyToExplorer(const char * full_path, const char * destination);


private:

public:

};

#endif // __FILESYSTEM_H__