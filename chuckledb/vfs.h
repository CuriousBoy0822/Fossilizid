/*
 * vfs.h
 *
 *  Created on: 2014-10-27
 *      Author: qianqians
 */
#ifndef _vfs_h
#define _vfs_h

#include <stdio.h>

namespace Fossilizid{
namespace chuckledb {

void * vfs_open(char * vfsname);

void * vfs_file_open(void * vfs, char * vfsfilename, int filesize);

void * vfs_file_close(void * vfsfile, char * vfsfilename);

} /* namespace chuckledb */
} /* namespace Fossilizid */

#endif //_vfs_h