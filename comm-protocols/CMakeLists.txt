PROJECT(comm-protocols)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6) 

AUX_SOURCE_DIRECTORY(. DIR_SRC)

SET(header_files
    enumtype.h
    elementinfo.h
    networkcmd.h
    vector.h
)

SET(SRC_LIST 
    ${DIR_SRC}
)

ADD_LIBRARY(comm-protocols ${SRC_LIST} ${header_files})