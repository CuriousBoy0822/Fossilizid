PROJECT(juggle_achieve_json)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6) 

file(GLOB reduce_head_file ./ ./*.h)
file(GLOB pluginjson_head_file ./json_plugin ./json_plugin/*.h)

set(head_file
    ${pluginjson_head_file}
    ${reduce_head_file}
)

AUX_SOURCE_DIRECTORY(./json_plugin pluginjson_SRC)

ADD_LIBRARY(juggle_achieve_json ${pluginjson_SRC} ${head_file})
