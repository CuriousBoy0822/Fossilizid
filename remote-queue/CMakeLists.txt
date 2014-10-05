PROJECT(remote-queue)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6) 

AUX_SOURCE_DIRECTORY(. DIR_SRC)
AUX_SOURCE_DIRECTORY(./windows windows_DIR_SRC)

SET(header_files
    queue.h
    acceptor.h
    channel.h
    endpoint.h
    ./windows/overlapped.h
    ./windows/handle.h
    ./windows/acceptorimpl.h
    ./windows/channelimpl.h
    ./windows/endpointimpl.h
    ./windows/queueimpl.h
)

SET(SRC_LIST 
    ${DIR_SRC}
    ${windows_DIR_SRC}
)

ADD_LIBRARY(remote-queue ${SRC_LIST} ${header_files})