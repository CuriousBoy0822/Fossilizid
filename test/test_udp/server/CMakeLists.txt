PROJECT(test_udp_server)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6) 

AUX_SOURCE_DIRECTORY(. DIR_SRC)

SET(header_files
)

SET(SRC_LIST 
    ${DIR_SRC}
)

add_executable(test_udp_server ${SRC_LIST} ${header_files})
target_link_libraries(test_udp_server reliablyt)