PROJECT(juggle)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6) 

file(GLOB interface_head_file ./interface ./interface/*.h)
file(GLOB service_head_file ./service ./service/*.h)

AUX_SOURCE_DIRECTORY(./service service_SRC)

SET(header_files
    ${interface_head_file}
    ${service_head_file}
)


SET(SRC_LIST 
    ${service_SRC}
)

ADD_LIBRARY(juggle ${SRC_LIST} ${header_files})
target_link_libraries(juggle uuid)