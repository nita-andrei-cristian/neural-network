cmake_minimum_required(VERSION 3.16)
project(neuralnetwork)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for lsp

add_subdirectory(${CMAKE_SOURCE_DIR}/llama.cpp)

add_executable(neuralnetwork ${CMAKE_SOURCE_DIR}/main.c)

target_link_libraries(neuralnetwork PRIVATE  llama ggml)

