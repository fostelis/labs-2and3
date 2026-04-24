cmake_minimum_required(VERSION 4.0)
project(laba2)

set(CMAKE_CXX_STANDARD 20)

add_executable(laba2 main.cpp
        tests.cpp
        tests.h
        algorithms.h
        bit_sequence.h
        list_sequence.h
        array_sequence.h
        sequence.h
        linked_list.h
        dynamic_array.h
        bit.h
        option.h
        exceptions.h
        mutable_list_sequence.h
        sequence_crtp.h
        tests.cpp)
