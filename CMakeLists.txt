cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
add_executable(archiver
        main.cpp Encode.cpp Encode.h Huffman.cpp Huffman.h Decode.cpp Decode.h)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
