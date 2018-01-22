cmake_minimum_required(VERSION 3.9)
project(DataStructure)

set(CMAKE_CXX_STANDARD 14)

add_executable(DataStructure
        iterator.h
        main.cpp
        memory.h
        rb_tree.h sequence.h single_list.h type_base.h stack.h queue.h heap.h)
