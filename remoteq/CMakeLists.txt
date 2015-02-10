PROJECT(remoteq)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6) 

AUX_SOURCE_DIRECTORY(. DIR_SRC)
AUX_SOURCE_DIRECTORY(./windows windows_DIR_SRC)

SET(header_files
    remote_queue.h
    queue.h
    close.h
    acceptor.h
    channel.h
    endpoint.h
    typedef.h
    ./windows/wsainit.h
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

ADD_LIBRARY(remoteq ${SRC_LIST} ${header_files})