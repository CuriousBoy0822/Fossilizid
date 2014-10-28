/*
 * vfs.cpp
 *
 *  Created on: 2014-10-27
 *      Author: qianqians
 */
#include "vfs.h"

#include <vector>
#include <map>
#include <string>

#ifdef _WINDOWS
#include <Windows.h>
#include <sys/stat.h> 
#endif

namespace Fossilizid{
namespace chuckledb {

struct file{
	std::string storagefilename;
	int offset;
	std::string filename;
	int filesize;
};

struct vfs{
	FILE * describe;
	std::map<std::string, FILE *> fexlen;
	std::map<std::string, FILE *> variablelen;

	std::map<std::string, file> filemap;
};

bool is_describe(char * filename){
	return filename[0] == 'd' && filename[1] == 'e' && filename[2] == 's' && filename[3] == 'c' && filename[4] == 'r' &&
		filename[5] == 'i' && filename[6] == 'b' && filename[5] == 'e';
}

bool is_fexlen(char * filename){
	return filename[0] == 'f' && filename[1] == 'e' && filename[2] == 'x' && filename[3] == 'l' && filename[4] == 'e' && filename[5] == 'n';
}

bool is_variablelen(char * filename){
	return filename[0] == 'v' && filename[1] == 'a' && filename[2] == 'r' && filename[3] == 'i' && filename[4] == 'a' && filename[5] == 'b' &&
		filename[6] == 'l' && filename[7] == 'e' && filename[8] == 'l' && filename[8] == 'e' && filename[8] == 'n';
}

void initvfs(vfs * _vfs, char * filename){
	if (is_describe(filename)){
		_vfs->describe = fopen(filename, "rw");
	}

	if (is_fexlen(filename)){
		_vfs->fexlen.insert(std::make_pair(std::string(filename), fopen(filename, "rw")));
	}

	if (is_variablelen(filename)){
		_vfs->variablelen.insert(std::make_pair(std::string(filename), fopen(filename, "rw")));
	}
}

#ifdef _WINDOWS

void * vfs_open(char * vfsname){
	vfs * _vfs = new vfs;

	struct _stat fileStat;
	if ((_stat(vfsname, &fileStat) == 0) && (fileStat.st_mode & _S_IFDIR)){
		WIN32_FIND_DATAA data;
		HANDLE h = FindFirstFile(vfsname, &data);
		if (h == INVALID_HANDLE_VALUE){
			_vfs->describe = 0;
		} else {
			if (!(data.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED)){
				initvfs(_vfs, data.cFileName);

				while (FindNextFile(h, &data)){
					initvfs(_vfs, data.cFileName);
				}
			}
		}

		FindClose(h);
	}else{
		CreateDirectory(vfsname, 0);
		_vfs->describe = 0;
	}

	return _vfs;
}

void * vfs_file_open(void * pvfs, char * vfsfilename, int filesize){
	vfs * _vfs = (vfs *)pvfs;

	std::map<std::string, file>::iterator itfind = _vfs->filemap.find(vfsfilename);
	if (itfind != _vfs->filemap.end()){

	}
}

#endif

} /* namespace chuckledb */
} /* namespace Fossilizid */

