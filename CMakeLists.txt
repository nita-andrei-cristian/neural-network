cmake_minimum_required(VERSION 3.16)
project(neuralnetwork)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for lsp

add_library(utils SHARED ${CMAKE_SOURCE_DIR}/c/lib/utils.c)
add_library(nodes SHARED ${CMAKE_SOURCE_DIR}/c/lib/nodes.c)
add_library(connections SHARED ${CMAKE_SOURCE_DIR}/c/lib/connections.c)

target_include_directories(nodes PUBLIC ${CMAKE_SOURCE_DIR}/c/lib)
target_include_directories(connections PUBLIC ${CMAKE_SOURCE_DIR}/c/lib)
target_include_directories(utils PUBLIC ${CMAKE_SOURCE_DIR}/c/lib)

add_executable(neuralnetwork ${CMAKE_SOURCE_DIR}/c/main.c)

target_link_libraries(neuralnetwork PRIVATE nodes connections utils )
# target_link_libraries(neuralnetwork PRIVATE llama ggml)
