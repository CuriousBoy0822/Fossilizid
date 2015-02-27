PROJECT(remote_queue_client)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6) 

INCLUDE_DIRECTORIES(../../../juggle/interface)
INCLUDE_DIRECTORIES(../juggle_achieve/json_plugin)

AUX_SOURCE_DIRECTORY(. DIR_SRC)
AUX_SOURCE_DIRECTORY(../juggle_achieve/json_plugin juggle_achieve_DIR_SRC)

SET(header_files
    ../juggle_scprit/client/jugglecaller.h
    ../juggle_achieve/channelserver.h
    ../juggle_achieve/json_plugin/json_plugin.h
)

SET(SRC_LIST 
    ${DIR_SRC}
    ${juggle_achieve_DIR_SRC}
    ../juggle_scprit/client/callerservicedefine.cpp
)

add_executable(test_service_client ${SRC_LIST} ${header_files})
target_link_libraries(test_service_client juggle uuid remoteq pool jsoncpp context)