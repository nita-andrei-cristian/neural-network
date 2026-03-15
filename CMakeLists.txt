cmake_minimum_required(VERSION 3.16)
project(neuralnetwork)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for lsp

add_library(utils SHARED ${CMAKE_SOURCE_DIR}/c/lib/utils.c)
add_library(menu SHARED ${CMAKE_SOURCE_DIR}/c/lib/menu.c)

add_library(connections SHARED ${CMAKE_SOURCE_DIR}/c/lib/neuro-engine/connections.c)
add_library(memory SHARED ${CMAKE_SOURCE_DIR}/c/lib/neuro-engine/memory.c)
add_library(nodes SHARED ${CMAKE_SOURCE_DIR}/c/lib/neuro-engine/nodes.c)
add_library(search SHARED ${CMAKE_SOURCE_DIR}/c/lib/neuro-engine/search.c)

add_library(core SHARED ${CMAKE_SOURCE_DIR}/c/lib/neuro-engine/core.c)

target_include_directories(nodes PUBLIC ${CMAKE_SOURCE_DIR}/c/lib/neuro-engine)
target_include_directories(connections PUBLIC ${CMAKE_SOURCE_DIR}/c/lib/neuro-engine)
target_include_directories(memory PUBLIC ${CMAKE_SOURCE_DIR}/c/lib/neuro-engine)
target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/c/lib/neuro-engine)
target_include_directories(menu PUBLIC ${CMAKE_SOURCE_DIR}/c/lib)
target_include_directories(utils PUBLIC ${CMAKE_SOURCE_DIR}/c/lib)

add_executable(neuralnetwork ${CMAKE_SOURCE_DIR}/c/main.c)

target_link_libraries(core PRIVATE nodes connections memory search utils) 
target_link_libraries(neuralnetwork PRIVATE menu core )
# target_link_libraries(neuralnetwork PRIVATE llama ggml)
