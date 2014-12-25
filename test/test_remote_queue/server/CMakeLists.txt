PROJECT(remote_queue_server)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6) 

AUX_SOURCE_DIRECTORY(. DIR_SRC)

SET(header_files
)

SET(SRC_LIST 
    ${DIR_SRC}
)

add_executable(remote_queue_server ${SRC_LIST} ${header_files})
target_link_libraries(remote_queue_server remote-queue pool jsoncpp_lib)