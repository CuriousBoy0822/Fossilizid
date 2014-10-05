PROJECT(Fossilizid)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8) 

find_package(Boost)
if (Boost_FOUND)
	link_directories(${Boost_INCLUDE_DIR}//stage//lib)
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
endif (Boost_FOUND)

MESSAGE(STATUS "This is BOOST lib dir " ${Boost_LIBRARIES})  
MESSAGE(STATUS "This is BOOST include dir " ${Boost_INCLUDE_DIR})  

MESSAGE(STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})  
MESSAGE(STATUS "This is SOURCE dir " ${PROJECT_SOURCE_DIR})  

set(Fossilizid_include_path ${PROJECT_SOURCE_DIR}//.. CACHE PATH "Fossilizid include path")
INCLUDE_DIRECTORIES(${Fossilizid_include_path})
MESSAGE(STATUS "This is include dir " ${Fossilizid_include_path}) 

set(Fossilizid_lib_path ${PROJECT_BINARY_DIR}/bin/Debug CACHE PATH "Fossilizid lib path")
link_directories(${Fossilizid_lib_path})
MESSAGE(STATUS "This is lib dir " ${Fossilizid_lib_path}) 

set(Fossilizid_bin_path ${PROJECT_BINARY_DIR}/bin CACHE PATH "Fossilizid bin path")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

ADD_SUBDIRECTORY(comm-protocols ${Fossilizid_bin_path}/comm-protocols)
ADD_SUBDIRECTORY(pool ${Fossilizid_bin_path}/pool)
ADD_SUBDIRECTORY(remote-queue ${Fossilizid_bin_path}/remote-queue)
ADD_SUBDIRECTORY(container ${Fossilizid_bin_path}/container)