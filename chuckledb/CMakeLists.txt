PROJECT(chuckledb)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6) 

AUX_SOURCE_DIRECTORY(. DIR_SRC)

SET(SRC_LIST 
    ${DIR_SRC}
)

SET(header_files
    vfs.h
)

add_executable(chuckledb ${SRC_LIST} ${header_files})