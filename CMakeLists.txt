cmake_minimum_required(VERSION 3.15)
project(FinalProjExamples)

# filesystem.h require C++17
set(CMAKE_CXX_STANDARD 17)

file(COPY sample_data/ DESTINATION sample_data/)
#file(COPY stopwords.txt DESTINATION stopwords.txt)

# show compiler output and enable warnings
set(CMAKE_VERBOSE_MAKEFILE ON)
add_compile_options(-Wall -Wextra -pedantic)

add_executable(rapidJSONExample rapidJSONExample.cpp)
add_executable(supersearch main.cpp Index_Handler.cpp Document_Parser.h Document_Parser.cpp Query_Processor.cpp User_Interface.cpp porter2_stemmer.cpp AVLTree.h)
include(CTest)
add_executable(testAVL test_AVLTree.cpp AVLTree.h Index_Handler.cpp catch2/catch.hpp) #removed hahsler's tree
add_test(NAME AVL_Tree COMMAND testAVL)
# this makes sure we also link rapidjson
target_include_directories(rapidJSONExample PRIVATE rapidjson/)

set(CMAKE_VERBOSE_MAKEFILE OFF)
